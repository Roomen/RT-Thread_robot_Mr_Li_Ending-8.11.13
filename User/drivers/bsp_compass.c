#include <rtthread.h>
#include "bsp_compass.h"
#include "bsp_oled.h"
#include "bsp_led.h"
#include "app_uart.h"
#include "board.h"
#include "stdio.h"
#include "app_uart.h"   //需要有app_uart.c
#include "operation.h"

/*
 * Author:     李少宾
 * data:       2018.04.02
 * Function:   获取角度传感器值。提供两种硬件支持：GY26和JY61；
 */


u8 angle_test;
COMPASS_ANGLE angle;

//JY61
#ifdef RT_USE_JY61

void compass_thread_entry(void* parameter)
{
    rt_uint8_t uart_rx_data0[11],temp;
    if (uart_open("uart2") != RT_EOK)
    {
        rt_kprintf("uart open error.\n");
         while (1)
         {
            rt_thread_delay(10);
         }
    }
//    rt_kprintf("%s",uart_device);
    while(1)
    {
        temp  = uart_getchar();
        if(temp==0x55)  //寻找包头
        {
            uart_rx_data0[0]  = temp;
            temp = uart_getchar();
            if(temp==0x53)
            {
                uart_rx_data0[1]  = temp;
                uart_rx_data0[2]  = uart_getchar(); //RollL
                uart_rx_data0[3]  = uart_getchar(); //RollH
                uart_rx_data0[4]  = uart_getchar(); //PitchL
                uart_rx_data0[5]  = uart_getchar(); //PitchH
                uart_rx_data0[6]  = uart_getchar(); //YawL
                uart_rx_data0[7]  = uart_getchar(); //YawH
                uart_rx_data0[8]  = uart_getchar(); //TL
                uart_rx_data0[9]  = uart_getchar(); //TH
                uart_rx_data0[10] = uart_getchar(); //Sum=0x55+0x53+RollH+RollL+PitchH+PitchL+YawH+YawL+TH+TL
                angle.yaw=(rt_int16_t)((uart_rx_data0[7]<<8)|uart_rx_data0[6])/32768.0*180.0;    //Roll=((RollH<<8)|RollL)/32768*180(°)
                angle.pitch=(rt_int16_t)((uart_rx_data0[5]<<8)|uart_rx_data0[4])/32768.0*180.0;
                angle.roll=(rt_int16_t)((uart_rx_data0[3]<<8)|uart_rx_data0[2])/32768.0*180.0;
                
                if(angle_test)
                {
                    rt_kprintf("x:%d\ty:%d\tz:%d\n\r",angle.yaw,angle.pitch,angle.roll);
                    oled.show_number (34,2,angle.yaw,3,16);
                    oled.show_number (34,4,angle.pitch,3,16);
                    oled.show_number (34,6,angle.roll,3,16);
                    rt_thread_delay(10);
                }
//                    rt_kprintf("\n%d",Angle_y);
//                    if (use_compass==1)//test code
//                        rt_kprintf("x:%d\ty:%d\n\r",Angle_x,Angle_y);
                rt_thread_delay(5);
            }
            else 
                break;
        }
    }
}

#include <finsh.h>

static void show_compass()
{
    rt_kprintf("\nAngle_x:%d\tAnlge_y:%d\tAngle_z:%d",angle.yaw,angle.pitch,angle.roll);
}

FINSH_FUNCTION_EXPORT(show_compass, show the angle of the smart car in serial)



void compass_thread_creat(void)
{
    rt_thread_t tid;
    // 创建compass线程 
    tid = rt_thread_create("compass",
                    compass_thread_entry, 
                    RT_NULL,
                    0x200, 
                    10,
                    6);
    // 创建成功则启动线程 
    if (tid != RT_NULL)
        rt_thread_startup(tid);
}


#endif //end RT_USE_JY61
//GY26
#ifdef RT_USE_GY26

void compass_thread_entry(void* parameter)
{
    rt_uint8_t uart_rx_data0[8];
    if (uart_open("uart2") != RT_EOK)//打开串口
    {
        rt_kprintf("uart open error.\n");
         while (1)
         {
            rt_thread_delay(10);
         }
    }
    while(1)
    {
        if(use_compass==1||angle_test==1)
        {
            uart_rx_data0[0]  = uart_getchar();
            if(uart_rx_data0[0]==13)
            {
                uart_rx_data0[1]  = uart_getchar();
                uart_rx_data0[2]  = uart_getchar();
                uart_rx_data0[3]  = uart_getchar();
                uart_rx_data0[4]  = uart_getchar();
                uart_rx_data0[5]  = uart_getchar();
                uart_rx_data0[6]  = uart_getchar();
                uart_rx_data0[7]  = uart_getchar();
                Angle_x= (uart_rx_data0[2]-48)*100+(uart_rx_data0[3]-48)*10+uart_rx_data0[4]-48;
            }
            if(angle_test)
            {
                oled.show_number (34,4,Angle_x,3,14);
            }
//            rt_kprintf("%d\n\r",Angle_x);
            use_compass=0;
        }
        else
        rt_thread_delay(5);
    }
}

void compass_thread_creat(void)
{
    rt_thread_t tid;
    // 创建test线程
    tid = rt_thread_create("compass",
                    compass_thread_entry, 
                    RT_NULL,
                    1024, 
                    10, 
                    5);
    // 创建成功则启动线程 
    if (tid != RT_NULL)
        rt_thread_startup(tid);
}

void refresh_angle(void)
{
    USART_SendData(USART2,0x31);
    use_compass=1;
}

#endif  //end RT_USE_GY26

