#include "run.h"

int main(int argc, char** argv)
{
   if (argc!=6) {
      fprintf(stderr, "Usage %s <input.root> <output.root> <start_event> <last_event> <run_params.txt>\n", argv[0]);
      return -1;
   }
   char* input_root_path = argv[1];
   char* output_root_path = argv[2];
   Long64_t start_iev = atoi(argv[3]);
   Long64_t last_iev = atoi(argv[4]);
   char* run_params_path = argv[5];
   Run run(input_root_path, output_root_path, run_params_path);
   run.Loop(start_iev, last_iev);

   return 0;
}
