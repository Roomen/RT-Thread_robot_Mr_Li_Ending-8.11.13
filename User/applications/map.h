#ifndef __MAP_H__
#define __MAP_H__

#include "stm32f4xx.h" 

#define second_order
extern u16 original_angle_x;
static void platform1_to_point4(void);
static void programme1(void);//Loop 1 一号门开着的情况
static void programme2(void);//Loop 2 一号门开着的情况
static void programme4(void);//Loop 4 一号门开着的情况
static void programme4_loop2(void);//4号门开着


static void go_to78platform(void);//通过7,8平台
static void home(void);//回家
int run_map_init(void);

static void door(void);
#endif

