#pragma once
#ifndef VGGPIO_HEAD
#define VGGPIO_HEAD
//通过定义 VGGPIO_NO_ALLOW_EXCEPTION 来阻止抛出异常
//#define VGGPIO_NO_ALLOW_EXCEPTION
/*
*       vGGpio head
* 针对 mwiringpi 的C++封装,针对于树莓派zero w
*  +-----+-----+---------+------+---+-Pi ZeroW-+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 | ALT0 | 1 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 | ALT0 | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 0 | IN   | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | IN   | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |  OUT | 1 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI | ALT0 | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO | ALT0 | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK | ALT0 | 0 | 23 || 24 | 1 | OUT  | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | OUT  | CE1     | 11  | 7   |
 |   0 |  30 |   SDA.0 |  OUT | 0 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
 |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
 |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  |
 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | IN   | GPIO.27 | 27  | 16  |
 |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  |
 |     |     |      0v |      |   | 39 || 40 | 1 | IN   | GPIO.29 | 29  | 21  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+-Pi ZeroW-+---+------+---------+-----+-----+
* 
*/

#include <string>
#include <tuple>
#include <memory>
#include <iostream>
#ifndef _DEBUG
#define _DO(args...)    do{printf(args);printf("\n");}while(0);
#else
#define _DO(args...)    
#endif

#if __cplusplus < 201703L
#error "at least C++17"
#endif
#undef _VG_CONSTEXPR17
#if __cplusplus >= 201703L
#define _VG_CONSTEXPR17 constexpr
#else
#define _VG_CONSTEXPR17 
#endif

#ifdef _VG_THROW
#undef _VG_THROW
#endif

#define _VGPIO_INVAILD_PIN  41 //不存在或者错误的pin

