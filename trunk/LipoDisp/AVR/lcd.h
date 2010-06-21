#ifndef LCD_H
	#define LCD_H

//*****************************************************************************
//                            I N C L U D E
//*****************************************************************************
#define F_CPU 16000000UL  

#include <util/delay.h>
#include <stdlib.h>     
// stdlib.h-examples:
// Integer in String for 	itoa( i, s, 10 ) i=Integer;s=Zielstring
// double/float in String 	dtostrf( f, 6, 3, s ) f=float;6=Gesamtstellen;3=Nachkommastelle;s=Zielstring

//*****************************************************************************
//                            D E F I N E
//*****************************************************************************

#define LCD_DELAY				1		//Delay in ms 
#define LCD_READ_DELAY			1		//Delay in us 
#define LCD_RESET_DELAY			1		//Reset duration
									
//other definitions
#define LOWBYTE(val)	 val & 0x00FF			
#define HIGHBYTE(val)	(val & 0xFF00) >> 8		

// LCD commands
//		command		 	value	description						
#define NO_OPERATION 	0x00   	//No Operation 				
#define SW_RESET		0x01 	//Software reset 
#define RD_DISP_INF		0x04 	//Read Display Identification
#define RD_DISP_STAT	0x09 	//Read Display Status 		
#define RD_DISP_PWRM	0x0A 	//Read Display Power Mode 	
#define RD_DISP_MADCTL	0x0B 	//Read Display MADCTL 		
#define RD_DISP_PIXF	0x0C 	//Read Display Pixel Format 
#define RD_DIPS_IMGM	0x0D 	//Read Display Image Mode 		
#define RD_DISP_SIGM	0x0E 	//Read Display Signal Mode 	
#define RD_DISP_STEST	0x0F 	//Read Display Self Diag. Res	
#define DISP_SLEEP_IN	0x10 	//Sleep in 						
#define DISP_SLEEP_OUT	0x11 	//Sleep out 				
#define PART_MODE_ON	0x12 	//Partial Mode On 			
#define NORM_MODE_ON	0x13 	//Normal Display Mode On 	
#define DISP_INV_OFF	0x20 	//Display Inversion off 	
#define DISP_INV_ON		0x21 	//Display Inversion on 			
#define GAMMA_SET_W1	0x26 	//Gamma Set W 1 format:
#define DISP_OFF		0x28 	//Display off 				
#define DISP_ON			0x29	//Display on 				
#define COLUMN_ADR_SET	0x2A 	//Column Address Set W 4 format:
#define PAGE_ADR_SET	0x2B 	//Page Address Set W 4 format:
#define MEM_WRITE		0x2C 	//Memory Write W Any length, 
#define SET_COLOUR		0x2D 	//Colour set W 192 format:
#define MEM_READ		0x2E 	//Memory Read R Anylength Successive video data stream
#define PART_AREA		0x30 	//Partial area W 4 format:
#define VERT_SCR		0x33 	//Vertical Scrolling Definition W 6 format:
#define TEARING_OFF		0x34 	//Tearing Effect Line Off C 0
#define TEARING_ON		0x35 	//Tearing Effect Line On W 1 1 byte for Tearing Effect Line Mode selection.
#define MEM_ACS_CNTRL	0x36 	//Memory Access Control W 1 1 byte for memory scan direction
#define VERT_SCR_START	0x37 	//Vertical scrolling start Address W 2 2 byte for line pointer
#define IDLE_MODE_OFF	0x38 	//Idle Mode off C 0 -
#define IDLE_MODE_ON	0x39 	//Idle Mode on C 0 -
#define SET_INTF_PX_F	0x3A 	//Interface pixel format W 1 Please Refer to Table in Section 9.2.33.
#define RD_ID1			0xDA 	//Read ID1 R (1) xx for xx Corporation
#define RD_ID2			0xDB 	//Read ID2 R (1) format: 128 to 255 for module version.
#define RD_ID3			0xDC 	//Read ID3 R (1) xx for this project.

//LCD Signal/Data-port
//#define TE  	         		x		//TE effect is ON when input is 0
#define LCD_DATA_PIN            PORTA.IN
#define LCD_DATA_DDR            PORTA.DIR
#define LCD_DATA_PORT           PORTA.OUT


#define LCD_WR_S  PORTD.OUTSET = (1<<0)//write at positive edge
#define LCD_WR_C  PORTD.OUTCLR = (1<<0)
#define LCD_CS_S  PORTD.OUTSET = (1<<2)//chip select active low
#define LCD_CS_C  PORTD.OUTCLR = (1<<2)
#define LCD_RE_S  PORTD.OUTSET = (1<<3)//reset active low
#define LCD_RE_C  PORTD.OUTCLR = (1<<3)
#define LCD_DC_S  PORTB.OUTSET = (1<<1)//Data=1/Command=0
#define LCD_DC_C  PORTB.OUTCLR = (1<<1)
#define LCD_RD_S  PORTD.OUTSET = (1<<1)//read the ID or frame memory bytes at rising edge
#define LCD_RD_C  PORTD.OUTCLR = (1<<1)

#define LCD_LIGHT_DIR PORTB.DIRSET = (1<<2)
#define LCD_LIGHT_ON PORTB.OUTSET = (1<<2)
#define LCD_LIGHT_OFF PORTB.OUTCLR = (1<<2)

//other defines
#define SW						0
#define RED						1
#define GREEN					2
#define BLUE					3

//*****************************************************************************
//                            P R O T O T Y P E S
//*****************************************************************************
extern void waitms(int ms);
extern void LCD_write_cmd(unsigned char cmd);
extern void LCD_write_data(unsigned char data);
extern unsigned char LCD_read_data();
extern void LCD_init(void);
extern void LCD_clear(void);
extern void LCD_draw_pixel(unsigned char red, unsigned char green, unsigned char blue,unsigned int x_pos,unsigned int y_pos );
extern void LCD_draw_line(unsigned char red, unsigned char green, unsigned char blue, unsigned int x_pos1,unsigned int y_pos1,unsigned int x_pos2,unsigned int y_pos2);
extern void LCD_draw_circle(unsigned char red, unsigned char green, unsigned char blue, unsigned int center_xpos, unsigned int center_ypos ,unsigned int radius);
extern void LCD_draw_filled_box(unsigned char red, unsigned char green, unsigned char blue, unsigned int x_pos,unsigned int y_pos,unsigned int width,unsigned int hight);
extern void LCD_draw_box(unsigned char red, unsigned char green, unsigned char blue, unsigned int x_pos,unsigned int y_pos,unsigned int width,unsigned int hight);
extern void LCD_draw_bmp(char *ptr_bmp, unsigned char colour, unsigned int x_pos, unsigned int y_pos);
extern void LCD_write_char(unsigned char letter, unsigned char red, unsigned char green, unsigned char blue, unsigned char size, unsigned int x_pos, unsigned int y_pos);
extern void LCD_write_ram_text(char *text, unsigned char red, unsigned char green, unsigned char blue, unsigned char size, unsigned int x_pos, unsigned int y_pos);
extern void LCD_write_flash_text(char *text, unsigned char red, unsigned char green, unsigned char blue, unsigned char size, unsigned int x_pos, unsigned int y_pos);
extern void LCD_show_init_screen(void);

#endif //LCD_H
