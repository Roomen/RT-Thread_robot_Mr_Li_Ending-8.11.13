#include <rtthread.h>
#include "robot_run.h"
#include "operation.h"
#include "bsp_led.h"
#include "bsp_oled.h"
#include "bsp_grayscale.h"
#include "track.h"
#include "bsp_motor.h"
#include "map.h"
#include "bsp_compass.h"

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t run_stack[1024];
struct rt_thread run_thread;

static void run_thread_entry(void* parameter)
{
    while(1)
    {
        if(run_flag == 1)
       {
            rt_thread_detach(&operation_thread);
            rt_thread_detach(&key_thread);
            oled.clear();
            run_map_init();
           //获取初始角度值
            rt_thread_delay(10);
            break;
        }
       else
        rt_thread_delay(100);
    }
    rt_thread_suspend(&run_thread);
}

int robot_run(void)
{
    rt_err_t result;

    /* init run_thread thread */
    result = rt_thread_init(&run_thread,
                            "run",
                            run_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&run_stack[0],
                            sizeof(run_stack),
                            16,
                            5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&run_thread);
    }
	
	return 0;

}





