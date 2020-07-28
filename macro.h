 #define NO_OF_UE 500
 #define TTI  10000
 #define DATA_RATE 256  // in Mbps 
 #define hard_handoff true   // Either hard_handoff or semi_soft is true
 #define semi_soft false 
 #define CBR 7   // CBR traffic arrival rate
 #define SUBCARRIER 1500  // No. of sub-carrier per BS, a 20 Mhz and 5 Mhz BSs are collocated  
 #define RBSIZE 12 // No. of subcarriers per resource block
 #define SYMBOL 6 // No. of OFDMA symbols per LTE-A frame
 #define MODULATION 1 // 1 for BPSK, 2 for QPSK, 4 for 16-QAM  and 6 for 64-QAM
 #define FRAME 10   // Framelength in milli-second
 #define RANGE 2.576 // 2.576 for 99% confidence interval, 1.960 for 95% confidence interval, 1.645 for 90% confidence interval

