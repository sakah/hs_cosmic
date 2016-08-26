#include "wiremap.h"

WireMap::WireMap()
{
   for (int ch=0; ch<MAX_CH; ch++) {
      layer_numbers_[ch] = -1;
      cell_numbers_[ch] = -1;
   }
}

void WireMap::Read(const char* path)
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
   while(fgets(line, sizeof(line), fp)) {
      sscanf(line, "%d %d %d %d %d %lf %lf %lf %lf %lf %lf %lf",
            &b, &ch, &l, &h, &w, &xhv, &yhv, &xc, &yc, &xro, &yro, &length);
      int gch = ch + b*48;
      layer_numbers_[gch] = l;
      cell_numbers_[gch] = w;
      chan_numbers_[l][w] = gch;
      board_numbers_[l][w] = b;
      num_cells_[l]++;
      zro_[l] = length/2.0;
      zhv_[l] = -zro_[l];

      wires_[l][w].MakeLine(TVector3(xro, yro, zro_[l]), TVector3(xhv, yhv, zhv_[l]));
   }
   fclose(fp);
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
