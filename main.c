#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"


#define PERIOD_TIME	   5000// 20K

int main(void)
 {
		FPUEnable();               //FPUʹ��
	    FPULazyStackingEnable();   //ʹ��FPU��ջ

	    //����ϵͳʱ��PLL�̶����Ϊ200MHz��tiva������Ƶ��Ϊ80M��
	    //2.5��Ƶ200/2.5=80M  ������������������  20150109
	    //����ȡ��4��Ƶ��200/4=50M
	    SysCtlClockSet( SYSCTL_SYSDIV_4   | SYSCTL_USE_PLL |
	    			    SYSCTL_OSC_MAIN   | SYSCTL_XTAL_16MHZ );

		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);   //ʹ��GPIOF�ڣ�LED��
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);//����GPIO��Ϊ���ģʽ
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);          //red
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);          //blue
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,GPIO_PIN_3); //green�������� ���Գ�ʼ����
	    return 0;

	    IntMasterEnable();//ϵͳ���жϿ�

	    Init_step_motor();




}


 void Init_step_motor( void )
 {
 	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);	        //����PWMʱ�ӣ�1��Ƶ
 	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);	//ʹ������PWM0ģ��
 	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//ʹ��PWM0ʹ�õ�����GPIOB
 	GPIOPinConfigure(GPIO_PB7_M0PWM1);			//����PB7���ù���ΪPWM
 	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);//����PB7ΪPWM0ģ��ʹ��
 	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);	//����PWM0ģ�飬PWM0������0�����¼��������������²���
 	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, PERIOD_TIME);		//����PB7��M0PWM1��������PWM����
 	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PERIOD_TIME / 2);//����PB7��M0PWM1����ռ�ձ�50%
 	PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);			//ʹ�����
 	PWMGenEnable(PWM0_BASE, PWM_GEN_0);						//����PWM0������0�Ķ�ʱ��
 	PWMSyncTimeBase(PWM0_BASE, PWM_GEN_0);				    //ʹ�ܷ�����ģ�������ͬ��

 //	      ��ʼ�� DIR+ PB6; EN+ PB5;
 //    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_6|GPIO_PIN_5);
 //    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_6,GPIO_PIN_6);
 //    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,0);
 }
