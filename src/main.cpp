#include <iostream>
#include "../src/emulator/BitOperations.h"
#include "../src/emulator/Memory.h"
#include "../src/emulator/Computer.h"
#include "../src/emulator/LoadRom.h"
#include <vector>
#include <bitset>
using namespace std;
int main(int argc, char *argv[])
{
	vector<uint8_t> instructions = load_rom("games/test4.bin"); // random bin file
	// vector<uint8_t> instructions = { 0xA9, 1, 0xE9, 2};
	load_instructions(instructions);
	run();
}
// bool checkCarryFlag(uint8_t result)
// {
// 	// Check carry from bit 7 (overflow) or underflow from bit 0
// 	return (result & 0x80) || (result == 0);
// }

// int main()
// {
// 	// Example: 255 + 1 (overflow)
// 	uint8_t num1 = 255;
// 	uint8_t num2 = 2;
// 	uint8_t result = num1 + num2;

// 	// Check carry flag
// 	bool carryFlag = checkCarryFlag(result);

// 	if (carryFlag)
// 	{
// 		std::cout << "Carry flag set (overflow or underflow)!" << std::endl;
// 		std::cout << "Result: " << std::bitset<8>(result) << std::endl;
// 	}
// 	else
// 	{
// 		std::cout << "Result: " << std::bitset<8>(result) << std::endl;
// 	}

// 	return 0;
// }