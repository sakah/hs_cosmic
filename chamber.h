#ifndef _CHAMBER_H
#define _CHAMBER_H

#include "param.h"
#include "event.h"
#include "wiremap.h"
#include "xtcurve.h"
#include "hit.h"
#include "track.h"

#include "TCanvas.h"
#include "TH2F.h"
#include "TMarker.h"
#include "TEllipse.h"

class Chamber
{
   public:
      Chamber();
      void SetAdcPeakThreshold(int adc_thre);
      void GetEvent(Event& event);
      void ClearEvent();
      void ReadWireMap(const char* path, double rot_deg);
      WireMap& GetWireMap();
      int GetNumHitCells(int side, int cid);
      int GetNumHitsInCell(int side, int cid, int icell);
      int GetHitCellNumber(int side, int cid, int icellhit);
      void GetHitCellNumberByMaxAdcPeak(int side, int cid, int& icell, int& ihit);
      Hit& GetHit(int side, int cid, int icell, int ihit);
      int GetNumLayerHits(int side, int cid);
      bool hasAllLayerHits();
      bool isGuardLayer(int side, int cid);
      bool isLayerUsed(int side, int cid);
      void PrintHits(XTcurve& xt, double adc_peak_thre);
      void DrawHits(Event& event, XTcurve& xt);
      void DrawTrack(Event& event, XTcurve& xt, Track& track, Line& line);
      void drawCDCup(Event& event);
      void drawCDCbottom(Event& event);
      void drawTrack(Event& event, XTcurve& xt, Track& track, Line& line);
      void SetT0(int bd, double t0);
      void SetVelocity(double vel);
      double GetT0(int bd);
      double GetT0(int cid, int icell);
      
   private:
      WireMap wiremap_;
      Hit hits_[MAX_SIDE][MAX_LAYER][MAX_CELL][MAX_CELL_HIT];
      double t0_[MAX_BOARD];
      double vel_;
      int adc_thre_; // rough cut on raw adc count. If adc<adc_thre, then corresponding tdc is not considerded as hit
                     // this cut is for adc-ped, not raw adc count. (update 2016/5/27 5:14)
};

#endif
