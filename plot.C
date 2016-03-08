void xt(int cid)
{
   char var[128];
   sprintf(var, "track_hitR[%d][%d]:track_fitX[%d][%d]", cid, cid, cid, cid);
   t->Draw(var, cut[cid]);
}
void resR(int cid)
{
   char var[128];
   sprintf(var, "track_resR_%d_%d", cid, cid);
   t->Draw(var, cut[cid]);
}
void resR_vs_fitX(int cid, double ymin, double ymax, char* opt)
{
   char var[128];
   sprintf(var, "track_resR_%d_%d:track_fitX[%d][%d]", cid, cid, cid, cid);
   TCut yrange = Form("track_resR_%d_%d>=%f && track_resR_%d_%d<=%f", cid, cid, ymin, cid, cid, ymax); 
   t->Draw(var, cut[cid] && yrange, opt);
}
