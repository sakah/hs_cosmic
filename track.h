#ifndef TRACK_H
#define TRACK_H

#include "param.h"
#include "hit.h"
#include "wiremap.h"
#include "xtcurve.h"

int g_debug_track;

class Track
{
   public:
      Track();
      void SetHit(int cid, Hit& hit);
      void SetSigma(double sigma);
      void SetHitZWithTangent(WireMap& wiremap, int itan);
      void SetHitZWithMinTangent(WireMap& wiremap);
      void MakeTangents(WireMap& wiremap, XTcurve& xt, int cid1, int cid2, double z1, double z2);
      Line& GetTangent(int itan);
      Line& GetMinTangent();
      int GetMinTangentNumber();
      double GetChi2OfTangent(WireMap& wiremap, XTcurve& xt, int itan);
      double GetChi2OfMinTangent(WireMap& wiremap, XTcurve& xt);
      double GetSigma();
      Hit& GetHit(int cid);
      void PrintTangents(WireMap& wiremap, XTcurve& xt);
      void DrawTangents();
      double GetXFromLine(WireMap& wiremap, int cid, Line& line);
      double GetXFromMinTangent(WireMap& wiremap, int cid);
      double GetResidualOfLine(WireMap& wiremap, XTcurve& xt, int cid, Line& line);
      double GetResidualOfMinTangent(WireMap& wiremap, XTcurve& xt, int cid);
      void PrintTrack(WireMap& wiremap, XTcurve& xt);

   private:
      Hit hits_[MAX_LAYER];
      Line tangents_[4]; // made by two circles 
      int min_itan_;
      double sigma_;

      void CalcPointsOnTangentials(
            double x2, double r1, double r2,
            double& xR1,
            double& yR1,
            double& xL1,
            double& yL1,
            double& xR2,
            double& yR2,
            double& xL2,
            double& yL2);

};

#endif
