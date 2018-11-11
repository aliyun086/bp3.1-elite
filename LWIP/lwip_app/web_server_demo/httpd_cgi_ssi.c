#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "lwip_comm.h"
#include "led.h"
#include "beep.h"
#include "tsensor.h"
#include "rtc.h"
#include "lcd.h"
#include <string.h>
#include <stdlib.h>
#include "ds18b20.h"
//#include "stdio.h"
#include "hem7200.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//lwip通用驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
 

#define NUM_CONFIG_CGI_URIS	3  //CGI的URI数量
#define NUM_CONFIG_SSI_TAGS	5  //SSI的TAG数量

//控制LED和BEEP的CGI handler
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char* LEDS2_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char* BEEP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[]);


extern vu8 bp_fetch_permitted;
extern vu8 data_upload[15];
extern vu8 cmd_received;
extern vu32 clk_counter;
extern vu8 SDAA[2];
//extern uint64_t SDAAA;
extern vu8 g;
extern vu8 d;
extern vu8 mb;

extern short Temperate;

static const char *ppcTAGs[]=  //SSI的Tag
{
	"g", //高压，收缩压
	"d", //低压，舒张压
	"w", //温度值
	"h", //时间
	"y"  //日期
};

static const tCGI ppcURLs[]= //cgi程序
{
	{"/leds.cgi",LEDS_CGI_Handler},
	{"/leds2.cgi",LEDS2_CGI_Handler},
	{"/beep.cgi",BEEP_CGI_Handler},
};


//当web客户端请求浏览器的时候,使用此函数被CGI handler调用
static int FindCGIParameter(const char *pcToFind,char *pcParam[],int iNumParams)
{
	int iLoop;
	for(iLoop = 0;iLoop < iNumParams;iLoop ++ )
	{
		if(strcmp(pcToFind,pcParam[iLoop]) == 0)
		{
			return (iLoop); //返回iLOOP
		}
	}
	return (-1);
}


//SSIHandler中需要用到的处理G的函数 , 获取收缩压值的函数
void G_Handler(char *pcInsert)
{ 

	//char G=127;
	char Digit1=0, Digit2=0, Digit3=0; 
	
		if(g==0) fetch_bp();
	
	
		Digit1 = g / 100;
	  Digit2 = (((short)g) % 100)/10;
    Digit3 = ((short)g) % 10;

	//准备添加到html中的数据
	*pcInsert 	  = (char)(Digit1+0x30);
	*(pcInsert+1) = (char)(Digit2+0x30);
	*(pcInsert+2) =	(char)(Digit3+0x30);   
		
}


//SSIHandler中需要用到的处理D的函数 , 获取舒张压的函数
void D_Handler(char *pcInsert)
{ 
	//char D=98;
	char Digit1=0, Digit2=0, Digit3=0; 		

		if(d==0)fetch_bp();
	
		Digit1 = d / 100;
    Digit2 = (((short)d) % 100)/10;
    Digit3= ((short)d) % 10;

	//准备添加到html中的数据
	*pcInsert 	  = (char)(Digit1+0x30);
	*(pcInsert+1) = (char)(Digit2+0x30);
	*(pcInsert+2) =	(char)(Digit3+0x30);

		
}

//SSIHandler中需要用到的处理内部温度传感器的函数
void Temperate_Handler(char *pcInsert)
{
	char Digit1=0, Digit2=0, Digit3=0, Digit4=0,Digit5=0; 
	
	

	
	//获取内部温度值 
	Temperate = fetcht();//Get_Temprate();//扩大100倍
	//printf("----- %d ------\n",Temperate);
	
	Digit1 = Temperate / 10000;
	Digit2 = (((short)Temperate) % 10000)/1000;
    Digit3 = (((short)Temperate) % 1000)/100 ;
    Digit4 = (((short)Temperate) % 100)/10;
    Digit5 = ((short)Temperate) % 10;
	//添加到html中的数据
	*pcInsert 	  = (char)(Digit1+0x30);
	*(pcInsert+1) = (char)(Digit2+0x30);
	*(pcInsert+2) =	(char)(Digit3+0x30);
	*(pcInsert+3) = '.';
	*(pcInsert+4) = (char)(Digit4+0x30);
	*(pcInsert+5) = (char)(Digit5+0x30);
}

