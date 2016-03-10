#ifndef CONFIG_LIST_H
#define CONFIG_LIST_H

#include "config.h"

class ConfigList
{
   public:
      ConfigList();
      ConfigList(const char* top_dir, const char* config_list_path);
      void ReadConfigList(const char* config_list_path);
      void PrintConfigList();
      Config* GetConfig(int icon);

   private:
      char top_dir_[128]; // depends on working directory
      char config_list_path_[128]; // relative path wrt top_dir
      int num_configs_;
      Config configs_[MAX_CONFIG];
};

#endif