namespace vGGpio {

using vstring = ::std::string;
using pos_t = unsigned char;
using statue_t = short;

enum pin_mode {
	INPUT, OUTPUT,
	PWM_OUTPUT, GPIO_CLOCK,
	SOFT_PWM_OUTPUT,
	SOFT_TONE_OUTPUT,
	PWM_TONE_OUTPUT,
};
enum pin_statue {
	LOW,HIGH
};
enum pi_model {
	PI_MODEL_A,
	PI_MODEL_B,
	PI_MODEL_AP,
	PI_MODEL_BP,
	PI_MODEL_2,
	PI_ALPHA,
	PI_MODEL_CM,
	PI_MODEL_07,
	PI_MODEL_3B,
	PI_MODEL_ZERO,
	PI_MODEL_CM3,
	PI_MODEL_ZERO_W = 12,
	PI_MODEL_3BP,
	PI_MODEL_3AP = 14,
	PI_MODEL_CM3P,
	PI_MODEL_4B,
};
enum pi_version {
	PI_VERSION_1,
	PI_VERSION_1_1,
	PI_VERSION_1_2,
	PI_VERSION_2,
};
enum pi_maker{
	PI_MAKER_SONY,
	PI_MAKER_EGOMAN,
	PI_MAKER_EMBEST,
	PI_MAKER_UNKNOWN
};
class basic_pin;
class pwm_controllor;
class pin;
extern pin& get(pos_t _pin);
namespace detail {
using namespace vGGpio;
_VG_CONSTEXPR17 int strcmpare(const char* _s1, const char* _s2) {
	using tt = vstring::traits_type;
	return tt::compare(_s1, _s2, ::std::min(tt::length(_s1), tt::length(_s2)));
}

// 物理引脚 BCM编码 wiringPi编码 -1代表无编码
using __pin_struct = ::std::tuple<vGGpio::pos_t, vGGpio::pos_t, vGGpio::pos_t, const char* const >;
_VG_CONSTEXPR17 static __pin_struct __pin_data[41] = {
	{1,  -1, -1, "3.3v"},	 {2,  -1, -1,  "5v"},
	{3,  2,  8,  "sda.1"},	 {4,  -1, -1,  "5v"},
	{5,  3,  9,  "scl.1"},	 {6,  -1, -1,  "0v"},
	{7,  4,  7,  "gpio.7"},	 {8,  14, 15,  "txd"},
	{9,  -1, -1, "0v"},	 {10, 15, 16,  "rxd"},
	{11, 17, 0, "gpio.0"},	 {12, 18,  1,  "gpio.1"},
	{13, 27, 2,  "gpio.2"},	 {14, -1, -1,  "0v"},
	{15, 22, 3,  "gpio.3"},	 {16, 23,  4,  "gpio.4"},
	{17, -1, -1, "3.3v"},	 {18, 24,  5,  "gpio.5"},
	{19, 10, 12, "mosi"},	 {20, -1, -1,  "0v"},
	{21, 9,  13, "miso"},	 {22, 25,  6,  "gpio.6"},
	{23, 11, 14, "sclk"},	 {24, 8,  10,  "ce0"},
	{25, -1, -1, "0v"},	 {26, 7,  11,  "ce1"},
	{27, 0,  30, "sda.0"},	 {28, 1,  31,  "scl.0"},
	{29, 5,  21, "gpio.21"}, {30, -1, -1,  "0v"},
	{31, 6,  22, "gpio.22"}, {32, 12, 26,  "gpio.26"},
	{33, 13, 23, "gpio.23"}, {34, -1, -1,  "0v"},
	{35, 19, 24, "gpio.24"}, {36, 16, 27,  "gpio.27"},
	{37, 26, 25, "gpio.25"}, {38, 20, 28,  "gpio.28"},
	{39, -1, -1, "0v"},	 {40, 21, 29,  "gpio.29"},
	{_VGPIO_INVAILD_PIN,-1,-1,"error"}//用来标记错误
};

template<class _Pre>
_VG_CONSTEXPR17 auto& _iter_pin_data(_Pre&& _fn) {
	for (int i = 1; i < 40; ++i) {
		if (_fn(__pin_data[i])) {
			return __pin_data[i];
		}
	}
	return __pin_data[40];
}

_VG_CONSTEXPR17 bool _is_invaild_board_port(pos_t _st) {
	return (_st < 1 || _st > 40);
}
_VG_CONSTEXPR17 bool _is_invaild_bcm_port(pos_t _st) {//28
	return (_st < 0 || _st > 27);
}
_VG_CONSTEXPR17 bool _is_invaild_wpi_port(pos_t _st) {
	return (_st < 0 || (_st > 18 && _st < 21) || _st>31);
}
//是否为有效可控制的
_VG_CONSTEXPR17 bool _is_invaild(const __pin_struct& _st) {
	return _is_invaild_board_port(::std::get<0>(_st)) ||
		_is_invaild_bcm_port(::std::get<1>(_st)) ||
		_is_invaild_wpi_port(::std::get<2>(_st));
}
inline bool _is_gpio_port(pos_t _st) {

	auto pin_data = [_st]() _VG_CONSTEXPR17{
		int pos = 0;
		for (pos_t i = 0; i != 40; ++i) {
			if (::std::get<0>(__pin_data[i]) == _st) {
				return __pin_data[i];
			}
		}
		return __pin_data[pos];
	}();
	if (!_is_invaild(pin_data)) {
		if (::std::get<2>(pin_data) != -1) {
			return !vstring::traits_type::compare(::std::get<3>(pin_data), "gpio", 4);
		}
		return false;
	}
	return (!_is_invaild(pin_data)) && (vstring::traits_type::compare(::std::get<3>(pin_data), "gpio", 4));
}
void __vg_exception(const char* const _msg);
}
class exception {
public:
	_VG_CONSTEXPR17 exception(const char* const _const_des) :
		const_description(_const_des) {
	}
	virtual const char*
		what() const {
		return const_description == nullptr ? " " : const_description;
	}

private:
	const char* const_description;
};
//基本pin
class basic_pin {
	using __pin_struct = detail::__pin_struct;
public:
	_VG_CONSTEXPR17 basic_pin() :pin_(_VGPIO_INVAILD_PIN), mark_(INPUT) {}
	_VG_CONSTEXPR17 basic_pin(pos_t _pin)
		: pin_(_pin), mark_(INPUT) {
	}
	_VG_CONSTEXPR17 basic_pin(basic_pin&& _pin) : pin_(_pin.pin_), mark_(INPUT) {
		_pin.pin_ = 0;
	}
	_VG_CONSTEXPR17 basic_pin(const basic_pin& _pin) : pin_(_pin.pin_), mark_(INPUT) {}
	_VG_CONSTEXPR17 basic_pin& operator=(const basic_pin& _pin) {
		this->pin_ = _pin.pin_;
		this->mark_ = _pin.mark_;
		return *this;
	}
	_VG_CONSTEXPR17 basic_pin& operator=(basic_pin&& _pin) {
		this->pin_ = _pin.pin_;
		this->mark_ = _pin.mark_;
		_pin.pin_ = 0;
		_pin.mark_ = 0;
		return *this;
	}
	~basic_pin();
	//返回bcm编码，不存在就返回-1
	_VG_CONSTEXPR17 int get_bcm()const{
		const auto& it = detail::_iter_pin_data([this](const __pin_struct& _pin) /*_VG_CONSTEXPR17*/{
		return (::std::get<0>(_pin) == this->pin_);
			});
		if (detail::_is_invaild(it)) {
			return -1;
		}
		return ::std::get<1>(it);
	}
	//返回wiringPi编码，不存在返回-1
	_VG_CONSTEXPR17 int get_wiringPi()const {
		const auto& it = detail::_iter_pin_data([this](const __pin_struct& _pin) /*_VG_CONSTEXPR17*/{
		return (::std::get<0>(_pin) == this->pin_);
			});
		if (detail::_is_invaild(it)) {
			return -1;
		}
		return ::std::get<2>(it);
	}
	//返回物理编码
	_VG_CONSTEXPR17 int get_board()const {
		return this->pin_;
	}
	//返回对应pin的名称
	_VG_CONSTEXPR17 const char* get_name()const {
		const auto& it = detail::_iter_pin_data([this](const __pin_struct& _pin) /*_VG_CONSTEXPR17*/{
		return (::std::get<0>(_pin) == this->pin_);
			});
		if (detail::_is_invaild(it)) {
			return ::std::get<3>(detail::__pin_data[_VGPIO_INVAILD_PIN - 1]);
		}
		return ::std::get<3>(it);
	}
	_VG_CONSTEXPR17 bool invaild()const {
		return detail::_is_invaild_board_port(pin_);
	}
	_VG_CONSTEXPR17 unsigned char statue() const {
		return mark_ & 0x7;
	}
	bool is_gpio_port()const {
		return detail::_is_gpio_port(pin_);
	}
	//重新恢复原始状态(没有PWM,输出)
	void reset();
private:
	pos_t pin_;
	unsigned int mark_;//标志位
	friend int setup();
	friend class pin;
};
//软件pin
class pwm_controllor:public basic_pin{
public:
	/*
	* start和begin的区别是 ：start是一开始启动使用的值，而
	* begin是set_pwm的偏移量 begin+input_value
	*/
	_VG_CONSTEXPR17 pwm_controllor(const basic_pin& _pin) : basic_pin(_pin) {}
	pwm_controllor(const basic_pin& _pin, int _start, int _begin, int _range);
	_VG_CONSTEXPR17 pwm_controllor(pwm_controllor&& _left) :basic_pin(std::move(_left)),
		begin_(_left.begin_),range_(_left.range_),start_(_left.start_) {
		_left.begin_ = 0;
		_left.range_ = 0;
		_left.start_ = 0;
	}
	~pwm_controllor(){
		stop();
	}
	_VG_CONSTEXPR17 int get_range() { return range_; }
	_VG_CONSTEXPR17 int get_begin() { return begin_; }
	void set_pwm(int _value);
	void set_begin(int _value);
	void set_range(int _value);
	void stop();//停止
private:
	mutable int begin_ = 0;
	mutable int range_ = 0;
	mutable int start_ = 0;
};
//获得唯一的pin
class pin {
public:
	pin() :data_(nullptr) {}
	pin(pos_t _pin){
		*this = get(_pin);
	}
	pin& operator=(pin&& _pin) noexcept {
		data_.swap(_pin.data_);
		return *this;
	}
	pin& operator=(const pin& _pin)noexcept {
		data_ = _pin.data_;
		return *this;
	}
	pin(pin&& _pin)noexcept {
		*this = _pin;
	}
	pin(const pin& _pin) noexcept {
		*this = _pin;
	}
	//设置工作状态
	void statue(pin_mode _t);
	pin_mode statue() const {
		return (pin_mode)data_->statue();
	}
	//写入
	void write(pin_statue _t);
	void reset() {
		data_->reset();
	}


