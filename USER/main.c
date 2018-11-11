#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	
#include "timer.h"
#include "sram.h"
#include "malloc.h"
#include "beep.h"
#include "tsensor.h"
#include "rtc.h"
#include "string.h"
#include "usmart.h"	
//#include "dm9000.h"
#include "enc28j60.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "httpd.h"
#include "ds18b20.h"
#include "stdio.h"
#include "hem7200.h"
#include "gpio.h"
/************************************************
 ALIENTEK战舰STM32开发板LWIP实验
 基于RAW API的WebServer Server实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


u8 buffer[10];

u8 cmd_verify[]={0xF0,0x20,0x21,0x01,0xE0};	   //血压计状态查询命令
u8 cmd_start[]={0xF0,0x20,0x21,0x02,0x31,0x00}; //血压计启动命令

u8 cmd_mode;

short temp,tempH,tempL;

long cnt=0;
u8 s=0;

vu8 USART1_RX_BUF[10];     //接收缓冲1，接收来自上位机的指令帧，4个字节
vu8 USART2_RX_BUF[20];	   //接收缓冲2，接收来自bp模块的数据帧，12个字节
vu8 data_upload[15]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
					0x00,0x00,0x00,0x00,0x00};	   //上传给上位机的数据帧，10个字节，预留15个。

//vu8 USART_RX_STA;          //接收状态标记   

vu8 RxCounter1=0x00;
vu8 RxCounter2=0x00;
vu8 USART1_flag=0x00;
vu8 USART2_flag=0x00;
vu16 bp_start=0x00;
vu8 bp_received=0x00;
vu8 bp_report=0x00;
vu8 cmd_received=0x00;

vu8 bp_fetch_permitted=0x00; 

vu32 clk_counter=0;
vu8 SDA[40]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
vu8 SDAA[2];
	
//中断中用到的变量
//uint64_t SDAAA;	
vu16 SDAAA;
vu8 SDAAM;
vu8 n=0;
vu8 m=0;
vu8 g=0;
vu8 d=0;
vu8 mb=0;
	
vu8 detect_wait=0;	
	
	
vu32 delay=671;	  //17+10+28+28+28+28+28+28+280+28+28*6=13*28+280+27=
	
short Temperate = 0;
	

/* Private function prototypes -----------------------------------------------*/

void NVIC_Configuration(void);
void RCC_Configuration(void);
void Init_All_Periph(void);

void Delay(vu32 nCount);

u8 BP_Init(void);
void bed_up(void);
void bed_down(void);
void bed_stop(void);

void clear_USART1_RX_BUF(void);
void disable_INT8(void);
void fetch_bp(void);



//加载UI
//mode:
//bit0:0,不加载;1,加载前半部分UI
//bit1:0,不加载;1,加载后半部分UI
void lwip_test_ui(u8 mode)
{
	//u8 speed;
	u8 buf[30]; 
	POINT_COLOR=RED;
	if(mode&1<<0)
	{
		LCD_Fill(30,30,lcddev.width,110,WHITE);	//清除显示
		LCD_ShowString(30,30,200,16,16,"WARSHIP STM32F103");
		LCD_ShowString(30,50,200,16,16,"LWIP Webserver Test");
		LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
		LCD_ShowString(30,90,200,16,16,"2015/3/21"); 	
	}
	if(mode&1<<1)
	{
		LCD_Fill(30,110,lcddev.width,lcddev.height,WHITE);	//清除显示
		LCD_ShowString(30,110,200,16,16,"lwIP Init Successed");
		if(lwipdev.dhcpstatus==2)sprintf((char*)buf,"DHCP IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);//打印动态IP地址
		else sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);//打印静态IP地址
		LCD_ShowString(30,130,210,16,16,buf); 
		//speed=DM9000_Get_SpeedAndDuplex();//得到网速
		//if(speed&1<<1)LCD_ShowString(30,150,200,16,16,"Ethernet Speed:10M");
		//else LCD_ShowString(30,150,200,16,16,"Ethernet Speed:100M");
	}
}

void RCC_Configuration(void)
{
	SystemInit(); 	
	//使能串口模块1时钟,使能串口所有GPIO模块时钟,使能AFIO模块时钟	
	RCC_APB2PeriphClockCmd(
	RCC_APB2Periph_USART1 | 
	RCC_APB2Periph_GPIOA | 
	RCC_APB2Periph_GPIOB | 
	RCC_APB2Periph_GPIOC |
	RCC_APB2Periph_GPIOD | 
	RCC_APB2Periph_GPIOE | 
	RCC_APB2Periph_GPIOF |
	RCC_APB2Periph_GPIOG | 
	RCC_APB2Periph_AFIO, 
	ENABLE);	  //
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	
}

