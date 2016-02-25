#include "track.h"
#include "TMath.h"

Track::Track()
{
   min_itan_ = -1;
   sigma_ = 0.2; // 200um
} 

void Track::SetHit(int cid, Hit& hit)
{
   hits_[cid] = hit;
}

void Track::SetSigma(double sigma)
{
   sigma_ = sigma;
}

void Track::SetHitZWithTangent(WireMap& wiremap, int itan)
{
   for (int cid=0; cid<MAX_LAYER; cid++) {
      Hit& hit = hits_[cid];
      int icell = hit.GetCellNumber();
      Line& wire = wiremap.GetWire(cid, icell);
      TVector3 pA;
      TVector3 pB;
      Line& tangent = GetTangent(itan);
      wire.GetClosestPoints(tangent, pA, pB);
      hit.SetZ(pA.Z());
   }
}
void Track::SetHitZWithMinTangent(WireMap& wiremap)
{
   SetHitZWithTangent(wiremap, min_itan_);
}

void Track::MakeTangents(WireMap& wiremap, XTcurve& xt, int cid1, int cid2, double z1, double z2)
{
   // Transform in new coordinate
   Hit& hit1 = hits_[cid1];
   Hit& hit2 = hits_[cid2];
   //hit1.SetZ(z1);
   //hit2.SetZ(z2);
   Line& wire1 = wiremap.GetWire(hit1.GetLayerNumber(), hit1.GetCellNumber());
   Line& wire2 = wiremap.GetWire(hit2.GetLayerNumber(), hit2.GetCellNumber());
   TVector3 wirepos1 = wire1.GetPosAtZ(z1);
   TVector3 wirepos2 = wire2.GetPosAtZ(z2);
   // Consider in 2D plane
   TVector3 wirepos1_2D = wirepos1;
   TVector3 wirepos2_2D = wirepos2;
   wirepos1_2D.SetZ(0.0);
   wirepos2_2D.SetZ(0.0);
   TVector3 relpos = wirepos2_2D-wirepos1_2D;
   if (g_debug_track>0) {
      wirepos1.Print();
      wirepos2.Print();
   }
   double x2 = relpos.Mag();
   double r1 = hit1.GetHitR(xt);
   double r2 = hit2.GetHitR(xt);

   double xR1;
   double yR1;
   double xL1;
   double yL1;
   double xR2;
   double yR2;
   double xL2;
   double yL2;
   CalcPointsOnTangentials(x2, r1, r2, xR1, yR1, xL1, yL1, xR2, yR2, xL2, yL2);
   tangents_[0].MakeLine(TVector3(xL1,  yL1, z1), TVector3(xR1,  yR1, z2));
   tangents_[1].MakeLine(TVector3(xL1, -yL1, z1), TVector3(xR1, -yR1, z2));
   tangents_[2].MakeLine(TVector3(xL2,  yL2, z1), TVector3(xR2,  yR2, z2));
   tangents_[3].MakeLine(TVector3(xL2, -yL2, z1), TVector3(xR2, -yR2, z2));
   double dx = relpos.X();
   double dy = relpos.Y();
   double relang = TMath::ATan2(dy, dx);
   for (int i=0; i<4; i++) {
      tangents_[i].Trans2D(wirepos1, relang);
   }

   if (g_debug_track>0) {
      printf("--- wirepos1 ---\n");
      wirepos1.Print();
      printf("--- wirepos2 ---\n");
      wirepos2.Print();
      printf("--- relang ---\n");
      printf("%f (deg)\n", relang/TMath::Pi()*180);
   }
}

Line& Track::GetTangent(int itan)
{
   return tangents_[itan];
}

Line& Track::GetMinTangent()
{
   return tangents_[min_itan_];
}

int Track::GetMinTangentNumber()
{
   return min_itan_;
}

double Track::GetChi2OfTangent(WireMap& wiremap, XTcurve& xt, int itan)
{
   double chi2 = 0.0;
   for (int cid=0; cid<MAX_LAYER; cid++) {
      Hit& hit = hits_[cid];
      if (!hit.HasHit()) continue;
      int icell = hit.GetCellNumber();
      Line& wire = wiremap.GetWire(cid, icell);
      TVector3 pA;
      TVector3 pB;
      Line& tangent = GetTangent(itan);
      wire.GetClosestPoints(tangent, pA, pB);
      hit.SetZ(pA.Z());
      double fitR = tangents_[itan].GetDistance(wire);
      double hitR = hit.GetHitR(xt);
      double dr = fitR - hitR;
      double hitZ = hit.GetZ();
      double fitZ = pA.Z();
      //printf("cid %d hitR %f fitR %f hitZ %f fitZ %f dr %f chi2 %f\n", cid, hitR, fitR, hitZ, fitZ, dr, chi2);
      chi2 += (dr/sigma_)*(dr/sigma_);
   }
   return chi2;
}

