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
rt_uint8_t door_oc[5],loop;  //��ֵΪ1��Ϊ���ţ�����Ϊ����
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
            rt_thread_delay(100);   //��ʼ�������а��Ƿ������񣺵ȴ�1s��ʼ�ߣ���һֱ�ȴ�
        }
        rt_thread_delay(1000);
        original_angle_x=angle.yaw;
        demo_start();   //���������ʱ�������ڼ�ʱ
        
        if(r_door==0||r_door==1)
        {
            rt_kprintf("\nLoop 1.");
            if(door_oc[0]==0)
            {
                platform1_to_point4();//����㵽�ڵ�4�ĳ������а�����ƽ̨2��3��4����ײ����1��2.
                
                mot_to_time(200,200,20);//
                
                rt_thread_delay(100);//��ת90��
                turn_angle(90,300,300);
                rt_thread_delay(100);
                
                door_oc[0] = cross_door(500,2800);//ȥһ���ſ��Ƿ���//2300
               
                rt_kprintf("\ndoor[0]:%d",door_oc[0]);
            }
            if (door_oc[0]==1)//���һ�����ǿ�����
            {
                //����Flash
                motor.go_ahead(80,80);
                w_door=1;
                Flash_EraseSector(FLASH_Sector_11);
                Flash_Write8BitDatas(DoorsAddr,1,&w_door);
                
                programme1();//����һ
                show_run_time();/////////////////////////////////////////////////////
                rt_thread_delay(3000);
                
                
                while(!Infrared_f)  //��ʼ�������а��Ƿ������񣺵ȴ�1s��ʼ�ߣ���һֱ�ȴ�
                {
                    rt_thread_delay(100);   
                }
                rt_thread_delay(1000);
                
                //Loop2��
                platform1_to_point4();//����㵽�ڵ�4�ĳ������а�����ƽ̨2��3��4����ײ����1��2.
                
                //�͵�һ��һ��
                rt_thread_delay(100);        
                turn_angle(90,300,300);        
                rt_thread_delay(100);//��ת90��
                
                door_oc[4] = cross_door(500,2300);//һ�����ǿ�����
                programme1();
                
                w_door=0;
                Flash_EraseSector(FLASH_Sector_11);
                Flash_Write8BitDatas(DoorsAddr,1,&w_door);
            }
            else//���һ�����ǹرյ�
            {
                //���˲��ж϶������Ƿ�������
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
                //����������ǿ���
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
                    }                           //��ʼ�������а��Ƿ������񣺵ȴ�1s��ʼ�ߣ���һֱ�ȴ�
                    rt_thread_delay(1000);
                    
                    platform1_to_point4();//����㵽�ڵ�4�ĳ������а�����ƽ̨2��3��4����ײ����1��2.
                    //���غ�ڶ���ֱ��ȥ������
                    rt_thread_delay(100);
                    turn_angle(40,400,400);
                    rt_thread_delay(100);
                    Road_to_time(500,500,200,0);
                    door_oc[4]=cross_door(500,2300);//�ſ϶��ǿ���
                    programme2();
                    w_door=0;
                    Flash_EraseSector(FLASH_Sector_11);
                    Flash_Write8BitDatas(DoorsAddr,1,&w_door);
                    //������֮��ĳ���
                }
                else//���������Ҳ���������ĺ��űض�����
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
                    while(!Infrared_f)  //��ʼ�������а��Ƿ������񣺵ȴ�1s��ʼ�ߣ���һֱ�ȴ�
                    {
                        rt_thread_delay(100);
                    }
                    rt_thread_delay(1000);
                    
                    platform1_to_point4();//����㵽�ڵ�4�ĳ������а�����ƽ̨2��3��4����ײ����1��2.
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
            platform1_to_point4();//����㵽�ڵ�4�ĳ������а�����ƽ̨2��3��4����ײ����1��2.
            //���غ�ڶ���ֱ��ȥ������
            rt_thread_delay(100);
            turn_angle(40,400,400);
            rt_thread_delay(100);
            Road_to_time(500,500,200,0);
            door_oc[4]=cross_door(500,2300);//�ſ϶��ǿ���
            programme2();
        } 
        else if(r_door==4)
        {
            rt_kprintf("\nJust go to door4");
            platform1_to_point4();
            programme4_loop2();
        }
        
        motor.stop();
        show_run_time();    //ֹͣ����ʾ����ʱ��
