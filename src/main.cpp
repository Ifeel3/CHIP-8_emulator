#include <iostream>
#include <string>
#include "Chip8.hpp"

void printImage(const uint32_t *image) {
	std::string tmp("##################################################################\n");
	for (int row = 0; row < 32; ++row) {
		for (int col = 0; col < 66; ++col) {
			if (col == 0 || col == 65) tmp = tmp.append(1, '#');
			else {
				if (image[64 * row + (col - 1)] > 0) tmp = tmp.append(1, '#');
				else tmp = tmp.append(1, ' ');
			}
		}
		tmp.append(1, '\n');
	}
	tmp = tmp.append("##################################################################\n");
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