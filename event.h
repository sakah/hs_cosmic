#ifndef _Event_
#define _Event_

#include "TFile.h"
#include "TTree.h"

#include "param.h"

class Event
{
   public:
      Event();
      Event(const char* path);
      void OpenRootFile(const char* path);
      void GetNext(bool skip_empty_event=true);
      void GetEntry(Long64_t event_number);
      Long64_t GetEntries();
      void PrintEvent();
      Long64_t GetEventNumber();
      int GetTriggerNumber();
      int GetAdc(int ch, int sample);
      int GetQ(int ch);
      int GetTdcNhit(int ch);
      int GetDriftTime(int ch, int ihit);
      int GetClockNumberDriftTime(int ch, int ihit);
      char* GetRootPath();
      double GetPedestal(int ch);

   private:
      TFile* f_;
      TTree* t_;
      char root_path_[128];
      Long64_t event_number_;
      int triggerNumber_;
      int adc_[MAX_CH][MAX_SAMPLE];
      int q_[MAX_CH];
      int tdcNhit_[MAX_CH];
      int driftTime_[MAX_CH][MAX_SAMPLE];
      int clockNumberDriftTime_[MAX_CH][MAX_SAMPLE];
};

#endif

/*
******************************************************************************
*Tree    :tree      : recbe                                                  *
*Entries :  2114226 : Total =     79574976262 bytes  File  Size = 4605366881 *
*        :          : Tree compression factor =  17.28                       *
******************************************************************************
*Br    0 :triggerNumber : triggerNumber/I                                    *
*Entries :  2114226 : Total  Size=    8490330 bytes  File Size  =    2979770 *
*Baskets :      331 : Basket Size=      51200 bytes  Compression=   2.85     *
*............................................................................*
*Br    1 :adc       : adc[96][32]/I                                          *
*Entries :2114226 : Total  Size=25980569420 bytes  File Size  = 3774443052 *
*Baskets :    10667 : Basket Size=    6266880 bytes  Compression=   6.88     *
*............................................................................*
*Br    2 :q         : q[96]/I                                                *
*Entries :  2114226 : Total  Size=  812252646 bytes  File Size  =  387141553 *
*Baskets :     4427 : Basket Size=     195584 bytes  Compression=   2.10     *
*............................................................................*
*Br    3 :tdcNhit   : tdcNhit[96]/I                                          *
*Entries :  2114226 : Total  Size=  812279232 bytes  File Size  =   26714721 *
*Baskets :     4427 : Basket Size=     195584 bytes  Compression=  30.40     *
*............................................................................*
*Br    4 :driftTime : driftTime[96][32]/I                                    *
*Entries :2114226 : Total  Size=25980633446 bytes  File Size  = 209802277 *
*Baskets :    10667 : Basket Size=    6267904 bytes  Compression= 123.83     *
*............................................................................*
*Br    5 :clockNumberDriftTime : clockNumberDriftTime[96][32]/I              *
*Entries :2114226 : Total  Size=25980750827 bytes  File Size  = 203928869 *
*Baskets :    10667 : Basket Size=    6270976 bytes  Compression= 127.40     *
*............................................................................*
*/
