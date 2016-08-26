#include "wiremap.h"
#include "TMath.h"
#include "TH2F.h"
#include "TMarker.h"

WireMap::WireMap()
{
   for (int ch=0; ch<MAX_CH; ch++) {
      layer_numbers_[ch] = -1;
      cell_numbers_[ch] = -1;
   }
   for (int cid=0; cid<MAX_LAYER; cid++) {
      for (int icell=0; icell<MAX_CELL; icell++) {
         layer_side_[cid][icell] = SIDE_NONE;
      }
   }
}

void WireMap::PrintMap()
{
   //for (int side=0; side<2; side++) {
   //   for (int cid=0; cid<MAX_LAYER; cid++) {
   //      printf("side %d cid %d --> used %d\n", side, cid, isLayerUsed(side, cid));
   //   }
   //}
   for (int cid=0; cid<MAX_LAYER; cid++) {
      for (int icell=0; icell<MAX_CELL; icell++) {
         printf("cid %d icell %d laeyr_side %d\n", cid, icell, layer_side_[cid][icell]);
      }
   }
}

int WireMap::get_marker_color(int boardId)
{
   int col = 0;
   if      (boardId==95) col=kBlack;
   else if (boardId==77) col=kRed;
   else if (boardId==59) col=kGreen;
   else if (boardId==41) col=kBlue;
   else if (boardId==50) col=kGreen;
   else if (boardId==68) col=kMagenta;
   else if (boardId==86) col=kRed;
   else col = 0;
   //printf("ch %d col %d\n", ch, col);
   return col;
}


bool WireMap::isTopLayer(int boardId)
{
   int bid = GetBoardIndex(boardId);
   if (bid>=1 && bid<=4) return true;
   return false;
};

bool WireMap::isBottomLayer(int boardId)
{
   int bid = GetBoardIndex(boardId);
   if (bid>=5 && bid<=7) return true;
   return false;
};

int WireMap::GetGlobalChan(int boardId, int chanId)
{
   int bid = GetBoardIndex(boardId);
   if (bid==-1) return -1;
   return bid*48 + chanId;
};

int WireMap::GetBoardIndex(int boardId)
{
   int bid=-1;
   if      (boardId==95) bid=1;
   else if (boardId==77) bid=2;
   else if (boardId==59) bid=3;
   else if (boardId==41) bid=4;
   else if (boardId==50) bid=5;
   else if (boardId==68) bid=6;
   else if (boardId==86) bid=7;
   return bid;
};

void WireMap::Draw()
{
   double xmin = -1000;
   double xmax =  1000;
   double ymin = -1000;
   double ymax =  1000;
   TH2F* h2 = new TH2F("h1", "", 100, xmin, xmax, 100, ymin, ymax);
   h2->SetStats(0);
   h2->Draw();

   for (int ch=0; ch<MAX_CH; ch++) {
      printf("ch %d boardId %d\n", ch, boardId_[ch]);
      double xc = xc_[ch];
      double yc = yc_[ch];
      TMarker * m = new TMarker(xc, yc, 20);
      m->SetMarkerColor(get_marker_color(boardId_[ch]));
      m->SetMarkerSize(0.2);
      m->Draw();
   }
}

