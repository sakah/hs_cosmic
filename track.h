#ifndef TRACK_H
#define TRACK_H

#include "param.h"
#include "hit.h"
#include "wiremap.h"
#include "xtcurve.h"

#include "TMinuit.h"
#include "TFitter.h"

int g_debug_track;

class Track
{
   public:
      Track();
      void SetHit(int cid, Hit& hit);
      void SetSigma(double sigma);
      void SetHitZWithTangent(WireMap& wiremap, int itan);
      void SetHitZWithMinTangent(WireMap& wiremap);
      void MakeTangents(WireMap& wiremap, XTcurve& xt, int cid1, int cid2, double z1, double z2);
      Line& GetTangent(int itan);
      Line& GetMinTangent();
      int GetMinTangentNumber();
      double GetChi2OfLine(WireMap& wiremap, XTcurve& xt, Line& line);
      double GetChi2OfMinTangent(WireMap& wiremap, XTcurve& xt);
      double GetSigma();
      Hit& GetHit(int cid);
      void PrintTangents(WireMap& wiremap, XTcurve& xt);
      void DrawTangents();
      double GetXFromLine(WireMap& wiremap, int cid, Line& line);
      double GetXFromMinTangent(WireMap& wiremap, int cid);
      double GetResidualOfLine(WireMap& wiremap, XTcurve& xt, int cid, Line& line);
      double GetResidualOfMinTangent(WireMap& wiremap, XTcurve& xt, int cid);
      void PrintTrack(WireMap& wiremap, XTcurve& xt);

      void InitFit(WireMap& wiremap, XTcurve& xt);
      void DoFit();
      void PrintFitResults();

   private:
      Hit hits_[MAX_LAYER];
      Line tangents_[4]; // made by two circles 
      int min_itan_;
      double sigma_;
      TFitter* minuit_;
      Line fit_line_;
      double fit_min_pars[4];
      double fit_err_pars[4];
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

      static void MinuitFunction(int& nDim, double* gout, double& result, double par[], int flg);

};

#endif
