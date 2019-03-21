#include "track.h"
#include "bsp_grayscale.h"
#include "bsp_motor.h"
#include "bsp_led.h"
#include <finsh.h>
#include <rtthread.h>
#include "usart.h"
#include "operation.h"
#include <stdio.h>
#include "bsp_compass.h"
#include "robot_run.h"
#include "bsp_oled.h"
#include "map.h"
#include "bsp_flash.h"

//单灰度向前循迹
void ForwardTrack(rt_int16_t l_speed,rt_int16_t r_speed)
{
    rt_mutex_take(grayscale_mutex,RT_WAITING_FOREVER);
    
    
    if(f[2]&&f[3]&&f[4]&&f[5])  motor.go_ahead(l_speed,       r_speed);
    //deviation:+-1
    else if(f[2]&&f[3]&&f[4])   motor.go_ahead(l_speed*0.90,  r_speed);
    else if(f[3]&&f[4]&&f[5])   motor.go_ahead(l_speed,       r_speed*0.90);
    else if(f[3]&&f[4])         motor.go_ahead(l_speed,       r_speed);
    //deviation:+-3
    else if(f[1]&&f[2]&&f[3])   motor.go_ahead(l_speed*0.85,  r_speed);
    else if(f[4]&&f[5]&&f[6])   motor.go_ahead(l_speed,       r_speed*0.85);
    //deviation:+-2
    else if(f[2]&&f[3])         motor.go_ahead(l_speed*0.85,  r_speed);
    else if(f[4]&&f[5])         motor.go_ahead(l_speed,       r_speed*0.85);
    //deviation:+-4
    else if(f[1]&&f[2])         motor.go_ahead(l_speed*0.80,   r_speed);
    else if(f[5]&&f[6])         motor.go_ahead(l_speed,       r_speed*0.80);
    //deviation:+-6
    else if(f[0]&&f[1])         motor.go_ahead(l_speed*0.7,   r_speed);
    else if(f[6]&&f[7])         motor.go_ahead(l_speed,       r_speed*0.7);
    //deviation:+-5
    else if(f[1])               motor.go_ahead(l_speed*0.78,   r_speed);
    else if(f[6])               motor.go_ahead(l_speed,       r_speed*0.78);
    
    else if(f[0])               motor.go_ahead(l_speed,       r_speed*0.78);
    
    else                        motor.go_ahead(l_speed*0.7,   r_speed*0.7);
//    if(f[2]&&f[3]&&f[4]&&f[5])  motor.go_ahead(l_speed,       r_speed);
//    //deviation:+-1
//    else if(f[2]&&f[3]&&f[4])   motor.go_ahead(l_speed*0.95,  r_speed);
//    else if(f[3]&&f[4]&&f[5])   motor.go_ahead(l_speed,       r_speed*0.95);
//    else if(f[3]&&f[4])         motor.go_ahead(l_speed,       r_speed);
//    //deviation:+-3
//    else if(f[1]&&f[2]&&f[3])   motor.go_ahead(l_speed*0.89,  r_speed);
//    else if(f[4]&&f[5]&&f[6])   motor.go_ahead(l_speed,       r_speed*0.90);
//    //deviation:+-2
//    else if(f[2]&&f[3])         motor.go_ahead(l_speed*0.915,  r_speed);
//    else if(f[4]&&f[5])         motor.go_ahead(l_speed,       r_speed*0.92);
//    //deviation:+-4
//    else if(f[1]&&f[2])         motor.go_ahead(l_speed*0.85,   r_speed);
//    else if(f[5]&&f[6])         motor.go_ahead(l_speed,       r_speed*0.85);
//    //deviation:+-6
//    else if(f[0]&&f[1])         motor.go_ahead(l_speed*0.7,   r_speed);
//    else if(f[6]&&f[7])         motor.go_ahead(l_speed,       r_speed*0.7);
//    //deviation:+-5
//    else if(f[1])               motor.go_ahead(l_speed*0.78,   r_speed);
//    else if(f[6])               motor.go_ahead(l_speed,       r_speed*0.78);
//    
//    else if(f[0])               motor.go_ahead(l_speed,       r_speed*0.78);
//    
//    else                        motor.go_ahead(l_speed*0.7,   r_speed*0.7);
        rt_mutex_release(grayscale_mutex);
//        rt_kprintf("\nf:  %d  %d  %d  %d  %d  %d  %d  %d",
//            !f[0],!f[1],!f[3],!f[3],!f[4],!f[5],!f[6],!f[7]);
        rt_thread_delay(1);
}

