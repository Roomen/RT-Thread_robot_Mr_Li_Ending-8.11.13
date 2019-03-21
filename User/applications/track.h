#ifndef __TRACK_H
#define __TRACK_H	

#include <rtthread.h>
#include <stm32f4xx.h>
#include <finsh.h>

extern u32 r_f;
#define Front_RRight    (f[7])
#define Front_Right     (f[6] || f[5])
#define Front_LLeft     (f[0])
#define Front_Left      (f[1] || f[2])
#define Front_Med       (f[3] || f[4])
#define Front_MMed      (f[2] || f[3] || f[4] || f[5])
#define Front_edge      (f[0] || f[7])
#define Front_All       (f[0]&&f[1]&&f[2]&&f[3]&&f[4]&&f[5]&&f[6]&&f[7])
#define Front_Eigher    (f[0]||f[1]||f[2]||f[3]||f[4]||f[5]||f[6]||f[7])

#define Back_RRight     (b[0])
#define Back_Right      (b[1] || b[2])
#define Back_LLeft      (b[7])
#define Back_Left       (b[6] || b[5])
#define Back_Med        (b[3] || b[4])
#define Back_MMed       (b[2] || b[3] || b[4] || b[5])
#define Back_edge       (b[0] || b[7])
#define Back_All        (b[0]&&b[1]&&b[2]&&b[3]&&b[4]&&b[5]&&b[6]&&b[7])
#define Back_Eigher		(b[0]||b[1]||b[2]||b[3]||b[4]||b[5]||b[6]||b[7])
/**
 * ѭ������
 */
void ForwardTrack(rt_int16_t l_speed,rt_int16_t r_speed);       //��ǰ���Ҷ�ѭ��
void Forward_Track_all(rt_int16_t l_speed,rt_int16_t r_speed);  //��ǰ˫�Ҷ�ѭ��
void BackTrack(rt_int16_t l_speed,rt_int16_t r_speed);          //��󵥻Ҷ�ѭ��    

void slow_acceleration(rt_uint32_t uint_speed,rt_uint32_t end_speed,rt_uint32_t ms);//���Լ���ѭ��

void Road_to_Board(rt_uint16_t l_speed,rt_uint16_t r_speed);    //ѭ����ƽ̨
void Road_to_time(rt_int16_t l_speed,rt_int16_t r_speed,rt_uint32_t ti,u8 mode);    //ѭ��һ��ʱ��
void mot_to_time(rt_int16_t l_speed,rt_int16_t r_speed,rt_uint32_t ms);   //��һ��ʱ�䣨��ѭ������λ��ms
void front_all_track_with_time(rt_int16_t l_speed,rt_int16_t r_speed,rt_uint32_t ms);   //˫�Ҷ�ѭ��һ��ʱ��

/**
 * ��������
 */
void turn_angle(int ang,int l_speed,int r_speed);     //ת�����(ang:ת�ĽǶȣ�speed:�ٶȣ���ͬ)
void lower_platform8(u16 speed);    //��ƽ̨8
void lower_platform(u16 speed);     //��ƽ̨
void platform(void);    //��ƽ̨
void hw_track(rt_int16_t speed);       //�����߽��к���ѭ��
void hw_track_with_time(rt_int16_t speed,rt_uint16_t ms);//������ʱѭ������
void road2view_spot(rt_uint16_t speed);     //��ײ����
void though_brige(u16 speed);       //ͨ������
static void turn_right_angle(rt_int32_t speed);
void through_qqb(void);         //ͨ�����ΰ�
void road_to_sloping(void);     //ͨ����б·��

/**
 * ѭ����·�ڵĺ���
 */
void truck2one_point(u8 point,u16 speed,u8 mode);   //ѭ����ĳ��·��(point: ���ĻҶȵ�)mode:0�����Ҷ�ѭ��  1��˫�Ҷ�ѭ��
void back_truck2one_point(u8 point,u16 speed);      //����ѭ����ĳ��·��(point: ���ĻҶȵ�)
void truck2one_point_use_laser(u8 point,u16 speed,u8 mode); //ѭ����ĳ��·�ڲ��ü��⵽���м�(�ʺϼ����������ĳ�)(point: ���ĻҶȵ�)
void back_truck2one_point_use_laser(u8 point,u16 speed);    //����ѭ����·�ڲ��ü���

/**
 * �����ٰ�ĳ���
 */
void throw_slow_down(rt_uint16_t l_speed,rt_uint16_t r_speed);
void throw_3slow_down(rt_uint16_t l_speed,rt_uint16_t r_speed);
void throw_3slow_downb(rt_uint16_t l_speed,rt_uint16_t r_speed);
    
/**
 * ����ϵ㣬�����������е������ֹͣ�����г̿��غ���������
 */
void travel_switch_test(void);//�ж�map����ͨ���г̿������½���
rt_uint8_t cross_door(rt_uint16_t speed,rt_uint16_t ms);

static void write_flash_11(int8_t write_door);

#endif
