#include <stdio.h>
#include "hit.h"
#include "TVector3.h"
#include "TMarker.h"
#include "TEllipse.h"

Hit::Hit()
{
   ClearHit();
}

Hit::Hit(WireMap* wiremap_ptr, XTcurve* xtcurve_ptr, int layer_number, int cell_number, int hit_number, int left_or_right, double drift_time, double t0, double q, double z, double fitX)
{
   wiremap_ptr_ = wiremap_ptr;
   xtcurve_ptr_ = xtcurve_ptr;
   layer_number_ = layer_number;
   cell_number_ = cell_number;
   hit_number_ = hit_number;
   left_or_right_ = left_or_right;
   drift_time_ = drift_time;
   t0_ = t0;
   q_ = q;
   z_ = z;
   fitX_ = fitX;
}

void Hit::ClearHit()
{
   layer_number_ = -1;
   cell_number_ = -1;
   hit_number_ = -1;
   left_or_right_ = XTcurve::RIGHT;
   drift_time_ = -1e10;
   t0_ = -1e10;
   q_ = -1e10;
   z_ = -1e10;
   fitX_ = -1e10;
}

int Hit::GetLayerNumber()
{
   return layer_number_;
}

int Hit::GetCellNumber()
{
   return cell_number_;
}

int Hit::GetHitNumber()
{
   return hit_number_;
}

int Hit::GetLeftOrRight()
{
   return left_or_right_;
}

double Hit::GetDriftTime()
{
   return drift_time_;
}

double Hit::GetDriftTimeFromT0()
{
   return drift_time_ - t0_;
}

double Hit::GetT0()
{
   return t0_;
}

double Hit::GetQ()
{
   return q_;
}

double Hit::GetZ()
{
   return z_;
}

double Hit::GetHitX()
{
   return xtcurve_ptr_->GetX(layer_number_, drift_time_ - t0_, left_or_right_);
}

double Hit::GetFitX()
{
   return fitX_;
}

double Hit::GetResX()
{
   return fitX_ - GetHitX();
}

double Hit::GetChi2()
{
   double sigma_r = xtcurve_ptr_->GetSigmaR(layer_number_, GetHitX());
   double dx = GetResX()/sigma_r;
   return dx*dx;
}

void Hit::PrintHit()
{
   printf("cid %2d icell %2d left_or_right %d t0 %3.2f drift_time %6.2f dT %6.2f --> hitX %6.2f fitX %6.2f resX %6.2f chi2 %6.2f hitZ %6.2f\n", 
         layer_number_, cell_number_, left_or_right_, t0_, drift_time_, GetDriftTimeFromT0(), GetHitX(), GetFitX(), GetResX(), GetChi2(), z_);
}

void Hit::DrawHit()
{
   TVector3 pos = wiremap_ptr_->GetWire(layer_number_, cell_number_).GetPosAtZ(z_);
   // draw wire center
   TMarker* m = new TMarker(pos.X(), pos.Y(), 20);
   m->SetMarkerSize(0.3);
   m->Draw();

   if (layer_number_==-1) {
      return;
   }

   // draw drift circle
   TEllipse* e = new TEllipse(pos.X(), pos.Y(), TMath::Abs(GetHitX()));
   if (hit_number_==0) {
      e->SetLineColor(kBlue);
   } else {
      e->SetLineColor(kRed);
   }
   e->SetFillStyle(4000);
   e->Draw();
}

