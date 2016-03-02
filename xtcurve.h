#ifndef _XTCURVE_H
#define _XTCURVE_H

#include "param.h"
#include "config.h"
#include "TF1.h"

class XTcurve
{
   public:
      enum { USE_CONST_XT, USE_PARAM_POL4 };
      enum { LEFT, RIGHT };
      XTcurve();
      virtual void Setup(Config& config);
      virtual double GetR(int cid, double T, int left_or_right);
      virtual double GetT(int cid, double X);
};

class ConstXTcurve : public XTcurve
{
   public:
      ConstXTcurve();
      virtual void Setup(Config& config);
      virtual double GetR(int cid, double T, int left_or_right);
      virtual double GetT(int cid, double X);
      void SetDriftVelocity(double drift_velocity);
      double GetDriftVelocity();
      void DrawXTcurve(int cid);

   private:
      double drift_velocity_; // mm/ns
};

class XTcurvePol4 : public XTcurve
{
   public:
      XTcurvePol4();
      virtual void Setup(Config& config);
      virtual double GetR(int cid, double T, int left_or_right);
      virtual double GetT(int cid, double X);
      virtual void DrawXTcurve(int cid);
      void DrawXTcurve2(int cid);
      void DrawXTcurveDiff(int cid);

   private:
      char params_txt_path[512];
      TF1* pol4_left_[MAX_LAYER];
      TF1* pol4_left_inverse_x_[MAX_LAYER];
      TF1* pol4_right_[MAX_LAYER];
      double tx_left_[MAX_LAYER][300];  // Divide 0 - 300 ns in 1ns step
      double tx_right_[MAX_LAYER][300]; // Divide 0 - 300 ns in 1ns step
};

#endif
