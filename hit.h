#ifndef _HIT_H
#define _HIT_H

#include "xtcurve.h"

class Hit
{
   public:
      Hit();
      int GetLayerNumber();
      int GetCellNumber();
      int GetChanNumber();
      int GetLeftRight();
      double GetDriftTime();
      double GetT0();
      double GetZ();
      double GetHitR(XTcurve& xt);
      bool HasHit();

      void SetLayerNumber(int layer_number);
      void SetCellNumber(int cell_number);
      void SetChanNumber(int chan_number);
      void SetLeftRight(int left_or_right);
      void SetDriftTime(double drift_time);
      void SetT0(double t0);
      void SetZ(double z);
      void SetHitFlag(bool has_hit);

      void PrintHit(XTcurve& xt);

   private:
      bool has_hit_;
      int layer_number_;
      int cell_number_;
      int chan_number_;
      int left_or_right_;
      double drift_time_;
      double t0_;
      double z_;
};

#endif


