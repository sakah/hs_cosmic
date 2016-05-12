#include "run.h"

int main(int argc, char** argv)
{
   if (argc!=5) {
      fprintf(stderr, "Usage %s <input.root> <output.root> <start_event> <last_event>\n", argv[0]);
      return -1;
   }
   char* input_root_path = argv[1];
   char* output_root_path = argv[2];
   Long64_t start_iev = atoi(argv[3]);
   Long64_t last_iev = atoi(argv[4]);
   Run run(input_root_path);
run.SetT0(-900, -900);
   run.Loop(output_root_path, start_iev, last_iev);

   return 0;
}
