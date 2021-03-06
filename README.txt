/*
* File:    README.txt
* This file is part of NBUT --- Robot Car
* COPYRIGHT (C) 2018, System Engineerging Development Team
* Mail:  lishaobin157@outlook.com
* QQ:  1256784485	
*
* File Description:
*
* Change Logs:
* Date           Author			Notes
* 2018-11-13     Li ShaoBin		8.11.13
*/

程序支持：
	已测试：
		1、电机驱动 
		2、GY26	地磁传感器(电子指南针)	用GY26的时候注意模块要离电机远一点，减小电机磁体对模块产生干扰
		3、JY61(六轴陀螺仪)				注：JY61和GY26都是测量角度的传感器,用来控制车的转角，所以只需一个即可，想用什么自己选择。
		4、LED
		5、OLED
		6、Finsh
		7、Laser(激光传感器)
		8、Key(按键)
		9、Infrared(红外传感器)
		10、寻迹可用(参数需自行调整)
/*****************************************************************************************************************************************************/
更新说明：
		8.11.13：
			修改程序风格，主要是结构体的应用,获取角度值时直接输入angle.xxx即可。
			主要修改内容：oled的调用，motor的调用，steering的调用，angle角度值的调用。
			建议重写track.c 里面内容较乱，建议使用结构体函数写法。写程序时只需要写一个单词再加个 . 就可以很快地选出后面的单词。
		8.5.02：
			到实际场地再做调整
		8.4.25：
			1、修改路线方案：不去龙王山和普陀山，改去“7兑”平台。可以多拿71-22=49分。

		提示：这是个尝试，因为7号平台前减速板不好走，若失败率较高，放弃此方案
		8.4.14-4.24：
			1、路线一（一号门开启）已经全部完成，稳定性：80%
			2、路线二（一号门关闭但二号门开启），基本完成，稳定性：75%
			3、添加部分函数的finsh支持，可以在上位机控制Smart Robot完成指定动作，e.g:输入：led(0,1)时led0亮，led(0,0)时led0灭……
		8.4.8~4.14日：
			1、更新了循迹前后灰度循迹的支持
			2、加入红外/激光传感器：PB8(右激光/红外)，PB9(左激光/红外)，用于长桥、跷跷板等红边循迹，命名：hwt_l(左)、hwt_r(右)
			3、添加行程开关，使用行程开关的常开触点，一端连在GND，另一端连在PF3
			4、上平台、下平台等函数都已经写好，每台车都配合自己程序调试好再使用。
			5、多个线程优化，简化使用。
		8.4.7日更新：
			1、更新了GY26的转角支持（但个人建议使用JY-61，经测试，地磁转角并不在水平面上，也就是说实际转角和地磁转角不同，且有较大差别(30°左右)）
			2、根据驱动板的要求，将电机驱动的参数进行了修改，具体在
				1、pwm的初始化里，预装值为1054，实际用时最多调到1000，不高于最大占空比的95%。
				2、pwm频率设要求为4~20K，因此经计算后设置分频为7，即装入7-1
			3、反思：pwm的参数并不是随便设置的，第一个值决定你的可调节分辨率，再同时和第二个值决定你的pwm频率。要根据具体设备来调整。
			4、电机驱动程序改进，即mot程序改动。原问题是当两轮速度一正一反时，电机转速会翻转（原本应该是100的速度，但会突变为1000-100）
			温馨提示：距离比赛没有多长时间了，希望同学们加油调试，取得好成绩！
		4月6日更新：
			转角函数的完善，转弯更快更准（JY-61）。原理： 通过两段式转弯。先按照设定的较快的速度转1/2设定的角度，再一以pwm100的速度旋转到指定角度。
		4月5日更新：
			转角的时候小车是在指定的时候停下来的，但是由于小车惯性的因素，小车会多转一个角度；例如当需要转90°时，若设置为90°，
				则实际转角大于90°，因此设置的时候我们可以设置在70~85°左右。
			暂时仅支持JY61，GY26的后续给出。
			此函数我会有后续完善，敬请期待更新。





			