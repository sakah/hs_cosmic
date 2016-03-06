void test_pol4(int n)
{
   TF1* f1 = new TF1("mypol4", "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x", 0, 10);
   f1->SetParameters(0, 1, 2, 3, 4);
   for (int i=0; i<n; i++) {
      double x = f1->GetX(1.2);
   }
}

void test_pol1(int n)
{
   TF1* f1 = new TF1("mypol1", "[0]+[1]*x", 0, 10);
   f1->SetParameters(0, 1);
   for (int i=0; i<n; i++) {
      double x = f1->GetX(1.2);
   }
}
