{
   TCut cut[9];
   for (int cid=0; cid<9; cid++) {
      TCut cut1 = Form("track_chi2[%d]/track_ndf[%d]<2", cid, cid);
      TCut cut2 = Form("track_hitR[%d][%d]>0 && track_hitR[%d][%d]<8", cid, cid, cid, cid);
      TCut cut3 = Form("track_fitX[%d][%d]>-8 && track_fitX[%d][%d]<8", cid, cid, cid, cid);
      cut[cid] = cut1 && cut2 && cut3;
   }
}