//双灰度向前循迹
void Forward_Track_all(rt_int16_t l_speed,rt_int16_t r_speed)
{
    rt_mutex_take(grayscale_mutex,RT_WAITING_FOREVER);
    
    if(Front_Med && Back_Med)           {motor.go_ahead(l_speed,r_speed);}             // test_1       tese_2
    else if(Front_Med && Back_Right)    {motor.go_ahead(l_speed*0.86,r_speed*0.98);}   //0.86 0.98
    else if(Front_Med && Back_Left)     {motor.go_ahead(l_speed*0.98,r_speed*0.86);}   //0.98 0.86

    else if(Front_Left && Back_Right)   {motor.go_ahead(l_speed*0.78,r_speed*0.87);}   //0.78 0.87   0.75 0.85
    else if(Front_Left && Back_Med)     {motor.go_ahead(l_speed*0.85,r_speed*0.95);}   //0.85 0.95
    else if(Front_Left && Back_Left)    {motor.go_ahead(l_speed*0.82,r_speed*0.93);}   //0.82 0.93   0.85 0.93

    else if(Front_Right && Back_Left)   {motor.go_ahead(l_speed*0.87,r_speed*0.78);}   //0.87 0.78   0.85 0.75
    else if(Front_Right && Back_Med)    {motor.go_ahead(l_speed*0.95,r_speed*0.85);}   //0.95 0.85
    else if(Front_Right && Back_Right)  {motor.go_ahead(l_speed*0.93,r_speed*0.82);}   //0.93 0.82   0.93 0.85
    
    else if(Front_RRight )              {motor.go_ahead(l_speed*0.8,r_speed*0.5);}
    else if(Front_LLeft )               {motor.go_ahead(l_speed*0.5,r_speed*0.8);}

    else    {motor.go_ahead(400,400);}
/**/
//    rt_kprintf("\nf:  %d  %d  %d  %d  %d  %d  %d  %d",
//            !f[0],!f[1],!f[3],!f[3],!f[4],!f[5],!f[6],!f[7]);
//    rt_kprintf("\tb:  %d  %d  %d  %d  %d  %d  %d  %d",
//             !b[7],!b[6],!b[5],!b[4],!b[3],!b[2],!b[1],!b[0]);
        rt_mutex_release(grayscale_mutex);
        rt_thread_delay(2);
}

//单灰度向后循迹
void BackTrack(rt_int16_t l_speed,rt_int16_t r_speed)
{
	rt_mutex_take(grayscale_mutex,RT_WAITING_FOREVER);
	
	if(b[2]&&b[3]&&b[4]&&b[5])      motor.retreat(l_speed,      r_speed);
    //deviation:+-1
    else if(b[2]&&b[3]&&b[4])       motor.retreat(l_speed,      r_speed*0.95);
    else if(b[3]&&b[4]&&b[5])       motor.retreat(l_speed*0.95, r_speed);
    else if(b[3]&&b[4])             motor.retreat(l_speed,      r_speed);
    //deviatbon:+-3
    else if(b[1]&&b[2]&&b[3])       motor.retreat(l_speed,      r_speed*0.89);
    else if(b[4]&&b[5]&&b[6])       motor.retreat(l_speed*0.90, r_speed);
    //deviatbon:+-2
    else if(b[2]&&b[3])             motor.retreat(l_speed,      r_speed*0.92);
    else if(b[4]&&b[5])             motor.retreat(l_speed*0.915,r_speed);
    //deviatbon:+-4
    else if(b[1]&&b[2])             motor.retreat(l_speed,      r_speed*0.85);
    else if(b[5]&&b[6])             motor.retreat(l_speed*0.85, r_speed);
    //deviatbon:+-6
    else if(b[0]&&b[1])             motor.retreat(l_speed,      r_speed*0.7);
    else if(b[6]&&b[7])             motor.retreat(l_speed*0.7,  r_speed);
    //deviatbon:+-5
    else if(b[1])                   motor.retreat(l_speed,      r_speed*0.78);
    else if(b[6])                   motor.retreat(l_speed*0.78, r_speed);
    
    else                            motor.retreat(l_speed*0.7,   r_speed*0.7);
        rt_mutex_release(grayscale_mutex);

	rt_mutex_release(grayscale_mutex);
	
	rt_thread_delay(10);
}

