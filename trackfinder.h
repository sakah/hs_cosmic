#ifndef TRACK_FINDER_H
#define TRACK_FINDER_H

#include "param.h"
#include "wiremap.h"
#include "track.h"
#include "xtcurve.h"
#include "chamber.h"

class TrackFinder
{
   public:
      TrackFinder(Chamber& chamber, int cid1, int cid2, FindRange range);
      TrackList GetTrackListOfChi2(Range chi2);

      int GetLayerNumber1();
      int GetLayerNumber2();
      FindRange& GetFindRange();

   private:
      int cid1_;
      int cid2_;
      FindRange range_;
      TrackList tracklist_;
};

#endif
