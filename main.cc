#include "run.h"

int main(int argc, char** argv)
{
   if (argc!=4) {
      fprintf(stderr, "Usage %s <input.root> <output.root> <max_events>\n", argv[0]);
      return -1;
   }
   char* input_root_path = argv[1];
   char* output_root_path = argv[2];
   int max_events = atoi(argv[3]);
   Run run(input_root_path);
   run.Loop(output_root_path, max_events);

   return 0;
}
