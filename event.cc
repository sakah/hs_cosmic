#include "event.h"

Event::Event()
{
   event_number_ = -1;
}

Event::Event(const char* path)
{
   event_number_ = -1;
   OpenRootFile(path);
}

void Event::OpenRootFile(const char* path)
{
   strcpy(root_path_, path);
   f_ = new TFile(path);
   t_ = (TTree*)f_->Get("tree");
   if (t_==NULL) {
      fprintf(stderr, "ERROR: failed to read tree %s\n", path);
      exit(1);
   }
   t_->SetBranchAddress("triggerNumber", &triggerNumber_);
   t_->SetBranchAddress("adc", adc_);
   t_->SetBranchAddress("q", q_);
   t_->SetBranchAddress("tdcNhit", tdcNhit_);
   t_->SetBranchAddress("driftTime", driftTime_);
   t_->SetBranchAddress("clockNumberDriftTime", clockNumberDriftTime_);
}

void Event::GetNext(bool skip_empty_event)
{
   int num_skip=0;
   for(Long64_t iev = event_number_+1; iev<GetEntries(); iev++) {
      GetEntry(iev);
      event_number_ = iev;
      if (!skip_empty_event) break;

      bool empty_event=true;
      for (int ch=0; ch<MAX_CH; ch++) {
         if (tdcNhit_[ch]!=0) {
            empty_event=false;
            break;
         }
      }
      if (!empty_event) break;
      num_skip++;
   }
   printf("num_skip %d event_number %lld\n", num_skip, event_number_);
}

void Event::GetEntry(Long64_t event_number)
{
  event_number_ = event_number;
   t_->GetEntry(event_number);
}

Long64_t Event::GetEntries()
{
   return t_->GetEntries();
}

void Event::PrintEvent()
{
   printf("iev %lld\n", event_number_);
   printf("triggerNumber %d\n", triggerNumber_);
   for (int ch=0; ch<96; ch++) {
      printf("ch %d TdcNhit %d q %d\n", ch, tdcNhit_[ch], q_[ch]);
      //for (int sample=0; sample<32; sample++) {
      //   printf("ch %d sample %d adc %d\n", ch, sample, adc_[ch][sample]);
      //}
      //for (int ihit=0; ihit<GetTdcNhit(ch); ihit++) {
      //   printf("ch %d ihit %d driftTime %d clockNumberDriftTime %d\n", ch, ihit, driftTime_[ch][ihit], clockNumberDriftTime_[ch][ihit]);
      //}
   }
}

Long64_t Event::GetEventNumber()
{
   return event_number_;
}
int Event::GetTriggerNumber()
{
   return triggerNumber_;
}
int Event::GetAdc(int ch, int sample)
{
   return adc_[ch][sample];
}
int Event::GetQ(int ch)
{
   return q_[ch];
}
int Event::GetTdcNhit(int ch)
{
   return tdcNhit_[ch];
}
int Event::GetDriftTime(int ch, int ihit)
{
   return driftTime_[ch][ihit];
}
int Event::GetClockNumberDriftTime(int ch, int ihit)
{
   return clockNumberDriftTime_[ch][ihit];
}
char* Event::GetRootPath()
{
   return root_path_;
}
void Event::SetT0(double t0)
{
   t0_ = t0;
}
double Event::GetT0()
{
   return t0_;
}
