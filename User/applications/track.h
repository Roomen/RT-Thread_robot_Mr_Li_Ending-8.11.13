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
 * 循迹函数
 */
void ForwardTrack(rt_int16_t l_speed,rt_int16_t r_speed);       //向前单灰度循迹
void Forward_Track_all(rt_int16_t l_speed,rt_int16_t r_speed);  //向前双灰度循迹
void BackTrack(rt_int16_t l_speed,rt_int16_t r_speed);          //向后单灰度循迹    

void slow_acceleration(rt_uint32_t uint_speed,rt_uint32_t end_speed,rt_uint32_t ms);//线性加速循迹

void Road_to_Board(rt_uint16_t l_speed,rt_uint16_t r_speed);    //循迹到平台
void Road_to_time(rt_int16_t l_speed,rt_int16_t r_speed,rt_uint32_t ti,u8 mode);    //循迹一段时间
void mot_to_time(rt_int16_t l_speed,rt_int16_t r_speed,rt_uint32_t ms);   //走一段时间（不循迹）单位：ms
void front_all_track_with_time(rt_int16_t l_speed,rt_int16_t r_speed,rt_uint32_t ms);   //双灰度循迹一段时间

/**
 * 其他程序
 */
void turn_angle(int ang,int l_speed,int r_speed);     //转弯程序(ang:转的角度；speed:速度，下同)
void lower_platform8(u16 speed);    //下平台8
void lower_platform(u16 speed);     //下平台
void platform(void);    //上平台
void hw_track(rt_int16_t speed);       //对两边进行红外循迹
void hw_track_with_time(rt_int16_t speed,rt_uint16_t ms);//红外延时循迹函数
void road2view_spot(rt_uint16_t speed);     //到撞景点
void though_brige(u16 speed);       //通过长桥
static void turn_right_angle(rt_int32_t speed);
void through_qqb(void);         //通过跷跷板
void road_to_sloping(void);     //通过倾斜路面

/**
 * 循迹到路口的函数
 */
void truck2one_point(u8 point,u16 speed,u8 mode);   //循迹到某个路口(point: 检测的灰度点)mode:0、单灰度循迹  1、双灰度循迹
void back_truck2one_point(u8 point,u16 speed);      //反向循迹到某个路口(point: 检测的灰度点)
void truck2one_point_use_laser(u8 point,u16 speed,u8 mode); //循迹到某个路口并用激光到正中间(适合激光在腰部的车)(point: 检测的灰度点)
void back_truck2one_point_use_laser(u8 point,u16 speed);    //反向循迹到路口并用激光

/**
 * 过减速板的程序
 */
void throw_slow_down(rt_uint16_t l_speed,rt_uint16_t r_speed);
void throw_3slow_down(rt_uint16_t l_speed,rt_uint16_t r_speed);
void throw_3slow_downb(rt_uint16_t l_speed,rt_uint16_t r_speed);
    
/**
 * 程序断点，加入后程序运行到这里会停止，按行程开关后会继续运行
 */
void travel_switch_test(void);//中断map，并通过行程开关重新进入
rt_uint8_t cross_door(rt_uint16_t speed,rt_uint16_t ms);

static void write_flash_11(int8_t write_door);

#endif
