#include "hit.h"
#include <stdio.h>

Hit::Hit()
{
   hit_number_ = -1;
   layer_number_ = -1;
   cell_number_ = -1;
   chan_number_ = -1;
   left_or_right_ = XTcurve::LEFT;
   drift_time_ = 0;
   q_ = 0;
   t0_ = -840;
   z_ = 0;
   has_hit_ = false;
   use_by_fit_ = true;
}

int Hit::GetHitNumber()
{
   return hit_number_;
}

int Hit::GetLayerNumber()
{
   return layer_number_;
}

int Hit::GetCellNumber()
{
   return cell_number_;
}

int Hit::GetChanNumber()
{
   return chan_number_;
}

int Hit::GetLeftRight()
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

double Hit::GetQ()
{
   return q_;
}

int Hit::GetAdc()
{
   return adc_;
}

double Hit::GetPedestal()
{
   return ped_;;
}

double Hit::GetT0()
{
   return t0_;
}

double Hit::GetZ()
{
   return z_;
}

bool Hit::HasHit()
{
   return has_hit_;
}

bool Hit::UseByFit()
{
   return use_by_fit_;
}

double Hit::GetHitR(XTcurve& xt)
{
   return xt.GetX(drift_time_ - t0_, left_or_right_);
}

void Hit::SetHitNumber(int hit_number)
{
   hit_number_ = hit_number;
}

void Hit::SetLayerNumber(int layer_number)
{
   layer_number_ = layer_number;
}

void Hit::SetCellNumber(int cell_number)
{
   cell_number_ = cell_number;
}

void Hit::SetChanNumber(int chan_number)
{
   chan_number_ = chan_number;
}

void Hit::SetLeftRight(int left_or_right)
{
   left_or_right_ = left_or_right;
}

void Hit::SetDriftTime(double drift_time)
{
   drift_time_ = drift_time;
}

void Hit::SetQ(double q)
{
   q_ = q;
}

void Hit::SetAdc(int adc)
{
   adc_ = adc;
}

void Hit::SetPedestal(double ped)
{
   ped_ = ped;
}

void Hit::SetT0(double t0)
{
   t0_ = t0;
}

void Hit::SetZ(double z)
{
   z_ = z;
}

void Hit::SetHitFlag(bool has_hit)
{
   has_hit_ = has_hit;
}

void Hit::SetUseByFitFlag(bool use_by_fit)
{
   use_by_fit_ = use_by_fit;
}

void Hit::PrintHit(XTcurve& xt)
{
   double dT = drift_time_- t0_;
   double hitR = xt.GetX(dT, left_or_right_);
   printf("cid %2d icell %2d ped %5.2f adc %4d q %5.1f adchit-ped %5.2f t0 %3.2f drift_time %6.2f dT %6.2f --> hitR %6.2f hitZ %6.2f\n", 
         layer_number_, cell_number_, ped_, adc_, q_, adc_-ped_, t0_, drift_time_, dT, hitR, z_);
}

