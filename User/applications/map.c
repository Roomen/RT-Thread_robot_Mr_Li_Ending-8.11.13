#include <rtthread.h>
#include "robot_run.h"
#include "operation.h"
#include "bsp_led.h"
#include "bsp_grayscale.h"
#include "map.h"
#include "robot_run.h"
#include "track.h"
#include "bsp_motor.h"
#include "bsp_compass.h"
#include "soft_timer.h"
#include "bsp_oled.h"
#include "bsp_flash.h"
#include <stdio.h>

ALIGN(RT_ALIGN_SIZE)
static u16 map_stack[10240];
static struct rt_thread map_thread;
rt_uint8_t door_oc[5],loop;  //若值为1则为开门，否则为关门
rt_uint16_t original_angle_x;
int8_t r_door,w_door;
static void map_thread_entry(void* parameter)
{
    
    while (1)
    {
//        throw_3slow_downb(400,400);
//        motor.stop();
//        while (1)
//        {
//            rt_thread_delay(100);
//        }
        
//        rt_led_init();
        Flash_Read8BitDatas(DoorsAddr,1,&r_door);
        door();
        oled.show_char(75,5,'.',12);
        oled.show_char(90,4,'s',16);
        while(!Infrared_f)
        {
            rt_thread_delay(100);   //开始，检测禁行板是否开启，否：等待1s开始走，否：一直等待
        }
        rt_thread_delay(1000);
        original_angle_x=angle.yaw;
        demo_start();   //启动软件定时器，用于计时
        
        if(r_door==0||r_door==1)
        {
            rt_kprintf("\nLoop 1.");
            if(door_oc[0]==0)
            {
                platform1_to_point4();//从起点到节点4的程序，其中包括上平台2、3、4，并撞景点1、2.
                
                mot_to_time(200,200,20);//
                
                rt_thread_delay(100);//右转90°
                turn_angle(90,300,300);
                rt_thread_delay(100);
                
                door_oc[0] = cross_door(500,2800);//去一号门看是否开启//2300
               
                rt_kprintf("\ndoor[0]:%d",door_oc[0]);
            }
            if (door_oc[0]==1)//如果一号门是开启的
            {
                //存入Flash
                motor.go_ahead(80,80);
                w_door=1;
                Flash_EraseSector(FLASH_Sector_11);
                Flash_Write8BitDatas(DoorsAddr,1,&w_door);
                
                programme1();//方案一
                show_run_time();/////////////////////////////////////////////////////
                rt_thread_delay(3000);
                
                
                while(!Infrared_f)  //开始，检测禁行板是否开启，否：等待1s开始走，否：一直等待
                {
                    rt_thread_delay(100);   
                }
                rt_thread_delay(1000);
                
                //Loop2：
                platform1_to_point4();//从起点到节点4的程序，其中包括上平台2、3、4，并撞景点1、2.
                
                //和第一趟一样
                rt_thread_delay(100);        
                turn_angle(90,300,300);        
                rt_thread_delay(100);//右转90°
                
                door_oc[4] = cross_door(500,2300);//一号门是开启的
                programme1();
                
                w_door=0;
                Flash_EraseSector(FLASH_Sector_11);
                Flash_Write8BitDatas(DoorsAddr,1,&w_door);
            }
            else//如果一号门是关闭的
            {
                //后退并判断二号门是否开启程序
                rt_thread_delay(100);
                Road_to_time(-300,-300,800,0);
                back_truck2one_point(0,400);
                Road_to_time(-200,-200,130,0);//160
                rt_thread_delay(100);
                turn_angle(-48,300,300);
                rt_thread_delay(100);
                Road_to_time(400,400,100,0);
                door_oc[1]=cross_door(500,2300);
                rt_kprintf("\ndoor[1]:%d",door_oc[1]);
                //如果二号门是开的
                if(door_oc[1]==1)
                {
                    motor.go_ahead(80,80);
                    w_door=2;
                    Flash_EraseSector(FLASH_Sector_11);
                    Flash_Write8BitDatas(DoorsAddr,1,&w_door);
                    programme2();
                    show_run_time();
                    //Loop2:
                    rt_thread_delay(3000);
                    while(!Infrared_f)
                    {
                        rt_thread_delay(100);
                    }                           //开始，检测禁行板是否开启，否：等待1s开始走，否：一直等待
                    rt_thread_delay(1000);
                    
                    platform1_to_point4();//从起点到节点4的程序，其中包括上平台2、3、4，并撞景点1、2.
                    //返回后第二趟直接去二号门
                    rt_thread_delay(100);
                    turn_angle(40,400,400);
                    rt_thread_delay(100);
                    Road_to_time(500,500,200,0);
                    door_oc[4]=cross_door(500,2300);//门肯定是开的
                    programme2();
                    w_door=0;
                    Flash_EraseSector(FLASH_Sector_11);
                    Flash_Write8BitDatas(DoorsAddr,1,&w_door);
                    //二号门之后的程序
                }
                else//如果二号门也不开启，四号门必定开启
                {
                    motor.go_ahead(-80,-80);
                    w_door=4;
                    Flash_EraseSector(FLASH_Sector_11);
                    Flash_Write8BitDatas(DoorsAddr,1,&w_door);
                    rt_thread_delay(100);
                    Road_to_time(-300,-300,500,0);
                    back_truck2one_point(7,400);
                    Road_to_time(-200,-200,250,0);//160
                    rt_thread_delay(100);
                    turn_angle(-42,300,300);
                    rt_thread_delay(100);
    //                Road_to_time(500,500,300,1);
//                    Road_to_time(500,500,1000,1);
                    slow_acceleration(100,500,700);
                    
                    programme4();
                    show_run_time();
                    //Loop2:
                    rt_thread_delay(3000);
                    while(!Infrared_f)  //开始，检测禁行板是否开启，否：等待1s开始走，否：一直等待
                    {
                        rt_thread_delay(100);
                    }
                    rt_thread_delay(1000);
                    
                    platform1_to_point4();//从起点到节点4的程序，其中包括上平台2、3、4，并撞景点1、2.
                    programme4();
                    w_door=0;
                    Flash_EraseSector(FLASH_Sector_11);
                    Flash_Write8BitDatas(DoorsAddr,1,&w_door);
                }
            }
        }
        else if(r_door==2)
        {
            rt_kprintf("\nJust go to door2");
            platform1_to_point4();//从起点到节点4的程序，其中包括上平台2、3、4，并撞景点1、2.
            //返回后第二趟直接去二号门
            rt_thread_delay(100);
            turn_angle(40,400,400);
            rt_thread_delay(100);
            Road_to_time(500,500,200,0);
            door_oc[4]=cross_door(500,2300);//门肯定是开的
            programme2();
        } 
        else if(r_door==4)
        {
            rt_kprintf("\nJust go to door4");
            platform1_to_point4();
            programme4_loop2();
        }
        
        motor.stop();
        show_run_time();    //停止并显示运行时间
//        LED1=0;
        while(1)
        {
            rt_thread_delay(100);
        }
    }
}

