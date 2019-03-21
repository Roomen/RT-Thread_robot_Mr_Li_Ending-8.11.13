/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup. 
 */

#include <board.h>
#include <rtthread.h>
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_oled.h"
#include "operation.h"
#include "robot_ui.h"
#include "bsp_grayscale.h"
#include "bsp_flash.h"
#include "track.h"
#include "robot_run.h"
#include "bsp_motor.h"
#include "bsp_compass.h"
#include "bsp_steering_engine.h"


#ifdef RT_USING_FINSH
#include <shell.h>
#include <finsh.h>
#endif


static void robot_hw_init(void);
static void robot_sw_init(void);


void rt_init_thread_entry(void* parameter)
{
	#ifdef RT_USING_FINSH
		/* init finsh */
		finsh_system_init();
	#endif
		robot_hw_init();
	    robot_sw_init();
}

int rt_application_init()
{
    rt_thread_t tid;
    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

static void robot_hw_init(void)
{   
//	oled_init();        //OLED��ʼ��
//	key_init();         //������ʼ��
//	led_init();         //LED��ʼ��
//    laser_init();       //���⴫������ʼ��
//    infrared_init();    //���⴫������ʼ��
	grayscale_init();   //�Ҷȴ�������ʼ��
	robot_ui_init();    //OLED UI��ʼ��
//	motor_init();       //���������ʼ��
//    hwt_init();         //ѭ�������ʼ��
//    travel_switch_init();//�г̿��س�ʼ��
//    steering_engine_init();//�����ʼ��
}

static void robot_sw_init(void)
{
    operation();
    robot_run();
}