//        LED1=0;
        while(1)
        {
            rt_thread_delay(100);
        }
    }
}

static void platform1_to_point4(void)//��ƽ̨1��ʼ��~
{
//��1��ƽ̨
    lower_platform(400);
    
    Road_to_time(400,400,300,0);
//ѭ��������
    Road_to_Board(550,550);
    
//������
    though_brige(400);
    
//ѭ����2��ƽ̨
//    Road_to_time(300,300,500,0);  /////////////////
//    slow_acceleration(100,520,500); //////////////////////////////////////////////////////////
    Road_to_time(300,300,500,0);
    Road_to_Board(520,520);
    
//��ƽ̨
    platform();

//����չʾ
    LED1 =0;
    rt_thread_delay(300);
    LED1 =1;
    rt_thread_delay(300);
    LED1 =0;
    rt_thread_delay(300);
    LED1 =1;
    
//��ת180��
    turn_angle(-180,400,400);
    rt_thread_delay(100);

//������(����ƽ̨)չʾ��ϣ���ʼ�ߣ���ƽ̨
    lower_platform(400);
    Road_to_time(450,450,500,0);
    Road_to_time(450,450,500,1);
    
//ѭ����·�ڢ� 

    truck2one_point(7,500,0);
    
    rt_thread_delay(100);
    mot_to_time(-200,-200,200);
    rt_thread_delay(100);
    
//ת��
    rt_thread_delay(100);
    turn_angle(30,300,300);
    rt_thread_delay(100);
    
//�����ٰ�
    throw_slow_down(400,400);
    Road_to_time(300,300,500,1);
    truck2one_point(0,450,1);
    mot_to_time(200,200,100);
    rt_thread_delay(100);
    
//��·�ڢ�
    turn_angle(-35,300,300);
    Road_to_time(500,500,800,0);
    Road_to_time(500,500,800,1);

//ȥ4��ƽ̨
    slow_acceleration(500,700,300);
    Road_to_time(700,700,800,1);
    Road_to_Board(600,600);
    platform();
    rt_thread_delay(100);
    turn_angle(-180,500,500);
    rt_thread_delay(100);

//��ƽ̨���ߵ��˽ڵ��
    lower_platform(400);
    Road_to_time(400,400,400,0);
    slow_acceleration(400,750,700);
    front_all_track_with_time(750,750,2300);//600,600,3200  //700,700,2550~2800(���廹�ÿ�Ħ��������)

//��3��ƽ̨
//    Road_to_time(500,500,1000,0);
    Road_to_Board(700,700);
    platform();
    rt_thread_delay(100);
    turn_angle(-180,500,500);
    rt_thread_delay(100);
    
//��ƽ̨3��ѭ�����ڵ�4
    lower_platform(400);
    Road_to_time(600,600,1000,1);
    truck2one_point_use_laser(7,500,1);
//    Road_to_time(200,200,150,1);
    
}

