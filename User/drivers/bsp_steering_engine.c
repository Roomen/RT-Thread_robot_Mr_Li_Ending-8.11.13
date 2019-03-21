#include "bsp_steering_engine.h"
#include "operation.h"
#include "bsp_led.h"

struct STEERING_DRVICE steering;

/*
 * 舵机初始化，使用TIM4:Ch1:PD12,Ch2:PD13,Ch3:PD14,Ch4:PD15;
 * PWM频率 120Hz
 */
static void steering_engine_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 

    GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //GPIOD12
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4); //GPIOD13
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4); //GPIOD14
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4); //GPIOD15

    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;  //GPIOD
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD,&GPIO_InitStructure);
     
    TIM_TimeBaseStructure.TIM_Prescaler=3000-1;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseStructure.TIM_Period=200-1;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                  

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM4, &TIM_OCInitStructure); 
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    
    TIM_OC2Init(TIM4, &TIM_OCInitStructure); 
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 

    TIM_OC3Init(TIM4, &TIM_OCInitStructure); 
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  

    TIM_OC4Init(TIM4, &TIM_OCInitStructure);  
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
     
    TIM_ARRPreloadConfig(TIM4,ENABLE);

    TIM_Cmd(TIM4, ENABLE);
    
}

/*
 * 控制舵机函数
 * ch: 控制的舵机
 * turn_angle: 舵机的转动角度
 * e.g:steering_engine (steering_engine_1,30);
    * 范围:15~65  若值在这个范围之外，则会损坏舵机
    * 调试时使用的是DXW90，其他型号舵机可能会略有不同
 */
static void steering_engine (u8 ch,u16 trun_to_angle)
{
    rt_uint8_t max_steering_pwm,min_steering_pwm;
    max_steering_pwm=65;
    min_steering_pwm=15;
    if(trun_to_angle<min_steering_pwm)
        trun_to_angle=min_steering_pwm;
    else if(trun_to_angle>max_steering_pwm)
        trun_to_angle=max_steering_pwm;
    switch (ch)
    {
    	case 1:steering_pwm1=trun_to_angle;
    		break;
    	case 2:steering_pwm2=trun_to_angle;
    		break;
        case 3:steering_pwm3=trun_to_angle;
    		break;
        case 4:steering_pwm4=trun_to_angle;
    		break;
    	default:
    		break;
    }
}

void steering_all_init(void)
{
    steering.init = steering_engine_init;
    steering.turn_angle = steering_engine;
    steering.init();
}

INIT_BOARD_EXPORT(steering_all_init);

#include <finsh.h>


FINSH_FUNCTION_EXPORT(steering_engine,Steering engine turn to angle.)
