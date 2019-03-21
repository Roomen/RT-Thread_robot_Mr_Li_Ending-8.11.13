#ifndef __BSP_MOTOR_H__
#define __BSP_MOTOR_H__
#include "sys.h" 
#include <rtthread.h>

#define left_wheel PDout(4)
#define right_wheel PDout(5)

#define pwm_r TIM3->CCR1
#define pwm_l TIM3->CCR2

//void motor_stop(void);
//void motor_init(void);
//void Dir_Init(void);
//void PWM_Init(uint16_t arr,uint16_t psc);

//void mot(rt_int8_t lift_speed,rt_int8_t right_speed);
//void motor(rt_int8_t speed_left,rt_int8_t speed_right);


struct MOTOR_DEVICE
{
	void	(*init)		(void);
	void	(*stop)     (void);
	void    (*go_ahead) (rt_int16_t lift_speed,rt_int16_t right_speed);
	void    (*retreat)  (rt_int16_t lift_speed,rt_int16_t right_speed);
};

extern struct MOTOR_DEVICE motor;

#endif
