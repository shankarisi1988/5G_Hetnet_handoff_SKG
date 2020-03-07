
 #include <iostream>
 #include <cstdlib>
 #include <cmath>
 #include <iomanip>
 #include <string>
 #include <map>
 #include <random>

 using namespace std;



#include "enBclass.h"
 
 #include "UEclass.h"
 // #include "RSSclass.h"


 #include "macro.h"


 UE ob[NO_OF_UE];
 enB base[5];
 int subcarrier[5];


 bool asso_control[NO_OF_UE][5];  
 float call_drop=0;
 long double throughput=0;




// #include "SINRclass.h"


Received_SINR  abc[NO_OF_UE][5];
Received_signal_strength xyz[NO_OF_UE][5];



void create_topology()
  {   
     int i, j, k;
     long double x, y, v, distance, temp_x, temp_y, path_loss,  RSS, uniform, normal, SINR,signal, interference,  data_rate;

  /*specify the co-rodinates of the deployed BSs. Here 4 unit of co-ordinates are equivalent to 1 meter*/
     
     base[0].set_enB(0, 1000, 1000);
     base[1].set_enB(1, 2000, 2000);
     base[2].set_enB(2, 3000, 3000);
     base[3].set_enB(3, 1000, 3000);
     base[4].set_enB(4, 3000, 1000);

     base[0].get_enB();
     base[1].get_enB();
     base[2].get_enB();
     base[3].get_enB();
     base[4].get_enB();

  /* Initializing the co-ordinates of the users*/

     for (i=0; i<NO_OF_UE; i++)
      {
        x= (rand()%100)*25;
        y= (rand()%100)*25;
        v=(rand()%100)*.10;
        ob[i].set_UE(i, x, y, v);
      }

    for(i=0; i<NO_OF_UE; i++)
      {
        ob[i].get_UE();
        ob[i].move();
        ob[i].get_UE();
      }

  /*Initial user association and subcarrier allocation*/

    for (int i=0; i<NO_OF_UE; i++)
      {
        for(int j=0; j<5; j++)
          {
             asso_control[i][j]=false;
          }
      }   

    for (int j=0; j<5; j++)
      {
		subcarrier[j]=0;
      }
  } 

void cellassociation()
{

     long double  distance, temp_x, temp_y, RSS , SINR, data_rate, max_rss, mindist;
     int bs_id, target_id;
     max_rss=-999;
     mindist= 5000;
     for (int i=0; i<NO_OF_UE; i++)
      {
         for(int j=0; j<5; j++)
          {
              /*Initially the users are associated to the nearest base stations*/

              temp_x=  (base[j].pos_x-ob[i].pos_x)*(base[j].pos_x-ob[i].pos_x);
              temp_y=  (base[j].pos_y-ob[i].pos_y)*(base[j].pos_y-ob[i].pos_y);         
              distance=sqrt(temp_x+temp_y);
              xyz[i][j].setrss(distance);
              RSS=xyz[i][j].computeRSS();

              if (distance<1000)
                {
               
                  if (distance < mindist)
                    {
                       mindist=distance;
                       bs_id=j;
                    }
                }
          }

	if (subcarrier[bs_id] <= SUBCARRIER)
			{
                      	   subcarrier[bs_id]++;
                           asso_control[i][bs_id]=true; 
			}

      }
}


long double compute_interference(int target, int user)
{
     /*This function computes interference received from neighbouring base stations of "target" base stations*/

     long double interference, power, temp_x, temp_y, temp, normal ;
     int netid, uid;
     netid=target;
     uid=user;
     interference=0;                  
     std::random_device rd;

     std::mt19937 e2(rd());

     std::normal_distribution<> dist(0, 1);

     normal=dist(e2);     

 	 for(int k=0; k<5; k++)
   		    {                      
      		      if (k!=netid)
           		     {
        	     	       temp_x=  (base[k].pos_x-ob[uid].pos_x)*(base[k].pos_x-ob[uid].pos_x);
                 	       temp_y=  (base[k].pos_y-ob[uid].pos_y)*(base[k].pos_y-ob[uid].pos_y);         
            		       temp=sqrt(temp_x+temp_y);
           		       normal=dist(e2);                           
        		       power=pow(temp, -4)*pow(10, normal/10);
        		       interference=interference+power;
         		     }
     		    }
         return (interference);
}

