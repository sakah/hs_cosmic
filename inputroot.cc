#include "inputroot.h"
#include "TCanvas.h"
#include "TH2F.h"

InputROOT::InputROOT(const char* path)
{
   event_number_ = -1;
   OpenRootFile(path);
}

void InputROOT::OpenRootFile(const char* root_path)
{
   strcpy(root_path_, root_path);
   f_ = new TFile(root_path);
   t_ = (TTree*)f_->Get("tree");
   if (t_==NULL) {
      fprintf(stderr, "ERROR: failed to read tree from %s\n", root_path);
      exit(1);
   }
   t_->SetBranchAddress("triggerNumber", &triggerNumber_);
   t_->SetBranchAddress("adc", adc_);
   t_->SetBranchAddress("q", q_);
   t_->SetBranchAddress("tdcNhit", tdcNhit_);
   t_->SetBranchAddress("driftTime", driftTime_);
   t_->SetBranchAddress("clockNumberDriftTime", clockNumberDriftTime_);
   printf("InputROOT %s is opend\n", root_path);
}

void InputROOT::GetNextEntry(bool skip_empty_event)
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
   printf("event_number %lld num_skip %d\n", event_number_, num_skip);
}

void InputROOT::GetEntry(Long64_t event_number)
{
  event_number_ = event_number;
   t_->GetEntry(event_number);
}

Long64_t InputROOT::GetEntries()
{
   return t_->GetEntries();
}

void InputROOT::PrintEvent()
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

Long64_t InputROOT::GetEventNumber()
{
   return event_number_;
}
int InputROOT::GetTriggerNumber()
{
   return triggerNumber_;
}
int InputROOT::GetAdc(int ch, int sample)
{
   return adc_[ch][sample];
}
int InputROOT::GetQ(int ch)
{
   return q_[ch];
}
int InputROOT::GetTdcNhit(int ch)
{
   return tdcNhit_[ch];
}
int InputROOT::GetDriftTime(int ch, int ihit)
{
   return driftTime_[ch][ihit];
}
int InputROOT::GetClockNumberDriftTime(int ch, int ihit)
{
   return clockNumberDriftTime_[ch][ihit];
}
char* InputROOT::GetRootPath()
{
   return root_path_;
}
void InputROOT::DrawCanvas(const char* canvas_name)
{
   TCanvas*c1 = new TCanvas(canvas_name, canvas_name, 700, 700);
   TH2F* h2 = new TH2F(Form("frame-%s", canvas_name), Form("%s Event# %lld", GetRootPath(), GetEventNumber()), 
         100, -100, 100, 100, 480, 680);
   h2->SetStats(0);
   h2->Draw();
}
