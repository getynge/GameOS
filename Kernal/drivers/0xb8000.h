#ifndef __H__0XB8000__H__
#define __H__0XB8000__H__
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "asmio.h"
size_t strlen(const char * str);
/*
Hardware text mode colors
*/
typedef enum vga_color
{
    COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
}vga_color;
uint8_t debugflag = 0x1;
//volatile uint16_t VGA_TERMINAL_BASE_IO;
uint8_t VGA_MAKE_COLOR(vga_color fg, vga_color bg){
    return fg | bg << 4;
}
uint16_t VGA_MAKE_ENTRY(char c, uint8_t color){
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | color16 << 8;
}
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 24;
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
uint16_t* terminal_invisable_buffer;
void VGA_TERMINAL_SETPOS(uint8_t x, uint8_t y){
	terminal_column = x;
	terminal_row = y;
}
void VGA_TERMINAL_NEWLINE(){
	VGA_TERMINAL_SETPOS(0, terminal_row+1);
}
void VGA_TERMINAL_SCROLL(int8_t direction_and_distance){
	if(direction_and_distance & (uint8_t) 0b10000000){

	}else{

	}
}
void VGA_TERMINAL_INIT()
{
	//uint16_t * BIO = (uint16_t*)0x0463;
	//uint16_t * BIO2 = (uint16_t*)0x0464;
	//VGA_TERMINAL_BASE_IO = (*BIO) | (*BIO2)<<8;
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = VGA_MAKE_COLOR(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = VGA_MAKE_ENTRY(' ', terminal_color);
		}
	}
}

void VGA_TERMINAL_SETCOLOR(uint8_t color)
{
	terminal_color = color;
}

void VGA_TERMINAL_PUT_ENTRY_AT(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = VGA_MAKE_ENTRY(c, color);
}

void VGA_TERMINAL_PUTCHAR(char c)
{
	VGA_TERMINAL_PUT_ENTRY_AT(c, terminal_color, terminal_column, terminal_row);
	if ( ++terminal_column == VGA_WIDTH )
	{
		terminal_column = 0;
		if ( ++terminal_row == VGA_HEIGHT )
		{
			terminal_row = VGA_HEIGHT-1;
			
		}
	}
}
void VGA_TERMINAL_CLEARBUFFER(){
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = 0x0;
		}
	}
	VGA_TERMINAL_SETPOS(0, 0);
}
void VGA_TERMINAL_WRITESTRING(const char* data)
{
	size_t datalen = strlen(data);
	for ( size_t i = 0; i < datalen; i++ )
		if(data[i] == '\n'){
			VGA_TERMINAL_NEWLINE();
		}else{
			VGA_TERMINAL_PUTCHAR(data[i]);
		}
}
#endif
