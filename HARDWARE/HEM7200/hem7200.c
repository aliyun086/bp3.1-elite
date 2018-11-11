#include "delay.h"
#include "stdio.h"

extern vu8 detect_wait;
extern vu8 bp_fetch_permitted;
extern vu32 clk_counter;
extern vu8 SDAA[2];
//extern uint64_t SDAAA;
extern vu16 SDAAA;
extern vu8 SDAAM;

extern vu8 n;
extern vu8 m;
extern vu8 g;
extern vu8 d;
extern vu8 mb;

extern vu8 cmd_received;


void disable_INT8(void){
 	
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	EXTI_ClearITPendingBit(EXTI_Line8);		 //*
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);  	//选择按键所在的GPIO管脚用作外部中断线路

	EXTI_InitStructure.EXTI_Line = EXTI_Line8;	//设置按键所有的外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //外部中断触发沿选择:EXTI_Trigger_Falling  EXTI_Trigger_Rising
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;		//使能外部中断新状态
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;	 //*   //先占优先级4位,共16级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				 //*   //先占优先级0位,从优先级4位
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道，改为禁止
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure); 	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

}

void disable_INT0(void){
 	
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	EXTI_ClearITPendingBit(EXTI_Line0);		 //*
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);  	//选择按键所在的GPIO管脚用作外部中断线路

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//设置按键所有的外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //外部中断触发沿选择:EXTI_Trigger_Falling  EXTI_Trigger_Rising
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;		//使能外部中断新状态
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;	 //*   //先占优先级4位,共16级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				 //*   //先占优先级0位,从优先级4位
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道，改为禁止
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure); 	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

}

void EXTI8_Init()  //设置按键功能
{
	
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	EXTI_ClearITPendingBit(EXTI_Line8);		 //*
	
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //外部中断触发沿选择:EXTI_Trigger_Falling  EXTI_Trigger_Rising
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		//使能外部中断新状态
	EXTI_Init(&EXTI_InitStructure);		
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //先占优先级2位；//4位,共16级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				 //*   //先占优先级2位    //0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道，改为禁止
	//NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure); 	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

}

void EXTI0_Init()  //设置按键功能
{
		GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//使能按键所在GPIO时钟,同时使能AFIO时钟(需要设置外部中断寄存器)  这句移动至main.c  RCC_Configuration()函数
	//RCC_APB2PeriphClockCmd(BUTTON_CLK[Button] | RCC_APB2Periph_AFIO, ENABLE);			
	
	
	EXTI_ClearITPendingBit(EXTI_Line0);		 //*
	
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//设置按键所有的外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //外部中断触发沿选择:EXTI_Trigger_Falling  EXTI_Trigger_Rising
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		//使能外部中断新状态
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
		//中断，测试，代替PC.8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;    //PC.0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);  	//选择按键所在的GPIO管脚用作外部中断线路
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	 //*   //先占优先级4位,共16级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				 //*   //先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道，改为禁止
	//NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure); 	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

}


void fetch_bp(void)
{

	
//	GPIO_WriteBit(GPIOC,GPIO_Pin_0,(BitAction)1);	//启动测量
//	delay_ms(5500);
//	GPIO_WriteBit(GPIOC,GPIO_Pin_0,(BitAction)0);
//	delay_ms(5500);
//	
//										   //等待
//	for(detect_wait=0;detect_wait<50;detect_wait++){
//		delay_ms(1000);
//	}
//	
//	GPIO_WriteBit(GPIOC,GPIO_Pin_0,(BitAction)1);	//关闭测量
//	delay_ms(5000);
//	GPIO_WriteBit(GPIOC,GPIO_Pin_0,(BitAction)0);
	
//	delay_ms(1000);
//	delay_ms(1000);	 				
//	
	delay_ms(1000);
	delay_ms(1000);

	
	GPIO_WriteBit(GPIOE,GPIO_Pin_4,(BitAction)1);	  //读数
	delay_ms(200);
	
	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)1);
	n=0;
	m=0;
	EXTI0_Init();
	bp_fetch_permitted=1; 			
	//GPIO_WriteBit(GPIOC,GPIO_Pin_7,0); //移至中断当中
	
	delay_ms(1000);
	delay_ms(1000);
	GPIO_WriteBit(GPIOE,GPIO_Pin_4,(BitAction)0);

   	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)1);
	disable_INT0();	
	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)0);

	delay_ms(5000);
	GPIO_WriteBit(GPIOE,(BitAction)GPIO_Pin_2,(BitAction)1);	//关闭读数
	delay_ms(1000);
	GPIO_WriteBit(GPIOE,(BitAction)GPIO_Pin_2,(BitAction)0);
	
	
	//printf("SDAAA = %llu  \n",SDAAA);
	//printf("------clk_counter = %u !!!!!!! \n",clk_counter);
	d = SDAAA & 0x00ff;
	g = (SDAAA & 0xff00) >> 8;
	g = g+25;
	mb = SDAAM;
	
	printf("SDAAA = %u  \n", SDAAA);
	printf(" gaoya = %d,  diya = %d ", g, d);
	printf(" %d  ", SDAAM);
	

}

void fetch_bp2(void)
{

	SDAAA=0;	      
	clk_counter=0;

	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)0); 
	delay_us(2);
	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)1); 

	EXTI0_Init();

	GPIO_WriteBit(GPIOE,GPIO_Pin_4,(BitAction)1);	  //读数
	//delay_ms(200);




	//GPIO_WriteBit(GPIOC,GPIO_Pin_7,0); //移至中断当中

	delay_ms(1000);
	delay_ms(1000);
	GPIO_WriteBit(GPIOE,GPIO_Pin_4,(BitAction)0);

	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)1);//时钟标签

	disable_INT0();
	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)0);


	delay_ms(1000);
	//GPIO_WriteBit(GPIOE,(BitAction)GPIO_Pin_2,(BitAction)1);	//关闭读数
	delay_ms(5000);
	//GPIO_WriteBit(GPIOE,(BitAction)GPIO_Pin_2,(BitAction)0);


	//printf("H bp = %d !!!!!!! \n",H);
	printf("SDAAA = %u  \n",SDAAA);
	//printf("------SDAA[1] = %d  \n",SDAA[1]);
	printf("------clk_counter = %u !!!!!!! \n",clk_counter);


	if(cmd_received){
	printf("oooooooooooo");
	cmd_received=0;
	}
		
}
