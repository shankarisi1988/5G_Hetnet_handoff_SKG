 #include <iostream>
 #include <iostream>
 #include <cstdlib>
 #include <cmath>
 

 #include "RSSclass.h"

  #include "macro.h"

   #include "SINRclass.h"

 // #include "UEclass.h"

 extern Received_signal_strength xyz[NO_OF_UE][5];
 extern Received_SINR  abc[NO_OF_UE][5];
 extern bool asso_control[NO_OF_UE][5];
 extern float call_drop;
 extern int subcarrier[5];
 extern long double throughput;

class handoff_protocol
  {
      public:
          virtual  double hothreshold()=0;
          virtual  double network_selection(int)=0;
          virtual  double execute_hard(int, int, double)=0;
	  virtual  double execute_softer(int, int, int , double , double)=0;
          
  };




class enB : Received_signal_strength, handoff_protocol
   {
     public: 

     double pos_x, pos_y, radius, ho_thr;
     int net_id;
     int target_network;
     public:

        void set_enB(int i, double x, double y)
         {
           net_id=i;
           pos_x=x;
           pos_y=y;  
           radius=1000; // 4 unit equivalent to 1 meter
         }

        double pathloss()
          {
            return(128.1+37.6*log(distance/1000));
          }

        void get_enB()     
          {
        //    cout << "Network id " << net_id << "\n" << "position x " << pos_x << "\n" << "position y " << pos_y << "\n";
          }
        double computemetric(int i, int j)
          {
              double temp; 
              temp=xyz[i][j].computeRSS();
              return (temp);
          }
       double hothreshold()
          {
              double temp;
   	      temp = pow(10, -9.5);
              ho_thr=temp;
              return (temp);
          }

       double network_selection(int user)
          {
              double max, temp;		
	      
          //    cout << "network selection phase \n\n";
              max=-9999;
             
              for(int i=0; i<5 ; i++)
                {
			temp=xyz[user][i].computeRSS();
			if ((temp > max) && (i != net_id))
                         {
				max=temp;
                                target_network=i;
 			 }
                }
		
	      
   
	 //    cout << "Target network is " << target_network << "\n\n";

             return (target_network);
          }

 /*Simulating hard handover*/ 

       double execute_hard(int uid, int target_id, double data_rate_target)
          {
	      
	      float call_arrival_probability, pareto, t;
	   //   cout << "execute hard handoff \n\n";

  		/*Association with the target network */

	      if (subcarrier[target_id]<=SUBCARRIER)
              {
	         asso_control[uid][target_id]=true;
                 subcarrier[target_id]++;
  	      }
              else
              {
                        std::random_device rd;

  	  	         std::mt19937 e2(rd());

		         std::poisson_distribution<int> dist(CBR);

   		         call_arrival_probability=dist(e2);

               }

		/*Deassociation with all other network*/

	      for(int k=0; k<5; k++)
                {
			if(k!=target_id)
                          {
				if (asso_control[uid][k]==true)
				{
                                      subcarrier[k] = subcarrier[k] - 12;
				}
			asso_control[uid][k]=false;
			  }
 		}
              
          }

      
    /*Simulating semisoft handover*/

     double execute_softer(int uid, int curr_id, int target_id, double data_rate_target, double data_rate_current)
         {
		
	      int user, target,  pareto, t;
	      double call_arrival_probability;

	      user=uid;
	      target=target_id;

		/*Association with the current and the target network */


	    /*Association with the target network */

	      if (subcarrier[target]<=SUBCARRIER){
	         asso_control[user][target]=true;
                 subcarrier[target]++;
  	      }

		/*Deassociation with all other network*/

	      for(int k=0; k<5; k++)
                {
			if((k!=target) && (k!=curr_id))
                          {
				if (asso_control[user][k]==true)
				{
                                      subcarrier[k] = subcarrier[k] - 12;
				}
			asso_control[user][k]=false;
			  }
 		}
         }
   };
