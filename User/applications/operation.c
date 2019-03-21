#include <rtthread.h>
#include <stm32f4xx.h>
#include "operation.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_oled.h"
#include "robot_ui.h"
#include "bsp_grayscale.h"
#include "bsp_flash.h"
#include "bsp_compass.h"
#include "track.h"
#include "bsp_motor.h"
#include "map.h"
#include "soft_timer.h"

/*  变量分配4字节对齐 */
ALIGN(RT_ALIGN_SIZE)

/*  静态线程的 线程堆栈*/
static rt_uint8_t operation_stack[0x200];
static rt_uint8_t key_stack[0x100];
/* 静态线程的 线程控制块 */
struct rt_thread operation_thread;
struct rt_thread key_thread;

/*定义画面变量Frame(extern 到 robot_ui.c) ，并初始化为第一幅画面 ：RobotRun*/
rt_uint8_t Frame = RobotRun;
/*定义存放 从 邮箱中读取的数据 的变量keycode(extern 到 robot_ui.c)*/
rt_uint32_t keycode ;

rt_mailbox_t key_value_mb = RT_NULL;

uint8_t run_flag;

    



/*线程operation_thread的入口函数*/
static void operation_thread_entry(void* paramete);

/****************************************************************************
* 功    能: 创建按键扫描线程和operation线程，以及两个线程的邮箱
* 入口参数：无
* 出口参数：无
* 说    明：无
* 调用方法：无
****************************************************************************/
rt_err_t operation(void)
{
    rt_err_t result;

	key_value_mb = rt_mb_create("keycode value mb",1,RT_IPC_FLAG_FIFO);
	if(key_value_mb == RT_NULL)
	{
		rt_kprintf("keycode value mailbox create failed!\r\n");
	}

    /* 创建operation线程 ： 优先级 16 ，时间片 5个系统滴答 */
    result = rt_thread_init(&operation_thread,
                            "operation",
                            operation_thread_entry, RT_NULL,
                            (rt_uint8_t*)&operation_stack[0], sizeof(operation_stack), 16, 5);

    if (result == RT_EOK)
    {
        rt_thread_startup(&operation_thread);
    }

    /* 创建key线程 ： 优先级 15 ，时间片 5个系统滴答 */
    result = rt_thread_init(&key_thread,
                            "keycode",
                            key_thread_entry, RT_NULL,
                            (rt_uint8_t*)&key_stack[0], sizeof(key_stack), 15, 5);

    if (result == RT_EOK)
    {
        rt_thread_startup(&key_thread);
    }
    
    compass_thread_creat();
    GetGrayscale();
    return 0;
}

/****************************************************************************
* 功    能: 通过按键 显示 和 切换操作
* 入口参数：无
* 出口参数：无
* 说    明：无
* 调用方法：无
****************************************************************************/
void operation_thread_entry(void* paramete)
{ 
    
    /* 无限循环*/
    while (1)
    {
		switch(Frame)
		{
			case RobotRun:
						   RobotRun_Frame();
					       break;
			case GrayScale:	  
					       GrayScale_Frame();
						   break;
		    case Laser:   
						   Laser_Frame();
						   break;
			case Compass: 
						   Compass_Frame();
					       break;
			case Motor:    Motor_Frame();
					       break;
			case Gray_Front:
						   Gray_Front_Frame();
					       break;
			case Gray_Back:
						   Gray_Back_Frame();
						   break;
			case Gray_Bool:
						   GrayBool_Frame();
						   break;
			default:				
						   break;
		}
        if (rt_mb_recv(key_value_mb, &keycode, RT_WAITING_FOREVER) == RT_EOK)
        {
            /*判断邮件内容*/
			
			if(keycode == KEY_VALUE_ENSURE)   
			{

				switch(Frame)
				{
					case RobotRun:
						          RobotRun_Operation();
							      break;
					case Laser:	  
					              Laser_Operation();
							      break;
				    case Compass:   
								  Compass_Operation();
								  break;
					case Motor: 
						          Motor_Operation();
					              break;
					case Gray_Front:
           						  GrayFront_Operation();
					              break;
					case Gray_Back:
								  GrayBack_Operation();
						          break;
					case Gray_Bool:
						          Bool_Operation();
					              break;
					case GrayScale:
								  Frame = Gray_Front;
								  break;
					default:				
								  break;				
				}
			
			}					   
		   else if(keycode == KEY_VALUE_DOWN)
		   {
				if(Frame == Gray_Front)
				{
					Frame = Gray_Back;
				}
				else if(Frame == Gray_Back)
				{
					Frame = Gray_Bool;
				}
				else if(Frame == Gray_Bool)
				{
					Frame = Gray_Front ;
				}
				else
				{
					if(++Frame > Motor)
					{
						Frame = RobotRun;
					}
				}
			   
		   }
		   else if(keycode == KEY_VALUE_UP)
		   {
				if(Frame == Gray_Front)
				{
					Frame = Gray_Back;
				}
				else if(Frame == Gray_Back)
				{
					Frame = Gray_Front;
				}
				else
				{
					if(--Frame < RobotRun)
					{
						Frame = Motor;
					}
				}		
		   }
		   else if(keycode == KEY_VALUE_CANCEL)
		   {
			   if(Frame == Gray_Front||Frame == Gray_Back||Frame == Gray_Bool)
			   {
				   Frame = GrayScale;
			   }
		   }
        }	  
     }
}

