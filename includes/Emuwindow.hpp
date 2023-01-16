#ifndef EMUWINDOW_HPP
# define EMUWINDOW_HPP

#include <X11/Xlib.h>
#include "datatypes.hpp"

class Emuwindow {
private:
	unsigned char m_scale;
	Display *m_display;
	Window m_window;

public:
	Emuwindow(unsigned char scale = 10);
	~Emuwindow();

	void draw(const uint32_t *image);
	void keyHandle(uint8_t **keypad);
};

#endif //EMUWINDOW_HPP