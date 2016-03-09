#ifndef WIREMAP_H
#define WIREMAP_H

#include "param.h"
#include "line.h"

class WireMap
{
   public:
      WireMap(const char* wiremap_path);
      double GetZRO();
      double GetZHV();
      int GetNumLayers();
      int GetNumCells(int cid);
      int GetLayerNumber(int ch);
      int GetCellNumber(int ch);
      int GetChanNumber(int cid, int icell);
      int GetBoardNumber(int cid, int icell);
      Line& GetWire(int cid, int icell);

   private:
      double zro_;
      double zhv_;
      int num_layers_;
      int num_cells_[MAX_LAYER];
      int layer_numbers_[MAX_CH];
      int cell_numbers_[MAX_CH];
      int chan_numbers_[MAX_LAYER][MAX_CELL];
      int board_numbers_[MAX_LAYER][MAX_CELL];
      Line wires_[MAX_LAYER][MAX_CELL];

};

#endif
