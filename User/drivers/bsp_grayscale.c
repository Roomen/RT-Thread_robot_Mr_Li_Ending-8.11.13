#include <rtthread.h>
#include "bsp_grayscale.h"
#include "bsp_oled.h"
#include "bsp_led.h"


#define GrayscaleControl_RCCG	            RCC_AHB1Periph_GPIOG
#define GrayscaleControl_RCCE	            RCC_AHB1Periph_GPIOE

#define GrayscaleControl_PORTG	            GPIOG
#define GrayscaleControl_PORTE	            GPIOE

#define A0_PIN	    GPIO_Pin_10
#define A1_PIN	    GPIO_Pin_11
#define A2_PIN	    GPIO_Pin_12
#define B0_PIN	    GPIO_Pin_7      //E
#define B1_PIN	    GPIO_Pin_8      //E
#define B2_PIN	    GPIO_Pin_1      //G

#define GrayscaleControl_PING  (A0_PIN|A1_PIN|A2_PIN|B2_PIN)
#define GrayscaleControl_PINE  (B0_PIN|B1_PIN)

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t get_grayscale_stack[512];
static struct rt_thread get_grayscale_thread;

// ָ�򻥳�����ָ��
rt_mutex_t grayscale_mutex = RT_NULL;


//����ɼ����Ҷ�ֵ������
int16_t  FrontGrayscale[24];              
int16_t  BackGrayscale[24];
uint8_t  Color = 0;
uint8_t  f[8];
uint8_t  b[8];

//�ⲿ���� ǰ�Ҷ����ڴ���Ҷ�ֵ������
int16_t F_White[8];
int16_t F_Green[8];
int16_t F_Black[8];
//�ⲿ���� ��Ҷ����ڴ���Ҷ�ֵ������
int16_t B_White[8];
int16_t B_Green[8];
int16_t B_Black[8];

//����洢��Flash�кʹ�Flash�ж�ȡ��Ĵ洢������
int16_t  F_white_green[8];              //���ڴ����ɫ����ɫ��ƽ��ֵ
int16_t  F_white_black[8];              //���ڴ����ɫ�ͻ�ɫ��ƽ��ֵ
int16_t  F_green_black[8];              //���ڴ����ɫ����ɫ��ƽ��ֵ

int16_t B_white_green[8];               //���ڴ����ɫ����ɫ��ƽ��ֵ
int16_t B_white_black[8];               //���ڴ����ɫ�ͻ�ɫ��ƽ��ֵ
int16_t B_green_black[8];               //���ڴ����ɫ����ɫ��ƽ��ֵ



static void get_grayscale_entry(void* parameter)
{
    while(1)
    {
        rt_mutex_take(grayscale_mutex,RT_WAITING_FOREVER);
        switch(Color)
        {
            case Front_WhiteGreen:
                F_Compare(F_white_green);break;
            case Front_WhiteBlack:
                F_Compare(F_white_black);break;
            case Front_GreenBlack:
                F_Compare(F_green_black);break;
            case Back_WhiteGreen:
                B_Compare(B_white_green);break;
            case Back_WhiteBlack:
                B_Compare(B_white_black);break;
            case Back_GreenBlack:
                B_Compare(B_green_black);break;
            case Front_Back_WhiteGreen:       //˫�Ҷ���ǰѭ��
                F_Compare(F_white_green);
                B_Compare(B_white_green);
                break;
            default:
                break;
        }
        rt_mutex_release(grayscale_mutex);
        rt_thread_delay(1);
    }
}
int GetGrayscale(void)
{
	rt_err_t result;

	grayscale_mutex = rt_mutex_create("grayscale_mutex", RT_IPC_FLAG_FIFO);
	// init get_grayscale_thread
    result = rt_thread_init(&get_grayscale_thread,
                            "get_gray",
                            get_grayscale_entry,
                            RT_NULL,
                            (rt_uint8_t*)&get_grayscale_stack[0],
                            sizeof(get_grayscale_stack),
                            14,
                            10);
    if (result == RT_EOK)
    {
        rt_thread_startup(&get_grayscale_thread);
    }
	return 0;
}
//��ʼ���Ҷȴ�����
void grayscale_init(void)
{
	//��ʼ��CD4051
    //ǰ�Ҷ�:PG10 PG11 PG12
    //��Ҷ�:PE1  PE8  PG1
    GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

    RCC_AHB1PeriphClockCmd(GrayscaleControl_RCCG|GrayscaleControl_RCCE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOAʱ��

    GPIO_InitStructure.GPIO_Pin =  GrayscaleControl_PING;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GrayscaleControl_PORTG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =GrayscaleControl_PINE;
    GPIO_Init(GrayscaleControl_PORTE, &GPIO_InitStructure);

    //��ʼ���Ҷȴ�������ADC
	//ǰ�Ҷ�:PA5
	//��Ҷ�:PB0
    //��ʼ��ADC1ͨ��5 IO��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PA5 ͨ��5
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA5 ͨ��5
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
    ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
    ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 

    ADC_Init(ADC1 , &ADC_InitStructure);//ADC��ʼ��

    ADC_Cmd(ADC1, ENABLE);//����ADת����
}

//�������ܣ����chͨ����ADֵ
//ch:ͨ��ֵ ADC1_ch5��ADC2_ch8
uint16_t Get_Adc(uint8_t ch)
{
//    LED1=!LED1;
    //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_56Cycles );	         //ADC1,ADCͨ��,15������,��߲���ʱ�������߾�ȷ��			    
    ADC_SoftwareStartConv(ADC1);		                                         //ʹ��ָ����ADC1�����ת����������		 
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));                              //�ȴ�ת������
    return ADC_GetConversionValue(ADC1);	                                     //�������һ��ADC1�������ת�����
}