//前灰度跑道循迹到板上
void Road_to_Board(rt_uint16_t l_speed,rt_uint16_t r_speed)   
{
    while(!(angle.pitch>300&&angle.pitch<350))
    {
        Color = Front_Back_WhiteGreen;
        Forward_Track_all(l_speed,r_speed);
        rt_thread_delay(5);
    }
    motor.stop();
}

//循迹一段时间,单位：ms;mode:0:danhui
void Road_to_time(rt_int16_t l_speed,rt_int16_t r_speed,rt_uint32_t ms,u8 mode)   
{
    ms=ms/14;
    while(1)
	{
       
		if(ms--)
        {
            if(mode)
            {
                Color = Front_Back_WhiteGreen;
                Forward_Track_all(l_speed,r_speed);//Forward_Track_all
                rt_thread_delay(5);
            }
            else
            {
                if(l_speed>=0&&r_speed>=0)
                {
                    Color = Front_WhiteGreen;//Front_Back_WhiteGreen
                    ForwardTrack(l_speed,r_speed);//Forward_Track_all
                    rt_thread_delay(5);
                }
                else
                {
                    Color = Back_WhiteGreen;//Front_Back_WhiteGreen
                    BackTrack(-l_speed,-r_speed);//Forward_Track_all
                    rt_thread_delay(5);
                }
            }
        }
        else
		{
			break;
		}
	}
    motor.stop();
}

//motor.go_ahead一段时间，单位未知。。。
void mot_to_time(rt_int16_t l_speed,rt_int16_t r_speed,rt_uint32_t ms)
{
    while(ms--)
    {
        motor.go_ahead(l_speed,r_speed);
        rt_thread_delay(1);
    }
    motor.stop();
}

//双灰度循迹一段时间，单位ms(准确)
void front_all_track_with_time(rt_int16_t l_speed,rt_int16_t r_speed,rt_uint32_t ms)
{
    ms/=7;
    while(1)
	{
        Color = Front_Back_WhiteGreen;//Front_Back_WhitGren
		if(ms--)
        {
			Forward_Track_all(l_speed,r_speed);//Forward_Track_all
            rt_thread_delay(5);
        }
        else
			break;
    }
}

//travel_switch
void road2view_spot(rt_uint16_t speed)
{
    Color = Front_Back_WhiteGreen;
    while(Infrared_f)
    {
        Forward_Track_all(speed,speed);
    }
    while(travel_switch)
    {
        Forward_Track_all(300,300);
    }
}

// 原地转弯指定角度  test speed：500
#ifdef RT_USE_JY61
void turn_angle(int ang,int l_speed,int r_speed)
{
    int angle0,angle1,angle2;//angle0为初始角，angle1为目标角度，angle2为首次转角（为了使转角更快更稳，采用两段式转发）
    rt_thread_delay(11);
    angle0=angle.yaw;
    angle1=(angle0-ang+360)%360;
    angle2=(angle0-ang/2+360)%360;
    if(l_speed>=500&&r_speed>=500)
    {
        l_speed=350;
        r_speed=350;
    }
    if(ang>0)
    {
        while(!(angle2==angle.yaw||angle2==angle.yaw-1||angle2==angle.yaw+1||angle2==angle.yaw+2))//先以设定速度转一半
        {
            motor.go_ahead(-l_speed,r_speed);
            rt_thread_delay(5);
        }
        motor.stop();
        rt_thread_delay(10);
        while(!(angle1==angle.yaw||angle1==angle.yaw-1||angle1==angle.yaw+1))//以100的速度转到指定角度
        {
            motor.go_ahead(-150,150);
            rt_thread_delay(5);
        }
    }
    else
    {
        while(!(angle2==angle.yaw||angle2==angle.yaw-1||angle2==angle.yaw-2||angle2==angle.yaw+1))
        {
            motor.go_ahead(l_speed,-r_speed);
            rt_thread_delay(5);
        }
        motor.stop();
        rt_thread_delay(10);
        while(!(angle1==angle.yaw||angle1==angle.yaw-1||angle1==angle.yaw-2||angle1==angle.yaw+1))
        {
            motor.go_ahead(150,-150);
            rt_thread_delay(5);
        }
    }
    motor.stop();
    rt_thread_delay(100);
}

