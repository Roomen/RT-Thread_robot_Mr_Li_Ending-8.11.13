#ifndef __MAP_H__
#define __MAP_H__

#include "stm32f4xx.h" 

#define second_order
extern u16 original_angle_x;
static void platform1_to_point4(void);
static void programme1(void);//Loop 1 һ���ſ��ŵ����
static void programme2(void);//Loop 2 һ���ſ��ŵ����
static void programme4(void);//Loop 4 һ���ſ��ŵ����
static void programme4_loop2(void);//4���ſ���


static void go_to78platform(void);//ͨ��7,8ƽ̨
static void home(void);//�ؼ�
int run_map_init(void);

static void door(void);
#endif

