#ifndef LAYER_H
#define LAYER_H

#include "cell.h"

class Layer
{
   public:
      Layer();
      Layer(int cid, int num_cells);
      int GetLayerNumber();
      int GetNumCells();
      Cell& GetCell(int icell);
      int GetHitCells(Cell** hit_cells);
      void ClearHits();
      void PrintHits();
      void DrawHits();

   private:
      int cid_;
      int num_cells_;
      Cell cells_[MAX_CELL];
};

#endif
