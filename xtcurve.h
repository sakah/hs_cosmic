#ifndef XTCURVE_H
#define XTCURVE_H

#include "param.h"
#include "TF1.h"

class XTcurve
{
   public:
      enum { USE_CONST_XT=0, USE_PARAM_POL4 };
      enum { LEFT=0, RIGHT };
      XTcurve();
      virtual double GetX(int cid, double T, int left_or_right);
      virtual double GetT(int cid, double X);
      virtual double GetSigmaR(int cid, double X);
      int GetXTcurveType(const char* xt_curve_name);
};

class XTcurveConst : public XTcurve
{
   public:
      XTcurveConst(double drift_velocity, double sigma_r);
      virtual double GetX(int cid, double T, int left_or_right);
      virtual double GetT(int cid, double X);
      virtual double GetSigmaR(int cid, double X);
      double GetDriftVelocity(int cid);
      void DrawXTcurve(int cid);

   private:
      double drift_velocity_; // mm/ns
      double sigma_r_;
};

class XTcurvePol4 : public XTcurve
{
   public:
      XTcurvePol4(const char* xt_param_path, double sigma_r);
      virtual double GetX(int cid, double T, int left_or_right);
      virtual double GetT(int cid, double X);
      virtual double GetSigmaR(int cid, double X);
      void DrawXTcurve(int cid);
      void DrawXTcurve2(int cid);
      void DrawXTcurveDiff(int cid);

   private:
      char xt_param_path_[512];
      double sigma_r_;

      TF1* pol4_left_[MAX_LAYER];
      TF1* pol4_left_inverse_x_[MAX_LAYER];
      TF1* pol4_right_[MAX_LAYER];
      double tx_left_[MAX_LAYER][300];  // Divide 0 - 300 ns in 1ns step
      double tx_right_[MAX_LAYER][300]; // Divide 0 - 300 ns in 1ns step
};

#endif
