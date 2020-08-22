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
char* Int2String(int num,char *str)//10���� 
{
	int i = 0;//ָʾ���str 
	if(num<0)//���numΪ��������num���� 
	{
		num = -num;
		str[i++] = '-';
	} 
	//ת�� 
	do
	{
		str[i++] = num%10+48;//ȡnum���λ �ַ�0~9��ASCII����48~57������˵����0+48=48��ASCII���Ӧ�ַ�'0' 
		num /= 10;//ȥ�����λ	
	}while(num);//num��Ϊ0����ѭ��
	
	str[i] = '\0';
	
	//ȷ����ʼ������λ�� 
	int j = 0;
	if(str[0]=='-')//����и��ţ����Ų��õ��� 
	{
		j = 1;//�ӵڶ�λ��ʼ���� 
		++i;//�����и��ţ����Խ����ĶԳ���ҲҪ����1λ 
	}
	//�Գƽ��� 
	for(;j<i/2;j++)
	{
		//�Գƽ������˵�ֵ ��ʵ����ʡ���м��������a+b��ֵ��a=a+b;b=a-b;a=a-b; 
		str[j] = str[j] + str[i-1-j];
		str[i-1-j] = str[j] - str[i-1-j];
		str[j] = str[j] - str[i-1-j];
	} 
	
	return str;//����ת�����ֵ 
}



int String2Int(char *str)//�ַ���ת���� 
{
	char flag = '+';//ָʾ����Ƿ������ 
	long res = 0;
	
	if(*str=='-')//�ַ��������� 
	{
		++str;//ָ����һ���ַ� 
		flag = '-';//����־��Ϊ���� 
	} 
	//����ַ�ת�������ۼӵ����res 
	while(*str>=48 && *str<57)//��������ֲŽ���ת��������0~9��ASCII�룺48~57 
	{
		res = 10*res+  *str++-48;//�ַ�'0'��ASCII��Ϊ48,48-48=0�պ�ת��Ϊ����0 
	} 
 
    if(flag == '-')//�����Ǹ��������
	{
		res = -res;
	}
 
	return (int)res;
}

////10�ĳ˷������������ж�λ��
//uint32_t HMI_Pow(uint8_t n)
//{
//	uint32_t  result=1;	 
//	while(n--)result*=10;    
//	return result;
//}	


//��ֵ���ͺ���
//USART_TypeDef* USARTx�� ѡ�񴮿������ע������ȳ�ʼ����ʹ�ܴ���
//u16 *data����λ��������λ��Ӧ�ĸı���ֵ����
//u8 len��*data����ĳ��ȡ�
//u16 Num����Ҫ��ʾ����ֵ��ע�⿪ͷ0����ʾ
//u8 len2����Ҫ��ʾ����ֵ��λ��
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

//�ַ������ͺ�����HMI_SendText(USART_TypeDef* USARTx,u8 *data,u8 len,u8 *text,u8 len2)
//            USART_TypeDef* USARTx�� ѡ�񴮿������ע������ȳ�ʼ����ʹ�ܸô���
//            u16 *data����λ�����ı�λ��Ӧ�ĸı���ֵ���룬��"t0.txt="��"t1.txt="
//          	u8 len��*data����ĳ��ȡ���"t0.txt="����Ϊ7��
//            u8 *text����Ҫ���������ʾ���ı�
//            u8 len2����Ҫ��ʾ���ı��ĳ��ȣ�Ӣ��һ���ַ�һ�ֽڡ�����һ���������ֽ�    

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
