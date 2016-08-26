#ifndef _WIREMAP_H
#define _WIREMAP_H

#include "param.h"
#include "line.h"
#include "TMath.h"

class WireMap
{
   public:
      WireMap();
      void Read(const char* path, double rot_deg);
      double GetZRO(int cid);
      double GetZHV(int cid);
      int GetNumLayers();
      int GetNumCells(int cid);
      int GetLayerNumber(int ch);
      int GetCellNumber(int ch);
      int GetChanNumber(int cid, int icell);
      int GetBoardNumber(int cid, int icell);
      Line& GetWire(int cid, int icell);

   private:
      double zro_[MAX_LAYER];
      double zhv_[MAX_LAYER];
      int num_layers_;
      int num_cells_[MAX_LAYER];
      int layer_numbers_[MAX_CH];
      int cell_numbers_[MAX_CH];
      int chan_numbers_[MAX_LAYER][MAX_CELL];
      int board_numbers_[MAX_LAYER][MAX_CELL];
      Line wires_[MAX_LAYER][MAX_CELL];

      void rotate(double x1, double y1, double &x2, double &y2, double rot_deg);
};

#endif
