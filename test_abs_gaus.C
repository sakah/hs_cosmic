double abs_gaus(double* x, double* par)
{
   double dx = (TMath::Abs(x[0]) - TMath::Abs(par[1]))/par[2];
   return par[0]*TMath::Exp(-0.5*dx*dx)/par[2]/TMath::Sqrt(2.0*TMath::Pi());
}
void test_abs_gaus(double par0, double par1, double par2)
{
   TF1* f1 =new TF1("abs_gaus", abs_gaus, 0, 500, 3);
   f1->SetParameters(par0, par1, par2);
   f1->SetNpx(1000);
   f1->Draw();
}


