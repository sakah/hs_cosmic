void c1-chamber-drawhitswithtrack()
{
//=========Macro generated from canvas: c1-chamber-drawhitswithtrack/
//=========  (Sun Aug 28 09:09:33 2016) by ROOT version6.06/04
   TCanvas *c1-chamber-drawhitswithtrack = new TCanvas("c1-chamber-drawhitswithtrack", "",0,45,500,1031);
   c1-chamber-drawhitswithtrack->Range(0,0,1,1);
   c1-chamber-drawhitswithtrack->SetFillColor(0);
   c1-chamber-drawhitswithtrack->SetBorderMode(0);
   c1-chamber-drawhitswithtrack->SetBorderSize(2);
   c1-chamber-drawhitswithtrack->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: p1
   TPad *p1 = new TPad("p1", "p1",0,0.8,1,1);
   p1->Draw();
   p1->cd();
   p1->Range(0,0,1,1);
   p1->SetFillColor(0);
   p1->SetBorderMode(0);
   p1->SetBorderSize(2);
   p1->SetFrameBorderMode(0);
   
   TPaveText *pt = new TPaveText(0,0,1,1,"br");
   TText *AText = pt->AddText("test/run_000052.root");
   AText = pt->AddText("Event# 1677");
   AText = pt->AddText("Trigger# 1313223");
   AText = pt->AddText("Test layer 15 (up)");
   AText = pt->AddText("chi2/NDF= 21097.14/14 (1506.94)");
   AText = pt->AddText("S1: x=   8.63 , y= 950.00 , z= -108.46 [mm]");
   AText = pt->AddText("S2: x=  10.32 , y= -950.00 , z= -31.33 [mm]");
   pt->Draw();
   p1->Modified();
   c1-chamber-drawhitswithtrack->cd();
  
// ------------>Primitives in pad: p2
   TPad *p2 = new TPad("p2", "p2",0,0,1,0.8);
   p2->Draw();
   p2->cd();
   p2->Range(0,0,1,1);
   p2->SetFillColor(0);
   p2->SetBorderMode(0);
   p2->SetBorderSize(2);
   p2->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: p2_1
   TPad *p2_1 = new TPad("p2_1", "p2_1",0.01,0.01,0.49,0.99);
   p2_1->Draw();
   p2_1->cd();
   p2_1->Range(-62.5,568.75,62.5,881.25);
   p2_1->SetFillColor(0);
   p2_1->SetBorderMode(0);
   p2_1->SetBorderSize(2);
   p2_1->SetFrameBorderMode(0);
   p2_1->SetFrameBorderMode(0);
   
   TH2F *h2mIup__1 = new TH2F("h2mIup__1","",100,-50,50,100,600,850);
   h2mIup__1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   h2mIup__1->SetLineColor(ci);
   h2mIup__1->GetXaxis()->SetLabelFont(42);
   h2mIup__1->GetXaxis()->SetLabelSize(0.035);
   h2mIup__1->GetXaxis()->SetTitleSize(0.035);
   h2mIup__1->GetXaxis()->SetTitleFont(42);
   h2mIup__1->GetYaxis()->SetLabelFont(42);
   h2mIup__1->GetYaxis()->SetLabelSize(0.035);
   h2mIup__1->GetYaxis()->SetTitleSize(0.035);
   h2mIup__1->GetYaxis()->SetTitleFont(42);
   h2mIup__1->GetZaxis()->SetLabelFont(42);
   h2mIup__1->GetZaxis()->SetLabelSize(0.035);
   h2mIup__1->GetZaxis()->SetTitleSize(0.035);
   h2mIup__1->GetZaxis()->SetTitleFont(42);
   h2mIup__1->Draw("");
   
   pt = new TPaveText(0.08793507,0.9052217,0.2552711,0.9356514,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetTextSize(0.1);
   AText = pt->AddText("Up");
   pt->Draw();
   TLine *line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   TEllipse *ellipse = new TEllipse(2.058724,640.0488,8.55,8.55,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(9.507822,656.0313,0.425,0.425,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(8.278308,672.0947,1.925,1.925,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(15.80287,688.0074,6.375,6.375,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(14.5,704.0814,4.6,4.6,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(5.411677,720.2499,2.05,2.05,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(4.032976,736.2967,3.675,3.675,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(4.442663,751.7336,2.975,2.975,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(0.840106,767.7928,6.575,6.575,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(10.73083,783.7655,1.75,1.75,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(7.045578,799.8514,0.425,0.425,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(21.29115,-687.8355,0.425,0.425,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(15.24254,-704.0413,1.825,1.825,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(15.14266,-720.087,4.7,4.7,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(8.881917,-736.2306,1.325,1.325,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(-16.42101,-751.5344,3.45,3.45,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(11.27298,-767.679,2.125,2.125,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(27.41258,-783.3282,4.775,4.775,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(4.909921,-799.8361,2.525,2.525,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   p2_1->Modified();
   p2->cd();
  
// ------------>Primitives in pad: p2_2
   TPad *p2_2 = new TPad("p2_2", "p2_2",0.51,0.01,0.99,0.99);
   p2_2->Draw();
   p2_2->cd();
   p2_2->Range(-62.5,-881.25,62.5,-568.75);
   p2_2->SetFillColor(0);
   p2_2->SetBorderMode(0);
   p2_2->SetBorderSize(2);
   p2_2->SetFrameBorderMode(0);
   p2_2->SetFrameBorderMode(0);
   
   TH2F *h2mIbottom__2 = new TH2F("h2mIbottom__2","",100,-50,50,100,-850,-600);
   h2mIbottom__2->SetStats(0);

   ci = TColor::GetColor("#000099");
   h2mIbottom__2->SetLineColor(ci);
   h2mIbottom__2->GetXaxis()->SetLabelFont(42);
   h2mIbottom__2->GetXaxis()->SetLabelSize(0.035);
   h2mIbottom__2->GetXaxis()->SetTitleSize(0.035);
   h2mIbottom__2->GetXaxis()->SetTitleFont(42);
   h2mIbottom__2->GetYaxis()->SetLabelFont(42);
   h2mIbottom__2->GetYaxis()->SetLabelSize(0.035);
   h2mIbottom__2->GetYaxis()->SetTitleSize(0.035);
   h2mIbottom__2->GetYaxis()->SetTitleFont(42);
   h2mIbottom__2->GetZaxis()->SetLabelFont(42);
   h2mIbottom__2->GetZaxis()->SetLabelSize(0.035);
   h2mIbottom__2->GetZaxis()->SetTitleSize(0.035);
   h2mIbottom__2->GetZaxis()->SetTitleFont(42);
   h2mIbottom__2->Draw("");
   
   pt = new TPaveText(0.1798862,0.9160598,0.3472222,0.9376141,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetTextSize(0.1);
   AText = pt->AddText("Bottom");
   pt->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(2.058724,640.0488,8.55,8.55,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(9.507822,656.0313,0.425,0.425,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(8.278308,672.0947,1.925,1.925,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(15.80287,688.0074,6.375,6.375,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(14.5,704.0814,4.6,4.6,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(5.411677,720.2499,2.05,2.05,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(4.032976,736.2967,3.675,3.675,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(4.442663,751.7336,2.975,2.975,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(0.840106,767.7928,6.575,6.575,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(10.73083,783.7655,1.75,1.75,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(7.045578,799.8514,0.425,0.425,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(21.29115,-687.8355,0.425,0.425,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(15.24254,-704.0413,1.825,1.825,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(15.14266,-720.087,4.7,4.7,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(8.881917,-736.2306,1.325,1.325,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(-16.42101,-751.5344,3.45,3.45,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(11.27298,-767.679,2.125,2.125,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(27.41258,-783.3282,4.775,4.775,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   line = new TLine(8.763969,799.8477,10.18322,-799.8155);

   ci = TColor::GetColor("#ff0000");
   line->SetLineColor(ci);
   line->Draw();
   
   ellipse = new TEllipse(4.909921,-799.8361,2.525,2.525,0,360,0);
   ellipse->SetFillStyle(4000);

   ci = TColor::GetColor("#0000ff");
   ellipse->SetLineColor(ci);
   ellipse->Draw();
   p2_2->Modified();
   p2->cd();
   p2->Modified();
   c1-chamber-drawhitswithtrack->cd();
   c1-chamber-drawhitswithtrack->Modified();
   c1-chamber-drawhitswithtrack->cd();
   c1-chamber-drawhitswithtrack->SetSelected(c1-chamber-drawhitswithtrack);
}