void turn2angle(int angle,int l_speed,int r_speed)
{
    rt_uint16_t angle0,angle1;
    angle0=angle;
    angle1=angle0-angle;
    turn_angle(angle1,l_speed,r_speed);
}



#endif

//下平台8
void lower_platform8(u16 speed)
{
    while(angle.pitch<5||angle.pitch>350)
    {
        motor.go_ahead(265,250);//270,250
        rt_thread_delay(5);
    }
    mot_to_time(200,200,100);
    rt_thread_delay(100);
    Color = Front_WhiteBlack;//Front_Back_WhiteGreen
    while(angle.pitch>5&&angle.pitch<350)
    {
        ForwardTrack(speed,speed);//Forward_Track_all // ForwardTrack
        rt_thread_delay(5);
    }
    motor.stop();
}
//线性加速循迹
void slow_acceleration(rt_uint32_t uint_speed,rt_uint32_t end_speed,rt_uint32_t ms)
{
    ms/=6;
    rt_uint32_t mms;
    mms=ms;
    Color = Front_Back_WhiteGreen;
    for(;mms>0;mms--)
    {
        Forward_Track_all(   uint_speed+((rt_uint32_t)((end_speed-uint_speed)*((1-(float)mms/ms)))),
                        uint_speed+((rt_uint32_t)((end_speed-uint_speed)*((1-(float)mms/ms)))));//Forward_Track_all // ForwardTrack
//        rt_kprintf("\n%d",uint_speed+((rt_uint32_t)((end_speed-uint_speed)*((1-(float)mms/ms)))));
        rt_thread_delay(5);
    }
    motor.stop();
}
//下平台
void lower_platform(u16 speed)
{
//    int angle0,num;//,angle1,angle2;
    rt_thread_delay(21);
//    angle0=angle.pitch;
    while(angle.pitch<5||angle.pitch>350)
    {
        motor.go_ahead(300,300);
        rt_thread_delay(5);
    }
    motor.stop();
    rt_thread_delay(100);
//    num=40;
    Color = Front_WhiteGreen;
    while(angle.pitch>5&&angle.pitch<350)
    {
        ForwardTrack(speed,speed);//Forward_Track_all
        rt_thread_delay(5);
    }
    motor.stop();
}
//上平台
void platform(void)
{
    
    while(angle.pitch<350&&angle.pitch>5)
    {
        Color = Front_WhiteBlack;
        ForwardTrack(350,350);
        rt_thread_delay(5);
    }
    mot_to_time(200,200,250);
}
//红边循迹
void hw_track(rt_int16_t speed)
{
    Color = Front_GreenBlack;
    if(((!hwt_l) && hwt_r)||f[0])
        motor.go_ahead(speed,speed*0.88);
    else if(hwt_l && (!hwt_r)||f[7])
        motor.go_ahead(speed*0.88,speed);
    else
        motor.go_ahead(speed,speed);
}
//红外延时循迹
void hw_track_with_time(rt_int16_t speed,rt_uint16_t ms)
{
    ms/=5;
    while(ms--)
    {
        hw_track(speed);
        rt_thread_delay(5);
    }
    motor.stop();
}
//通过长桥
void though_brige(u16 speed)
{
    
    while(!(angle.pitch<5||angle.pitch>355))
    {
        hw_track(300);
        rt_thread_delay(5);
    }
    mot_to_time(300,300,300);
    while((angle.pitch<5||angle.pitch>355))
    {
        hw_track(speed);
        rt_thread_delay(5);
    }
    mot_to_time(400,400,300);
    while(!(angle.pitch<18||angle.pitch>355))
    {
//        ForwardTrack(200,200);
//        rt_thread_delay(5);
        motor.go_ahead(200,200);
//        hw_track(speed);
        rt_thread_delay(5);
    }
    motor.stop();
//    rt_thread_delay(100);
//    turn_right_angle(150);
//    rt_thread_delay(100);
}

