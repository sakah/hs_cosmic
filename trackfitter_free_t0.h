#ifndef TRACK_FITTER_FREE_T0_H
#define TRACK_FITTER_FREE_T0_H

#include "trackfitter.h"

class TrackFitterFreeT0 : public TrackFitter
{
   public:
      TrackFitterFreeT0(Track ini_track, FitRangeFreeT0 range);
      virtual void   DoFit();
      virtual void   GetIniParams(double* ini_params);
      virtual void   GetFitParams(double* fit_params);
      virtual int    GetNumParams();
      virtual double GetChi2();
      virtual double GetNDF();
      virtual double GetRedChi2();

      FitRangeFreeT0 GetFitRangeFreeT0();

   private:
      FitRangeFreeT0 range_;
      static void MinuitFunction_with_free_t0(int& nDim, double* gout, double& result, double par[], int flg);

};

#endif