static void platform1_to_point4(void)//从平台1开始到~
{
//下1号平台
    lower_platform(400);
    
    Road_to_time(400,400,300,0);
//循迹到长桥
    Road_to_Board(550,550);
    
//过长桥
    though_brige(400);
    
//循迹到2号平台
//    Road_to_time(300,300,500,0);  /////////////////
//    slow_acceleration(100,520,500); //////////////////////////////////////////////////////////
    Road_to_time(300,300,500,0);
    Road_to_Board(520,520);
    
//上平台
    platform();

//亮灯展示
    LED1 =0;
    rt_thread_delay(300);
    LED1 =1;
    rt_thread_delay(300);
    LED1 =0;
    rt_thread_delay(300);
    LED1 =1;
    
//旋转180°
    turn_angle(-180,400,400);
    rt_thread_delay(100);

//在网咖(二号平台)展示完毕，开始走，下平台
    lower_platform(400);
    Road_to_time(450,450,500,0);
    Road_to_time(450,450,500,1);
    
//循迹到路口① 

    truck2one_point(7,500,0);
    
    rt_thread_delay(100);
    mot_to_time(-200,-200,200);
    rt_thread_delay(100);
    
//转弯
    rt_thread_delay(100);
    turn_angle(30,300,300);
    rt_thread_delay(100);
    
//过减速板
    throw_slow_down(400,400);
    Road_to_time(300,300,500,1);
    truck2one_point(0,450,1);
    mot_to_time(200,200,100);
    rt_thread_delay(100);
    
//到路口③
    turn_angle(-35,300,300);
    Road_to_time(500,500,800,0);
    Road_to_time(500,500,800,1);

//去4号平台
    slow_acceleration(500,700,300);
    Road_to_time(700,700,800,1);
    Road_to_Board(600,600);
    platform();
    rt_thread_delay(100);
    turn_angle(-180,500,500);
    rt_thread_delay(100);

//下平台并走到了节点④
    lower_platform(400);
    Road_to_time(400,400,400,0);
    slow_acceleration(400,750,700);
    front_all_track_with_time(750,750,2300);//600,600,3200  //700,700,2550~2800(具体还得看摩擦力……)

//上3号平台
//    Road_to_time(500,500,1000,0);
    Road_to_Board(700,700);
    platform();
    rt_thread_delay(100);
    turn_angle(-180,500,500);
    rt_thread_delay(100);
    
//下平台3并循迹到节点4
    lower_platform(400);
    Road_to_time(600,600,1000,1);
    truck2one_point_use_laser(7,500,1);
//    Road_to_time(200,200,150,1);
    
}

