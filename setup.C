{
   //Run run1("../root/cdcKEK16/input/sub/run2010_000.root");
   Run run1("../root/cdcKEK16/input/sub/run2001_000.root");
   run1.SetVelocity(0.025);
   run1.SetT0(-900, -900);
   run1.SetAdcPeakThreshold(10);
   //run1.DrawBestTangent(9, 1, 7, 10);
   run1.DrawFit(16, 1, 7, 10, 5);
   //run1.DrawFit(64, 1, 7, 10, 5);
   //run1.PrintHits();
}
