#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configlist.h"

ConfigList::ConfigList()
{
}

ConfigList::ConfigList(const char* top_dir, const char* config_list_path)
{
   strcpy(top_dir_, top_dir);
   strcpy(config_list_path_, config_list_path);

   ReadConfigList(config_list_path);
}

void ConfigList::ReadConfigList(const char* config_list_path)
{
   FILE* fp = fopen(config_list_path, "r");
   if (fp==NULL) {
      fprintf(stderr, "ERROR: cannot open config_list_file '%s'\n", config_list_path);
      return;
   }
   char line[128];
   char config_path[128];
   num_configs_ = 0;
   while(fgets(line, sizeof(line), fp)) {
      sscanf(line, "%s", config_path);
      printf("number %d config_path %s\n", num_configs_, config_path);

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

Config* ConfigList::GetConfig(int config_number)
{
   //return configs_[icon];
   if (num_configs_>=MAX_CONFIG) {
      fprintf(stderr, "Warning: num_configs reach max %d. New config is not genrated.\n", MAX_CONFIG);
      return NULL;
   }
   for (int icon=0; icon<num_configs_; icon++) {
      int confnum = runs_[icon]->GetConfigNumber();
      //printf("icon %d config_number %d\n", icon, config_number);
      if (config_number == confnum) {
         return configs_[icon];
      }
   }

   configs_[num_configs_] = new Config(this, run_number);
   return configs_[num_configs_++];
}
