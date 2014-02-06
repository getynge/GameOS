#include "drivers/0xb8000.h"
#include "drivers/PS2.h"
uint16_t LMem;
uint32_t HMem;
size_t strlen(const char * str){
    size_t ret = 0;
    if(str != 0x0){
        while(str[ret] != 0x0){
            ret += 1;
        }
    }
    return ret;
}
void (*writestring)(const char * data);
#if defined(__cplusplus)
extern "C"
#endif // defined
int _kmain(){
    (*writestring)("Polling for low memory...");
    LMem = LowMemCNT();
    if(LMem <= 0x0){
        (*writestring)("Polling failed, will halt");
        hang();
    }else{
        (*writestring)("Low memory existent allocated\n");
    }
    VGA_TERMINAL_INIT();
    outportb(PS2_CONTROLLER_STATE, PS2_DISABLE_FIRST_PORT);  //disable ports for now to prevent writing to the input buffer when we don't want it
    outportb(PS2_CONTROLLER_STATE, PS2_DISABLE_SECOND_PORT);
    inportb(PS2_CONTROLLER_DATA); //flush the input buffer by reading it
    writestring = &VGA_TERMINAL_WRITESTRING;
    (*writestring)("kernel has booted without error\n");
    (*writestring)("beginning debugging process\n");
    PS2_CONTROLLER_INIT();
    if(VGA_TERMINAL_BASE_IO == 0x3D4){
        (*writestring)("BIOS base IO is 0x3D4\n");
    }else{
        (*writestring)("BIOS base port is not standard\n");
        (*writestring)("Will use non-standard base port call\n");
    }
    (*writestring)("debugging completed, no fatal errors detected\n");
    (*writestring)("press ENTER to finish boot\n");
    char * cmd; //bytes 0x00100001 through 0x00100201 are reserved for command line arguments

    cmd = (char*) 0x00100001;
    uint16_t curpos = 0;
    while(true){
	uint8_t state = inportb(PS2_CONTROLLER_STATE);
	if(state & 0b00000001)
	{
		uint8_t key = inportb(PS2_CONTROLLER_DATA);
		inportb(PS2_CONTROLLER_DATA);
		char res = PS2_KEYBOARD_CODE_HANDLER(0x02, key);
		if(res == 0x0){
			continue;
		}
		if(res == '\n' && debugflag == 0x1){
			VGA_TERMINAL_CLEARBUFFER();
			debugflag = 0;
			(*writestring)("sysrqterm>");
			continue;
		}
		if(res == '\n'){
			(*writestring)("\nsysrqterm>");
			continue;
		}
		if(res != 0){
			if(curpos < 512){
				cmd[curpos] = res;
				curpos += 1;
				VGA_TERMINAL_PUTCHAR(res);
			}
		}
	}
    }
    return 0;
}