//�Ҷ�ƽ��ֵ�˲�
int16_t Get_Adc_Average(int8_t ch,int8_t times)
{
	int32_t temp_val=0;
	int8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		rt_hw_us_delay(1);
	}
	return temp_val/times;
}

//int HD(uint8_t i,uint8_t j)
//{
//	int t;
//	if(i==1)
//	{
//		switch(j)
//		{
//			case 1:A=0;B=0;C=0;rt_hw_us_delay(1);t=Get_Adc(5);break;
//			case 2:A=1;B=0;C=0;rt_hw_us_delay(1);t=Get_Adc(5);break;
//			case 3:A=0;B=1;C=0;rt_hw_us_delay(1);t=Get_Adc(5);break;
//			case 4:A=1;B=1;C=0;rt_hw_us_delay(1);t=Get_Adc(5);break;
//			case 5:A=0;B=0;C=1;rt_hw_us_delay(1);t=Get_Adc(5);break;
//			case 6:A=0;B=1;C=1;rt_hw_us_delay(1);t=Get_Adc(5);break;
//			case 7:A=1;B=0;C=1;rt_hw_us_delay(1);t=Get_Adc(5);break;
//			case 8:A=1;B=1;C=1;rt_hw_us_delay(1);t=Get_Adc(5);break;
//			default:break;
//	     }
//	}
//	else if(i==2)
//	{
//		switch(j)
//		{
//			case 1:D=1;E=1;F=1;rt_hw_us_delay(1);t=Get_Adc(8);break;
//			case 2:A=1;B=0;C=0;rt_hw_us_delay(1);t=Get_Adc(8);break;
//			case 3:D=1;E=0;F=1;rt_hw_us_delay(1);t=Get_Adc(8);break;
//			case 4:D=0;E=0;F=1;rt_hw_us_delay(1);t=Get_Adc(8);break;
//			case 5:D=1;E=1;F=0;rt_hw_us_delay(1);t=Get_Adc(8);break;
//			case 6:D=0;E=1;F=0;rt_hw_us_delay(1);t=Get_Adc(8);break;
//			case 7:D=1;E=0;F=0;rt_hw_us_delay(1);t=Get_Adc(8);break;
//			case 8:D=0;E=0;F=0;rt_hw_us_delay(1);t=Get_Adc(8);break;
//			default:break;
//	     }
//	}
//		
//	return t;
//}
//�ɼ����Ҷ�ֵ 
//��ڲ���Ϊ���ļ��ж����ȫ�ֱ���(TOP)
void Get_FGrayscale(int16_t* grayscale)
{

//8
    A=1;B=1;C=1;
    rt_hw_us_delay(1);
    grayscale[7]=Get_Adc_Average(5,1);//��ȡͨ��5��ת��ֵ��3��ȡƽ
//7
    A=0;B=1;C=1;
    rt_hw_us_delay(1);
    grayscale[6]=Get_Adc_Average(5,1);
//6
    A=1;B=0;C=1;
    rt_hw_us_delay(1);
    grayscale[5]=Get_Adc_Average(5,1);
//5
    A=0;B=0;C=1;
    rt_hw_us_delay(1);
    grayscale[4]=Get_Adc_Average(5,1);
//4
    A=1;B=1;C=0;
    rt_hw_us_delay(1);
    grayscale[3]=Get_Adc_Average(5,1);
//3
    A=0;B=1;C=0;
    rt_hw_us_delay(1);
    grayscale[2]=Get_Adc_Average(5,1);
//2
    A=1;B=0;C=0;
    rt_hw_us_delay(1);
    grayscale[1]=Get_Adc_Average(5,1);
//1
    A=0;B=0;C=0;
    rt_hw_us_delay(1);
    grayscale[0]=Get_Adc_Average(5,1);
}

void Get_BGrayscale(int16_t* grayscale)
{
//8
		D=1;E=1;F=1;
        rt_hw_us_delay(1);
		grayscale[7]=Get_Adc_Average(8,1);           

//7
		D=0;E=1;F=1;
        rt_hw_us_delay(1);
		grayscale[6]=Get_Adc_Average(8,1);           
		
//6
		D=1;E=0;F=1;
        rt_hw_us_delay(1);
		grayscale[5]=Get_Adc_Average(8,1);            
		
//5
		D=0;E=0;F=1;
        rt_hw_us_delay(1);
		grayscale[4]=Get_Adc_Average(8,1);           
		
//4
		D=1;E=1;F=0;
        rt_hw_us_delay(1);
		grayscale[3]=Get_Adc_Average(8,1);           
		
//3
		D=0;E=1;F=0;
        rt_hw_us_delay(1);
		grayscale[2]=Get_Adc_Average(8,1);            
		
//2
		D=1;E=0;F=0;
        rt_hw_us_delay(1);
		grayscale[1]=Get_Adc_Average(8,1);           
		
//1
		D=0;E=0;F=0;
        rt_hw_us_delay(1);
		grayscale[0]=Get_Adc_Average(8,1);           		
}
void F_Compare(int16_t* aver)
{
	uint8_t i;
	int16_t grayscale[8];
	Get_FGrayscale(grayscale);
	for(i=0;i<8;i++)
	{
		f[i] = (grayscale[i]>aver[i]);
	}
}

void B_Compare(int16_t* aver)
{
	uint8_t i;
	int16_t grayscale[8];
	Get_BGrayscale(grayscale);
	for(i=0;i<8;i++)
	{
		b[i] = (grayscale[i]>aver[i]);
	}
}
