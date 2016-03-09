#ifndef _CELL_H
#define _CELL_H

#include "hit.h"

class Cell
{
   public:
      Cell();
      Cell(int cid, int icell);
      void ClearHits();
      void AddHit(Hit& hit);
      int GetLayerNumber();
      int GetCellNumber();
      int GetNumHits();
      int GetHits(Hit** hits);
      void PrintHits();
      void DrawHits();

   private:
      int cid_;
      int icell_;
      int num_hits_;
      Hit hits_[MAX_CELL_HIT];
};

#endif
