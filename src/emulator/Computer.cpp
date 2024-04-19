#include <bitset>
#include <cstdio>
#include <iostream>
#include <map>
#include <thread>
// #include <pthread.h>
// #include <unistd.h>

#include "BitOperations.h"

#include "AddressMode.h"
#include "Bus.h"
#include "Instructions.h"
#include "LoadRom.h"
#include "Memory.h"
#include "StatusRegister.h"

using namespace std;

// typedef void (*instructionPointer)(uint8_t, CPU &);
// typedef void (*instructionPointer)(AddressMode, CPU &);
using instructionPointer = void (*)(AddressMode, CPU &);

uint8_t current_instruction = 0;
uint8_t param = 0;

struct Instruction
{
	instructionPointer i;
	AddressMode addressmode;
};
std::map<uint8_t, Instruction> instructionMap;

void run(CPU cpu);
void initializeInstructionMap();

/**
 * Initializes the CPU object and memories.
 */
void init(string file_name)
{
	initializeInstructionMap();
	vector<uint8_t> test = {0xa9, 0xa, 0xa0, 0xa, 0xa2, 0xa, 0x00};
	Rom rom;
	rom.PRG = test;
	rom.CHR = test;
	rom.mapper = 0;
	rom.mirror = MirrorType::FOUR_SCREEN;
	Bus bus(rom);
	// vector<uint8_t> v = file_tobyte_vector(file_name);
	// Bus bus(load_rom(v));
	// Bus bus(rom);
	bus.write_16bit(0xFFFC, 0x8000);
	CPU cpu;
	cpu.PC = bus.read_16bit(0xFFFC);
	bus.fill(cpu.PC);
	cpu.A_Reg = 0;
	cpu.status = 0;
	cpu.X_Reg = 0;
	cpu.Y_Reg = 0;
	cpu.stack_pointer = 0xfd;
	cpu.bus = bus;
	cpu.bus.clock_cycles = 0;
	run(cpu);
}

/**
 * Loads instructions and their associated function in the emulator into a map.
 * May be read for documenting which codes are associated with each instruction.
 */
