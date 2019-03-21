#include "bsp_motor.h"
#include <rtthread.h>
#include <rtdevice.h>


struct MOTOR_DEVICE motor;

//电机方向初始化 
//PF1 PF3
static void Dir_Init(void)
{   //PD4、PD5
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOF时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
}

//电机PWM初始化 
//PB4 PB5 TIM3 ch1,ch2
static void PWM_Init(uint16_t arr,uint16_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 

//    GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3); //GPIOB?
//    GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3); //GPIOB?
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3); //GPIOB?′
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3); //GPIOB?

    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_4 | GPIO_Pin_5;  //GPIOB
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(GPIOB,&GPIO_InitStructure);               //
     
    TIM_TimeBaseStructure.TIM_Prescaler=psc;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseStructure.TIM_Period=arr;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);                  

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM3, &TIM_OCInitStructure); 
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
    TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 

//    TIM_OC3Init(TIM3, &TIM_OCInitStructure); 
//    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  

//    TIM_OC4Init(TIM3, &TIM_OCInitStructure);  
//    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
     
    TIM_ARRPreloadConfig(TIM3,ENABLE);

    TIM_Cmd(TIM3, ENABLE);
}

//dir:0向前  1向后
//RL_Tire：0右轮 1左轮
static void mot(rt_int16_t left_speed,rt_int16_t right_speed)
{
    if (left_speed > 0&&right_speed > 0)
    {
        pwm_l =1053-left_speed ;
        pwm_r =1053-right_speed;
        left_wheel = 1 ;
        right_wheel = 1 ;
    }
    else if (left_speed <= 0&&right_speed <= 0)
    {
        pwm_l =-left_speed;
        pwm_r =-right_speed;
        left_wheel = 0 ;
        right_wheel = 0 ;
    }
    else if (left_speed > 0&&right_speed <= 0)
    {
        pwm_l =left_speed;
        pwm_r =1053+right_speed;
        left_wheel = 1 ;
        right_wheel = 0 ;
    }
    else if (left_speed <= 0&&right_speed > 0)
    {
        pwm_l =1053+left_speed;
        pwm_r =right_speed;
        left_wheel = 0 ;
        right_wheel = 1 ;
    }
}

static void motor_stop(void)
{
    mot(0,0);
//	pwm_l =0;
//    pwm_r =0;
//    left_wheel = 0;
//    right_wheel = 0;
}

static void back_motor(rt_int16_t left_speed,rt_int16_t right_speed)
{
	mot(- left_speed,- right_speed);
}

//电机初始化
static void motor_init(void)
{
    Dir_Init();
	PWM_Init(1054-1,7-1);    //使用1054是为了当pwm为1000时，pwm占空比大于95%
	motor_stop();
}

void motot_all_init(void)
{
    motor.init = motor_init;
    motor.stop = motor_stop;
    motor.go_ahead = mot;
    motor.retreat = back_motor;
    motor.init();
}

INIT_BOARD_EXPORT(motot_all_init);
