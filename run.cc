#include "run.h"
#include "config.h"
#include "inputroot.h"
#include "event.h"

Run::Run(Config* config_ptr, int run_number)
{
   config_ptr_ = config_ptr;
   config_ptr_->MakeAll(run_number);
   run_number_ = run_number;
}

Event* Run::GetNextEvent(bool skip_empty_event)
{
   config_ptr_->GetInputROOT_Ptr()->GetNextEntry(skip_empty_event);
   config_ptr_->GetChamber_Ptr()->GetEvent();
   return config_ptr_->GetEvent_Ptr();
}

Event* Run::GetEvent(Long64_t event_number)
{
   config_ptr_->GetInputROOT_Ptr()->GetEntry(event_number);
   config_ptr_->GetChamber_Ptr()->GetEvent();
   return config_ptr_->GetEvent_Ptr();
}

int Run::GetRunNumber()
{
   return run_number_;
}