static void programme1(void)//Loop 1 一号门开着
{
    //刚刚过门，去7,8号平台
    slow_acceleration(100,600,500);
    truck2one_point(7,600,1);
    Road_to_time(600,600,200,1);
    truck2one_point(7,600,1);
    Road_to_time(500,500,200,1);
    truck2one_point_use_laser(0,500,1);
//    Road_to_time(200,200,20,0);
    
//    Road_to_time(200,200,200,1);
    rt_thread_delay(100);
    turn_angle(-90,300,300);
    rt_thread_delay(100);
    
//    slow_acceleration(100,500,800);
    Road_to_time(300,300,300,0);
    Road_to_time(500,500,300,1);
    truck2one_point_use_laser(7,500,1);///////////////////////////////////////////////////use_laser
    Road_to_time(200,200,20,0);
    rt_thread_delay(100);
    turn_angle(90,500,500);
    rt_thread_delay(100);
    
    Road_to_time(400,400,100,1);
    go_to78platform();//7,8平台

//到节点16
    Road_to_time(400,400,500,0);
    Road_to_time(600,600,1300,1);
//travel_switch_test();/////////////////////////////////
    truck2one_point_use_laser(7,500,1);
    Road_to_time(200,200,25,0);
    rt_thread_delay(100);
    turn_angle(90,300,300);
    rt_thread_delay(100);
//上平台5
    slow_acceleration(100,700,600);
    Road_to_Board(700,700);
    platform();
    rt_thread_delay(100);
    turn_angle(180,500,500);
    rt_thread_delay(100);
//到节点8
    lower_platform(400);

    slow_acceleration(400,700,500);
    truck2one_point(0,700,1);
    Road_to_time(700,700,100,1);
    truck2one_point(0,700,1);
    Road_to_time(700,700,100,1);
    truck2one_point(7,600,1);
    mot_to_time(200,200,200);
    rt_thread_delay(100);
    turn_angle(90,500,500);
    rt_thread_delay(100);

//从一号门到节点4
    Road_to_time(500,500,500,0);
    Road_to_time(500,500,800,1);
    door_oc[4]=cross_door(500,2000);
    truck2one_point_use_laser(0,500,1);
//    Road_to_time(200,200,180,0);
    rt_thread_delay(100);
    turn_angle(90,400,400);
    rt_thread_delay(100);

//从节点4到节点2
    Road_to_time(300,300,700,0);
    truck2one_point_use_laser(0,600,1);
    Road_to_time(200,200,350,0);
    rt_thread_delay(100);
    turn_angle(-30,300,300);
    rt_thread_delay(100);
    Road_to_time(300,300,300,0);

//过跷跷板
    through_qqb();

//到节点2
    truck2one_point_use_laser(0,500,0);
    rt_thread_delay(100);
    mot_to_time(200,200,200);
    turn_angle(30,300,300);
    rt_thread_delay(100);

//回家啊~~~~~~~！！！
    Road_to_time(300,300,600,0);
    Road_to_Board(400,400);
    platform();
    rt_thread_delay(100);
    turn_angle(180,500,500);
    rt_thread_delay(100);

}


