#include <rtthread.h>
#ifndef __OPERATION_H__
#define __OPERATION_H__

enum FRAME
{
	RobotRun = 1,
    GrayScale,
	Laser,
	Compass,
	Motor,	
	Gray_Front,
	Gray_Back,  
    Gray_Bool  
	
};





rt_err_t operation(void);
void RobotRun_Operation(void);
void GrayFront_Operation(void);
void GrayBack_Operation(void);
void Laser_Operation(void);
void Compass_Operation(void);
void Motor_Operation(void);
void Bool_Operation(void);

extern rt_mailbox_t key_value_mb;
extern uint8_t run_flag;
//extern rt_mutex_t run_mutex;

extern struct rt_thread operation_thread;
extern struct rt_thread key_thread;
#endif
