{
   Run run1("../root/cdcKEK16/input/sub/run2010_000.root");
   run1.SetVelocity(0.025);
   run1.SetT0(-900, -900);
   run1.SetAdcPeakThreshold(10);
   run1.GetEntry(113);
   run1.PrintHits();
}