static void programme2(void)//2号门开着
{
    //去节点9
    slow_acceleration(100,600,500);
    Road_to_time(650,650,1000,1);
    truck2one_point(7,500,0);
    mot_to_time(300,300,200);//150//170
    rt_thread_delay(100);
    turn_angle(137,500,500);
    rt_thread_delay(100);
    //去7,8平台的路上
    
    slow_acceleration(100,500,500);
//    slow_acceleration(100,500,500);
    Road_to_time(500,500,500,0);
    
    truck2one_point(0,500,1);
    Road_to_time(250,250,280,0);
    rt_thread_delay(100);
    turn_angle(-90,500,500);
    rt_thread_delay(100);
    
    Road_to_time(200,200,100,0);
    Road_to_time(600,600,1400,1);
    go_to78platform();
    
    //到节点16
    slow_acceleration(100,600,500);
//    Road_to_time(700,600,1300,1);
    truck2one_point(7,600,1);
    Road_to_time(600,600,100,1);
    truck2one_point_use_laser(7,500,1);
    Road_to_time(200,200,80,0);
    rt_thread_delay(100);
    turn_angle(90,500,500);
    rt_thread_delay(100);
//上平台5
//    Road_to_time(400,400,400,1);
    slow_acceleration(100,730,500);
    Road_to_Board(730,730);//700
    platform();
    rt_thread_delay(100);
    turn_angle(180,500,500);
    rt_thread_delay(100);
//到节点9
    lower_platform(400);
//    Road_to_time(500,500,500,1);
//    Road_to_time(680,680,1200,1);//650
    slow_acceleration(100,500,300);
    truck2one_point(0,500,1);
    Road_to_time(200,200,100,0);
    rt_thread_delay(100);
    turn_angle(40,500,500);
    rt_thread_delay(100);
    Road_to_time(400,400,400,0);
    slow_acceleration(100,650,700);
    door_oc[4]=cross_door(500,2300);
    
//到节点4
    Road_to_time(400,400,400,0);
//    Road_to_time(600,400,400,0);
    truck2one_point_use_laser(7,400,1);
//    slow_acceleration(200,500,500);/////////////////////////////////改
    mot_to_time(200,200,300);
    rt_thread_delay(100);
    turn_angle(140,400,400);
    rt_thread_delay(100);
    
    slow_acceleration(100,500,700);
    truck2one_point_use_laser(0,500,1);
    
    Road_to_time(300,300,350,0);
    
    //转弯
    rt_thread_delay(100);
    turn_angle(-30,300,300);
    rt_thread_delay(100);
    slow_acceleration(100,300,500);
    
    home();
    
}

static void programme4(void)//4号门开着
{
    //去节点5
    Road_to_time(500,500,1000,1);
    
    truck2one_point_use_laser(7,500,1);
    mot_to_time(260,260,200);

    rt_thread_delay(100);
    turn_angle(90,400,400);
    rt_thread_delay(100);
    //过4号门到节点9
    door_oc[4]=cross_door(500,2000);
    Road_to_time(400,400,500,0);
    truck2one_point_use_laser(0,500,1);
    mot_to_time(200,200,50);
    
    rt_thread_delay(100);
    turn_angle(90,300,300);
    rt_thread_delay(100);
    
    //去节点16
    Road_to_time(400,400,500,0);
    truck2one_point_use_laser(0,500,1);
    
    mot_to_time(200,200,40);

    rt_thread_delay(100);
    turn_angle(-90,300,300);
    rt_thread_delay(100);
    //去7、8号平台
    Road_to_time(400,400,400,0);
    slow_acceleration(400,600,200);
    truck2one_point(0,600,1);
    go_to78platform();
    
    //到节点16
    slow_acceleration(100,400,300);
    Road_to_time(500,500,400,1);
    truck2one_point(7,600,1);
    Road_to_time(600,600,100,1);
    truck2one_point(7,500,1);
    mot_to_time(200,200,50);
    
    rt_thread_delay(100);
    turn_angle(90,300,300);
    rt_thread_delay(100);

    //到节点9
    slow_acceleration(100,500,400);
    truck2one_point_use_laser(7,500,1);
    mot_to_time(200,200,50);
    
    rt_thread_delay(100);
    turn_angle(-90,300,300);
    rt_thread_delay(100);
    
    //过4号门并到节点5
    slow_acceleration(100,500,500);
    Road_to_time(500,500,800,1);
    door_oc[4]=cross_door(500,2300);
    
    Road_to_time(400,400,300,0);
    truck2one_point_use_laser(7,500,1);
    mot_to_time(200,200,40);
    
    rt_thread_delay(100);
    turn_angle(-90,300,300);
    rt_thread_delay(100);
    
    //到节点3
    slow_acceleration(100,500,200);
    truck2one_point_use_laser(7,500,1);
    Road_to_time(350,350,300,0);
    
    rt_thread_delay(100);
    turn_angle(140,300,300);
    rt_thread_delay(100);
    
    home();
}

