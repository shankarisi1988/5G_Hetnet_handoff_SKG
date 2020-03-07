 #include <iostream>
 #include <cstdlib>
 #include <cmath>

 class mobility
    {

   public:

      virtual void move()=0;

    }; 



class UE 
    {
      public: 

      int id, temp, timer, handoff_event;
      double  pos_x, pos_y, velocity, throughput, outage;

      public:

        UE() {timer=0; throughput=0;  handoff_event=0; outage=0;}

        void set_UE(int i, double x, double y, double v)
          {
            id=i;
            pos_x=x;
            pos_y=y;
            velocity=v;
          }

        void get_UE()     
          {
     //       cout << "User id " << id << "\n" << " position x " << pos_x << "\n" << " position y " << pos_y << "\n" << " velocity " << velocity << "\n";
          }

        void move()
        {
           temp=(rand()%100)*velocity;
           pos_x=pos_x + velocity*cos(temp);
           pos_y=pos_y + velocity*sin(temp);
        }
 };
