#ifndef _WIREMAP_H
#define _WIREMAP_H

#include "param.h"
#include "line.h"

class WireMap
{
   public:
      enum {
         SIDE_NONE = -1,
         SIDE_TOP = 0,
         SIDE_BOTTOM = 1
      };
      WireMap();
      void Read(const char* path, double rot_deg);
      double GetZHV(int cid);
      double GetZ0(int cid);
      double GetZRO(int cid);
      int GetNumLayers();
      int GetNumCells(int cid);
      int GetLayerNumber(int ch);
      int GetCellNumber(int ch);
      int GetChanNumber(int cid, int icell);
      int GetBoardNumber(int cid, int icell);
      Line& GetWire(int cid, int icell);
      void Draw();
      int GetLayerSide(int cid, int icell);
      bool isLayerUsed(int side, int cid);
      void PrintMap();

   private:
      double zro_[MAX_LAYER];
      double zhv_[MAX_LAYER];
      double xc_[MAX_CH];
      double yc_[MAX_CH];
      double xro_[MAX_CH];
      double yro_[MAX_CH];
      double xhv_[MAX_CH];
      double yhv_[MAX_CH];

      int num_layers_;
      int num_cells_[MAX_LAYER];
      int layer_numbers_[MAX_CH];
      int cell_numbers_[MAX_CH];
      int chan_numbers_[MAX_LAYER][MAX_CELL];
      int board_numbers_[MAX_LAYER][MAX_CELL];
      Line wires_[MAX_LAYER][MAX_CELL];

      int boardId_[MAX_CH];
      void rotate(double x1, double y1, double &x2, double &y2, double rot_deg);

      int get_marker_color(int ch);
      bool isTopLayer(int boardId);
      bool isBottomLayer(int boardId);
      int GetGlobalChan(int boardId, int chanId);
      int GetBoardIndex(int boardId);

      int layer_side_[MAX_LAYER][MAX_CELL];

};

#endif
