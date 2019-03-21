#ifndef __bsp_steering_engine_H__
#define __bsp_steering_engine_H__
#include "sys.h"
#include <rtthread.h>

#define steering_engine_1 1
#define steering_engine_2 2
#define steering_engine_3 3
#define steering_engine_4 4

#define steering_pwm1 TIM4->CCR1 
#define steering_pwm2 TIM4->CCR2 
#define steering_pwm3 TIM4->CCR3 
#define steering_pwm4 TIM4->CCR4 

struct STEERING_DRVICE
{
    void    (*init)             (void);
    void    (*turn_angle)       (u8 ch,u16 trun_to_angle);
};

extern struct STEERING_DRVICE steering;

//void steering_engine_init(void);
//void steering_engine (u8 ch,u16 trun_to_angle);



#endif

