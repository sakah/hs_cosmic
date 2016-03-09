#ifndef TRACK_LIST_H
#define TRACK_LIST_H

class TrackList
{
   public:
      TrackList();
      Track GetTrackOfMinChi2();
      Track& GetTrack(int idx);
      void DrawTrack(int idx);

   private:
      int num_tracks_;
      Track tracks_[MAX_TRACK];
};

#endif


