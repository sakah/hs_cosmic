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
      Track();
      void SetHit(int side, int cid, Hit& hit);
      void SetSigma(double sigma);
      void SetHitZWithLine(WireMap& wiremap, Line& line);
      void SetHitZWithMinTangent(WireMap& wiremap, XTcurve& xt);
      void MakeTangents(WireMap& wiremap, XTcurve& xt, int side1, int side2, int cid1, int cid2, double z1, double z2);
      Line& GetTangent(int itan);
      double GetChi2OfLine(WireMap& wiremap, XTcurve& xt, Line& line, bool verbose=false);
      Line& GetMinTangent(WireMap& wiremap, XTcurve& xt);
      double GetSigma();
      Hit& GetHit(int side, int cid);
      void PrintTangents(WireMap& wiremap, XTcurve& xt);
      void DrawTangents();
      double GetXFromLine(WireMap& wiremap, int side, int cid, Line& line);
      double GetXFromMinTangent(WireMap& wiremap, XTcurve& xt, int side, int cid);
      double GetResidualOfLine(WireMap& wiremap, XTcurve& xt, int side, int cid, Line& line);
      double GetResidualOfMinTangent(WireMap& wiremap, XTcurve& xt, int side, int cid);
      void PrintTrackWithLine(WireMap& wiremap, XTcurve& xt, Line& line);

      void InitFit(WireMap& wiremap, XTcurve& xt, int test_side, int test_cid, bool verbose=true);
      void DoFit(WireMap& wiremap, XTcurve& xt);
      void PrintFitResults();
      int GetNumHits();
      int GetNumHitsUseByFit();
      int GetNumParameters();
      int GetNDF();
      double GetChi2();
      double GetRedChi2();
      Line& GetInitialLine();
      Line& GetFitLine();
      void SetTestLayerNumber(int test_side, int test_cid);
      int GetTestLayerSide();
      int GetTestLayerNumber();

   private:
      Hit hits_[MAX_SIDE][MAX_LAYER]; // [side][layer]
      Line tangents_[4]; // made by two circles 

      int test_side_;
      int test_cid_; // this layers' hit is not included in hits
      double sigma_;
      TFitter* minuit_;
      Line ini_line_;
      Line fit_line_;
      double ini_pars_[4];
      double fit_min_pars_[4];
      double fit_err_pars_[4];
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
