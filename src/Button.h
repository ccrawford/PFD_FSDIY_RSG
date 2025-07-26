/*
	Button - a small library for Arduino to handle button debouncing
	
	MIT licensed.
*/

#ifndef Button_h
#define Button_h
#include "Arduino.h"

class Button
{
	public:
    Button() {};
		Button(const String btn_name, uint8_t pin, uint16_t debounce_ms = 100, bool repeat=false);
    	void initialize(const String btn_name, uint8_t pin, uint16_t debounce_ms, bool repeat=false);
		void begin();
		char read();
		bool toggled();
		bool pressed();
		bool held();
		bool released();
		bool has_changed();
		void update();
    String get_name();
		
		const static bool PRESSED = LOW;
		const static bool RELEASED = HIGH;
	
	private:
		String   _name = "undefined_name";
		uint8_t  _pin = 0;
		uint16_t _delay = 10;
		bool     _state = false;
		uint32_t _ignore_until = 10;
		bool     _has_changed = false;
		bool 	_repeat =false;
		uint16_t _pressedMs = 0;
};

#endif