void run_simulation()
 {
     long double  distance, temp_x, temp_y, RSS , SINR, data_rate, ho_thr, metric,  data_rate_target, data_rate_current, SINR_target, SINR_current, noise,  pareto, t,  call_arrival_probability,   max, pbs, poverlap, cuser, pdc;
     long double temp_handoff, temp_throughput, temp_outage, temp_timer;
     int target_id;
     int flag;
  //   cuser=0;
   //  pdc=0;
     
     for(int k=0; k<TTI; k++)
       {
           //   cout << "<------------TTI  " <<  k <<  "-------------->\n\n\n";
              for (int i=0; i<NO_OF_UE; i++)
                  {
  		          ob[i].move();
                          ob[i].timer++;
			
			  flag=1;

			  for(int j=0; j<5; j++)
			   {
				 if (asso_control[i][j]==true)
					{
					  flag=0;
                                          break;
					}
				 else 
				  {
					continue;
			          }					
			   }
                         
                         if (flag)
			  {

				std::random_device rd;

  	  	     		std::mt19937 e2(rd());

		     		// std::uniform_real_distribution<double> dist(0.0, 1.0);

				std::poisson_distribution<int> dist(CBR);

   		   		call_arrival_probability=dist(e2);

				ob[i].outage=ob[i].outage+call_arrival_probability;
			  }

       			  for(int j=0; j<5; j++)
        			  {
           				   temp_x=  (base[j].pos_x-ob[i].pos_x)*(base[j].pos_x-ob[i].pos_x);
          				   temp_y=  (base[j].pos_y-ob[i].pos_y)*(base[j].pos_y-ob[i].pos_y);         
           				   distance=sqrt(temp_x+temp_y);

          				    if (distance<1000)
            					    {
							
         					         xyz[i][j].setrss(distance);
            						 abc[i][j].set_SINR(distance);
             						 RSS=xyz[i][j].computeRSS();
						         

                                                         // computing handover decision metric and handover threshold  

							 metric=base[j].computemetric(i,j);
							 ho_thr=base[j].hothreshold();

                 		                            if (asso_control[i][j]==true)
                    						  {
										for(int k=0; k<5; k++)
											{
                                                                                            if (k!=j)
												{
												   if(asso_control[i][k]==true)
													{
       												          asso_control[i][k]=false;
												          subcarrier[k]--;
													}
												}
                                                                                        }
												
										/*Estimating data rate from target network*/

											 target_id=base[j].network_selection(i);

										
										
										            noise=(compute_interference(j,i)*subcarrier[j])/SUBCARRIER;
	      										    SINR_current=abc[i][j].computeSINR(noise);
												
												noise=(compute_interference(target_id,i)*subcarrier[target_id])/SUBCARRIER;
											  
                                                                                           
											    SINR_target=abc[i][target_id].computeSINR(noise);
											 
											
												/* Throughput computation  */  											    

            										    data_rate_target=(RBSIZE*SYMBOL*MODULATION*log(1+SINR_target))/(FRAME*log(2));
	       										    data_rate_current=(RBSIZE*SYMBOL*MODULATION*log(1+SINR_current))/(FRAME*log(2));

											
									
												/*Handover */

								

                         						  if (metric < ho_thr)
                                                                                   {
                             							     

										      target_id=base[j].network_selection(i);  // base j is computing the optimal target network
                                                  													      

										    

 										      if (target_id==j)   /*if current network is the target network*/
												continue;

 										      else if (hard_handoff)  /*hard handover is going to occur*/
 											{
 												
  											    ob[i].timer=ob[i].timer+20;  
											    ob[i].handoff_event=ob[i].handoff_event+20;
										            noise=(compute_interference(j,i)*subcarrier[j])/SUBCARRIER;
	      										    SINR_current=abc[i][j].computeSINR(noise);
												
												noise=(compute_interference(target_id,i)*subcarrier[target_id])/SUBCARRIER;
											  
                                                                                           
											    SINR_target=abc[i][target_id].computeSINR(noise);
											  //  cout << "SINR target" << SINR_target << "\n";
											    
            										    data_rate_target=(RBSIZE*SYMBOL*MODULATION*log(1+SINR_target))/(FRAME*log(2));
	       										    data_rate_current=(RBSIZE*SYMBOL*MODULATION*log(1+SINR_current))/(FRAME*log(2));

											

            										   
												if((data_rate_current >= DATA_RATE)   && (subcarrier[j] < SUBCARRIER))
												{
												
													std::random_device rd;

  	  	     											std::mt19937 e2(rd());

		     									// std::uniform_real_distribution<double> dist(0.0, 1.0);

													std::poisson_distribution<int> dist(CBR);

   		   											call_arrival_probability=dist(e2);
											
													ob[i].throughput=ob[i].throughput+data_rate_current*call_arrival_probability;
												}
				
											   
											    
	       										   

												/*computing outage probability */
												
												if (data_rate_current < DATA_RATE)
												{
													
													    std::random_device rd;

  	  	     									                    std::mt19937 e2(rd());

		     									       		 //   std::uniform_real_distribution<double> dist(0.0, 1.0);

													   std::poisson_distribution<int> dist(CBR);

													
   														 call_arrival_probability=dist(e2);
                                                                                            
													    ob[i].outage=ob[i].outage+call_arrival_probability;
													
												}
												
											    if(subcarrier[target_id] <=SUBCARRIER)
											      {
										                  base[j].execute_hard(i, target_id, data_rate_target);  // execute hard handoff
											      }
											    else
											      {
												 std::random_device rd;

  	  	     									         std::mt19937 e2(rd());

											         std::poisson_distribution<int> dist(CBR);
												 call_arrival_probability=dist(e2);
												 ob[i].outage=ob[i].outage+call_arrival_probability;
											      }
											}
										    
										     else if (semi_soft)    /*Semisoft handover is enabled between base station j and target_id*/
											{
											     ob[i].timer=ob[i].timer+20;

											     target_id=base[j].network_selection(i);

								                            if(subcarrier[j] < SUBCARRIER)
                                                                                            {
										            noise=(compute_interference(j,i)*subcarrier[j])/SUBCARRIER;
	      										    SINR_current=abc[i][j].computeSINR(noise);
											    data_rate_current=(RBSIZE*SYMBOL*MODULATION*log(1+SINR_current))/(FRAME*log(2));
                                                                                            }
											    else
											    {
												 data_rate_current=0;
											    }
											    	
											    if(subcarrier[target_id] < SUBCARRIER)
											     {
											       noise=(compute_interference(target_id,i)*subcarrier[target_id])/SUBCARRIER;
                                                                                             
											       SINR_target=abc[i][target_id].computeSINR(noise);
											 
                									       data_rate_target=(RBSIZE*SYMBOL*MODULATION*log(1+SINR_target))/(FRAME*log(2));
                                                                                             }
	       										   else
											    {
												 data_rate_target=0;
											    }
			

 							                        	base[j].execute_softer(i,j, target_id, data_rate_target, data_rate_current);  


											 										  		 
                                                                                                                 std::random_device rd;

  	     												 	 std::mt19937 e2(rd());

	     													 std::poisson_distribution<int> dist(CBR);

   													         call_arrival_probability=dist(e2);

														 if (data_rate_current > data_rate_target)
														  {
												 ob[i].throughput=ob[i].throughput + data_rate_current*call_arrival_probability;
														  }
														else
 														  {
											         ob[i].throughput=ob[i].throughput + data_rate_target*call_arrival_probability;
														  }
	
													/*computing outage probability */

												if(ob[i].throughput <= DATA_RATE) 
												     {
														
														 std::random_device rd;

  	     												 	 std::mt19937 e2(rd());

	     													 std::poisson_distribution<int> dist(CBR);

   													         call_arrival_probability=dist(e2);
														
													         ob[i].outage=ob[i].outage+call_arrival_probability;

												     }								     											   
											} // end of semi soft handover
										   }// end of metric
                         						  else
                           							  { 
											/* users in current network are scheduled */
											
		        								std::random_device rd;

  	  	     									std::mt19937 e2(rd());

		     									// std::uniform_real_distribution<double> dist(0.0, 1.0);

											std::poisson_distribution<int> dist(CBR);

   		   									call_arrival_probability=dist(e2);
												
											ob[i].timer=ob[i].timer+20;
											
										            noise=(compute_interference(j,i)*subcarrier[j])/SUBCARRIER;
	      										    SINR_current=abc[i][j].computeSINR(noise);
											    data_rate_current=(RBSIZE*SYMBOL*MODULATION*log(1+SINR_current))/(FRAME*log(2));
                                                                                            
							    				if(data_rate_current > DATA_RATE)
											{	
											  ob[i].throughput=ob[i].throughput+data_rate_current*call_arrival_probability;
											}									 

											if (data_rate_current < DATA_RATE)
												{
													
  													   call_arrival_probability=dist(e2);
													   
													   ob[i].outage=ob[i].outage+call_arrival_probability;
													
												}
                                                                                  }
                    						  }
                					    else
                  						   {
								//		cout << "\n\n Not associated with BS" << j; 
                  				 		   }
            
               					   }
         			 }
     		  }
           
 	}

        temp_handoff=0;
        temp_timer=0;
        temp_throughput=0;
        temp_outage=0;

	for (int k=0; k< NO_OF_UE; k++)
         {
		temp_handoff=temp_handoff+ob[k].handoff_event;
		temp_timer=temp_timer+ob[k].timer;
       		temp_throughput=temp_throughput+ob[k].throughput;
                temp_outage=temp_outage+ob[k].outage;
	 }
        
	  
   cout << "Call arrival rate: " << CBR   << "\n";
   cout << "Traffic load: " << NO_OF_UE << "\n";
   cout << "System throughput (Mbps): " << (temp_throughput/temp_timer) << "\n";
   cout << "Handover failure probability: " << (temp_outage)/(temp_timer*CBR) << "\n";
 }




 int main()
 {
     create_topology();
     cellassociation();
     run_simulation();
 }
