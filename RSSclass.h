 #include <iostream>
 #include <cstdlib>
 #include <cmath>



 class compute_pathloss
  {
      public:
          virtual  double pathloss()=0;
  };


class Received_signal_strength : compute_pathloss
{
   public:

       double distance;

   public:

   void setrss(double k) {distance=k;}

   double pathloss()
    {

         return(24+45*log((distance+20)/1000));
    }

   double computeRSS()
    {
      double path_loss, temp, signal, normal;   
      path_loss=pathloss(); 


    std::random_device rd;

    std::mt19937 e2(rd());

    std::normal_distribution<> dist(0, 1);

    normal=dist(e2);

    signal=pow(distance, -4)*pow(10, normal/10);

    return (signal);
    }
};
