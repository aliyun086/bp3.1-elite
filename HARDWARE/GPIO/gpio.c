#include "gpio.h"

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//推挽输出通用设置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	

	//输出，LED0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0   PB.5 
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //输出高   LED低电平有效
	//输出，LED1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1  PE.5 
	GPIO_Init(GPIOE, &GPIO_InitStructure);	  			
	GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //输出高 	
	//输出，作为血压模块启动按键继电器控制端
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    //PE.2		 
	GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 
	//GPIO_SetBits(GPIOE,GPIO_Pin_2); 		
	//输出，作为血压模块记忆读出按键继电器控制端。	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		//PE.4 
	GPIO_Init(GPIOE, &GPIO_InitStructure);	  				
	// GPIO_SetBits(GPIOE,GPIO_Pin_4); 
	//输出，作为时钟位置检测信号。
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;        //PC.7
	GPIO_Init(GPIOC, &GPIO_InitStructure); 	
	GPIO_SetBits(GPIOC,GPIO_Pin_7);
	
	
	
	//输入，SDA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;     // PC.9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	//移动到EXTI0_Init()中
//	//中断，测试，想代替PC.8
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;    //PC.0
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);  	//选择按键所在的GPIO管脚用作外部中断线路
	
	//中断，SCL
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			// PC.8           //？？  使用前请选禁止JTAG使用此引脚	 低电平有效,输入引脚需上拉设置，反之设成下拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//  GPIO_Mode_IN_FLOATING浮空输入     GPIO_Mode_IPU  上拉输入   GPIO_Mode_IPD 下拉输入	
	GPIO_Init(GPIOC, &GPIO_InitStructure);			
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);  	//选择按键所在的GPIO管脚用作外部中断线路
	
	
	
	//初始化作为继电器控制。
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);

//	//初始化PB9作为继电器控制
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);


//	//初始化PC0作为血压模块启动按键继电器控制端。
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);



}