static void programme1(void)//Loop 1 һ���ſ���
{
    //�ոչ��ţ�ȥ7,8��ƽ̨
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
    go_to78platform();//7,8ƽ̨

//���ڵ�16
    Road_to_time(400,400,500,0);
    Road_to_time(600,600,1300,1);
//travel_switch_test();/////////////////////////////////
    truck2one_point_use_laser(7,500,1);
    Road_to_time(200,200,25,0);
    rt_thread_delay(100);
    turn_angle(90,300,300);
    rt_thread_delay(100);
//��ƽ̨5
    slow_acceleration(100,700,600);
    Road_to_Board(700,700);
    platform();
    rt_thread_delay(100);
    turn_angle(180,500,500);
    rt_thread_delay(100);
//���ڵ�8
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

//��һ���ŵ��ڵ�4
    Road_to_time(500,500,500,0);
    Road_to_time(500,500,800,1);
    door_oc[4]=cross_door(500,2000);
    truck2one_point_use_laser(0,500,1);
//    Road_to_time(200,200,180,0);
    rt_thread_delay(100);
    turn_angle(90,400,400);
    rt_thread_delay(100);

//�ӽڵ�4���ڵ�2
    Road_to_time(300,300,700,0);
    truck2one_point_use_laser(0,600,1);
    Road_to_time(200,200,350,0);
    rt_thread_delay(100);
    turn_angle(-30,300,300);
    rt_thread_delay(100);
    Road_to_time(300,300,300,0);

//�����ΰ�
    through_qqb();

//���ڵ�2
    truck2one_point_use_laser(0,500,0);
    rt_thread_delay(100);
    mot_to_time(200,200,200);
    turn_angle(30,300,300);
    rt_thread_delay(100);

//�ؼҰ�~~~~~~~������
    Road_to_time(300,300,600,0);
    Road_to_Board(400,400);
    platform();
    rt_thread_delay(100);
    turn_angle(180,500,500);
    rt_thread_delay(100);

}


static void programme2(void)//2���ſ���
{
    //ȥ�ڵ�9
    slow_acceleration(100,600,500);
    Road_to_time(650,650,1000,1);
    truck2one_point(7,500,0);
    mot_to_time(300,300,200);//150//170
    rt_thread_delay(100);
    turn_angle(137,500,500);
    rt_thread_delay(100);
    //ȥ7,8ƽ̨��·��
    
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
    
    //���ڵ�16
    slow_acceleration(100,600,500);
//    Road_to_time(700,600,1300,1);
    truck2one_point(7,600,1);
    Road_to_time(600,600,100,1);
    truck2one_point_use_laser(7,500,1);
    Road_to_time(200,200,80,0);
    rt_thread_delay(100);
    turn_angle(90,500,500);
    rt_thread_delay(100);
//��ƽ̨5
//    Road_to_time(400,400,400,1);
    slow_acceleration(100,730,500);
    Road_to_Board(730,730);//700
    platform();
    rt_thread_delay(100);
    turn_angle(180,500,500);
    rt_thread_delay(100);
//���ڵ�9
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
    
//���ڵ�4
    Road_to_time(400,400,400,0);
//    Road_to_time(600,400,400,0);
    truck2one_point_use_laser(7,400,1);
//    slow_acceleration(200,500,500);/////////////////////////////////��
    mot_to_time(200,200,300);
    rt_thread_delay(100);
    turn_angle(140,400,400);
    rt_thread_delay(100);
    
    slow_acceleration(100,500,700);
    truck2one_point_use_laser(0,500,1);
    
    Road_to_time(300,300,350,0);
    
    //ת��
    rt_thread_delay(100);
    turn_angle(-30,300,300);
    rt_thread_delay(100);
    slow_acceleration(100,300,500);
    
    home();
    
}

static void programme4(void)//4���ſ���
{
    //ȥ�ڵ�5
    Road_to_time(500,500,1000,1);
    
    truck2one_point_use_laser(7,500,1);
    mot_to_time(260,260,200);

    rt_thread_delay(100);
    turn_angle(90,400,400);
    rt_thread_delay(100);
    //��4���ŵ��ڵ�9
    door_oc[4]=cross_door(500,2000);
    Road_to_time(400,400,500,0);
    truck2one_point_use_laser(0,500,1);
    mot_to_time(200,200,50);
    
    rt_thread_delay(100);
    turn_angle(90,300,300);
    rt_thread_delay(100);
    
    //ȥ�ڵ�16
    Road_to_time(400,400,500,0);
    truck2one_point_use_laser(0,500,1);
    
    mot_to_time(200,200,40);

    rt_thread_delay(100);
    turn_angle(-90,300,300);
    rt_thread_delay(100);
    //ȥ7��8��ƽ̨
    Road_to_time(400,400,400,0);
    slow_acceleration(400,600,200);
    truck2one_point(0,600,1);
    go_to78platform();
    
    //���ڵ�16
    slow_acceleration(100,400,300);
    Road_to_time(500,500,400,1);
    truck2one_point(7,600,1);
    Road_to_time(600,600,100,1);
    truck2one_point(7,500,1);
    mot_to_time(200,200,50);
    
    rt_thread_delay(100);
    turn_angle(90,300,300);
    rt_thread_delay(100);

    //���ڵ�9
    slow_acceleration(100,500,400);
    truck2one_point_use_laser(7,500,1);
    mot_to_time(200,200,50);
    
    rt_thread_delay(100);
    turn_angle(-90,300,300);
    rt_thread_delay(100);
    
    //��4���Ų����ڵ�5
    slow_acceleration(100,500,500);
    Road_to_time(500,500,800,1);
    door_oc[4]=cross_door(500,2300);
    
    Road_to_time(400,400,300,0);
    truck2one_point_use_laser(7,500,1);
    mot_to_time(200,200,40);
    
    rt_thread_delay(100);
    turn_angle(-90,300,300);
    rt_thread_delay(100);
    
    //���ڵ�3
    slow_acceleration(100,500,200);
    truck2one_point_use_laser(7,500,1);
    Road_to_time(350,350,300,0);
    
    rt_thread_delay(100);
    turn_angle(140,300,300);
    rt_thread_delay(100);
    
    home();
}