//SSIHandler中需要用到的处理RTC时间的函数
void RTCTime_Handler(char *pcInsert)
{
//	u8 hour,min,sec;
//	
//	hour = calendar.hour;
//	min  = calendar.min;
//	sec  = calendar.sec;
//	*pcInsert = 	(char)((hour/10) + 0x30);
//	*(pcInsert+1) = (char)((hour%10) + 0x30);
//	*(pcInsert+2) = ':';
//	*(pcInsert+3) = (char)((min/10) + 0x30);
//	*(pcInsert+4) = (char)((min%10) + 0x30);
//	*(pcInsert+5) = ':';
//	*(pcInsert+6) = (char)((sec/10) + 0x30);
//	*(pcInsert+7) = (char)((sec%10) + 0x30);

	char Digit1=0, Digit2=0, Digit3=0; 		

	if(mb==0)fetch_bp();

	Digit1 = mb / 100;
	Digit2 = (((short)mb) % 100)/10;
	Digit3= ((short)mb) % 10;

	//准备添加到html中的数据
	*pcInsert 	  = (char)(Digit1+0x30);
	*(pcInsert+1) = (char)(Digit2+0x30);
	*(pcInsert+2) =	(char)(Digit3+0x30);
}

//SSIHandler中需要用到的处理RTC日期的函数
void RTCdate_Handler(char *pcInsert)
{
	u16 year,month,date,week;

	year  = calendar.w_year;
	month = calendar.w_month;
	date  = calendar.w_date;
	week  = calendar.week;
	*pcInsert = '2';
	*(pcInsert+1) = '0';
	*(pcInsert+2) = (char)(((year%100)/10) + 0x30);
	*(pcInsert+3) = (char)((year%10) + 0x30);
	*(pcInsert+4) = '-';
	*(pcInsert+5) = (char)((month/10) + 0x30);
	*(pcInsert+6) = (char)((month%10) + 0x30);
	*(pcInsert+7) = '-';
	*(pcInsert+8) = (char)((date/10) + 0x30);
	*(pcInsert+9) = (char)((date%10) + 0x30);
	*(pcInsert+10) = ' ';
	*(pcInsert+11) = 'w';
	*(pcInsert+12) = 'e';
	*(pcInsert+13) = 'e';
	*(pcInsert+14) = 'k';
	*(pcInsert+15) = ':';
	*(pcInsert+16) = (char)(week + 0x30);
	
}
//SSI的Handler句柄
static u16_t SSIHandler(int iIndex,char *pcInsert,int iInsertLen)
{
	switch(iIndex)
	{
		case 0: 
				G_Handler(pcInsert);
				break;
		case 1: 
				D_Handler(pcInsert);
				break;
		case 2:
				Temperate_Handler(pcInsert);
				break;
		case 3:
				RTCTime_Handler(pcInsert);
				break;
		case 4:
				RTCdate_Handler(pcInsert);
				break;
		
	}
	return strlen(pcInsert);
}

