#ifndef HIT_H
#define HIT_H

#include "wiremap.h"
#include "xtcurve.h"

class Hit
{
   public:
      Hit();
      Hit(WireMap* wiremap_ptr, XTcurve* xtcurve_ptr, int layer_number, int cell_number, int hit_number, int left_or_right, double drift_time, double t0, double q, double z, double fitX);

      void ClearHit();
      int GetLayerNumber();
      int GetCellNumber();
      int GetHitNumber();
      int GetLeftOrRight();
      double GetDriftTime();
      double GetDriftTimeFromT0();
      double GetT0();
      double GetQ();
      double GetZ();
      double GetHitX();
      double GetFitX();
      double GetResX();
      double GetChi2();
      TVector3 GetHitPosInChamber();
      TVector3 GetHitPosInCell();

      void PrintHit();
      void DrawHit();

   private:
      WireMap* wiremap_ptr_;
      XTcurve* xtcurve_ptr_;
      int layer_number_;
      int cell_number_;
      int hit_number_;
      int left_or_right_;
      double drift_time_;
      double t0_;
      double q_;
      double z_;
      double fitX_;
};

#endif