void RobotRun_Operation(void)
{
	Flash_Read16BitDatas(FrontGrayscaleAddr,8,F_white_green);
	Flash_Read16BitDatas(FrontGrayscaleAddr+2*8,8,F_white_black);
	Flash_Read16BitDatas(FrontGrayscaleAddr+2*16,8,F_green_black);
	Flash_Read16BitDatas(BackGrayscaleAddr,8,B_white_green);
	Flash_Read16BitDatas(BackGrayscaleAddr+2*8,8,B_white_black);
	Flash_Read16BitDatas(BackGrayscaleAddr+2*16,8,B_green_black);
	
	run_flag = 1;
}

void GrayFront_Operation(void)
{
	static uint8_t flag = 0;
    static int16_t* color = F_White;
	oled.clear();
	oled.show_string(0,0,"white:",12);
	while(1)
	{
		Get_FGrayscale(color);
		Show_Grayscale(color);
		if (rt_mb_recv(key_value_mb, &keycode, 5) == RT_EOK)
		{
			if(flag == 0)
			{
				if(keycode == KEY_VALUE_ENSURE)
				{
				    oled.show_string(0,0,"green:",12);
					flag = 1 ;
					color = F_Green;
				}
				else if(keycode == KEY_VALUE_CANCEL)
				{
					Frame = Gray_Front ;
					break;
				}
			}
			else if(flag == 1)
			{
				if(keycode == KEY_VALUE_ENSURE)
				{
					oled.show_string(0,0,"black:",12);		
					color = F_Black;
					flag = 2;
				}
				else if(keycode == KEY_VALUE_CANCEL)
				{
					Frame = Gray_Front ;
					break;
				}
			}
			else if(flag == 2)
			{
				if(keycode == KEY_VALUE_ENSURE)
				{
                    /*flash 储存 average of white-green*/
                    /*flash 储存 average of white-black*/
					/*flash 储存 average of green-black*/
					
					SaveSystemParam(F_Grayscale);
					
					oled.show_string(0,6,"save completed:",12);	
					if (rt_mb_recv(key_value_mb, &keycode, RT_WAITING_FOREVER) == RT_EOK)
					{
						if(keycode == KEY_VALUE_CANCEL)
						{
							flag = 0;
							color = F_White;
							Frame = Gray_Front ;
							break;
						}
					}
					
				}
				else if(keycode == KEY_VALUE_CANCEL)
				{
					flag = 0;
					color = F_White;					
					Frame = Gray_Front ;
					break;
				}
			}
		}
	}

}

void GrayBack_Operation(void)
{
	static uint8_t flag = 0; 
	static int16_t* color = B_White;
	oled.clear();
	oled.show_string(0,0,"white:",12);
	while(1)
	{
		Get_BGrayscale(color);
		Show_Grayscale(color);
		if (rt_mb_recv(key_value_mb, &keycode, 5) == RT_EOK)
		{
			if(flag == 0)
			{
				 	
				if(keycode == KEY_VALUE_ENSURE)
				{
					oled.show_string(0,0,"green:",12);
					flag = 1 ;
					color = B_Green;
				}
				else if(keycode == KEY_VALUE_CANCEL)
				{
					Frame = Gray_Back;
					break;
				}
			}
			else if(flag == 1)
			{
	
				if(keycode == KEY_VALUE_ENSURE)
				{
					oled.show_string(0,0,"black:",12);
					color = B_Black;
					flag = 2;
				}
				else if(keycode == KEY_VALUE_CANCEL)
				{
					Frame = Gray_Back;
					break;
				}
			}
			else if(flag == 2)
			{
				if(keycode == KEY_VALUE_ENSURE)
				{
                    /*flash 储存 average of white-green*/
                    /*flash 储存 average of white-black*/
					/*flash 储存 average of green-black*/
					SaveSystemParam(B_Grayscale);
					
					oled.show_string(0,2,"storage completed:",12);	
					if (rt_mb_recv(key_value_mb, &keycode, RT_WAITING_FOREVER) == RT_EOK)
					{
						if(keycode == KEY_VALUE_CANCEL)
						{
							flag = 0;
							color = B_White;
							Frame = Gray_Back;
							break;
						}
					}
				}
				else if(keycode == KEY_VALUE_CANCEL)
				{
					flag = 0;
					color = B_White;
					Frame = Gray_Back;
					break;
				}
			}
		}
	}
}	