//CGI LED控制句柄   未使用，将要改为床身抬起控制。
const char* LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	u8 i=0;  //注意根据自己的GET的参数的多少来选择i值范围
	iIndex = FindCGIParameter("LED1",pcParam,iNumParams);  //找到led的索引号
	if (iIndex != -1)
	{
		LED1=1;  
		for (i=0; i<iNumParams; i++) {
		  if (strcmp(pcParam[i] , "LED1")==0) {
				if(strcmp(pcValue[i], "LED1ON") ==0) {	
					delay_ms(1000);	
					GPIO_WriteBit(GPIOF,GPIO_Pin_0,(BitAction)1);
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);					
					GPIO_WriteBit(GPIOF,GPIO_Pin_0,(BitAction)0);
				}
				else if(strcmp(pcValue[i],"LED1OFF") == 0){
					delay_ms(1000);
					GPIO_WriteBit(GPIOF,GPIO_Pin_1,(BitAction)1);
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					GPIO_WriteBit(GPIOF,GPIO_Pin_1,(BitAction)0);
				}
		  }		
			
//		  if (strcmp(pcParam[i] , "BEEP")==0) {
//				if(strcmp(pcValue[i], "BEEPON") ==0) {	
//					delay_ms(1000);	
//					GPIO_WriteBit(GPIOF,GPIO_Pin_2,(BitAction)1);
//					delay_ms(5000);	
//					GPIO_WriteBit(GPIOF,GPIO_Pin_2,(BitAction)0);
//				}
//				else if(strcmp(pcValue[i],"BEEPOFF") == 0){
//					delay_ms(1000);
//					GPIO_WriteBit(GPIOF,GPIO_Pin_3,(BitAction)1);
//					delay_ms(5000);
//					GPIO_WriteBit(GPIOF,GPIO_Pin_3,(BitAction)0);
//				}
//		  }			
		  if (strcmp(pcParam[i] , "button2")==0) {
				if(strcmp(pcValue[i], "SEND") ==0)  
					printf("i 00 = %d !!!!!!! \n",i);								
		  }			
		}
	}


		return "/angle.shtml";	
}

//CGI LED控制句柄   未使用，将要改为床身抬起控制。
const char* LEDS2_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	u8 i=0;  //注意根据自己的GET的参数的多少来选择i值范围
	iIndex = FindCGIParameter("LED2",pcParam,iNumParams);  //找到led的索引号
	if (iIndex != -1)
	{
		LED1=1;  
		for (i=0; i<iNumParams; i++) {		 
			
		  if (strcmp(pcParam[i] , "LED2")==0) {
				if(strcmp(pcValue[i], "LED2ON") ==0) {	
					delay_ms(1000);	
					GPIO_WriteBit(GPIOF,GPIO_Pin_2,(BitAction)1);
					delay_ms(1000);	
					delay_ms(1000);
					delay_ms(1000);
					
					
					GPIO_WriteBit(GPIOF,GPIO_Pin_2,(BitAction)0);
				}
				else if(strcmp(pcValue[i],"LED2OFF") == 0){
					delay_ms(1000);
					GPIO_WriteBit(GPIOF,GPIO_Pin_3,(BitAction)1);
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
					
					GPIO_WriteBit(GPIOF,GPIO_Pin_3,(BitAction)0);
				}
		  }			
		  if (strcmp(pcParam[i] , "button2")==0) {
				if(strcmp(pcValue[i], "SEND") ==0)  
					printf("i 00 = %d !!!!!!! \n",i);								
		  }			
		}
	}

		return "/angle.shtml";	
}

//BEEP的CGI控制句柄   改为血压启动控制
const char *BEEP_CGI_Handler(int iIndex,int iNumParams,char *pcParam[],char *pcValue[])
{
	u8 detect_wait=0;

	iIndex = FindCGIParameter("button_bp",pcParam,iNumParams);  //找到BEEP的索引号

	if(iIndex != -1) 	//找到BEEP索引号
	{
		LED1=0; //打开LED1
		delay_ms(500);
		LED1=1; //关闭LED1
		delay_ms(500);
	
	
		GPIO_WriteBit(GPIOE,GPIO_Pin_2,(BitAction)1);	//启动测量
		delay_ms(550);
		GPIO_WriteBit(GPIOE,GPIO_Pin_2,(BitAction)0);
		delay_ms(550);		
		for(detect_wait=0;detect_wait<50;detect_wait++){		delay_ms(1000);		}
		GPIO_WriteBit(GPIOE,GPIO_Pin_2,(BitAction)1);	//关机
		delay_ms(550);
		GPIO_WriteBit(GPIOE,GPIO_Pin_2,(BitAction)0);		
		
		delay_ms(2550);
		
		fetch_bp();
  }

	return "/bp.shtml";
}

//SSI句柄初始化
void httpd_ssi_init(void)
{  
	//配置SSI句柄
	http_set_ssi_handler(SSIHandler,ppcTAGs,NUM_CONFIG_SSI_TAGS);
}

//CGI句柄初始化
void httpd_cgi_init(void)
{ 
	//配置CGI句柄
	http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);
}







