#include "vGGpio.h"
//#include <wiringPi.h>
#include <softPwm.h>

#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>
void vsetPinMode(int _pin, int _statue) {
	//pinMode(_pin, _statue);
	std::stringstream sstr;
	sstr << u8"gpio export " << _pin << (_statue ? u8" out" : u8" in");
	system(sstr.str().c_str());
	std::cout << "exec: " << sstr.str() << std::endl;
}
#if 0
// LED Pin - wiringPi pin 0 是 BCM_GPIO 17。
// 利用 wiringPiSetupSys 进行初始化时必须使用 BCM 编号
// 选择其他 pin 编号时，请同时使用 BCM 编号
// 更新 Property Pages - Build Events - Remote Post-Build Event 命令
// 它使用 gpio 导出进行 wiringPiSetupSys 的设置



#define	LED_RIGHT	17
#define LED_LEFT 21
#define PWM_PIN 18
void init_gpio_mode() {
	//vsetPinMode(LED_LEFT, OUTPUT);
	//vsetPinMode(LED_RIGHT, OUTPUT);
	//vsetPinMode(20, OUTPUT);
	//vsetPinMode(16, OUTPUT);
	vsetPinMode(PWM_PIN, PWM_OUTPUT);

	//pwmSetMode(PWM_MODE_MS);
	//pwmWrite(PWM_PIN, 100);
}

int main(void)
{
	wiringPiSetup();

	init_gpio_mode();
	/*
	bool light=false;
	while (true)
	{
		std::string temp;
		std::getline(std::cin, temp);

		digitalWrite(LED_LEFT, light);
		digitalWrite(LED_RIGHT, !light);
		digitalWrite(20, light);
		digitalWrite(16, !light);
		light = !light;
	}
	*/
	int bright;
	while (1) {
		for (bright = 0; bright < 1000; bright++) {
			// bright为 0 ~ 1000，最大是1023
			pwmWrite(PWM_PIN, bright);
			delay(1);
		}
		for (bright = 1000; bright >= 0; bright--) {
			pwmWrite(PWM_PIN, bright);
			delay(1);
		}
	}
	return 0;
}
#endif 
#if 0
#define PWM_PIN     0
//返回对象
class pwm_control {
public:
	pwm_control() {
	}
	pwm_control(int _pin, int _start, int _begin, int _range) {
		pin_ = _pin;
		begin_ = _begin;
		range_ = _range;
		start_ = _start;
		vsetPinMode(pin_, OUTPUT);
		softPwmCreate(pin_, _start, _begin + _range);//创建PWM控制器，在任意GPIO口输出PWM波形
	}
	void set_pwm(int _value) {
		//检查
		softPwmWrite(pin_, begin_ + _value);
	}
private:
	int pin_;
	int begin_;
	int range_;
	int start_;
};
pwm_control vcreatePWM(int _pin, int _start, int _begin, int _range) {
	pwm_control contorl(_pin, _start, _begin, _range);
	return contorl;
}
int main1(void)
{
	int bright = 0;
	// 初始化
	wiringPiSetup();
	//vsetPinMode(PWM_PIN, OUTPUT);
	int range = 50;//范围
	//软件PWM
	//softPwmCreate(PWM_PIN, 50, range);//创建PWM控制器，在任意GPIO口输出PWM波形

	auto ctor = vcreatePWM(PWM_PIN, 0, 0, 100);

	while (1) {
		for (bright = 0; bright < range; bright++) {
			ctor.set_pwm(bright);
			//softPwmWrite(PWM_PIN, bright);
			delay(10);
			//delayMicroseconds(100);
		}
		for (bright = range; bright >= 0; bright--) {
			ctor.set_pwm(bright);
			//softPwmWrite(PWM_PIN, bright);
			delay(10);
			//delayMicroseconds(100);
		}
	}
	softPwmStop(PWM_PIN);
	return 0;
}
#endif 
#if 0
int main() {
	vGGpio::setup();
	vGGpio::basic_pin pin(11);
	vGGpio::pwm_controllor pwm(pin, 0, 10, 100);
#if 0
	pwm.stop();
	pin.set_statue(vGGpio::OUTPUT);
	while (true)
	{
		pin.write(vGGpio::HIGH);
		vGGpio::delay(1000);
		pin.write(vGGpio::LOW);
		vGGpio::delay(1000);
		//std::cout << pin.get_name() << "  " << pin.get_bcm() << "  " << pin.get_wiringPi() << std::endl;
	}
#else
	pwm.set_statue(vGGpio::OUTPUT);
	int bright = 0;
	int range = 100;
	std::cout << "work" << std::endl;;
	std::cout << pwm.get_bcm() << pwm.get_name();
	pwm.set_pwm(200);
	while (1) {
		for (bright = 0; bright < range; bright++) {
			pwm.set_pwm(bright);
			vGGpio::delay(10);
		}
		for (bright = range; bright >= 0; bright--) {
			pwm.set_pwm(bright);
			vGGpio::delay(10);
		}
	}
#endif //频闪灯
	return 0;
}
#endif 

int main() {
	namespace vg = vGGpio;
	std::cout << "init statue:" << vg::setup() << std::endl;
	auto pin = vg::get(11);
#if 0
	pin.statue(vg::OUTPUT);
	pin.write(vg::HIGH);
	vg::delay(2000);
	pin.write(vg::LOW);
#else
	pin.statue(vg::SOFT_PWM_OUTPUT);
	int bright = 0;
	int range = 100;
	pin.pwm_create(0, 100);
	for (int i = 0; i != 5; ++i) {
		for (bright = 0; bright < range; bright++) {
			pin.pwm_set(bright);
			vGGpio::delay(10);
		}
		for (bright = range; bright >= 0; bright--) {
			pin.pwm_set(bright);
			vGGpio::delay(10);
		}
	}

#endif
	return 0;
}