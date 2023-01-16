#include "Chip8.hpp"
#include "Emuwindow.hpp"
#include <iostream>

int main(int argc, char **argv) {
    Chip8 chip8;
	Emuwindow *window;
	if (argc == 3) window = new Emuwindow(atoi(argv[1]));
	else window = new Emuwindow(10);

	if (argc >= 2 && argc <= 3) {
		if (argc == 2) chip8.LoadRom(argv[1]);
		else chip8.LoadRom(argv[2]);
		while (true) {
			window->keyHandle(&chip8.keypad);
			chip8.Cycle();
			window->draw(chip8.display);
			usleep((1000000 / 60) / 2);
		}
	}
    return 0;
}