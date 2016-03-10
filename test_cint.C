class Hoge
{
   public:
      Hoge(int a, int b)
      {
      };
      void Print() {};
   private:
};


class Hoge2
{
   public:
      Hoge2(int a, int b)
         :a(1,2)
      {
      };
      Hoge& GetHoge() { return a; };
      Hoge GetHoge2() { return a; };
      Hoge* GetHoge3() { return &a; };
      Hoge* GetHoge4(int a2, int b2) { return &a; };
   private:
      Hoge a;
};