void initializeInstructionMap()
{
#pragma region LDA
	instructionMap.insert(make_pair(0xA9, Instruction{(instructionPointer)LDA, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0xA5, Instruction{(instructionPointer)LDA, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0xB5, Instruction{(instructionPointer)LDA, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0xAD, Instruction{(instructionPointer)LDA, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0xBD, Instruction{(instructionPointer)LDA, AddressMode::ABSOLUTE_X}));
	instructionMap.insert(make_pair(0xB9, Instruction{(instructionPointer)LDA, AddressMode::ABSOLUTE_Y}));
	instructionMap.insert(make_pair(0xA1, Instruction{(instructionPointer)LDA, AddressMode::INDIRECT_X}));
	instructionMap.insert(make_pair(0xB1, Instruction{(instructionPointer)LDA, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region LDX

	instructionMap.insert(make_pair(0xA2, Instruction{(instructionPointer)LDX, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0xA6, Instruction{(instructionPointer)LDX, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0xB6, Instruction{(instructionPointer)LDX, AddressMode::ZERO_PAGE_Y}));
	instructionMap.insert(make_pair(0xAE, Instruction{(instructionPointer)LDX, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0xBE, Instruction{(instructionPointer)LDX, AddressMode::ABSOLUTE_Y}));

#pragma endregion

#pragma region LDY

	instructionMap.insert(make_pair(0xA0, Instruction{(instructionPointer)LDY, AddressMode::IMMEDIATE}));
	instructionMap.insert(make_pair(0xA4, Instruction{(instructionPointer)LDY, AddressMode::ZERO_PAGE}));
	instructionMap.insert(make_pair(0xB4, Instruction{(instructionPointer)LDY, AddressMode::ZERO_PAGE_X}));
	instructionMap.insert(make_pair(0xAC, Instruction{(instructionPointer)LDY, AddressMode::ABSOLUTE}));
	instructionMap.insert(make_pair(0xBC, Instruction{(instructionPointer)LDY, AddressMode::ABSOLUTE_X}));

#pragma endregion
	
	
	// instructionMap.insert(make_pair(0xA9, (instructionPointer)LDA));
	// instructionMap.insert(make_pair(0xA5, (instructionPointer)LDA));
	// instructionMap.insert(make_pair(0xB5, (instructionPointer)LDA));
	// instructionMap.insert(make_pair(0xAD, (instructionPointer)LDA));
	// instructionMap.insert(make_pair(0xBD, (instructionPointer)LDA));
	// instructionMap.insert(make_pair(0xB9, (instructionPointer)LDA));
	// instructionMap.insert(make_pair(0xA1, (instructionPointer)LDA));
	// instructionMap.insert(make_pair(0xB1, (instructionPointer)LDA));

	// instructionMap.insert(make_pair(0xA2, (instructionPointer)LDX));
	// instructionMap.insert(make_pair(0xA6, (instructionPointer)LDX));
	// instructionMap.insert(make_pair(0xB6, (instructionPointer)LDX));
	// instructionMap.insert(make_pair(0xAE, (instructionPointer)LDX));
	// instructionMap.insert(make_pair(0xBE, (instructionPointer)LDX));

	// instructionMap.insert(make_pair(0xA0, (instructionPointer)LDY));
	// instructionMap.insert(make_pair(0xA4, (instructionPointer)LDY));
	// instructionMap.insert(make_pair(0xB4, (instructionPointer)LDY));
	// instructionMap.insert(make_pair(0xAC, (instructionPointer)LDY));
	// instructionMap.insert(make_pair(0xBC, (instructionPointer)LDY));

	// instructionMap.insert(make_pair(0x69, (instructionPointer)ADC));
	// instructionMap.insert(make_pair(0x65, (instructionPointer)ADC));
	// instructionMap.insert(make_pair(0x75, (instructionPointer)ADC));
	// instructionMap.insert(make_pair(0x6D, (instructionPointer)ADC));
	// instructionMap.insert(make_pair(0x7D, (instructionPointer)ADC));
	// instructionMap.insert(make_pair(0x79, (instructionPointer)ADC));
	// instructionMap.insert(make_pair(0x71, (instructionPointer)ADC));
	// instructionMap.insert(make_pair(0x61, (instructionPointer)ADC));

	// instructionMap.insert(make_pair(0xE9, (instructionPointer)SBC));
	// instructionMap.insert(make_pair(0xE5, (instructionPointer)SBC));
	// instructionMap.insert(make_pair(0xF5, (instructionPointer)SBC));
	// instructionMap.insert(make_pair(0xED, (instructionPointer)SBC));
	// instructionMap.insert(make_pair(0xFD, (instructionPointer)SBC));
	// instructionMap.insert(make_pair(0xF9, (instructionPointer)SBC));
	// instructionMap.insert(make_pair(0xF1, (instructionPointer)SBC));
	// instructionMap.insert(make_pair(0xE1, (instructionPointer)SBC));

	// instructionMap.insert(make_pair(0x29, (instructionPointer)AND));
	// instructionMap.insert(make_pair(0x25, (instructionPointer)AND));
	// instructionMap.insert(make_pair(0x35, (instructionPointer)AND));
	// instructionMap.insert(make_pair(0x2D, (instructionPointer)AND));
	// instructionMap.insert(make_pair(0x3D, (instructionPointer)AND));
	// instructionMap.insert(make_pair(0x39, (instructionPointer)AND));
	// instructionMap.insert(make_pair(0x21, (instructionPointer)AND));
	// instructionMap.insert(make_pair(0x31, (instructionPointer)AND));

	// instructionMap.insert(make_pair(0x09, (instructionPointer)ORA));
	// instructionMap.insert(make_pair(0x05, (instructionPointer)ORA));
	// instructionMap.insert(make_pair(0x15, (instructionPointer)ORA));
	// instructionMap.insert(make_pair(0x0D, (instructionPointer)ORA));
	// instructionMap.insert(make_pair(0x1D, (instructionPointer)ORA));
	// instructionMap.insert(make_pair(0x19, (instructionPointer)ORA));
	// instructionMap.insert(make_pair(0x01, (instructionPointer)ORA));
	// instructionMap.insert(make_pair(0x11, (instructionPointer)ORA));

	// instructionMap.insert(make_pair(0x2A, (instructionPointer)ROL));
	// instructionMap.insert(make_pair(0x26, (instructionPointer)ROL));
	// instructionMap.insert(make_pair(0x36, (instructionPointer)ROL));
	// instructionMap.insert(make_pair(0x2E, (instructionPointer)ROL));
	// instructionMap.insert(make_pair(0x3E, (instructionPointer)ROL));

	// instructionMap.insert(make_pair(0x6A, (instructionPointer)ROR));
	// instructionMap.insert(make_pair(0x66, (instructionPointer)ROR));
	// instructionMap.insert(make_pair(0x76, (instructionPointer)ROR));
	// instructionMap.insert(make_pair(0x6E, (instructionPointer)ROR));
	// instructionMap.insert(make_pair(0x7E, (instructionPointer)ROR));

	// instructionMap.insert(make_pair(0x86, (instructionPointer)STX));
	// instructionMap.insert(make_pair(0x96, (instructionPointer)STX));
	// instructionMap.insert(make_pair(0x8E, (instructionPointer)STX));

	// instructionMap.insert(make_pair(0x84, (instructionPointer)STY));
	// instructionMap.insert(make_pair(0x94, (instructionPointer)STY));
	// instructionMap.insert(make_pair(0x8C, (instructionPointer)STY));

	// instructionMap.insert(make_pair(0x85, (instructionPointer)STA));
	// instructionMap.insert(make_pair(0x95, (instructionPointer)STA));
	// instructionMap.insert(make_pair(0x8D, (instructionPointer)STA));
	// instructionMap.insert(make_pair(0x9D, (instructionPointer)STA));
	// instructionMap.insert(make_pair(0x99, (instructionPointer)STA));
	// instructionMap.insert(make_pair(0x81, (instructionPointer)STA));
	// instructionMap.insert(make_pair(0x91, (instructionPointer)STA));

	// instructionMap.insert(make_pair(0x4A, (instructionPointer)LSR));
	// instructionMap.insert(make_pair(0x46, (instructionPointer)LSR));
	// instructionMap.insert(make_pair(0x56, (instructionPointer)LSR));
	// instructionMap.insert(make_pair(0x4E, (instructionPointer)LSR));
	// instructionMap.insert(make_pair(0x5E, (instructionPointer)LSR));

	// instructionMap.insert(make_pair(0x0A, (instructionPointer)ASL));
	// instructionMap.insert(make_pair(0x06, (instructionPointer)ASL));
	// instructionMap.insert(make_pair(0x16, (instructionPointer)ASL));
	// instructionMap.insert(make_pair(0x0E, (instructionPointer)ASL));
	// instructionMap.insert(make_pair(0x1E, (instructionPointer)ASL));

	// instructionMap.insert(make_pair(0x49, (instructionPointer)EOR));
	// instructionMap.insert(make_pair(0x45, (instructionPointer)EOR));
	// instructionMap.insert(make_pair(0x55, (instructionPointer)EOR));
	// instructionMap.insert(make_pair(0x4D, (instructionPointer)EOR));
	// instructionMap.insert(make_pair(0x5D, (instructionPointer)EOR));
	// instructionMap.insert(make_pair(0x59, (instructionPointer)EOR));
	// instructionMap.insert(make_pair(0x41, (instructionPointer)EOR));
	// instructionMap.insert(make_pair(0x51, (instructionPointer)EOR));

	// instructionMap.insert(make_pair(0xE8, (instructionPointer)INX));

	// instructionMap.insert(make_pair(0xC8, (instructionPointer)INY));

	// instructionMap.insert(make_pair(0xCA, (instructionPointer)DEX));

	// instructionMap.insert(make_pair(0xC6, (instructionPointer)DEC));
	// instructionMap.insert(make_pair(0xD6, (instructionPointer)DEC));
	// instructionMap.insert(make_pair(0xCE, (instructionPointer)DEC));
	// instructionMap.insert(make_pair(0xDE, (instructionPointer)DEC));

	// instructionMap.insert(make_pair(0xE6, (instructionPointer)INC));
	// instructionMap.insert(make_pair(0xF6, (instructionPointer)INC));
	// instructionMap.insert(make_pair(0xEE, (instructionPointer)INC));
	// instructionMap.insert(make_pair(0xFE, (instructionPointer)INC));

	// instructionMap.insert(make_pair(0x88, (instructionPointer)DEY));

	// instructionMap.insert(make_pair(0x18, (instructionPointer)CLC));

	// instructionMap.insert(make_pair(0x38, (instructionPointer)SEC));

	// instructionMap.insert(make_pair(0xD8, (instructionPointer)CLD));

	// instructionMap.insert(make_pair(0xF8, (instructionPointer)SED));

	// instructionMap.insert(make_pair(0x78, (instructionPointer)SEI));

	// instructionMap.insert(make_pair(0x58, (instructionPointer)CLI));

	// instructionMap.insert(make_pair(0xB8, (instructionPointer)CLV));

	// instructionMap.insert(make_pair(0x40, (instructionPointer)RTI));

	// instructionMap.insert(make_pair(0x4C, (instructionPointer)JMP));
	// instructionMap.insert(make_pair(0x6C, (instructionPointer)JMP));

	// instructionMap.insert(make_pair(0xF0, (instructionPointer)BEQ));

	// instructionMap.insert(make_pair(0xD0, (instructionPointer)BNE));

	// instructionMap.insert(make_pair(0xC9, (instructionPointer)CMP));
	// instructionMap.insert(make_pair(0xC5, (instructionPointer)CMP));
	// instructionMap.insert(make_pair(0xD5, (instructionPointer)CMP));
	// instructionMap.insert(make_pair(0xCD, (instructionPointer)CMP));
	// instructionMap.insert(make_pair(0xDD, (instructionPointer)CMP));
	// instructionMap.insert(make_pair(0xD9, (instructionPointer)CMP));
	// instructionMap.insert(make_pair(0xC1, (instructionPointer)CMP));
	// instructionMap.insert(make_pair(0xD1, (instructionPointer)CMP));

	// instructionMap.insert(make_pair(0xC0, (instructionPointer)CPY));
	// instructionMap.insert(make_pair(0xC4, (instructionPointer)CPY));
	// instructionMap.insert(make_pair(0xCC, (instructionPointer)CPY));

	// instructionMap.insert(make_pair(0xE0, (instructionPointer)CPX));
	// instructionMap.insert(make_pair(0xE4, (instructionPointer)CPX));
	// instructionMap.insert(make_pair(0xEC, (instructionPointer)CPX));

	// instructionMap.insert(make_pair(0x24, (instructionPointer)BIT));
	// instructionMap.insert(make_pair(0x2C, (instructionPointer)BIT));

	// instructionMap.insert(make_pair(0x90, (instructionPointer)BCC));

	// instructionMap.insert(make_pair(0xB0, (instructionPointer)BCS));

	// instructionMap.insert(make_pair(0x10, (instructionPointer)BPL));

	// instructionMap.insert(make_pair(0x30, (instructionPointer)BMI));

	// instructionMap.insert(make_pair(0x50, (instructionPointer)BVC));

	// instructionMap.insert(make_pair(0x70, (instructionPointer)BVS));

	// instructionMap.insert(make_pair(0x20, (instructionPointer)JSR));

	// instructionMap.insert(make_pair(0x60, (instructionPointer)RTS));

	// instructionMap.insert(make_pair(0x28, (instructionPointer)PLP));

	// instructionMap.insert(make_pair(0x08, (instructionPointer)PHP));

	// instructionMap.insert(make_pair(0x68, (instructionPointer)PLA));

	// instructionMap.insert(make_pair(0x48, (instructionPointer)PHA));

	// instructionMap.insert(make_pair(0xAA, (instructionPointer)TAX));

	// instructionMap.insert(make_pair(0x8A, (instructionPointer)TXA));

	// instructionMap.insert(make_pair(0xBA, (instructionPointer)TXS));

	// instructionMap.insert(make_pair(0xBA, (instructionPointer)TSX));

	// instructionMap.insert(make_pair(0x98, (instructionPointer)TYA));

	// instructionMap.insert(make_pair(0xA8, (instructionPointer)TAY));
}
void printCPU_stats(CPU cpu)
{
	printf("A_Reg: %x \n", cpu.A_Reg);
	printf("X_Reg: %d \n", cpu.X_Reg);
	printf("Y_Reg: %d \n", cpu.Y_Reg);
	printf("Program Counter: 0x%X \n", cpu.bus.get_PC());
	printf("Stack Pointer: 0x%X \n", cpu.stack_pointer);
	bitset<7> status(cpu.status);
	cpu.bus.print_clock();
	cout << "cpu status register: " << status << endl;
}
/**
 * Executes instructions in a loop and handles proper/improper exits.
 */
void run(CPU cpu)
{
	while (cpu.PC < 0xFFFF)
	{
		if (check_brk(cpu) != 0)
		{
			continue;
		}
		current_instruction = cpu.bus.read_8bit(cpu.bus.get_PC());
		if (current_instruction == 0xEA)
		{
			continue;
		}
		// Equivalent to, in English, if instructionMap contains current_instruction.
		if (instructionMap.find(current_instruction) == instructionMap.end())
		{
			if (current_instruction == 0xEA)
			{
				// NOP
			}
			else if (current_instruction == 0x00)
			{

				if (check_interrupt_disabled(cpu) != 0)
				{
					continue;
				}
				set_brk(cpu, 1);
				cpu.bus.write_8bit(cpu.stack_pointer, cpu.status);
				cpu.bus.fetch_next();
				cpu.stack_pointer -= 2;
				cpu.bus.write_16bit(cpu.stack_pointer, cpu.bus.get_PC());
				printf("Halt instruction encountered.\n");
				printCPU_stats(cpu);
				cout << "program succesfully ended" << endl;
				return;
			}
			else
			{
				cout << "Unrecognized instruction encountered." << endl;
				printf("Current instruction 0x%x is unrecognized. \n", current_instruction);
				printCPU_stats(cpu);
				cout << "Program exited unsuccessfully" << endl;
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			// instructionMap.at(current_instruction)(current_instruction, cpu);
			Instruction a = instructionMap.at(current_instruction);
			a.i(a.addressmode, cpu);
		}
		cpu.bus.print_clock();
		// this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
