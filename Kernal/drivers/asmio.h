#ifndef __H__ASMIO__H__
#define __H__ASMIO__H__
inline void hang(){
	hang:;
	asm volatile ("cli;"
		      "hlt;"
		       : : );
	goto hang;
}
inline uint8_t inportb(uint16_t port){
	uint8_t ret;
	asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
	return ret;
}
inline void outportb(uint16_t port, uint8_t value){
	asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}
inline uint16_t LowMemCNT(){
    uint16_t ret = 0x0;
    asm volatile (".start:;"
                  "XOR %%AX, %%AX;"
                  "INT 0x12;"
                  "JC failed;"
                  "TEST %%AX, %%AX;"
                  "JZ failed;"
                  "MOV %%AX, %0"
                  :"=g" (ret): );
    return ret;
    failed:
        hang();
        return 0x0;
}
inline uint32_t HighMemCNT(){

}

#endif