static void turn_right_angle(rt_int32_t speed)
{
    rt_int8_t right_flag=0;
    Color = Front_WhiteGreen;
    while(!(f[3]||f[4]))//先以设定速度转一半
    {
        if(f[0]||f[1]||f[2])
        {
            motor.go_ahead(1,speed);
            rt_thread_delay(5);
            right_flag=1;
        }
        else if (f[5]||f[6]||f[7])
        {
            motor.go_ahead(speed,1);
            rt_thread_delay(5);
            right_flag=-1;
        }
        else if(f[3]||f[4])
            break;
    }
    if(right_flag!=0)
        turn_angle(right_flag*3,150,150);
    
    rt_thread_delay(100);
    motor.stop();
//    use_compass=0;
}


//通过跷跷板
void through_qqb(void)
{
    Road_to_Board(400,400);
    hw_track_with_time(300,680);
    while(angle.pitch<=350&&angle.pitch>300)
    {
        hw_track(100);
    }
    rt_thread_delay(450);
    Road_to_time(300,300,500,0);
    motor.stop();
}
//通过倾斜路面
void road_to_sloping(void)
{
    Color = Front_WhiteGreen ;rt_thread_delay(10);
    while(f[0]||f[1]||f[2]||f[3]||f[4]||f[5]||f[6]||f[7])
    {
        Color = Front_WhiteGreen;//Front_GreenBlack ;//Front_WhiteGreen;
        ForwardTrack(500,500);
        rt_thread_delay(5);
    }
}
//循迹到路口
void truck2one_point(u8 point,u16 speed,u8 mode)//mode:0、单灰度循迹  1、双灰度循迹
{
    while(!f[point])
    {
        if (mode)
        {
            Color = Front_Back_WhiteGreen;
            Forward_Track_all(speed,speed);
            rt_thread_delay(5);
        }
        else
        {
            Color = Front_WhiteGreen;
            ForwardTrack(speed,speed);
            rt_thread_delay(5);
        }
    }
    motor.stop();
}
//反向循迹到路口
void back_truck2one_point(u8 point,u16 speed)
{
    while(!b[point])
    {
            Color = Back_WhiteGreen;
            BackTrack(speed,speed);
            rt_thread_delay(5);
    }
    motor.stop();
}
//循迹到路口并用激光
void truck2one_point_use_laser(u8 point,u16 speed,u8 mode)
{
    
    while(!f[point])
    {
        if (mode)
        {
            Color = Front_Back_WhiteGreen;
            Forward_Track_all(speed,speed);
            rt_thread_delay(5);
        }
        else
        {
            Color = Front_WhiteGreen;
            ForwardTrack(speed,speed);
            rt_thread_delay(5);
        }
    }
    motor.stop();
    if(point>4)
    {
        while(!laser_r)
        {
            if (mode)
            {
                Color = Front_Back_WhiteGreen;
                Forward_Track_all(150,150);
                rt_thread_delay(5);
            }
            else
            {
                Color = Front_WhiteGreen;
                ForwardTrack(150,150);
                rt_thread_delay(5);
            }
        }
    }
    else
    {
        while(!laser_l)
        {
            if (mode)
            {
                Color = Front_Back_WhiteGreen;
                Forward_Track_all(150,150);
                rt_thread_delay(5);
            }
            else
            {
                Color = Front_WhiteGreen;
                ForwardTrack(150,150);
                rt_thread_delay(5);
            }
        }
    }
    motor.stop();
}

//反向循迹到路口并用激光
void back_truck2one_point_use_laser(u8 point,u16 speed)
{
    
    while(!b[point])
    {
        Color = Back_WhiteGreen;
        BackTrack(speed,speed);
        rt_thread_delay(5);
    }
    motor.stop();
    if(point<4)
    {
        while(!laser_r)
        {
            Color = Back_WhiteGreen;
            BackTrack(200,200);
            rt_thread_delay(5);
        }
    }
    else
    {
        while(!laser_l)
        {
            Color = Back_WhiteGreen;
            BackTrack(200,200);
            rt_thread_delay(5);
        }
    }
    motor.stop();
}

