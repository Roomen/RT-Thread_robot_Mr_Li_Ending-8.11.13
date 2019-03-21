#ifndef __BSP_LED_H__
#define __BSP_LED_H__
#include "sys.h"
#include <rtthread.h>
// led define

//#define led1_rcc                    RCC_AHB1Periph_GPIOF
//#define led1_gpio                   GPIOF
//#define led1_pin                    (GPIO_Pin_9)

//#define led2_rcc                    RCC_AHB1Periph_GPIOF
//#define led2_gpio                   GPIOF
//#define led2_pin                    (GPIO_Pin_10)

//#define LED1 PFout(9)
//#define LED2 PFout(10)

//LED
#define led1_rcc                    RCC_AHB1Periph_GPIOG
#define led1_gpio                   GPIOG
#define led1_pin                    (GPIO_Pin_15)
#define led0_pin                    (GPIO_Pin_6)

#define LED1 PGout(6)
#define LED0 PGout(15)

//Laser
#define laser0_rcc                    RCC_AHB1Periph_GPIOC
#define laser1_rcc                    RCC_AHB1Periph_GPIOD
#define laser0_gpio                   GPIOC
#define laser1_gpio                   GPIOD
#define laser0_pin                    (GPIO_Pin_1)
#define laser1_pin                    (GPIO_Pin_2)

#define laser_l PCin(1)
#define laser_r PDin(2)

//Infrared
#define Infrared_rcc                    RCC_AHB1Periph_GPIOE
#define Infrared_gpio                   GPIOE
#define Infrared_f_pin                    (GPIO_Pin_0)
#define Infrared_B_pin                    (GPIO_Pin_9)

#define Infrared_f PEin(0)
#define Infrared_B PEin(9)


/*
 * 红外初始化
 * 红外值: 
   * 0、检测到返回值    
   * 1、未检测到返回值
 */
#define hwt_rcc                    RCC_AHB1Periph_GPIOB
#define hwt_gpio                   GPIOB
#define hwt_l_pin                   (GPIO_Pin_8)
#define hwt_r_pin                   (GPIO_Pin_9)
#define hwt_door_pin                (GPIO_Pin_3)

#define hwt_l       PBin(9)
#define hwt_r       PBin(8)
#define hwt_door    PBin(3)

//travel_switch
#define travel_switch_rcc                    RCC_AHB1Periph_GPIOF
#define travel_switch_gpio                   GPIOF
#define travel_switch_pin                    (GPIO_Pin_3)

#define travel_switch PFin(3)

//void led_init(void);
//static void led_thread_entry(void* parameter);
//void led_init(void);
//void laser_init(void);
//int rt_led_init(void);
//void infrared_init(void);
//void hwt_init(void);
//void  travel_switch_init(void);

static void led(rt_uint8_t led,rt_uint8_t mode);

#endif
