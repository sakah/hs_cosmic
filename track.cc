#include "track.h"
#include "TMath.h"

static Track *Track_obj;

Track::Track()
{
   sigma_ = 0.2; // 200um
   test_cid_ = -1;  // use all of hits in fitting
   fit_func_type_ = FIT_FUNC_TYPE_FIX_T0;
   fit_num_params_ = 4;
   minuit_ = NULL;
} 

void Track::ClearFitFlag()
{
   for (int cid=0; cid<MAX_LAYER; cid++) {
      hits_[cid].SetUseByFitFlag(false);
   }
}

void Track::SetHit(int cid, Hit& hit)
{
   hits_[cid] = hit;
}

void Track::SetSigma(double sigma)
{
   sigma_ = sigma;
}

void Track::SetHitZWithLine(WireMap& wiremap, Line& line)
{
   for (int cid=0; cid<MAX_LAYER; cid++) {
      Hit& hit = hits_[cid];
      int icell = hit.GetCellNumber();
      Line& wire = wiremap.GetWire(cid, icell);
      TVector3 pA;
      TVector3 pB;
      wire.GetClosestPoints(line, pA, pB);
      hit.SetZ(pA.Z());
   }
}
void Track::SetHitZWithMinTangent(WireMap& wiremap, XTcurve& xt)
{
   SetHitZWithLine(wiremap, GetMinTangent(wiremap, xt));
}

int Track::MakeTangents(WireMap& wiremap, XTcurve& xt, int cid1, int cid2, double z1, double z2)
{
   // Transform in new coordinate
   Hit& hit1 = hits_[cid1];
   Hit& hit2 = hits_[cid2];
   if (!hit1.UseByFit() || !hit2.UseByFit()) {
      return 0;
   }
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

   if (g_debug_track) {
      printf("--- wirepos1 ---\n");
      wirepos1.Print();
      printf("--- wirepos2 ---\n");
      wirepos2.Print();
      printf("--- relang ---\n");
      printf("%f (deg)\n", relang/TMath::Pi()*180);
   }
   return 4;
}

Line& Track::GetTangent(int itan)
{
   return tangents_[itan];
}

double Track::GetChi2OfLine(WireMap& wiremap, XTcurve& xt, Line& line)
{
   double chi2 = 0.0;
   for (int cid=0; cid<MAX_LAYER; cid++) {
      Hit& hit = hits_[cid];
      if (!hit.HasHit()) continue;
      if (!hit.UseByFit()) continue;
      int icell = hit.GetCellNumber();
      Line& wire = wiremap.GetWire(cid, icell);
      TVector3 pA;
      TVector3 pB;
      wire.GetClosestPoints(line, pA, pB);
      hit.SetZ(pA.Z());
      double fitR = line.GetDistance(wire);
      double hitR = hit.GetHitR(xt);
      double dr = fitR - hitR;
      double hitZ = hit.GetZ();
      double fitZ = pA.Z();
      //printf("cid %d hitR %f fitR %f hitZ %f fitZ %f dr %f chi2 %f\n", cid, hitR, fitR, hitZ, fitZ, dr, chi2);
      chi2 += (dr/sigma_)*(dr/sigma_);
   }
   //printf("==> chi2 %f\n", chi2);
   return chi2;
}

Line& Track::GetMinTangent(WireMap& wiremap, XTcurve& xt)
{
   int min_itan=0;
   double min_chi2 = 1e10;
   for (int itan=0; itan<4; itan++) {
      double chi2 = GetChi2OfLine(wiremap, xt, GetTangent(itan));
      //printf("itan %d chi2 %f\n", itan, chi2);
      if (chi2 < min_chi2) {
         min_chi2 = chi2;
         min_itan = itan;
      }
   }
   return tangents_[min_itan];
}

double Track::GetSigma()
{
   return sigma_;
}

Hit& Track::GetHit(int cid)
{
   return hits_[cid];
}

void Track::PrintHits(XTcurve& xt)
{
   printf("--- hits ---\n");
   for (int cid=0; cid<MAX_LAYER; cid++) {
      hits_[cid].PrintHit(xt);
   }
}

