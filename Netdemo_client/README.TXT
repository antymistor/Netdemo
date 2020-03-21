1. Add files W5500.c , w5500.h , Config.h to your project.
2. Please read and set relative parameters in file "Config.h"!!!
3. Call function W5500_Initialization() to initialise related configurations,and
   than you can call functions void Netprintf(unsigned char *fmt,...) and 
   void datarec_process(char *Buffer) to send data and process recevied data.
4.How to send data in Ethernet way:
   Call function void Netprintf(unsigned char *fmt,...) to send data to  device with destiantion ip.
   The function Netprintf can be called totally similarly to the function printf() .
5.How to process data received in Ethernet way:
   After initialization, anytime data received,the interrupt request will be process immediately.
   Than the function datarec_process will be callbacked immediately and the pointer of new 
  data (char *Buffer) delivered. You can realize the function datarec_process to do something with
  the data received. And if nothing to do with the data recceived ,a void function is still necessary.