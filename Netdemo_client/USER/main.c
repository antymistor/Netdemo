#include "stm32f10x.h"		
#include "W5500.h"			
#include "delay.h"
#include <string.h>
#include "math.h"
int main(void)
{
	u8 i=0;
	delay_init();
	W5500_Initialization();		//W5500≥ı ºªı≈‰÷√
	while (1)
	{
		i++;
		Netprintf("D %.2f %d",128*sin((float)i/6)+128,i);
	  delay_ms(50);
	}	
}
//User can realize function "datarec_process" anywhere
//When W5500 receive new data ,the function datarec_process will be callbacked immediately 
//and the pointer of new data (char *Buffer) delivered  .
void datarec_process(char *Buffer)
{
	Netprintf(Buffer);
}



