#include <iostream>
#include <string>
#include "Chip8.hpp"

void printImage(const uint32_t *image) {
	std::string tmp("");
	for (int i = 0; i < 32*64; i++) {
		if (image[i] > 0) tmp = tmp.append(1, '#');
		else tmp = tmp.append(1, ' ');
		if (i % 64 == 0) tmp = tmp.append(1, '\n');
	}
	std::cout << tmp << std::endl;
}

int main(int argc, char **argv) {
    Chip8 chip8;
    if (argc == 2) chip8.LoadRom(argv[1]);
    while (true) {
		chip8.Cycle();
		printImage(chip8.display);
		usleep(1000000/60);
	}
    return 0;
}