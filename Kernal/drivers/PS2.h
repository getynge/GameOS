#define PS2_READ_FROM_BYTE_0 0x20
#define PS2_WRITE_TO_BYTE_0 0x60
#define TEST_PS2_CONTROLLER 0xAA
#define PS2_CONTROLLER_TEST_PASSED 0x55
#include "asmio.h"
#define PS2_SET_SCAN_CODE_SET 0xF0
#define PS2_CONTROLLER_TEST_FAILED 0xFC
#define PS2_ENABLE_FIRST_PORT 0xAE
#define PS2_ENABLE_SECOND_PORT 0xA8
#define PS2_DISABLE_FIRST_PORT 0xAD
#define PS2_DISABLE_SECOND_PORT 0xA7
#define PS2_IDENTIFY_DEVICE 0xF2
#define PS2_TEST_FIRST_PORT 0xAB
#define PS2_TEST_SECOND_PORT 0xA9
#define PS2_PORT_TEST_PASSED 0x00
#define PS2_PORT_CLOCK_LINE_STUCK_LOW 0x01
#define PS2_PORT_CLOCK_LINE_STUCK_HIGH 0x02
#define PS2_PORT_DATA_LINE_STUCK_LOW 0x03
#define PS2_PORT_DATA_LINE_STUCK_HIGH 0x04
const uint16_t PS2_CONTROLLER_STATE = 0x64;
const uint16_t PS2_CONTROLLER_DATA = 0x60;
uint8_t PS2_PORT_COUNT = 0;
void (*writestring)(const char * data);
inline void PS2_CONTROLLER_BUSY(){
	while(!(inportb(PS2_CONTROLLER_STATE) & 0b00000001)){
		(*writestring)(".");
	}
	return;
}
inline uint8_t PS2_READ_FROM_OFFSET(uint8_t offset){
	uint8_t result = offset + 0x21;
	if(result > 0x3f){
		result = 0x3f;
	}
	return result;
}
inline uint8_t PS2_WRITE_FROM_OFFSET(uint8_t offset){
	uint8_t result = offset + 0x61;
	if(result > 0x7f){
		result = 0x7f;
	}
}
void PS2_CONTROLLER_SELF_TEST(){
	//clear the buffer to prevent incorrect test data
	inportb(PS2_CONTROLLER_DATA);
	(*writestring)("Performing PS/2 controller self test");
	outportb(PS2_CONTROLLER_STATE, TEST_PS2_CONTROLLER);
	uint8_t result = 0;
	PS2_CONTROLLER_BUSY();
	(*writestring)("\n");
	result = inportb(PS2_CONTROLLER_DATA);
	if(result == PS2_CONTROLLER_TEST_PASSED){
		(*writestring)("The PS/2 controller appears to be working");
	}else if(result == PS2_CONTROLLER_TEST_PASSED){
		(*writestring)("The PS/2 controller does not appear to be working\n");
		(*writestring)("Will now halt...");
		hang();
	}else{
		(*writestring)("The PS/2 controller did not return a valid code\n");
		(*writestring)("Will now halt...");
		hang();
	}
	(*writestring)("\n");
}
void PS2_PORT_TEST(){
	inportb(PS2_CONTROLLER_DATA);
	outportb(PS2_CONTROLLER_STATE, PS2_TEST_FIRST_PORT);
	PS2_CONTROLLER_BUSY();
	uint8_t result = inportb(PS2_CONTROLLER_DATA);
	if(result == PS2_PORT_TEST_PASSED){
		(*writestring)("The first PS/2 port appears to be working\n");
	}else{
		(*writestring)("ERROR: PS/2 port test failed, will halt");
		hang();
	}
	result = 0x04;
	if(PS2_PORT_COUNT == 2){
		inportb(PS2_CONTROLLER_DATA);
		outportb(PS2_CONTROLLER_STATE, PS2_TEST_SECOND_PORT);
		PS2_CONTROLLER_BUSY();
		result = inportb(PS2_CONTROLLER_DATA);
		if(result == PS2_PORT_TEST_PASSED){
			(*writestring)("The second PS/2 port appears to be working\n");
		}else{
			(*writestring)("ERROR: PS/2 port test failed, will halt");
			hang();
		}
	}
}
void PS2_CONTROLLER_INIT(){
	PS2_CONTROLLER_SELF_TEST();
	outportb(PS2_CONTROLLER_STATE, PS2_READ_FROM_BYTE_0);
	PS2_CONTROLLER_BUSY();
	uint8_t configuration_byte = inportb(PS2_CONTROLLER_DATA);
	if(configuration_byte & 0b00100000){
		PS2_PORT_COUNT = 2;
		(*writestring)("There are two PS/2 ports on this machine\n");
	}else{
		(*writestring)("There is one PS/2 port on this machine\n");
		PS2_PORT_COUNT = 1;
	}
	configuration_byte = configuration_byte & 0b10111100;
	outportb(PS2_CONTROLLER_STATE, PS2_WRITE_TO_BYTE_0);
	outportb(PS2_CONTROLLER_DATA, configuration_byte);
	inportb(PS2_CONTROLLER_DATA);
	PS2_PORT_TEST();
	outportb(PS2_CONTROLLER_STATE, PS2_ENABLE_FIRST_PORT);
	(*writestring)("PS/2 port one (assumed keyboard) has been enabled\n");
	if(PS2_PORT_COUNT == 2){
		outportb(PS2_CONTROLLER_STATE, PS2_ENABLE_SECOND_PORT);
		(*writestring)("PS/2 port two (assumed mouse) has been enabled\n");
	}
	outportb(PS2_CONTROLLER_DATA, 0x02);
	outportb(PS2_CONTROLLER_DATA, PS2_SET_SCAN_CODE_SET); 
}
char PS2_KEYBOARD_CODE_HANDLER(uint8_t mode, uint8_t code){
	if(mode == 0x02){
		switch(code){
		case 0x5A:
			return '\n';
		case 0x2C:
			return 't';
		case 0x32:
			return 'b';
		case 0x35:
			return 'y';
		case 0x3C:
			return 'u';
		case 0x44:
			return 'o';
		case 0x4D:
			return 'p';
		case 0x42:
			return 'k';
		case 0x34:
			return 'g';
		case 0x1C:
			return 'a';
		case 0x24:
			return 'e';
		case 0x29:
			return ' ';
		default:
			return 0x0;
		}
	}else{
		return 0x0;
	}
}
