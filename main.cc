#include <stdio.h>
#include <stdlib.h>
#include "Rtypes.h"


int main(int argc, char** argv)
{
   if (argc!=7) {
      fprintf(stderr, "Usage %s <top_dir> <config_list.txt> <input.root> <output.root> <start_event> <last_event>\n", argv[0]);
      return -1;
   }
   char* top_dir = argv[1];
   char* config_list_path = argv[2];
   char* input_root_path = argv[3];
   char* output_root_path = argv[4];
   Long64_t start_iev = atoi(argv[5]);
   Long64_t last_iev = atoi(argv[6]);
   //Run run(top_dir, config_list_path, input_root_path, output_root_path);
   //run.Loop(start_iev, last_iev);

   return 0;
}
