#ifndef TRACK_H
#define TRACK_H

#include "param.h"
#include "hit.h"
#include "wiremap.h"
#include "xtcurve.h"

class Track
{
   public:
      Track(Line line, HitList hit_list);

      Line& GetLine();
      HitList& GetHistList();
      void PrintTrack();
      void DrawTrack();
      double GetXAtLayer(int cid);
      double GetChi2();

   private:
      Line line_;
      HitList hit_list_;


};

#endif
