#include "drivers/0xb8000.h"
#include "drivers/PS2.h"
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
    VGA_TERMINAL_INIT();
    if(VGA_TERMINAL_BASE_IO == 0x3D4){
	(*writestring)("BIOS base IO is 0x3D4");
    }
    outportb(PS2_CONTROLLER_STATE, PS2_DISABLE_FIRST_PORT);  //disable ports for now to prevent writing to the input buffer when we don't want it
    outportb(PS2_CONTROLLER_STATE, PS2_DISABLE_SECOND_PORT);
    inportb(PS2_CONTROLLER_DATA); //flush the input buffer by reading it
    writestring = &VGA_TERMINAL_WRITESTRING;
    (*writestring)("kernel has booted without error\n");
    (*writestring)("beginning debugging process\n");
    PS2_CONTROLLER_INIT();
    while(true){

    }
    return 0;
}
