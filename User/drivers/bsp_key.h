#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include <rtthread.h>
#include "stm32f4xx.h"
#include "sys.h"
#define DEBOUNCE_SHORT_TIME     2   		// �ᴥ��������ʱ��5����λ��50���룩
#define DEBOUNCE_LONG_TIME      3//10 // ������ʱ��DEBOUNCE_COUT_FIRST+DEBOUNCE_COUT_INTERVAL*DEBOUNCE_LONG_TIME����λ��50���룩
#define DEBOUNCE_COUT_FIRST     20 	// ���������ʱ��100����λ��50���룩
#define DEBOUNCE_COUT_INTERVAL  3   // ���������ʱ��20����λ��50���룩

#define KEY_Cancel          PCin(10)

#define KEY_Ok              PAin(14)

#define KEY_Up              PAin(13)

enum KEY_VALUE
{
	KEY_VALUE_ERR = 0,
	KEY_VALUE_UP,
	KEY_VALUE_DOWN,
	KEY_VALUE_ENSURE,
	KEY_VALUE_CANCEL
};


void key_thread_entry(void* parameter);

#endif /* __KEY_H */
