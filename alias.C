{
   for (int cid=0; cid<9; cid++) {
      for (int cid=0; cid<9; cid++) {
         t->SetAlias(Form("track_resR_%d_%d",cid,cid), 
               Form("abs(track_fitX[%d][%d])-track_hitR[%d][%d]", cid,cid,cid,cid));
         t->SetAlias(Form("track_hitdT_%d_%d",cid,cid), 
               Form("track_hitT[%d][%d]-track_fitT0[%d][%d]", cid,cid,cid,cid));
      }
   }
}