//过减速板
void throw_slow_down(rt_uint16_t l_speed,rt_uint16_t r_speed)
{
    rt_uint8_t ms,ms1;
    ms=158;//130
    ms1=150;//130
    Color=Front_GreenBlack;
    while((!f[0])||(!f[7]))//(angle.yaw<5||angle.yaw>355)
    {
        Color = Front_GreenBlack;//Front_WhiteGreen;
        ForwardTrack(l_speed,r_speed);
        rt_thread_delay(5);
    }
    while(ms--)
    {
        Color = Front_WhiteBlack;//Front_WhiteBlack;
        ForwardTrack(200,240);//350.400
        rt_thread_delay(5);
    }
    while(ms1--)
    {
        Color = Front_WhiteBlack;//Front_WhiteBlack;
        ForwardTrack(240,200);//400,350
        rt_thread_delay(5);
    }
    motor.stop();
}

//过减速板
void throw_3slow_down(rt_uint16_t l_speed,rt_uint16_t r_speed)
{
    rt_uint8_t ms,ms1,ms2;
    ms=158;//158;//130//80有点少//100少了一点点点点
    ms1=165;//130//120多
    ms2=165;
    Color = Front_GreenBlack;
    while(!f[0]||!f[7])//(angle.yaw<5||angle.yaw>355)
    {
        Color = Front_GreenBlack;//Front_WhiteGreen;
        ForwardTrack(l_speed,r_speed);
        rt_thread_delay(5);
    }
    while(ms--)
    {
        Color = Front_WhiteBlack;//Front_WhiteBlack;
        ForwardTrack(240,200);//350.400//350,300//330,300//240,200
        rt_thread_delay(5);
    }
//    travel_switch_test();
    while(ms1--)
    {
        Color = Front_WhiteBlack;//Front_WhiteBlack;
        ForwardTrack(200,245);//400,350
        rt_thread_delay(5);
    }
//    travel_switch_test();
    while(ms2--)
    {
        Color = Front_WhiteBlack;//Front_WhiteBlack;
        ForwardTrack(245,200);//400,350
        rt_thread_delay(5);
    }
    motor.stop();
    
}

void throw_3slow_downb(rt_uint16_t l_speed,rt_uint16_t r_speed)
{
    rt_uint8_t ms,ms1,ms2;
    ms=158;//158;//130//80有点少//100少了一点点点点//158
    ms1=165;//130//120多//150
    ms2=165;
    Color = Front_GreenBlack;
    while(!f[0]||!f[7])//(angle.yaw<5||angle.yaw>355)
    {
        Color = Front_GreenBlack;//Front_WhiteGreen;
        ForwardTrack(l_speed,r_speed);
        rt_thread_delay(5);
    }
    while(ms--)
    {
        Color = Front_WhiteBlack;//Front_WhiteBlack;
        ForwardTrack(200,250);//350.400//350,300//330,300//240,200
        rt_thread_delay(5);
    }
//    travel_switch_test();
    while(ms1--)
    {
        Color = Front_WhiteBlack;//Front_WhiteBlack;
        ForwardTrack(245,200);//400,350
        rt_thread_delay(5);
    }
//    travel_switch_test();
    while(ms2--)
    {
        Color = Front_WhiteBlack;//Front_WhiteBlack;
        ForwardTrack(200,250);//400,350
        rt_thread_delay(5);
    }
    motor.stop();
    
}

//断点测试
void travel_switch_test(void)
{
    motor.stop();
    while(1)
    {
        if(!travel_switch)
        {
            rt_thread_delay(300);
            break;
        }
        rt_thread_delay(10);
    }
}

