#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TCanvas.h"
#include "TLine.h"
#include "TH2F.h"
#include "TGraph.h"

#include "config.h"
#include "xtcurve.h"

//__________________________________________________
XTcurve::XTcurve()
{
}

int XTcurve::GetXTcurveType(const char* xt_curve_name)
{
   int xt_curve_type=-1;
   if (strcmp(xt_curve_name, "XT_CONST")==0) xt_curve_type = XT_CONST;
   else if (strcmp(xt_curve_name, "XT_PARAM_POL4")==0) xt_curve_type = XT_PARAM_POL4;
   else {
      fprintf(stderr, "ERROR: XTcurve unkonwn xt_curve_name '%s'\n", xt_curve_name);
   }
   return xt_curve_type;
}
double XTcurve::GetX(int cid, double T, int left_or_right)
{
   return 0;
}

double XTcurve::GetT(int cid, double X)
{
   return 0;
}

double XTcurve::GetSigmaR(int cid, double X)
{
   return 0;
}

//__________________________________________________
XTcurveConst::XTcurveConst(double drift_velocity, double sigma_r)
{
   drift_velocity_ = drift_velocity;
   sigma_r_ = sigma_r;
}

double XTcurveConst::GetX(int cid, double T, int left_or_right)
{
   int sign=1;
   if (left_or_right==XTcurve::LEFT) {
      sign = -1;
   }
   return sign * T * drift_velocity_;
}

double XTcurveConst::GetT(int cid, double X)
{
   return X/drift_velocity_;
}

double XTcurveConst::GetSigmaR(int cid, double X)
{
   return sigma_r_;
}

double XTcurveConst::GetDriftVelocity(int cid)
{
   return drift_velocity_;
}

void XTcurveConst::DrawXTcurve(int cid)
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
XTcurvePol4::XTcurvePol4(const char* xt_param_path, double sigma_r)
{
   for (int cid=0; cid<MAX_LAYER; cid++) {
      pol4_left_[cid] = NULL;
      pol4_left_inverse_x_[cid] = NULL;
      pol4_right_[cid] = NULL;
   }

   printf("XTcurvePol4::XTcurvePol4. xt_param_path %s\n", xt_param_path);
   if (strcmp(xt_param_path, "NOT_USED")==0) {
      fprintf(stderr, "xt_param_path is not set. exit.\n");
      exit(1);
   }
   sigma_r_ = sigma_r;

   FILE* fp = fopen(xt_param_path, "r");
   if (fp==NULL) {
      fprintf(stderr, "ERROR: cannot open file %s\n", xt_param_path);
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

         for (int ns=0; ns<300; ns++) {
            tx_left_[cid][ns] = pol4_left_[cid]->GetX(ns);
         }

      } else {
         pol4_right_[cid] = new TF1(Form("pol4_right_%d", cid), "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x", 0, 8);
         pol4_right_[cid]->SetParameters(pol0, pol1, pol2, pol3, pol4);
         pol4_right_[cid]->SetNpx(1000);
         pol4_right_[cid]->SetLineColor(kRed);

         for (int ns=0; ns<300; ns++) {
            tx_right_[cid][ns] = pol4_right_[cid]->GetX(ns);
         }
      }
   }
   fclose(fp);
}

double XTcurvePol4::GetX(int cid, double T, int left_or_right)
{
   double x = -1e10;
   int ns = T;
   if (left_or_right==XTcurve::LEFT) {
      if (T<0) x = 0;
      else if (T>=300) x = -tx_left_[cid][299];
      else x = -tx_left_[cid][ns];
   } else {
      if (T<0) x = 0;
      else if (T>=300) x = tx_right_[cid][299];
      else x = tx_right_[cid][ns];
   }
   return x;
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

double XTcurvePol4::GetSigmaR(int cid, double X)
{
   return sigma_r_;
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