void NVIC_Configuration(void)
{ 
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	//设置优先级分组：先占优先级0位,从优先级4位
	                                                //                抢占优先级   ，响应优先级
	
	//设置向量表的位置和偏移
	#ifdef  VECT_TAB_RAM  
		/* Set the Vector Table base location at 0x20000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 		//向量表位于RAM
	#else  /* VECT_TAB_FLASH  */
		/* Set the Vector Table base location at 0x08000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   //向量表位于FLASH
	#endif


	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//USART1中断
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1


	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//USART2中断
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART2 
}

int main(void)
{	 
	u32 i;
	
	RCC_Configuration();
	delay_init();	    	//延时函数初始化	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	//NVIC_Configuration();
	
	GPIO_Configuration();
	
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			    //LED端口初始化
	LCD_Init();				//初始化LCD		

	RTC_Init();				//RTC初始化

	TIM3_Int_Init(1000,719);//定时器3频率为100hz
 	//usmart_dev.init(72);	//初始化USMART		

	POINT_COLOR = RED; 		
	
	printf("\n  DS18B20 TEST     \n");
	
	lwip_test_ui(1);		//加载前半部分UI		    
	
	while(lwip_comm_init()) //lwip初始化
	{
		LCD_ShowString(30,110,200,20,16,"LWIP Init Falied!");
		delay_ms(1200);
		LCD_Fill(30,110,230,130,WHITE); //清除显示
		LCD_ShowString(30,110,200,16,16,"Retrying...");  
	}
	LCD_ShowString(30,110,200,20,16,"LWIP Init Success!");
 	LCD_ShowString(30,130,200,16,16,"DHCP IP configing...");
#if LWIP_DHCP   //使用DHCP
	while((lwipdev.dhcpstatus!=2)&&(lwipdev.dhcpstatus!=0XFF))//等待DHCP获取成功/超时溢出
	{
		
		lwip_periodic_handle();	//LWIP内核需要定时处理的函数
		//lwip_pkt_handle();
	}
#endif
	lwip_test_ui(2);		//加载后半部分UI 
	delay_ms(500);			//延时1s
	delay_ms(500);
	httpd_init();  			//Web Server模式
	

//					delay_ms(6000);	
//					GPIO_WriteBit(GPIOF,GPIO_Pin_0,(BitAction)1);
//					delay_ms(6000);	
//					GPIO_WriteBit(GPIOF,GPIO_Pin_0,(BitAction)0);
//				
//			
//					delay_ms(6000);
//					GPIO_WriteBit(GPIOF,GPIO_Pin_1,(BitAction)1);
//					delay_ms(6000);
//					GPIO_WriteBit(GPIOF,GPIO_Pin_1,(BitAction)0);
//		
//					delay_ms(6000);	
//					GPIO_WriteBit(GPIOF,GPIO_Pin_2,(BitAction)1);
//					delay_ms(6000);	
//					GPIO_WriteBit(GPIOF,GPIO_Pin_2,(BitAction)0);
//			
//					delay_ms(6000);
//					GPIO_WriteBit(GPIOF,GPIO_Pin_3,(BitAction)1);
//					delay_ms(6000);
//					GPIO_WriteBit(GPIOF,GPIO_Pin_3,(BitAction)0);
//				

	
	printf("\n  DS18B20 TEST     \n");
		
	while(DS18B20_Init())//初始化DS18B20,兼检测18B20
	{  
		printf("\n  DS18B20 Check Failed!  \n");
			delay_ms(500);		  
			printf("\n  Please Check!     \n");
			delay_ms(500);
	}
		printf("\n DS18B20 Ready!        \n");
	
	

	
  	while(1)
	{	
		lwip_periodic_handle();
		//lwip_pkt_handle();  
		i++;
		//if(i%5000==0)  //LED0=!LED0;
		//delay_us(100);
			
//			GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)1);delay_ms(500);
//	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)0);
//	delay_ms(500);
//	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)1);
					
//		GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)1); 
//		delay_us(2);
//		GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)0);
//		delay_us(2);		
		//if(i==50){i=0;fetcht();}
		
	}
}
