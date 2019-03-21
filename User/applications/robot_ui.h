#ifndef __ROBOT_UI_H
#define __ROBOT_UI_H

void robot_ui_init(void);

void RobotRun_Frame(void);
void GrayScale_Frame(void);
void Laser_Frame(void);
void Compass_Frame(void);
void Motor_Frame(void);
void Gray_Front_Frame(void);
void Gray_Back_Frame(void);
void GrayBool_Frame(void);

void Show_FrontBool(void);
void Show_BackBool(void);
void Show_Grayscale(int16_t* grayscale);






#endif  

