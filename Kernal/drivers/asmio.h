#ifndef __H__ASMIO__H__
#define __H__ASMIO__H__
inline uint8_t inportb(uint16_t port){
	uint8_t ret;
	asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
	return ret;
}
inline void outportb(uint16_t port, uint8_t value){
	asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}

inline void hang(){
	hang:;
	asm volatile ("cli"
		      "hlt"
		       : : );
	goto hang;
}
#endif