void Track::PrintTangents(WireMap& wiremap, XTcurve& xt)
{
   printf("--- tangents ---\n");
   for (int itan=0; itan<4; itan++) { 
      tangents_[itan].PrintLine();
      printf("chi2 %f\n", GetChi2OfLine(wiremap, xt, GetTangent(itan)));
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
   if (g_debug_track) {
      printf("pB: "); pB.Print();
      printf("pW: "); pW.Print();
   }
   if (fitx < xwire) {
      return -dist;
   }
   return dist;
}

double Track::GetXFromMinTangent(WireMap& wiremap, XTcurve& xt, int cid)
{
   return GetXFromLine(wiremap, cid, GetMinTangent(wiremap, xt));
}

double Track::GetResidualOfLine(WireMap& wiremap, XTcurve& xt, int cid, Line& line)
{
   Hit& hit = hits_[cid];
   return TMath::Abs(GetXFromLine(wiremap, cid, line)) - hit.GetHitR(xt);
}

double Track::GetResidualOfMinTangent(WireMap& wiremap, XTcurve& xt, int cid)
{
   return GetResidualOfLine(wiremap, xt, cid, GetMinTangent(wiremap, xt));
}

void Track::PrintTrackWithLine(WireMap& wiremap, XTcurve& xt, Line& line)
{
   printf("line's chi2  %lf\n", GetChi2OfLine(wiremap, xt, line));
   Line& min_tangent = GetMinTangent(wiremap, xt);
   for (int cid=0; cid<MAX_LAYER; cid++) {
      Hit& hit = hits_[cid];
      if (!hit.HasHit()) continue;
      int icell = hit.GetCellNumber();
      Line& wire = wiremap.GetWire(cid, icell);
      double fitX = GetXFromLine(wiremap, cid, line);
      double fitR = line.GetDistance(wire);
      double dT = hit.GetDriftTime() - hit.GetT0();
      double hitR = hit.GetHitR(xt);
      printf("cid %2d icell %2d t0 %3.2f drift_time %6.2f dT %6.2f --> hitR %6.2f fitR %6.2f fitX %6.2f hitZ %6.2f fitR-hitR %6.2f\n",
            cid, icell, hit.GetT0(), hit.GetDriftTime(), dT, hitR, fitR, fitX, hit.GetZ(), fitR-hitR);
   }
}

WireMap* g_wiremap_ptr;
XTcurve* g_xt_ptr;
Line* g_fit_line;
void Track::InitFit(WireMap& wiremap, XTcurve& xt, int test_cid, int fit_func_type, bool verbose)
{
   SetFitFuncType(fit_func_type);

   Track_obj = this;
   if (fit_func_type_==FIT_FUNC_TYPE_FIX_T0)  { fit_num_params_ = 4; }
   if (fit_func_type_==FIT_FUNC_TYPE_FREE_T0) { fit_num_params_ = 5; }
   if (minuit_!=NULL) delete minuit_;
   minuit_ = new TFitter(fit_num_params_);

   if (!verbose) {
      Int_t ierr=0;
      gMinuit->SetPrintLevel(-1); 
   //   gMinuit->mnexcm("SET NOWarnings",0,0,ierr);
   }

   if (fit_func_type_ == FIT_FUNC_TYPE_FIX_T0)  { minuit_->SetFCN(Track::MinuitFunction_with_fix_t0); }
   if (fit_func_type_ == FIT_FUNC_TYPE_FREE_T0) { minuit_->SetFCN(Track::MinuitFunction_with_free_t0); }

   SetTestLayerNumber(test_cid);

   g_wiremap_ptr = &wiremap;
   g_xt_ptr = &xt;
   g_fit_line = &fit_line_;
}

void Track::DoFit(WireMap& wiremap, XTcurve& xt)
{
   ini_line_ = GetMinTangent(wiremap, xt);
   ini_line_.GetACDF(ini_pars_[0], ini_pars_[1], ini_pars_[2], ini_pars_[3]);

   double A_ini = ini_pars_[0];
   double C_ini = ini_pars_[1];
   double D_ini = ini_pars_[2];
   double F_ini = ini_pars_[3];
   double A_step = 1.0;
   double C_step = 1.0;
   double D_step = 1.0;
   double F_step = 1.0;
   minuit_->SetParameter(0,"A",A_ini,A_step,-1,1);
   minuit_->SetParameter(1,"C",C_ini,C_step,0,0);
   minuit_->SetParameter(2,"D",D_ini,D_step,-1,1);
   minuit_->SetParameter(3,"F",F_ini,F_step,0,0);

   if (fit_func_type_==FIT_FUNC_TYPE_FREE_T0) {
      // Set initial t0
      double T_ini = 0;
      for (int cid=0; cid<MAX_LAYER; cid++) {
         if (hits_[cid].HasHit()) {
            T_ini = hits_[cid].GetT0();
            break;
         }
      }
      ini_pars_[4] = T_ini;
      double T_step = 0.5;
      minuit_->SetParameter(4,"T",T_ini,T_step,0,0);
   }

   //double arglist[10];
   //arglist[0] = 0;
   //minuit_->ExecuteCommand("SET PRINT",arglist,1);
   minuit_->ExecuteCommand("SIMPLEX",0,0);
   minuit_->ExecuteCommand("MIGRAD",0,0);

   // get result
   for (int i=0; i<fit_num_params_; i++) {
      fit_min_pars_[i] = minuit_->GetParameter(i);
      fit_err_pars_[i] = minuit_->GetParError(i);
   }
   minuit_->GetStats(fit_chi2_,fit_edm_,fit_errdef_,fit_nvpar_,fit_nparx_);

#if 0
   double a,b,c,d,e,f;
   fit_line_.GetABCDEF(a,b,c,d,e,f);
   printf("A) %f %f %f %f %f %f\n", a,b,c,d,e,f);
   double A = fit_min_pars_[0];
   double B = TMath::Sqrt(1-A*A);
   double C = fit_min_pars_[1];
   double D = fit_min_pars_[2];
   double E = TMath::Sqrt(1-D*D);
   double F = fit_min_pars_[3];
   fit_line_.MakeLine(A,B,C,D,E,F);
   printf("B) %f %f %f %f %f %f\n", A,B,C,D,E,F);
   printf("make line after fitting\n");
   fit_line_.PrintLine();
#endif

   // update line (extend line) for drawing purpose
   Hit& hit1 = hits_[1];
   Hit& hit7 = hits_[7];
   Line& wire1 = wiremap.GetWire(1, hit1.GetCellNumber());
   Line& wire7 = wiremap.GetWire(7, hit7.GetCellNumber());
   TVector3 pA1;
   TVector3 pB1;
   TVector3 pA7;
   TVector3 pB7;
   fit_line_.GetClosestPoints(wire1, pA1, pB1);
   fit_line_.GetClosestPoints(wire7, pA7, pB7);
   Line newline;
   newline.MakeLine(pA1, pA7);
   //printf("pA1 "); pA1.Print();
   //printf("pA7 "); pA7.Print();
   //double dx1 = ini_line_.GetP2().X()- ini_line_.GetP1().X();
   //double dy1 = ini_line_.GetP2().Y()- ini_line_.GetP1().Y();
   //double dz1 = ini_line_.GetP2().Z()- ini_line_.GetP1().Z();
   //double dx2 = fit_line_.GetP2().X()- fit_line_.GetP1().X();
   //double dy2 = fit_line_.GetP2().Y()- fit_line_.GetP1().Y();
   //double dz2 = fit_line_.GetP2().Z()- fit_line_.GetP1().Z();
   //printf("dx1 %f dy1 %f dz1 %f dy1/dx1 %f dy1/dx1 %f\n", dx1, dy1, dz1, dy1/dx1, dy1/dz1);
   //printf("dx2 %f dy2 %f dz1 %f dy2/dx2 %f dy1/dx1 %f\n", dx2, dy2, dz2, dy2/dx2, dy2/dz2);
   fit_line_ = newline;

   //printf("comapre with iniline and fitline\n");
   //ini_line_.PrintLine();
   //fit_line_.PrintLine();

   // apply hits
   SetHitZWithLine(wiremap, fit_line_);
}

void Track::PrintFitResults()
{
   char ch = 'A';
   for (int i=0; i<fit_num_params_; i++) {
      if (i==0) ch = 'A';
      if (i==1) ch = 'C';
      if (i==2) ch = 'D';
      if (i==3) ch = 'F';
      if (i==4) ch = 'T';
      printf("[%c] ini_par %8.2f => fit_par %8.2f +/- %8.2f\n", ch, ini_pars_[i], fit_min_pars_[i], fit_err_pars_[i]);
   }
   printf("fit_chi2_ / ndf  = %f (%f / %d)\n", fit_chi2_/GetNDF(), fit_chi2_, GetNDF());
}

int Track::GetNumHits()
{
   int num_hits = 0;
   for (int cid=0; cid<MAX_LAYER; cid++) {
      num_hits += hits_[cid].HasHit();
   }
   return num_hits;
}

int Track::GetNumHitsUseByFit()
{
   int num_hits = 0;
   for (int cid=0; cid<MAX_LAYER; cid++) {
      bool has_hit = hits_[cid].HasHit();
      bool use_by_fit = hits_[cid].UseByFit();
      if (has_hit && use_by_fit) {
         num_hits++;
      }
   }
   return num_hits;
}


int Track::GetTestLayerNumber()
{
   return test_cid_;
}

int Track::GetNumParameters()
{
   // line in 3D space can be described with 4 parameters
   return fit_num_params_;
}

int Track::GetFitFuncType()
{
   return fit_func_type_;
}

int Track::GetNDF()
{
   return GetNumHitsUseByFit() - GetNumParameters();
}

Line& Track::GetInitialLine()
{
   return ini_line_;
}

Line& Track::GetFitLine()
{
   return fit_line_;
}

void Track::SetNumParameters(int num_params)
{
   // line in 3D space can be described with 4 parameters
   fit_num_params_ = num_params;
}

void Track::SetTestLayerNumber(int test_cid)
{
    test_cid_ = test_cid;
    for (int cid=0; cid<MAX_LAYER; cid++) {
       // clear 
       hits_[cid].SetUseByFitFlag(true);
       if (test_cid!=-1) {
          hits_[test_cid_].SetUseByFitFlag(false);
       }
    }
}

void Track::SetFitFuncType(int fit_func_type)
{
   fit_func_type_ = fit_func_type;
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

void Track::MinuitFunction_with_fix_t0(int& nDim, double* gout, double& result, double par[], int flg)
{
   // Line in 3D space is described in the following
   // Ax + By + C = 0; normal to vector (A,B) and distance from origin (x,y)=(0,0) is C/sqrt(A*A+B*B)
   // Dy + Ez + F = 0; normal to vector (D,E) and distance from origin (y,z)=(0,0) is F/sqrt(D*D+E*E)
   // vector (A,B) and (D,E) is normalized
   
   double A = par[0];
   double B = TMath::Sqrt(1-A*A);
   double C = par[1];
   double D = par[2];
   double E = TMath::Sqrt(1-D*D);
   double F = par[3];
   Track_obj->fit_line_.MakeLine(A,B,C,D,E,F);
   result = Track_obj->GetChi2OfLine(*g_wiremap_ptr, *g_xt_ptr, *g_fit_line);
}

void Track::MinuitFunction_with_free_t0(int& nDim, double* gout, double& result, double par[], int flg)
{
   // Line in 3D space is described in the following
   // Ax + By + C = 0; normal to vector (A,B) and distance from origin (x,y)=(0,0) is C/sqrt(A*A+B*B)
   // Dy + Ez + F = 0; normal to vector (D,E) and distance from origin (y,z)=(0,0) is F/sqrt(D*D+E*E)
   // vector (A,B) and (D,E) is normalized
   
   double A = par[0];
   double B = TMath::Sqrt(1-A*A);
   double C = par[1];
   double D = par[2];
   double E = TMath::Sqrt(1-D*D);
   double F = par[3];
   double t0 = par[4];
   for (int cid=0; cid<MAX_LAYER; cid++) {
      Hit& hit = Track_obj->GetHit(cid);
      if (hit.HasHit()) {
         hit.SetT0(t0);
      }
   }
   Track_obj->fit_line_.MakeLine(A,B,C,D,E,F);
   result = Track_obj->GetChi2OfLine(*g_wiremap_ptr, *g_xt_ptr, *g_fit_line);
}

