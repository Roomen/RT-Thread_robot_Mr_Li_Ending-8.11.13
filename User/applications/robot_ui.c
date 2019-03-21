#include <board.h>
#include <rtthread.h>
#include "bsp_oled.h"
#include "robot_ui.h"
#include "bmp.h"
#include "bsp_grayscale.h"
#include "bsp_key.h"
#include "operation.h"
#include "bsp_flash.h"


void robot_ui_init(void)
{
    
    oled.clear();
    oled.show_string(0,0,"  Motor         ",12);
    oled.show_string(0,2,"->RobotRun      ",12);
    oled.show_string(0,4,"  GrayScale     ",12);
    oled.show_string(0,6,"  Laser&Infrared",12);

//    int8_t aklsdfjkaslj=0;
//    
//    if(!KEY_Up)
//    {
//        aklsdfjkaslj=4;
//        Flash_EraseSector(FLASH_Sector_11);
//        Flash_Write8BitDatas(DoorsAddr,1,&aklsdfjkaslj);
//        oled.show_string(0,0,"Flash Is",12);
//        oled.show_number(80,0,aklsdfjkaslj,1,16);
//        while(!KEY_Cancel)
//        {
//            rt_thread_delay(10);
//        }
//        rt_thread_delay(500);
//    }
//    
//    if(!KEY_Cancel)
//    {
//        aklsdfjkaslj=0;
//        Flash_EraseSector(FLASH_Sector_11);
//        Flash_Write8BitDatas(DoorsAddr,1,&aklsdfjkaslj);
//        oled.show_string(0,0,"Flash Is Cleared",12);
//        while(!KEY_Cancel)
//        {
//            rt_thread_delay(10);
//        }
//        rt_thread_delay(500);
//    }
//    if(!KEY_Ok)
//    {
//        oled.clear();
//        oled.show_string(0,0,"  Motor         ",12);
//        oled.show_string(0,2,"->RobotRun      ",12);
//        oled.show_string(0,4,"  GrayScale     ",12);
//        oled.show_string(0,6,"  Laser&Infrared",12);
//        while(!KEY_Ok)
//        {
//            rt_thread_delay(10);
//        }
//        rt_thread_delay(500);
//    }
//    else
//        RobotRun_Operation();
}

void RobotRun_Frame(void)
{
//	oled.clear();
	oled.show_string(0,0,"  Motor         ",12);
	oled.show_string(0,2,"->RobotRun      ",12);
	oled.show_string(0,4,"  GrayScale     ",12);
	oled.show_string(0,6,"  Laser&Infrared",12);
}
void GrayScale_Frame(void)
{
//	oled.clear();
	oled.show_string(0,0,"  RobotRun      ",12);
	oled.show_string(0,2,"->GrayScale     ",12);
	oled.show_string(0,4,"  Laser&Infrared",12);
	oled.show_string(0,6,"  Compass       ",12);
}
void Laser_Frame(void)
{
//	oled.clear();
	oled.show_string(0,0,"  GrayScale     ",12);
	oled.show_string(0,2,"->Laser&Infrared",12);
	oled.show_string(0,4,"  Compass       ",12);
    oled.show_string(0,6,"  Motor         ",12);
    
}
void Compass_Frame(void)
{
//	oled.clear();
	oled.show_string(0,0,"  Laser&Infrared",12);
	oled.show_string(0,2,"->Compass       ",12);
    oled.show_string(0,4,"  Motor         ",12);
    oled.show_string(0,6,"  RobotRun      ",12);
}
void Motor_Frame(void)
{
//	oled.clear();
    oled.show_string(0,0,"  Compass       ",12);
	oled.show_string(0,2,"->Motor         ",12);
    oled.show_string(0,4,"  RobotRun      ",12);
	oled.show_string(0,6,"  GrayScale     ",12);
}
void Gray_Front_Frame(void)
{
	oled.clear();
    oled.show_string(0,0,"  Bool          ",12);
	oled.show_string(0,2,"->Front         ",12);
	oled.show_string(0,4,"  Back          ",12);
	
}
void Gray_Back_Frame(void)
{
	oled.clear();
	oled.show_string(0,0,"  Front         ",12);
	oled.show_string(0,2,"->Back          ",12);
	oled.show_string(0,4,"  Bool          ",12);
}
void GrayBool_Frame(void)
{
	oled.clear();
	oled.show_string(0,0,"  Back          ",12);
	oled.show_string(0,2,"->Bool          ",12);
    oled.show_string(0,4,"  Front         ",12);
}

void Show_FrontBool(void)
{
	oled.show_number(20,3,f[0],1,12);   
	oled.show_number(32,3,f[1],1,12);   
	oled.show_number(44,3,f[2],1,12);    
	oled.show_number(56,3,f[3],1,12);    
	oled.show_number(68,3,f[4],1,12);   
	oled.show_number(80,3,f[5],1,12);    
	oled.show_number(92,3,f[6],1,12);    
	oled.show_number(104,3,f[7],1,12);	
}

void Show_BackBool(void)
{
	oled.show_number(20 ,5,b[0],1,12);   
	oled.show_number(32 ,5,b[1],1,12);   
	oled.show_number(44 ,5,b[2],1,12);    
	oled.show_number(56 ,5,b[3],1,12);    
	oled.show_number(68 ,5,b[4],1,12);   
	oled.show_number(80 ,5,b[5],1,12);    
	oled.show_number(92 ,5,b[6],1,12);    
	oled.show_number(104,5,b[7],1,12);
}
void Show_Grayscale(int16_t* grayscale)
{
	oled.show_number(0 ,2,grayscale[0],4,12);
	oled.show_number(32,2,grayscale[1],4,12);
	oled.show_number(64,2,grayscale[2],4,12);
	oled.show_number(96,2,grayscale[3],4,12);
	oled.show_number(0 ,4,grayscale[4],4,12);
	oled.show_number(32,4,grayscale[5],4,12);
	oled.show_number(64,4,grayscale[6],4,12);
	oled.show_number(96,4,grayscale[7],4,12);
}





