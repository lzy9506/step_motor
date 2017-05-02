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
		FPUEnable();               //FPU使能
	    FPULazyStackingEnable();   //使能FPU堆栈

	    //配置系统时钟PLL固定输出为200MHz，tiva最高输出频率为80M，
	    //2.5分频200/2.5=80M  激光器不能正常工作  20150109
	    //所以取了4分频，200/4=50M
	    SysCtlClockSet( SYSCTL_SYSDIV_4   | SYSCTL_USE_PLL |
	    			    SYSCTL_OSC_MAIN   | SYSCTL_XTAL_16MHZ );

		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);   //使能GPIOF口，LED灯
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);//设置GPIO口为输出模式
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);          //red
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);          //blue
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,GPIO_PIN_3); //green灯亮起来 测试初始程序
	    return 0;

	    IntMasterEnable();//系统总中断开

	    Init_step_motor();




}


 void Init_step_motor( void )
 {
 	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);	        //设置PWM时钟，1分频
 	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);	//使能外设PWM0模块
 	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能PWM0使用的外设GPIOB
 	GPIOPinConfigure(GPIO_PB7_M0PWM1);			//配置PB7复用功能为PWM
 	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);//配置PB7为PWM0模块使用
 	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);	//配置PWM0模块，PWM0生成器0，向下计数并且立即更新参数
 	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, PERIOD_TIME);		//设置PB7（M0PWM1）产生的PWM周期
 	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, PERIOD_TIME / 2);//设置PB7（M0PWM1）的占空比50%
 	PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);			//使能输出
 	PWMGenEnable(PWM0_BASE, PWM_GEN_0);						//启动PWM0发生器0的定时器
 	PWMSyncTimeBase(PWM0_BASE, PWM_GEN_0);				    //使能发生器模块计数器同步

 //	      初始化 DIR+ PB6; EN+ PB5;
 //    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_6|GPIO_PIN_5);
 //    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_6,GPIO_PIN_6);
 //    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,0);
 }
