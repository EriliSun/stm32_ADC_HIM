/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    //change USART
    while ((USART1->SR & 0X40) == 0); //wait send
    USART1->DR = (uint8_t) ch;
    return ch;
}
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
char* Int2String(int num,char *str)//10进制 
{
	int i = 0;//指示填充str 
	if(num<0)//如果num为负数，将num变正 
	{
		num = -num;
		str[i++] = '-';
	} 
	//转换 
	do
	{
		str[i++] = num%10+48;//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0' 
		num /= 10;//去掉最低位	
	}while(num);//num不为0继续循环
	
	str[i] = '\0';
	
	//确定开始调整的位置 
	int j = 0;
	if(str[0]=='-')//如果有负号，负号不用调整 
	{
		j = 1;//从第二位开始调整 
		++i;//由于有负号，所以交换的对称轴也要后移1位 
	}
	//对称交换 
	for(;j<i/2;j++)
	{
		//对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b; 
		str[j] = str[j] + str[i-1-j];
		str[i-1-j] = str[j] - str[i-1-j];
		str[j] = str[j] - str[i-1-j];
	} 
	
	return str;//返回转换后的值 
}



int String2Int(char *str)//字符串转数字 
{
	char flag = '+';//指示结果是否带符号 
	long res = 0;
	
	if(*str=='-')//字符串带负号 
	{
		++str;//指向下一个字符 
		flag = '-';//将标志设为负号 
	} 
	//逐个字符转换，并累加到结果res 
	while(*str>=48 && *str<57)//如果是数字才进行转换，数字0~9的ASCII码：48~57 
	{
		res = 10*res+  *str++-48;//字符'0'的ASCII码为48,48-48=0刚好转化为数字0 
	} 
 
    if(flag == '-')//处理是负数的情况
	{
		res = -res;
	}
 
	return (int)res;
}

////10的乘方函数，用于判断位数
//uint32_t HMI_Pow(uint8_t n)
//{
//	uint32_t  result=1;	 
//	while(n--)result*=10;    
//	return result;
//}	


//数值发送函数
//USART_TypeDef* USARTx： 选择串口输出，注意必须先初始化、使能串口
//u16 *data：上位机上数字位相应的改变数值代码
//u8 len：*data代码的长度。
//u16 Num：需要显示的数值，注意开头0不显示
//u8 len2：需要显示的数值的位数
void HMI_SendNum(UART_HandleTypeDef *huart,uint8_t *data,uint8_t len,uint16_t Num,uint8_t len2)
{
//	uint8_t t;
	uint8_t end = 0XFF;
	char Str[10];
//	int i=0,j=0;
//  for(t=0;t<len;t++)
//		{
	HAL_UART_Transmit(huart,(uint8_t *)data,len,1000);	
	while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);
			
			//USART_SendData(USARTx,data[t]); 					
//    }
//  for(t=0;t<len2;t++)	
//    {
//while(Num)
//{
//	temp[i++]=Num%10+'0';
//	Num = Num/10;
//		
//}
//	temp[i]='\0';
//	i = i-1;
//while(i>=0)
//{
//	Str[j++]=temp[i--];
//	Str[j]='\0';
//}

	Int2String(Num,Str);
	  //printf("adc_val is %d\n",(uint8_t *)Num);
	HAL_UART_Transmit(huart,(uint8_t *)Str,len2,10000);
	  //printf("adc_val is %d\n",(uint16_t *)Num);
	while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);

    //USART_SendData(USARTx,'0'+(Num/HMI_Pow(len2-t-1))%10); 


//    }
	HAL_UART_Transmit(huart,&end,1,20);		
  while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);
	 
   //USART_SendData(USARTx,0XFF);
              
  HAL_UART_Transmit(huart,&end,1,20);    	
  while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);
   
             
  HAL_UART_Transmit(huart,&end,1,20);    	
  while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);
   

}

//字符串发送函数：HMI_SendText(USART_TypeDef* USARTx,u8 *data,u8 len,u8 *text,u8 len2)
//            USART_TypeDef* USARTx： 选择串口输出，注意必须先初始化、使能该串口
//            u16 *data：上位机上文本位相应的改变数值代码，如"t0.txt="、"t1.txt="
//          	u8 len：*data代码的长度。如"t0.txt="长度为7。
//            u8 *text：需要串口输出显示的文本
//            u8 len2：需要显示的文本的长度，英文一个字符一字节、汉字一个字两个字节    

void HMI_SendText(UART_HandleTypeDef *huart,uint8_t *data,uint8_t len,uint8_t *text,uint8_t len2)
{
//	uint8_t t;
	uint8_t end = 0XFF;
	uint8_t start_end = 0X22;
//  for(t=0;t<len;t++)
//   {	
   HAL_UART_Transmit(huart,(uint8_t *)data,len,1000);	
   while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);
 					
//   }
	 HAL_UART_Transmit(huart,&start_end,1,1000);	
	 while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);

     //USART_SendData(USARTx,0X22); 
//  for(t=0;t<len2;t++)	
//   {
	 HAL_UART_Transmit(huart,(uint8_t *)text,len2,1000);
   while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);

     //USART_SendData(USARTx,text[t]); 

//  }
   HAL_UART_Transmit(huart,&start_end,1,1000);
	 while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);

   HAL_UART_Transmit(huart,&end,1,1000);				
   while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);

              
   HAL_UART_Transmit(huart,&end,1,1000);       	
   while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);

             
   HAL_UART_Transmit(huart,&end,1,1000);       	
   while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);





}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
