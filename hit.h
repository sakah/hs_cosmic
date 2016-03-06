#ifndef _HIT_H
#define _HIT_H

#include "xtcurve.h"

class Hit
{
   public:
      Hit();
      void ClearHit();

      int GetLayerNumber();
      int GetCellNumber();
      int GetChanNumber();
      int GetLeftRight();
      double GetDriftTime();
      double GetDriftTimeFromT0();
      double GetQ();
      double GetT0();
      double GetZ();
      double GetHitR(XTcurve& xt);
      bool HasHit();
      bool UseByFit();

      void SetLayerNumber(int layer_number);
      void SetCellNumber(int cell_number);
      void SetChanNumber(int chan_number);
      void SetLeftRight(int left_or_right);
      void SetDriftTime(double drift_time);
      void SetQ(double q);
      void SetT0(double t0);
      void SetZ(double z);
      void SetHitFlag(bool has_hit);
      void SetUseByFitFlag(bool use_by_fit);

      void PrintHit(XTcurve& xt);

   private:
      bool has_hit_;
      bool use_by_fit_;
      int layer_number_;
      int cell_number_;
      int chan_number_;
      int left_or_right_;
      double drift_time_;
      double q_;
      double t0_;
      double z_;
};

#endif