void WireMap::Read(const char* path, double rot_deg)
{
   // PT4
   //zro_ = -299.585;
   //zhv_ = +299.585;

   FILE* fp = fopen(path, "r");
   if (fp==NULL) {
      fprintf(stderr, "ERROR: failed to open %s\n", path);
      exit(1);
   }
   char line[128];
   fgets(line, sizeof(line), fp); // skip header
   int b;
   int ch;
   int l;
   int h;
   int w;
   double xhv;
   double yhv;
   double xc;
   double yc;
   double xro;
   double yro;
   double length;

   num_layers_ = MAX_LAYER;
   for (int cid=0; cid<MAX_LAYER; cid++) {
      num_cells_[cid] = 0;
   }
   int num_cell_used=0;
   while(fgets(line, sizeof(line), fp)) {
      sscanf(line, "%d %d %d %d %d %lf %lf %lf %lf %lf %lf %lf",
            &b, &ch, &l, &h, &w, &xhv, &yhv, &xc, &yc, &xro, &yro, &length);

      //int gch = ch + b*48;
      int boardIndex = GetBoardIndex(b); // board index in ROOT file
      int gch = GetGlobalChan(b, ch);
      if (gch==-1) continue;

      printf("HOGE gch %d l %d w %d\n", gch, l, w);

      layer_numbers_[gch] = l;
      cell_numbers_[gch] = w;
      chan_numbers_[l][w] = gch;
      board_numbers_[l][w] = boardIndex;
      boardId_[gch] = b;
      num_cells_[l]++;
      zro_[l] = length/2.0;
      zhv_[l] = -zro_[l];

      double xro_rot=0;
      double yro_rot=0;
      double xhv_rot=0;
      double yhv_rot=0;
      double xc_rot=0;
      double yc_rot=0;
      rotate(xro, yro, xro_rot, yro_rot, rot_deg);
      rotate(xhv, yhv, xhv_rot, yhv_rot, rot_deg);
      rotate(xc, yc, xc_rot, yc_rot, rot_deg);
      printf("b %d gch %d xc_rot %lf yc_rot %lf\n", b, gch, xc_rot, yc_rot);

      wires_[l][w].MakeLine(TVector3(xro_rot, yro_rot, zro_[l]), TVector3(xhv_rot, yhv_rot, zhv_[l]));

      xc_[gch] = xc_rot;
      yc_[gch] = yc_rot;
      xro_[gch] = xro_rot;
      yro_[gch] = yro_rot;
      xhv_[gch] = xhv_rot;
      yhv_[gch] = yhv_rot;

      int side = SIDE_TOP;
      if (isBottomLayer(b)) side = SIDE_BOTTOM;
      layer_side_[l][w] = side;

      //bool is_used = isLayerUsed(side, l);
      //printf("HOGE b %d ch %d l %d w %d side %d  is_used %d nun_cell_used %d\n", b, ch, l, w, side, is_used, num_cell_used);
      num_cell_used++;

   }
   fclose(fp);
   printf("%d cell used\n", num_cell_used);
}


double WireMap::GetZRO(int cid)
{
   return zro_[cid];
}

double WireMap::GetZHV(int cid)
{
   return zhv_[cid];
}

int WireMap::GetNumLayers()
{
   return num_layers_;
}

double WireMap::GetZ0(int cid)
{
   return (GetZRO(cid)+GetZHV(cid))/2.0;
}

int WireMap::GetNumCells(int cid)
{
   return num_cells_[cid];
}

int WireMap::GetLayerNumber(int ch)
{
   return layer_numbers_[ch];
}

int WireMap::GetCellNumber(int ch)
{
   return cell_numbers_[ch];
}

int WireMap::GetChanNumber(int cid, int icell)
{
   return chan_numbers_[cid][icell];
}

int WireMap::GetBoardNumber(int cid, int icell)
{
   return board_numbers_[cid][icell];
}

Line& WireMap::GetWire(int cid, int icell)
{
   return wires_[cid][icell];
}

void WireMap::rotate(double x1, double y1, double &x2, double &y2, double rot_deg)
{
   double rot_rad = rot_deg/180.0*TMath::Pi();
   x2 = x1*TMath::Cos(rot_rad) - y1*TMath::Sin(rot_rad);
   y2 = x1*TMath::Sin(rot_rad) + y1*TMath::Cos(rot_rad);
}

int WireMap::GetLayerSide(int cid, int icell)
{
   return layer_side_[cid][icell];
}

bool WireMap::isLayerUsed(int side, int cid)
{
   for (int icell=0; icell<MAX_CELL; icell++) {
      if (layer_side_[cid][icell]==side) {
         return true;
      }
   }
   return false;
}