double Track::GetChi2OfMinTangent(WireMap& wiremap, XTcurve& xt)
{
   double min_chi2 = 1e10;
   for (int itan=0; itan<4; itan++) {
      double chi2 = GetChi2OfTangent(wiremap, xt, itan);
      if (chi2 < min_chi2) {
         min_chi2 = chi2;
         min_itan_ = itan;
      }
   }
   return min_chi2;
}

double Track::GetSigma()
{
   return sigma_;
}

Hit& Track::GetHit(int cid)
{
   return hits_[cid];
}

void Track::PrintTangents(WireMap& wiremap, XTcurve& xt)
{
   printf("--- tangents ---\n");
   for (int itan=0; itan<4; itan++) { 
      tangents_[itan].PrintLine();
      printf("chi2 %f\n", GetChi2OfTangent(wiremap, xt, itan));
   }
}

void Track::DrawTangents()
{
   int cols[4] = {kRed, kBlue, kCyan, kMagenta};
   for (int i=0; i<4; i++) { 
      tangents_[i].DrawLine(cols[i]); 
   }
}

double Track::GetXFromLine(WireMap& wiremap, int cid, Line& line)
{
   Hit& hit = hits_[cid];
   if (!hit.HasHit()) return -1000;
   int icell = hit.GetCellNumber();
   Line& wire = wiremap.GetWire(cid, icell);
   TVector3 pA;
   TVector3 pB;
   double dist = wire.GetClosestPoints(line, pA, pB);
   double fitx = pB.X();
   TVector3 pW = wire.GetPosAtZ(pA.Z());
   double xwire = pW.X();
   if (g_debug_track>0) {
      printf("pB: "); pB.Print();
      printf("pW: "); pW.Print();
   }
   if (fitx < xwire) {
      return -dist;
   }
   return dist;
}

double Track::GetXFromMinTangent(WireMap& wiremap, int cid)
{
   return GetXFromLine(wiremap, cid, GetMinTangent());
}

double Track::GetResidualOfLine(WireMap& wiremap, XTcurve& xt, int cid, Line& line)
{
   Hit& hit = hits_[cid];
   return TMath::Abs(GetXFromLine(wiremap, cid, line)) - hit.GetHitR(xt);
}

double Track::GetResidualOfMinTangent(WireMap& wiremap, XTcurve& xt, int cid)
{
   return GetResidualOfLine(wiremap, xt, cid, GetMinTangent());
}

void Track::PrintTrack(WireMap& wiremap, XTcurve& xt)
{
   printf("min_itan_ %d\n", min_itan_);
   printf("min_chi2  %lf\n", GetChi2OfMinTangent(wiremap, xt));
   Line& min_tangent = GetMinTangent();
   for (int cid=0; cid<MAX_LAYER; cid++) {
      Hit& hit = hits_[cid];
      if (!hit.HasHit()) continue;
      int icell = hit.GetCellNumber();
      Line& wire = wiremap.GetWire(cid, icell);
      double fitX = GetXFromMinTangent(wiremap, cid);
      double fitR = min_tangent.GetDistance(wire);
      double dT = hit.GetDriftTime() - hit.GetT0();
      double hitR = hit.GetHitR(xt);
      printf("cid %2d icell %2d t0 %3.2f drift_time %6.2f dT %6.2f --> hitR %6.2f fitR %6.2f fitX %6.2f hitZ %6.2f fitR-hitR %6.2f\n",
            cid, icell, hit.GetT0(), hit.GetDriftTime(), dT, hitR, fitR, fitX, hit.GetZ(), fitR-hitR);
   }
}

void Track::CalcPointsOnTangentials(
      double x2, double r1, double r2,
      double& xR1,
      double& yR1,
      double& xL1,
      double& yL1,
      double& xR2,
      double& yR2,
      double& xL2,
      double& yL2)
{
   xL1 = (r1*r1 + r1*r2)/x2;
   yL1 = TMath::Sqrt(r1*r1 - (r1*r1+r1*r2)/x2*(r1*r1+r1*r2)/x2);
   xR1 = ((x2*x2-r2*r2-r1*r2)/x2);
   yR1 = -TMath::Sqrt(r2*r2 - (-r2*r2-r1*r2)/x2*(-r2*r2-r1*r2)/x2);
   xL2 = (r1*r1 - r1*r2)/x2;
   yL2 = TMath::Sqrt(r1*r1 - (r1*r1-r1*r2)/x2*(r1*r1-r1*r2)/x2);
   xR2 = ((x2*x2-r2*r2+r1*r2)/x2);
   yR2 = TMath::Sqrt(r2*r2 - (-r2*r2+r1*r2)/x2*(-r2*r2+r1*r2)/x2);
   if (g_debug_track>0) {
      printf("xR1 %lf yR1 %lf\n", xR1, yR1);
      printf("xL1 %lf yL1 %lf\n", xL1, yL1);
      printf("xR2 %lf yR2 %lf\n", xR2, yR2);
      printf("xL2 %lf yL2 %lf\n", xL2, yL2);
   }
}

