#include "bsp_led.h"
#include <rtthread.h>
#include <rtdevice.h>
#include "operation.h"
#include "bsp_key.h"
//ALIGN(RT_ALIGN_SIZE)
//static rt_uint8_t led_stack[ 512 ];
//static struct rt_thread led_thread;

static void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(led1_rcc, ENABLE);
  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = led1_pin ;
	GPIO_Init(led1_gpio, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = led0_pin ;
	GPIO_Init(led1_gpio, &GPIO_InitStructure);
    
	GPIO_SetBits(led1_gpio,led1_pin);
    GPIO_SetBits(led1_gpio,led0_pin);

}

static void laser_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(laser0_rcc, ENABLE);
    RCC_AHB1PeriphClockCmd(laser1_rcc, ENABLE);


  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = laser0_pin ;
	GPIO_Init(laser0_gpio, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = laser1_pin ;
	GPIO_Init(laser1_gpio, &GPIO_InitStructure);
    
	GPIO_SetBits(laser0_gpio,laser0_pin);
    GPIO_SetBits(laser1_gpio,laser1_pin);
}

static void infrared_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(Infrared_rcc, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = Infrared_f_pin ;
	GPIO_Init(laser0_gpio, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = Infrared_B_pin ;
	GPIO_Init(Infrared_gpio, &GPIO_InitStructure);
    
//    GPIO_SetBits(Infrared_gpio,Infrared_B);
}

static void hwt_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(hwt_rcc, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = hwt_l_pin ;
	GPIO_Init(hwt_gpio, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = hwt_r_pin ;
	GPIO_Init(hwt_gpio, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = hwt_door_pin ;
	GPIO_Init(hwt_gpio, &GPIO_InitStructure);

}


static void  travel_switch_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(travel_switch_rcc, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = travel_switch_pin ;
	GPIO_Init(travel_switch_gpio, &GPIO_InitStructure);
    
    
//    GPIO_SetBits(Infrared_gpio,Infrared_B);
}

static void peripheral_init(void)
{
	led_init();         //LED初始化
    laser_init();       //激光传感器初始化
    infrared_init();    //红外传感器初始化
    hwt_init();         //循迹红外初始化
    travel_switch_init();//行程开关初始化
}

INIT_BOARD_EXPORT(peripheral_init);

#include <finsh.h>

static void led(rt_uint8_t led,rt_uint8_t mode)
{
    if(led==0)
    {
        switch (mode)
        {
            case 0: LED0=1;break;
            case 1: LED0=0;break;
            default: break;
        }
    }
    else
    {
        switch (mode)
        {
            case 0: LED1=1;break;
            case 1: LED1=0;break;
            default: break;
        }
    }
}

FINSH_FUNCTION_EXPORT(led,set led mood 0:off 1:on)


//LED线程

//static void led_thread_entry(void* parameter)
//{
//    LED0=!LED0;
//    while (1)
//    {
//        LED1 = !LED1;
//        LED0 = !LED0;
//        rt_thread_delay(500);
//    }
//}


//int rt_led_init(void)
//{
//    rt_thread_t init_thread;

//    rt_err_t result;

//    // init led thread 
//    result = rt_thread_init(&led_thread,
//                            "led",
//                            led_thread_entry,
//                            RT_NULL,
//                            (rt_uint8_t*)&led_stack[0],
//                            sizeof(led_stack),
//                            20,
//                            5);
//    if (result == RT_EOK)
//    {
//        rt_thread_startup(&led_thread);
//    }
//	
//	return 0;

//}

