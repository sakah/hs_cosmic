#include "xtcurve.h"

XTcurve::XTcurve()
{
}

double XTcurve::GetX(double T, int left_or_right)
{
   return 0;
}

double XTcurve::GetT(double X)
{
   return 0;
}

ConstXTcurve::ConstXTcurve()
{
}

void ConstXTcurve::SetDriftVelocity(double drift_velocity)
{
   drift_velocity_ = drift_velocity;
}

double ConstXTcurve::GetDriftVelocity()
{
   return drift_velocity_;
}

double ConstXTcurve::GetX(double T, int left_or_right)
{
   return T * drift_velocity_;
}

double ConstXTcurve::GetT(double X)
{
   return X/drift_velocity_;
}
