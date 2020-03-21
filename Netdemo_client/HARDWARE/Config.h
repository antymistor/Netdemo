#ifndef	_Config_
#define	_Config_

/*W5500 GPIO assign,But the GPIO assigns can not be changed here.
 W5500_RST		GPIOA-2	
 W5500_INT		GPIOA-3
 W5500_SCS		GPIOA-4	
 W5500_SCK		GPIOA-5		 
 W5500_MISO		GPIOA-6	
 W5500_MOSI		GPIOA-7	
*/

//Local       IP: IP0.IP1.IP2.IP3_L
//Destination IP: IP0.IP1.IP2.IP3_D 
//IP and port can be reseted here
#define IP0 192
#define IP1 168
#define IP2 1
#define IP3_L 199       //Local IP
#define IP3_D 190       //Destination port
#define S0port_L 5000   //Local port
#define S0port_D 6000   //Destination port


//communication protocol = wormode, the wormode can be TCP_CLIENT or UDP_MODE.
//The workmode can be defined as below.
//#define workmode TCP_CLIENT
#define workmode UDP_MODE


#endif

