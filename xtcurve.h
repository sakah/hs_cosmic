#ifndef _XTCURVE_H
#define _XTCURVE_H

class XTcurve
{
   public:
      enum { LEFT, RIGHT};
      XTcurve();
      virtual double GetX(double T, int left_or_right);
      virtual double GetT(double X);

   private:

};

class ConstXTcurve : public XTcurve
{
   public:
      ConstXTcurve();
      void SetDriftVelocity(double drift_velocity);
      double GetDriftVelocity();
      virtual double GetX(double T, int left_or_right);
      virtual double GetT(double X);

   private:
      double drift_velocity_; // mm/ns
};

#endif
