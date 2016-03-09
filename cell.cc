#include <stdio.h>
#include "cell.h"

Cell::Cell()
{
   cid_ = -1;
   icell_ = -1;
   ClearHits();
}

Cell::Cell(int cid, int icell)
{
   cid_ = cid;
   icell_ = icell;
}

void Cell::ClearHits()
{
   num_hits_ = 0;
}

void Cell::AddHit(Hit& hit)
{
   if (num_hits_>=MAX_CELL_HIT) {
      fprintf(stderr, "Warning Too many hits.. Skip to add hit [Cell::AddHit]\n");
      return;
   }
   hits_[num_hits_] = hit;
   num_hits_++;
}

int Cell::GetLayerNumber()
{
   return cid_;
}

int Cell::GetCellNumber()
{
   return icell_;
}

int Cell::GetNumHits()
{
   return num_hits_;
}

int Cell::GetHits(Hit** hits)
{
   for (int ihit=0; ihit<num_hits_; ihit++) {
      hits[ihit] = &hits_[ihit];
   }
   return num_hits_;
}

void Cell::PrintHits()
{
   for (int ihit=0; ihit<num_hits_; ihit++) {
      hits_[ihit].PrintHit();
   }
}

void Cell::DrawHits()
{
   for (int ihit=0; ihit<num_hits_; ihit++) {
      hits_[ihit].DrawHit();
   }
}
