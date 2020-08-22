/* Private includes ----------------------------------------------------------*/
#include "delay.h"


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */


/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */


/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint16_t fac_us = 0; 
static uint16_t fac_ms = 0;  
/* USER CODE END PV */


/* USER CODE BEGIN 0 */
void delay_init()
{
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
	fac_us = SystemCoreClock/8000000;
	fac_ms=(uint16_t)fac_us*1000;
}
/* USER CODE END 0 */


/* USER CODE BEGIN 1 */
void delay_ms(uint16_t nms)
{
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)nms*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;							//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;       					//��ռ�����
}
/* USER CODE END 1 */


/* USER CODE BEGIN 2 */
void delay_us(uint32_t nus)
{
	uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//ʱ�����	  		 
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����	 
}
/* USER CODE END 2 */
