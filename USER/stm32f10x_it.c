/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "dm9000.h"
#include "delay.h"

extern u8 USART1_RX_BUF[64];     //接收缓冲1，接收来自上位机的指令
extern u8 USART2_RX_BUF[64];	 //接收缓冲2，接收来自bp模块的数据
extern u8 USART_RX_STA;       //接收状态标记   

extern long cnt;		
extern u8 s;

extern short tempH, tempL;

extern vu8 RxCounter1;
extern vu8 RxCounter2;
extern vu8 USART1_flag;
extern vu8 USART2_flag;
extern vu8 bp_start;
extern vu8 bp_received;
extern vu8 bp_report;
extern vu8 bp_fetch_permitted;
extern vu8 cmd_received;

extern vu8 data_upload[15];
extern vu32 clk_counter;
extern vu8 SDA[40];
extern vu8 SDAA[2];


//extern uint64_t SDAAA;
extern vu16 SDAAA;
extern vu8 SDAAM;
extern vu8 n;
extern vu8 m;
 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void EXTI0_IRQHandler(void){
	
	n++;	
	if(n>82 && n<=100){	
		m++;		
		if(m<=8){
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))	{
				SDAAA=SDAAA<<1;SDAAA=SDAAA|0x0001; 				
			}	else{
				SDAAA=SDAAA<<1; SDAAA=SDAAA&0xfffe; 
			}
		}
		else m=0;
	}	
	
	if(n>100 && n<=108){	
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))	{
				SDAAM=SDAAM<<1;SDAAM=SDAAM|0x0001; 				
			}	else{
				SDAAM=SDAAM<<1; SDAAM=SDAAM&0xfffe; 
			}
	}	
	
//GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)0);
//__NOP();__NOP();__NOP();
//GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)1);		

			
//clk_counter=clk_counter+0x00000001;   //不能加这句，加了后只进一次中断。原因不明。 超长数的加法费时？？
	
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
}


//void EXTI9_5_IRQHandler(void)
//{


//	if(EXTI_GetITStatus(EXTI_Line8)!=RESET)
//	{
//		
//		cmd_received=1;

//		if(bp_fetch_permitted){
//			if(clk_counter<(8+delay)){ 
//				GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)0); //时钟标签

//				SDA[0]<<=1;
//				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))    SDA[0]++;	
//			}
//			if(clk_counter>=(9+delay) && clk_counter< 17+delay) { SDA[1]<<=1;	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))    SDA[1]++;	}
//		
//			
//			if(clk_counter>=(82+delay) && clk_counter< 90+delay)	{ SDA[9]<<=1;	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))    SDA[9]++;	}
//			if(clk_counter>=91+delay && clk_counter< 99+delay)	{ SDA[10]<<=1;	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))    SDA[10]++;	}
//			if(clk_counter>=100+delay && clk_counter< 108+delay)  { SDA[11]<<=1;	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))    SDA[11]++;	}
//					
//			clk_counter++;
//					 
//		}
//		if(clk_counter==108+delay){
//			//GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)0);
//			data_upload[0]=0xF0;
//			data_upload[1]=0xF2;
//			//data_upload[2]=SDA[0];//0xF0;
//			//data_upload[3]=SDA[1];//0xF0;
//			//data_upload[4]=0xFF;
//			//data_upload[5]=0xFF;
//			data_upload[6]=(SDA[9]+25);
//			data_upload[7]=SDA[10];
//			data_upload[8]=SDA[11];
//			//data_upload[9]=0xFF;
//				
//			clk_counter=0;
//			bp_fetch_permitted=0;
//		}	
//	}
//	 EXTI->PR = EXTI_Line8;


//	//从dm9000.c中移动过，
//	EXTI_ClearITPendingBit(EXTI_Line6); //清除中断线6挂起标志位
//	while(DM9000_INT == 0)
//	{
//		DMA9000_ISRHandler();
//	}
//	
//}

/***************************
从bp3.0复制过来，因为新板取消了ARM上位机，所以暂时无用。傻逼开发板在usart.c中还有一个串口中断处理程序，小心！  2017.12.17
****************************/

///**
//  * @brief  This function handles USART1 interrupt request.
//  * @param  None
//  * @retval None
//  */
////注意,读取USARTx->SR能避免莫名其妙的错误 
//void USART1_IRQHandler(void)	//串口1中断服务程序	 接收来自上位机的指令
//{
//	u8 Res;	
//	Res=Res;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
//	{
//		Res =USART_ReceiveData(USART1);			
//		if(Res==0xF0 )	 { RxCounter1=0; USART1_flag=1;}
//		if(USART1_flag)     USART1_RX_BUF[RxCounter1++]=Res;
//		if(RxCounter1==4) {cmd_received=1; RxCounter1=0;}
//	}	
//}


///**
//  * @brief  This function handles USART2 interrupt request.
//  * @param  None
//  * @retval None
//  */
//void USART2_IRQHandler(void)	//串口2中断服务程序	  接收来自bp模块的数据
//{
//	u8 Res;
//	Res=Res;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
//	{
//		Res =USART_ReceiveData(USART2);			
//		if(Res==0xF0 )	 { RxCounter2=0; USART2_flag=1;}
//		if(USART2_flag)     USART2_RX_BUF[RxCounter2++]=Res;
//		
//		if(RxCounter2==12) {bp_received=1; RxCounter2=0;}
//		
//		//if(USART2_RX_BUF[5]==0xE0 && USART2_RX_BUF[6]){bp_report=1; RxCounter2=0; }
///*		if(RxCounter2>5){
//			if(USART2_RX_BUF[5]){bp_report=1; RxCounter2=0;}
//			//if(RxCounter2==12) {bp_received=1; RxCounter2=0;}
//		}
//  */
//	}
//}

