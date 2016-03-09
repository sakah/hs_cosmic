#include "configlist.h"

ConfigList::ConfigList(const char* top_dir, const char* config_list_path)
{
   strcpy(top_dir_, top_dir);
   strcpy(config_list_path_, config_list_path);

   FILE* fp = fopen(config_list_path, "r");
   if (fp==NULL) {
      fprintf(stderr, "ERROR: cannot open config_list_file '%s'\n", config_list_path);
      return;
   }
   char line[128];
   int number;
   char config_path[128];
   while(fgets(line, sizeof(line), fp)) {
      sscanf(line, "%d %s", &number, config_path);
      printf("number %d config_path %s\n", number, config_path);

      if (num_configs_>=MAX_CONFIG) {
         printf("Warning: ConfigList reached max %d. Break here.\n", num_configs_);
         break;
      }
      configs_[num_configs_].ReadConfig(config_path);
      num_configs_++;
   }
   fclose(fp);
}

void ConfigList::PrintConfigList()
{
   for (int iconfig=0; iconfig<num_configs_; iconfig++) {
      printf("--- config %d ----\n", iconfig);
      configs_[iconfig].PrintConfig();
   }
}
