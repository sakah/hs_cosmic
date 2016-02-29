#include <stdio.h>
#include <stdlib.h>

#include "TCanvas.h"
#include "TLine.h"
#include "TH2F.h"
#include "TGraph.h"
#include "xtcurve.h"

//__________________________________________________
XTcurve::XTcurve()
{
}

double XTcurve::GetR(int cid, double T, int left_or_right)
{
   return 0;
}

double XTcurve::GetT(int cid, double X)
{
   return 0;
}

void XTcurve::DrawXTcurve(int cid)
{
   // do nothing
}

//__________________________________________________
ConstXTcurve::ConstXTcurve()
{
}

void ConstXTcurve::SetDriftVelocity(double drift_velocity)
{
   drift_velocity_ = drift_velocity;
}

double ConstXTcurve::GetDriftVelocity()
{
   return drift_velocity_;
}

double ConstXTcurve::GetR(int cid, double T, int left_or_right)
{
   return T * drift_velocity_;
}

double ConstXTcurve::GetT(int cid, double X)
{
   return X/drift_velocity_;
}

void ConstXTcurve::DrawXTcurve(int cid)
{
   double x1 = -8.0;
   double x2 =  8.0;
   double y1 = GetT(cid, x1);
   double y2 = GetT(cid, x2);
   TLine* l1 = new TLine(0, 0, x1, y1);
   TLine* l2 = new TLine(0, 0, x2, y2);
   l1->Draw();
   l2->Draw();
}

//__________________________________________________
XTcurvePol4::XTcurvePol4()
{
   for (int cid=0; cid<MAX_LAYER; cid++) {
      pol4_left_[cid] = NULL;
      pol4_left_inverse_x_[cid] = NULL;
      pol4_right_[cid] = NULL;
   }
}

void XTcurvePol4::ReadParametersTextFile(const char* path)
{
   FILE* fp = fopen(path, "r");
   if (fp==NULL) {
      fprintf(stderr, "ERROR: cannot open file %s\n", path);
      exit(1);
   }
   char line[128];
   int cid;
   int left_or_right;
   double pol0;
   double pol1;
   double pol2;
   double pol3;
   double pol4;
   while(fgets(line, sizeof(line), fp)) {
      sscanf(line, "%d %d %lf %lf %lf %lf %lf", &cid, &left_or_right, &pol0, &pol1, &pol2, &pol3, &pol4);
      printf("%d %d %lf %lf %lf %lf %lf\n", cid, left_or_right, pol0, pol1, pol2, pol3, pol4);
      if (left_or_right == XTcurve::LEFT) {
         pol4_left_[cid] = new TF1(Form("pol4_left_%d", cid), "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x", -8, 0);
         pol4_left_[cid]->SetParameters(pol0, pol1, pol2, pol3, pol4);
         pol4_left_[cid]->SetNpx(1000);
         pol4_left_[cid]->SetLineColor(kBlue);

         pol4_left_inverse_x_[cid] = new TF1(Form("pol4_left_inverse_x_%d", cid), "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x", 0, 8);
         pol4_left_inverse_x_[cid]->SetParameters(pol0, -pol1, pol2, -pol3, pol4); // x -> -x
         pol4_left_inverse_x_[cid]->SetNpx(1000);
         pol4_left_inverse_x_[cid]->SetLineColor(kBlue);

      } else {
         pol4_right_[cid] = new TF1(Form("pol4_right_%d", cid), "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x", 0, 8);
         pol4_right_[cid]->SetParameters(pol0, pol1, pol2, pol3, pol4);
         pol4_right_[cid]->SetNpx(1000);
         pol4_right_[cid]->SetLineColor(kRed);
      }
   }
   fclose(fp);
}

double XTcurvePol4::GetR(int cid, double T, int left_or_right)
{
   double r = -1e10;
   if (left_or_right==XTcurve::LEFT) {
      if (pol4_left_[cid]) {
         r = pol4_left_[cid]->GetX(T);
      }
   }
   if (pol4_right_[cid]) {
      r = pol4_right_[cid]->GetX(T);
   }
   return r;
}

double XTcurvePol4::GetT(int cid, double X)
{
   double t = -1e10;
   if (X<0) {
      if (pol4_left_[cid]) {
         t = pol4_left_[cid]->Eval(X);
      }
   }
   if (pol4_right_[cid]) {
      t = pol4_right_[cid]->Eval(X);
   }
   return t;
}

void XTcurvePol4::DrawXTcurve(int cid)
{
   if (pol4_left_[cid]) pol4_left_[cid]->Draw("same");
   if (pol4_right_[cid]) pol4_right_[cid]->Draw("same");
}

void XTcurvePol4::DrawXTcurve2(int cid)
{
   if (pol4_left_[cid]==NULL) return;

   TCanvas* c1 = new TCanvas("c1-drawxtcurve2");
   c1->SetGrid();
   TH2F *h2 = new TH2F(Form("drawxtcurve2-%d", cid), "", 100, 0, 10, 100, 0, 500);
   h2->SetTitle(Form("LayerNumber %d; R [mm]; T [ns]", cid));
   h2->SetStats(0);
   h2->Draw();
   if (pol4_left_inverse_x_[cid]) pol4_left_inverse_x_[cid]->Draw("same");
   if (pol4_right_[cid]) pol4_right_[cid]->Draw("same");
}

void XTcurvePol4::DrawXTcurveDiff(int cid)
{
   if (pol4_left_[cid]==NULL) return;

   const int num = 1000;
   double x[num];
   double y[num];
   double step = 8.0/num;
   for (int ix=0; ix<num; ix++) {
      x[ix] = ix*step;
      y[ix] = pol4_right_[cid]->Eval(x[ix]) - pol4_left_inverse_x_[cid]->Eval(x[ix]);
   }
   TGraph* gr = new TGraph(num, x, y);
   gr->SetLineColor(kRed);
   gr->SetMarkerColor(kRed);

   TCanvas* c1 = new TCanvas("c1-drawxtxurvediff");
   c1->SetGrid();
   TH2F *h2 = new TH2F(Form("drawxtcurvediff-%d", cid), "", 100, 0, 10, 100, -20, 20);
   h2->SetTitle(Form("LayerNumber %d; R [mm]; T_right - T_left [ns]", cid));
   h2->SetStats(0);
   h2->Draw();
   gr->Draw("pl same");
}