//过门
rt_uint8_t cross_door(rt_uint16_t speed,rt_uint16_t ms)
{
    Road_to_time(500,500,300,0);
    rt_uint8_t door;
    rt_uint32_t ttt=500;
    Color = Front_GreenBlack;
    
    while((!f[0])||(!f[7]))//((angle.pitch>355||angle.pitch<10)&&!Infrared_f)
    {
        Color = Front_GreenBlack;//Front_GreenBlack;//Front_WhiteGreen;//Front_Back_WhiteGreen
        ForwardTrack(speed,speed);//Forward_Track_all
        rt_thread_delay(5);
    }
    motor.stop();
//    Road_to_Board(400,400);
//    travel_switch_test();
    rt_thread_delay(100);
//    travel_switch_test();
    ms=2800;
    ms /= 14;
    motor.stop();
    rt_thread_delay(10);
    if(!Infrared_f)
    {
        door=0;
    }
    else
    {
        while(ms--)
        {
            Color = Front_WhiteBlack;//Front_WhiteGreen;
            ForwardTrack(250,250);
            rt_thread_delay(5);
        }
        door=1;
    }
    motor.stop();
    return door;
}




static void write_flash_11(int8_t write_door)
{
    Flash_EraseSector(FLASH_Sector_11);
    Flash_Write8BitDatas(DoorsAddr,1,&write_door);
    rt_kprintf("\nWrite Flash Sector 11 with %d Success!",write_door);
}

FINSH_FUNCTION_EXPORT(turn_angle, turn angle. e.g: turn_angle(90,500))
FINSH_FUNCTION_EXPORT_ALIAS(truck2one_point,  ftruck2p,  truck2one_point(point,speed,mode))
FINSH_FUNCTION_EXPORT_ALIAS(back_truck2one_point,  btruck2p,  back_truck2one_point(point,speed))
FINSH_FUNCTION_EXPORT_ALIAS(truck2one_point_use_laser,  ftruck2puse,  truck2one_point_use_laser(point,speed,mode))
FINSH_FUNCTION_EXPORT_ALIAS(back_truck2one_point_use_laser, btruck2puse   ,back_truck2one_point_use_laser(point,speed))
FINSH_FUNCTION_EXPORT_ALIAS(Road_to_Board,  road2board  ,Road_to_Board(left speed,right speed))
FINSH_FUNCTION_EXPORT_ALIAS(Road_to_time,  road2time    ,Road_to_time(left speed,right speed,time(ms),mode))
FINSH_FUNCTION_EXPORT_ALIAS(mot_to_time,  mot2time  ,mot_to_time(left speed,right speed,time(ms)))
FINSH_FUNCTION_EXPORT_ALIAS(front_all_track_with_time, front2time     ,front_all_track_with_time(left speed,right speed,time(ms)))
FINSH_FUNCTION_EXPORT_ALIAS(turn2angle, turn2angle  ,e.g:turn2angle(angle,left speed,right speed))
FINSH_FUNCTION_EXPORT_ALIAS(write_flash_11,  write_flash, write_flash(which door))

























#ifdef RT_USE_GY26
void turn_angle(int ang,u16 speed)
{
    int angle0,angle1,angle2;//angle0为初始角，angle1为目标角度，angle2为首次转角（为了使转角更快更稳，采用两段式转发）
    refresh_angle();
    refresh_angle();
    refresh_angle();
    rt_thread_delay(25);//必要的延时，否则接收不到数据。
    angle0=angle.yaw;
    angle1=(angle0+ang+360)%360;
    angle2=(angle0+ang/2+360)%360;
    if(ang>=0)
    {
        while(!(angle2==angle.yaw||angle2==angle.yaw-1||angle2==angle.yaw+1||angle2==angle.yaw+2))
        {
            motor.go_ahead(-speed,speed);
            refresh_angle();
            rt_thread_delay(5);
        }
        motor.stop();
        rt_thread_delay(10);
        while(!(angle1==angle.yaw||angle1==angle.yaw-1||angle1==angle.yaw+1))
        {
            motor.go_ahead(-900,900);
            refresh_angle();
            rt_thread_delay(5);
        }
    }
    else
    {
        while(!(angle2==angle.yaw||angle2==angle.yaw-1||angle2==angle.yaw-2||angle2==angle.yaw+1))
        {
            motor.go_ahead(speed,-speed);
            refresh_angle();
            rt_thread_delay(5);
        }
        motor.stop();
        rt_thread_delay(10);
        while(!(angle1==angle.yaw||angle1==angle.yaw-1||angle1==angle.yaw-2||angle1==angle.yaw+1))
        {
            motor.go_ahead(100,-100);
            refresh_angle();
            rt_thread_delay(5);
        }
    }
    motor.stop();
}
#endif