static void programme4_loop2(void)//4���ſ���
{
    Road_to_time(500,500,1000,1);
    //ȥ�ڵ�5
    Road_to_time(500,500,500,1);
    truck2one_point_use_laser(7,500,1);
    mot_to_time(200,200,200);

    rt_thread_delay(100);
    turn_angle(90,400,400);
    rt_thread_delay(100);
    //��4���ŵ��ڵ�9
    door_oc[4]=cross_door(500,2000);
    Road_to_time(400,400,500,0);
    truck2one_point_use_laser(0,500,1);
    mot_to_time(200,200,50);
    
    rt_thread_delay(100);
    turn_angle(90,300,300);
    rt_thread_delay(100);
    
    //ȥ�ڵ�16
    Road_to_time(400,400,500,0);
    truck2one_point_use_laser(0,500,1);
    
    mot_to_time(200,200,40);

    rt_thread_delay(100);
    turn_angle(-90,300,300);
    rt_thread_delay(100);
    //ȥ7��8��ƽ̨
    Road_to_time(400,400,400,0);
    slow_acceleration(400,600,200);
    truck2one_point(0,600,1);
    go_to78platform();
    
    //���ڵ�16
    slow_acceleration(100,400,300);
    Road_to_time(500,500,400,1);
    
    truck2one_point(7,600,1);
    Road_to_time(600,600,100,1);
    truck2one_point(7,500,1);
    mot_to_time(200,200,50);
    
    rt_thread_delay(100);
    turn_angle(90,300,300);
    rt_thread_delay(100);

    //��5��ƽ̨
    slow_acceleration(100,700,600);
    Road_to_time(700,700,500,1);
    Road_to_Board(700,700);
    platform();
    
    rt_thread_delay(100);
    turn_angle(180,350,350);
    rt_thread_delay(100);
    
    
    //���ڵ�9
    lower_platform(400);
    slow_acceleration(400,600,600);
    truck2one_point_use_laser(7,500,1);
    mot_to_time(200,200,50);
    
    rt_thread_delay(100);
    turn_angle(90,300,300);
    rt_thread_delay(100);
    
    //��4���Ų����ڵ�5
    slow_acceleration(100,500,500);
    Road_to_time(500,500,800,1);
    door_oc[4]=cross_door(500,2300);
    
    Road_to_time(400,400,300,0);
    truck2one_point_use_laser(7,500,1);
    mot_to_time(200,200,40);
    
    rt_thread_delay(100);
    turn_angle(-90,300,300);
    rt_thread_delay(100);
    
    //���ڵ�3
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
    //�����
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
//��ƽ̨����б·����
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

    
//�����ΰ�
    through_qqb();

//���ڵ�2
    truck2one_point_use_laser(0,500,0);
    mot_to_time(200,200,200);
    rt_thread_delay(100);
    turn_angle(30,300,300 );
    rt_thread_delay(100);
    
//�ؼҰ�~~~~~~~������
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
 * ����go_map�߳�
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

