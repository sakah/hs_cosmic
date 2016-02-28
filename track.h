#ifndef TRACK_H
#define TRACK_H

#include "param.h"
#include "hit.h"
#include "wiremap.h"
#include "xtcurve.h"

#include "TMinuit.h"
#include "TFitter.h"

class Track
{
   public:
      enum {
         FIT_FUNC_TYPE_FIX_T0,
         FIT_FUNC_TYPE_FREE_T0,
      };
      Track();
      void SetHit(int cid, Hit& hit);
      void SetSigma(double sigma);
      void SetHitZWithLine(WireMap& wiremap, Line& line);
      void SetHitZWithMinTangent(WireMap& wiremap, XTcurve& xt);
      void MakeTangents(WireMap& wiremap, XTcurve& xt, int cid1, int cid2, double z1, double z2);
      Line& GetTangent(int itan);
      double GetChi2OfLine(WireMap& wiremap, XTcurve& xt, Line& line);
      Line& GetMinTangent(WireMap& wiremap, XTcurve& xt);
      double GetSigma();
      Hit& GetHit(int cid);
      void PrintTangents(WireMap& wiremap, XTcurve& xt);
      void DrawTangents();
      double GetXFromLine(WireMap& wiremap, int cid, Line& line);
      double GetXFromMinTangent(WireMap& wiremap, XTcurve& xt, int cid);
      double GetResidualOfLine(WireMap& wiremap, XTcurve& xt, int cid, Line& line);
      double GetResidualOfMinTangent(WireMap& wiremap, XTcurve& xt, int cid);
      void PrintTrackWithLine(WireMap& wiremap, XTcurve& xt, Line& line);

      void InitFit(WireMap& wiremap, XTcurve& xt, int test_cid, int fit_func_type, bool verbose=true);
      void DoFit(WireMap& wiremap, XTcurve& xt);
      void PrintFitResults();
      int GetNumHits();
      int GetNumHitsUseByFit();
      int GetTestLayerNumber();
      int GetNumParameters();
      int GetFitFuncType();
      int GetNDF();
      Line& GetInitialLine();
      Line& GetFitLine();
      void SetNumParameters(int num_params);
      void SetTestLayerNumber(int test_cid);
      void SetFitFuncType(int fit_func_type);

   private:
      Hit hits_[MAX_LAYER];
      Line tangents_[4]; // made by two circles 

      int test_cid_; // this layers' hit is not included in hits
      double sigma_;
      int fit_func_type_;
      int fit_num_params_;
      TFitter* minuit_;
      Line ini_line_;
      Line fit_line_;
      double ini_pars_[5];
      double fit_min_pars_[5];
      double fit_err_pars_[5];
      double fit_chi2_;
      double fit_edm_;
      double fit_errdef_;
      int fit_nvpar_;
      int fit_nparx_;

      void CalcPointsOnTangentials(
            double x2, double r1, double r2,
            double& xR1,
            double& yR1,
            double& xL1,
            double& yL1,
            double& xR2,
            double& yR2,
            double& xL2,
            double& yL2);

      static void MinuitFunction_with_fix_t0(int& nDim, double* gout, double& result, double par[], int flg);
      static void MinuitFunction_with_free_t0(int& nDim, double* gout, double& result, double par[], int flg);

};

#endif
