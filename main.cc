#include "run.h"

int main(int argc, char** argv)
{
   if (argc!=6) {
      fprintf(stderr, "Usage %s <input.root> <output.root> <start_event> <last_event> <t0>\n", argv[0]);
      return -1;
   }
   char* input_root_path = argv[1];
   char* output_root_path = argv[2];
   Long64_t start_iev = atoi(argv[3]);
   Long64_t last_iev = atoi(argv[4]);
   double t0 = atof(argv[5]);
   Run run(input_root_path);
   //run.SetT0(-900, -900);
   //run.SetT0(-890, -890);
   //run.SetT0(-895, -895);
   run.SetT0(t0, t0);
   run.SetAdcPeakThreshold(250);
   run.Loop(output_root_path, start_iev, last_iev);

   return 0;
}