static void programme4_loop2(void)//4号门开着
{
    Road_to_time(500,500,1000,1);
    //去节点5
    Road_to_time(500,500,500,1);
    truck2one_point_use_laser(7,500,1);
    mot_to_time(200,200,200);

    rt_thread_delay(100);
    turn_angle(90,400,400);
    rt_thread_delay(100);
    //过4号门到节点9
    door_oc[4]=cross_door(500,2000);
    Road_to_time(400,400,500,0);
    truck2one_point_use_laser(0,500,1);
    mot_to_time(200,200,50);
    
    rt_thread_delay(100);
    turn_angle(90,300,300);
    rt_thread_delay(100);
    
    //去节点16
    Road_to_time(400,400,500,0);
    truck2one_point_use_laser(0,500,1);
    
    mot_to_time(200,200,40);

    rt_thread_delay(100);
    turn_angle(-90,300,300);
    rt_thread_delay(100);
    //去7、8号平台
    Road_to_time(400,400,400,0);
    slow_acceleration(400,600,200);
    truck2one_point(0,600,1);
    go_to78platform();
    
    //到节点16
    slow_acceleration(100,400,300);
    Road_to_time(500,500,400,1);
    
    truck2one_point(7,600,1);
    Road_to_time(600,600,100,1);
    truck2one_point(7,500,1);
    mot_to_time(200,200,50);
    
    rt_thread_delay(100);
    turn_angle(90,300,300);
    rt_thread_delay(100);

    //上5号平台
    slow_acceleration(100,700,600);
    Road_to_time(700,700,500,1);
    Road_to_Board(700,700);
    platform();
    
    rt_thread_delay(100);
    turn_angle(180,350,350);
    rt_thread_delay(100);
    
    
    //到节点9
    lower_platform(400);
    slow_acceleration(400,600,600);
    truck2one_point_use_laser(7,500,1);
    mot_to_time(200,200,50);
    
    rt_thread_delay(100);
    turn_angle(90,300,300);
    rt_thread_delay(100);
    
    //过4号门并到节点5
    slow_acceleration(100,500,500);
    Road_to_time(500,500,800,1);
    door_oc[4]=cross_door(500,2300);
    
    Road_to_time(400,400,300,0);
    truck2one_point_use_laser(7,500,1);
    mot_to_time(200,200,40);
    
    rt_thread_delay(100);
    turn_angle(-90,300,300);
    rt_thread_delay(100);
    
    //到节点3
    slow_acceleration(100,500,200);
    truck2one_point_use_laser(7,500,1);
    Road_to_time(350,350,300,0);
    
    rt_thread_delay(100);
    turn_angle(140,300,300);
    rt_thread_delay(100);
    
    home();
}

static void go_to78platform(void)
{
    //上珠峰
    Road_to_time(400,400,500,0);
    Road_to_Board(400,400);
    mot_to_time(275,275,250);//190//250
    rt_thread_delay(200);
    turn_angle(90,350,100);//500,250//300,100
    rt_thread_delay(100);
    mot_to_time(400,400,400);
    Road_to_Board(600,600);
    platform();
    
    second_order
    
    Road_to_Board(400,400);
    platform();
    
    
    
    rt_thread_delay(100);
    turn_angle(180,500,500);
    rt_thread_delay(100);
//下平台到倾斜路面上
    lower_platform8(330);
    second_order
    
    Road_to_time(200,200,800,0);
    lower_platform8(330);
    
   
    
    Road_to_time(500,500,1200,1);
    road_to_sloping();
    mot_to_time(250,210,200);
    hw_track_with_time(300,800);
    mot_to_time(300,330,500);
    Road_to_time(400,400,800,1);
    throw_3slow_down(400,400);
    Road_to_Board(400,400);
    platform();
    mot_to_time(200,200,100);
    rt_thread_delay(100);
    turn_angle(180,300,300);
    rt_thread_delay(100);
    lower_platform8(350);
    Road_to_time(400,400,800,0);
    throw_3slow_downb(400,400);
    
    road_to_sloping();
    mot_to_time(320,320,650);//300
    
    rt_thread_delay(100);
    turn_angle(90,250,400);
    rt_thread_delay(100);
}

static void home(void)
{

    
//过跷跷板
    through_qqb();

//到节点2
    truck2one_point_use_laser(0,500,0);
    mot_to_time(200,200,200);
    rt_thread_delay(100);
    turn_angle(30,300,300 );
    rt_thread_delay(100);
    
//回家啊~~~~~~~！！！
    Road_to_time(300,300,600,0);
    Road_to_Board(400,400);
    platform();
    rt_thread_delay(100);
    turn_angle(180,500,500);
    rt_thread_delay(100);
}


#include <finsh.h>

static void door(void)
{

    rt_kprintf("\ndoor_oc:%d%d%d%d\tr_door:%d\tw_door:%d",door_oc[0],door_oc[1],door_oc[2],door_oc[3],r_door,w_door);
}

FINSH_FUNCTION_EXPORT(door,show door)








/**
 * 创建go_map线程
 */
int run_map_init(void)
{
    rt_err_t result;

    /* init run_thread thread */
    result = rt_thread_init(&map_thread,
                            "go_map",
                            map_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&map_stack[0],
                            sizeof(map_stack),
                            16,
                            5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&map_thread);
    }	
	return 0;
}