	int bcm() const{
		return data_->get_bcm();
	}
	int wiringPi() const {
		return data_->get_wiringPi();
	}
	int borad() const {
		return data_->get_board();
	}
	const char* const name()const {
		return data_->get_name();
	}
	bool invaild()const {
		return data_->invaild();
	}
	bool gpio()const {
		return data_->is_gpio_port();
	}
	//创建pwm控制器(全局析构函数中会自动停止)
	bool pwm_create(int _value, int _range)const;
	void pwm_stop()const;
	void pwm_set(unsigned int _value)const;
private:
	//首次初始化使用
	void __init_data(pos_t _pin) {
		if (basic_pin(_pin).invaild()) {
			detail::__vg_exception("Pin is invaild");
		}
		data_.reset(new basic_pin(_pin));
	}
	std::shared_ptr<basic_pin> data_;

	friend int setup();
};
//static pin __spin[40];

//RPI zero w [1,40]
//都是通过物理位置获取pin
//获取pin实例
inline static basic_pin get_pin(pos_t _pos) {
	if (detail::_is_invaild_board_port(_pos)) {
		return basic_pin(_VGPIO_INVAILD_PIN);
	}
	return basic_pin(_pos);
}

//获取pin的名称,不存在的pin会返回空
_VG_CONSTEXPR17 const char* get_pin_name(pos_t _gpio_pos) {
	for (int i = 0; i < 40; ++i) {
		if (::std::get<0>(detail::__pin_data[i]) == _gpio_pos) {
			return ::std::get<3>(detail::__pin_data[i]);
		}
	}
	return nullptr;
}
//根据名称获取pin
inline static basic_pin get_pin(const char* const _gpio_name) {
	for (int i = 0; i < 40; ++i) {
		if (!detail::strcmpare(::std::get<3>(detail::__pin_data[i]), _gpio_name)) {
			return basic_pin(::std::get<0>(detail::__pin_data[i]));
		}
	}
	return basic_pin(41);
}
extern int setup();
extern void delay(unsigned int ms);
extern void delayMicroseconds(unsigned int mms);
}
#endif