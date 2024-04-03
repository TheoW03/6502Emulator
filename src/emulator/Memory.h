#include <iostream>
#include <vector>
using namespace std;

// #ifndef BUS_H
// #define BUS_H
// struct Bus
// {
//     virtual void helloWorld();
// };
// #endif
static int clock_cycles = 0;

uint8_t read_8bit(uint16_t address);

void write_16bit(uint16_t address, uint16_t value);

void write_8bit(uint16_t address, uint8_t value);

uint16_t read_16bit(uint16_t address);

void load_instructions(vector<uint8_t> instructions);
