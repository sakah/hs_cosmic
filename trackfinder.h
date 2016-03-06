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
      int SetTracks(Chamber& chamber, XTcurve& xt, bool include_outer_guard_layer);
      void FindBestTrack(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z1, double z2);
      void FindBestTrack(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z_step);
      void MakeTangents(Chamber& chamber, XTcurve& xt, int cid1, int cid2, double z1, double z2);
      Track& GetBestTrack();
      int GetNumTracks();
      void PrintTracks(WireMap& wiremap, XTcurve& xt);
      void SetMaxRadius(double max_r);
      void SetMinZ(double z);
      void SetMaxZ(double z);
      void SetMinZ1(double z);
      void SetMinZ2(double z);
      void SetMaxTracks(int max_tracks);
      double GetMaxRadius();
      double GetMinZ();
      double GetMaxZ();
      double GetMinZ1();
      double GetMinZ2();
      int GetMaxTracks();

   private:
      int num_tracks_;
      Track tracks_[MAX_TRACK];
      int min_itrack_;
      double max_r_;
      double min_z_;
      double max_z_;
      double min_z1_;
      double min_z2_;
      int max_tracks_;

      int SetTracks_with_outer_guard_layer(Chamber& chamber, XTcurve& xt);
      int SetTracks_without_guard_layers(Chamber& chamber, XTcurve& xt);
};

#endif
