double pol4_x(double* x, double* par)
{
   double x2 = x[0] - par[5];
   return par[0] + par[1]*x2 + par[2]*x2*x2 + par[3]*x2*x2*x2 + par[4]*x2*x2*x2*x2;
}
