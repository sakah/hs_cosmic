#ifndef TRACKFINDER_H
#define TRACKFINDER_H

#include "param.h"
#include "wiremap.h"
#include "track.h"
#include "xtcurve.h"
#include "chamber.h"

class TrackFinder
{
   public:
      TrackFinder();
      bool HasFound();
      bool FindBestTrack(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z1, double z2);
      bool FindBestTrack(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z_step);
      Track& GetBestTrack();
      int GetNumTracks();
      void PrintTracks(WireMap& wiremap, XTcurve& xt);
      int MakeTracks(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z1, double z2);
      void SetMaxRadius(double max_r);
      void SetMinZ(double z);
      void SetMaxZ(double z);
      void SetMinZ1(double z);
      void SetMinZ2(double z);
      double GetMaxRadius();
      double GetMinZ();
      double GetMaxZ();
      double GetMinZ1();
      double GetMinZ2();

   private:
      bool has_found_;
      int num_tracks_;
      Track tracks_[MAX_TRACK];
      int min_itrack_;
      double max_r_;
      double min_z_;
      double max_z_;
      double min_z1_;
      double min_z2_;
};

#endif
