#include <iostream>
 #include <cstdlib>
 #include <cmath>
 #include <iomanip>
 #include <string>
 #include <map>
 #include <random>



class Received_SINR
{

   private:

       double distance;

   public:
       
       void set_SINR(double k) {distance=k/1000;}

       double computeSINR(double cqi)
      
          {
               double  interference, signal, normal, temp_x, temp_y, sinr, power, temp;   
	       interference=cqi;
  
               if (distance < 2)
               {
                    distance = 2;
                }
               

                if (interference < pow(2, -13))
                 {
			interference = pow(2, -13);
                  }
           
           /*computing log-normal shadow fading*/

                std::random_device rd;

  		  std::mt19937 e2(rd());

  		  std::normal_distribution<> dist(0, 1);

   		 normal=dist(e2);

  		  signal=pow(distance, -4)*pow(10, normal/10);
                   
                  sinr=signal/interference;
                
		 if(sinr < pow(2, -10))
                 {
                      sinr = pow (2, -10);
                  } 

  		  return (sinr);
         }
};
