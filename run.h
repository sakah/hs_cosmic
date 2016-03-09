#ifndef RUN_H
#define RUN_H

#include "Rtypes.h" // Long64_t
class Config;
class Event;

class Run
{
   public:
      Run(Config* config_ptr, int run_number);
      Event GetNextEvent(bool skip_empty_event);
      Event GetEvent(Long64_t event_number);

   private:
      Config* config_ptr_;
};

#endif
