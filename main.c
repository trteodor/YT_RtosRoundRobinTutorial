#include "stdint.h"
#include "osKernel.h"

uint32_t count0,count1,count2;

void Task0(void)
{
	while(1)
	{
		count0++;
	}
	
}


void Task1(void)
{

	while(1)
	{
		count1++;
		osThreadYield();
	}
	
}


void Task2(void)
{
	while(1)
	{
		count2++;
	}
	
}




int main(void)
{

    nnOsKernelInit();
    osKernelAddThreads(Task0,Task1,Task2);
    osKernelLaunch(10u);

    while(1)
    {

    }

    
}