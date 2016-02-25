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
      void GetEvent(Event& event);
      void ClearEvent();
      void ReadWireMap(const char* path);
      WireMap& GetWireMap();
      int GetNumHitCells(int cid);
      int GetNumHitsInCell(int cid, int icell);
      int GetHitCellNumber(int cid, int icellhit);
      Hit& GetHit(int cid, int icell, int ihit);
      void PrintHits(XTcurve& xt);
      void DrawHits(Event& event, XTcurve& xt);
      void DrawHitsWithTrack(Event& event, XTcurve& xt, Track& track, int itan);
      void SetT0(int bd, double t0);
      double GetT0(int bd);
      double GetT0(int cid, int icell);
      
   private:
      WireMap wiremap_;
      Hit hits_[MAX_LAYER][MAX_CELL][MAX_CELL_HIT];
      double t0_[MAX_BOARD];
};

#endif
