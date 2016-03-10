#ifndef CHAMBER_H
#define CHAMBER_H

#include "param.h"
#include "layer.h"

class Config;
class WireMap;
class XTcurve;
class InputROOT;

class Chamber
{
   public:
      Chamber(Config* config_ptr);
      void ClearHits();
      void GetEvent();
      Layer& GetLayer(int cid);
      int GetHitLayers(Layer** hit_layers);
      void PrintHits();
      void DrawHits();
      int GetNumBoards();
      
   private:
      WireMap* wiremap_ptr_;
      XTcurve* xtcurve_ptr_;
      InputROOT* input_root_ptr_;
      int num_boards_;
      double t0_boards_[MAX_BOARD];

      Layer layers_[MAX_LAYER];
};

#endif