void Laser_Operation(void)
{
	/*left :   right :  获取激光的值并显示*/
	oled.clear();
    oled.show_string(0,0,"Laser:",12);
    oled.show_string(0,1,"L:",12);
    oled.show_string(48,1,"R:",12);
    oled.show_string(0,3,"Infrared:",12);
    oled.show_string(0,4,"F:",12);
    oled.show_string(48,4,"B:",12);
    oled.show_string(0,5,"hw:",12);
    oled.show_string(0,6,"L:",12);
    oled.show_string(48,6,"R:",12);
    
	while(1)
	{
        oled.show_number(24,1,laser_l,1,12);
        oled.show_number(72,1,laser_r,1,12);
        oled.show_number(24,4,Infrared_f,1,12);
        oled.show_number(72,4,Infrared_B,1,12);
        oled.show_number(24,6,hwt_l,1,12);
        oled.show_number(72,6,hwt_r,1,12);
		if(rt_mb_recv(key_value_mb, &keycode, 5) == RT_EOK)
		{
            if(keycode == KEY_VALUE_CANCEL) break;
		}
	}
}

void Compass_Operation(void)
{
	oled.clear();
	while(1)
	{
        angle_test=1;
        #ifdef RT_USE_GY26
            USART_SendData(USART2,0x31);
        #endif
		if(rt_mb_recv(key_value_mb, &keycode,1) == RT_EOK)
		{
            if(keycode == KEY_VALUE_CANCEL)
            {
                rt_thread_delay(10);
                angle_test=0;
                break;
            }
		}
        rt_thread_delay(10);
	}
}

void Motor_Operation(void)
{
	oled.clear();
	while(1)
	{
		oled.show_string(0,0,"L_Speed:",12);
		oled.show_string(0,2,"R_Speed:",12);
		if(rt_mb_recv(key_value_mb, &keycode, RT_WAITING_FOREVER) == RT_EOK)
		{
            while(1)
            {
                if(laser_l == 0)
                {
                    motor.go_ahead(200,200);
                }
                else
                {
                    motor.stop();
                }
                if(!PCin(10))
                {
                    motor.stop();
                    break;
                }
            }
		}
	}
}

void Bool_Operation(void)
{
	static uint8_t flag = 0;
	static int16_t* F_color = F_white_green;
	static int16_t* B_color = B_white_green;

	Flash_Read16BitDatas(FrontGrayscaleAddr,8,F_white_green);
	Flash_Read16BitDatas(FrontGrayscaleAddr+2*8,8,F_white_black);
	Flash_Read16BitDatas(FrontGrayscaleAddr+2*16,8,F_green_black);
	
	
	Flash_Read16BitDatas(BackGrayscaleAddr,8,B_white_green);
	Flash_Read16BitDatas(BackGrayscaleAddr+2*8,8,B_white_black);
	Flash_Read16BitDatas(BackGrayscaleAddr+2*16,8,B_green_black);
	
	oled.clear();
	oled.show_string(0,0,"white-green:",12);
	oled.show_string(0,2,"Front:",12);
	oled.show_string(0,4,"Back:",12);
	while(1)
	{
		F_Compare(F_color);
		B_Compare(B_color);
		Show_BackBool();
		Show_FrontBool();
		
		
		if (rt_mb_recv(key_value_mb, &keycode, 2) == RT_EOK)
		{
			if(flag == 0)
			{
				if(keycode == KEY_VALUE_ENSURE)
				{
				    oled.show_string(0,0,"white-black:",12);
					flag = 1 ;
					F_color = F_white_black;
					B_color = B_white_black;				
				}
				else if(keycode == KEY_VALUE_CANCEL)
				{
					Frame = Gray_Bool ;
					break;
				}
			}
			else if(flag == 1)
			{
				if(keycode == KEY_VALUE_ENSURE)
				{
					oled.show_string(0,0,"green-black:",12);		
					F_color = F_green_black;
					B_color = B_green_black;
					flag = 2;
				}
				else if(keycode == KEY_VALUE_CANCEL)
				{
					Frame = Gray_Bool ;
					break;
				}
			}
			else if(flag == 2)
			{				 		

			    if(keycode == KEY_VALUE_CANCEL)
				{
					flag = 0;
					F_color = F_white_green;
					B_color = B_white_green;					
					Frame = Gray_Bool ;
					break;
				}
			}
		}
	}
}

