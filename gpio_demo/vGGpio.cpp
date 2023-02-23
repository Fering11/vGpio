#include "vGGpio.h"
#include <exception>
#include <stdexcept>
#include <wiringPi.h>
#include <softPwm.h>
#include <assert.h>
#include <sstream>

#include <iostream>

#ifdef _VG_THROW
#undef _VG_THROW
#endif
#define _VGPIO_INVAILD_PIN  41 //不存在或者错误的pin

//wiring在VS编程必须添加system命令
void __setPinMode(int _pin, int _statue) {
	pinMode(_pin, _statue);
	::std::stringstream sstr;
	sstr << u8"gpio export " << _pin << (_statue ? u8" out" : u8" in");
	system(sstr.str().c_str());
}

using namespace vGGpio::detail;
namespace vGGpio {
static pin __spin[40] = { };
namespace detail {

void __vg_exception(const char* const _msg) {
#ifndef VGGPIO_NO_ALLOW_EXCEPTION
	throw std::runtime_error(reinterpret_cast<const char* const>(_msg));
#else
	(void());
#endif
} //detail
}

//输入物理编码
extern pin& get(pos_t _pin) {
	if (detail::_is_invaild_board_port(_pin)) {
		detail::__vg_exception("Pin is vaild");
	}
	return __spin[_pin - 1];
}

int setup(){
	int result = wiringPiSetupSys();
	for (pos_t i = 0; result != -1, i != 40; ++i) {
		__spin[i].data_.reset(new basic_pin(i + 1));
		//__spin[i].__init_data(i + 1);
	}
	return result;
}

void delay(unsigned int ms){
	::delay(ms);
}

void delayMicroseconds(unsigned int mms){
	::delayMicroseconds(mms);
}

basic_pin::~basic_pin(){
	_DO("pin:%d, statue:%d ,release", pin_, statue());
	reset();
}
void basic_pin::reset() {
	//gpio端口才可以使用这个
	if (is_gpio_port()) {
		switch (statue())
		{
		case INPUT:
		case OUTPUT:
		case PWM_OUTPUT:
		case GPIO_CLOCK:
		case PWM_TONE_OUTPUT:
		case SOFT_TONE_OUTPUT: {
			digitalWrite(get_bcm(), LOW);
			__setPinMode(get_bcm(), INPUT); //默认在INPUT状态
			_DO("pin:%d,reset normally", pin_);
			break;
		}
		case SOFT_PWM_OUTPUT: {
			softPwmStop(get_bcm());
			digitalWrite(get_bcm(), LOW);
			__setPinMode(get_bcm(), INPUT); //默认在INPUT状态
			_DO("pin:%d,reset pwm", pin_)
			break;
		}
		default:
			break;
		}
	}
}



void pin::statue(pin_mode _t){
	switch (_t)
	{
	case INPUT:
	case OUTPUT:
	case PWM_OUTPUT:
	case GPIO_CLOCK:
	case SOFT_PWM_OUTPUT:
	case SOFT_TONE_OUTPUT:
	case PWM_TONE_OUTPUT: {
		__setPinMode(data_->get_bcm(), _t);
		break;
	}
	default:
		break;
	}
	(data_->mark_ &= 0xFFFFFFF8) |= _t;
	_DO("pin:%d,mark:%d", borad(), (data_->mark_))
}
void pin::write(pin_statue _t){
	digitalWrite(bcm(), _t);
}

bool pin::pwm_create(int _value, int _range)const {
	softPwmCreate(bcm(), _value, _range);
}

void pin::pwm_stop()const {
	softPwmStop(bcm());
}

void pin::pwm_set(unsigned int _value)const {
	softPwmWrite(bcm(), _value);
}

pwm_controllor::pwm_controllor(const basic_pin& _pin, int _start, int _begin, int _range) :basic_pin(_pin) {
	begin_ = _begin;
	range_ = _range;
	start_ = _start;
	__setPinMode(get_bcm(), OUTPUT);
	softPwmCreate(get_bcm(), _start, _begin + _range);
}

void pwm_controllor::set_pwm(int _value){
	softPwmWrite(get_bcm(), _value);
}

void pwm_controllor::set_begin(int _value){
	begin_ = _value;
	softPwmCreate(get_bcm(), start_, begin_ + range_);
}

void pwm_controllor::set_range(int _value){
	range_ = _value;
	softPwmCreate(get_bcm(), start_, begin_ + range_);
}

void pwm_controllor::stop(){
	softPwmStop(get_bcm());
}

}

