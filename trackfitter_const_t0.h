#ifndef TRACK_FITTER_CONST_T0_H
#define TRACK_FITTER_CONST_T0_H

#include "trackfitter.h"

class TrackFitterConstT0 : public TrackFitter
{
   public:
      TrackFitterConstT0(Track ini_track, FitRangeConstT0 range);
      virtual void   DoFit();
      virtual void   GetIniParams(double* ini_params);
      virtual void   GetFitParams(double* fit_params);
      virtual int    GetNumParams();
      virtual double GetChi2();
      virtual double GetNDF();
      virtual double GetRedChi2();

      FitRangeConstT0 GetFitRangeConstT0();

   private:
      FitRangeConstT0 range_;
      static void MinuitFunction_with_fix_t0(int& nDim, double* gout, double& result, double par[], int flg);

};

#endif
