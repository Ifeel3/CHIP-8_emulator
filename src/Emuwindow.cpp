#include "Emuwindow.hpp"

Emuwindow::Emuwindow(unsigned char scale) : m_scale(scale) {
	if (m_scale < 1) m_scale = 1;
	m_display = XOpenDisplay(0);
	if (m_display) m_window = XCreateWindow(m_display, DefaultRootWindow(m_display), 0, 0, 64 * scale, 32 * scale, 0, CopyFromParent, CopyFromParent, CopyFromParent, 0, 0);
}

Emuwindow::~Emuwindow() {
	XDestroyWindow(m_display, m_window);
	XCloseDisplay(m_display);
}

void Emuwindow::draw(const uint32_t *image) {
	GC gc = XCreateGC(m_display, m_window, 0,0);
	XSetForeground(m_display, gc, 0x00000000);
	XFillRectangle(m_display, m_window, gc, 0, 0, 64 * m_scale, 32 * m_scale);
	XSetForeground(m_display, gc, 0xFFFFFFFF);
	for (int row = 0; row < 32; ++row) {
		for (int col = 0; col < 64; ++col) {
			if (image[row * 64 + col] > 0) XFillRectangle(m_display, m_window, gc, col * m_scale, row * m_scale, m_scale, m_scale);
		}
	}
	XMapWindow(m_display, m_window);
	XFlush(m_display);
}

void Emuwindow::keyHandle(uint8_t **keypad) {
	XSelectInput(m_display, m_window, KeyPressMask|KeyReleaseMask);
	while (XPending(m_display)) {
		XEvent KeyEvent;
		XNextEvent(m_display, &KeyEvent);
		uint8_t pressed = (KeyEvent.type == KeyPress) ? 1 : 0;
		switch (KeyEvent.xkey.keycode) {
			case 10: (*keypad)[0x1] = pressed; break;
			case 11: (*keypad)[0x2] = pressed; break;
			case 12: (*keypad)[0x3] = pressed; break;
			case 13: (*keypad)[0xC] = pressed; break;
			case 24: (*keypad)[0x4] = pressed; break;
			case 25: (*keypad)[0x5] = pressed; break;
			case 26: (*keypad)[0x6] = pressed; break;
			case 27: (*keypad)[0xD] = pressed; break;
			case 38: (*keypad)[0x7] = pressed; break;
			case 39: (*keypad)[0x8] = pressed; break;
			case 40: (*keypad)[0x9] = pressed; break;
			case 41: (*keypad)[0xE] = pressed; break;
			case 52: (*keypad)[0xA] = pressed; break;
			case 53: (*keypad)[0x0] = pressed; break;
			case 54: (*keypad)[0xB] = pressed; break;
			case 55: (*keypad)[0xF] = pressed; break;
		}
	}
}

//10-13
//24-27
//38-41
//52-55