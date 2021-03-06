/*
	This is the �eber-LCD

	(c) 2010 J�rg Schmidt

*/

#ifndef LCD_H
	#define LCD_H

//*****************************************************************************
//                            I N C L U D E
//*****************************************************************************

#include <avr/io.h>
//#include <stdlib.h>
// stdlib.h-examples:
// Integer in String for 	itoa( i, s, 10 ) i=Integer;s=Zielstring
// double/float in String 	dtostrf( f, 6, 3, s ) f=float;6=Gesamtstellen;3=Nachkommastelle;s=Zielstring

//*****************************************************************************
//                            D E F I N E
//*****************************************************************************

/*#define LCD_DELAY				1		//Delay in ms
#define LCD_READ_DELAY			1		//Delay in us 
#define LCD_RESET_DELAY			1		//Reset duration
									
//other definitions
#define LOWBYTE(val)	 val & 0x00FF			
#define HIGHBYTE(val)	(val & 0xFF00) >> 8		*/

#define LOW(Data) (*((uint8_t *)(&(Data))))
#define HIGH(Data) (*(((uint8_t *)&(Data))+1))  // obacht! Klammersetzung! // fixme anders?

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
#define LCD_DATA_PIN            PORTA_IN
#define LCD_DATA_DDR            PORTA_DIR
#define LCD_DATA_PORT           PORTA_OUT


#define LCD_WR_S  PORTD_OUTSET = (1<<0)//write at positive edge
#define LCD_WR_C  PORTD_OUTCLR = (1<<0)
#define LCD_CS_S  PORTD_OUTSET = (1<<2)//chip select active low
#define LCD_CS_C  PORTD_OUTCLR = (1<<2)
#define LCD_RE_S  PORTD_OUTSET = (1<<3)//reset active low
#define LCD_RE_C  PORTD_OUTCLR = (1<<3)
#define LCD_DC_S  PORTB_OUTSET = (1<<1)//Data=1/Command=0
#define LCD_DC_C  PORTB_OUTCLR = (1<<1)
#define LCD_RD_S  PORTD_OUTSET = (1<<1)//read the ID or frame memory bytes at rising edge
#define LCD_RD_C  PORTD_OUTCLR = (1<<1)

#define LCD_LIGHT_DIR PORTB_DIRSET = (1<<2)
#define LCD_LIGHT_ON PORTB_OUTSET = (1<<2)
#define LCD_LIGHT_OFF PORTB_OUTCLR = (1<<2)
#define LCD_LIGHT_TGL PORTB_OUTTGL = (1<<2)

//other defines
#define CHARSPACE 2 		//space between 2 letter in pixel (function lcd_print)
#define TABSIZE	40			//Distance between 2 tabs in pixel
#define FONTHEIGHT 18 		//height of font in pixel
#define BPP12 1				//12 bit per pixel color mode
#define BPP24 2				//24 bit per pixel color mode
#define ORIENTATION_0    1	//screen orientation is 0�
#define ORIENTATION_90   2	//screen orientation is 90�
#define ORIENTATION_180  3  //screen orientation is 180�
#define ORIENTATION_270  4	//screen orientation is 270�
//some colours
#define BLACK  0x00,0x00,0x00
#define WHITE  0xff,0xff,0xff
#define RED    0xff,0x00,0x00
#define GREEN  0x00,0xff,0x00
#define BLUE   0x00,0x00,0xff
#define YELLOW 0xff,0xff,0x00
#define ORANGE 0xff,0xff,0x00
#define GREY   0x77,0x77,0x77
#define DGREY   0xAA,0xAA,0xAA

#define abs(X) ((X)<0?-(X):(X))
#define FONT 0
#define BACK 1

//*****************************************************************************
//                            P R O T O T Y P E S
//*****************************************************************************
void lcd_write_cmd(uint8_t cmd);		//write command to lcd
void lcd_write_data(uint8_t data);		//write data to lcd
uint8_t lcd_read_data();				//read data from lcd
void lcd_init(uint8_t gucColorMode, uint8_t gucOrientation);	// init lcd with colormode 12/24bpp and orientation 0�-270�
void lcd_clear(void);					//clear the screen
void lcd_draw_pixel(uint8_t red, uint8_t green, uint8_t blue, uint16_t x_pos, uint16_t y_pos );
void lcd_draw_line(uint8_t red, uint8_t green, uint8_t blue, uint16_t x_pos1,uint16_t y_pos1,uint16_t x_pos2,uint16_t y_pos2);
void lcd_draw_circle(uint8_t red, uint8_t green, uint8_t blue, uint16_t center_xpos, uint16_t center_ypos ,uint16_t radius);
void lcd_draw_box(uint8_t red, uint8_t green, uint8_t blue, uint16_t x_pos,uint16_t y_pos,uint16_t width,uint16_t hight);
void lcd_draw_filled_box(uint8_t red, uint8_t green, uint8_t blue, uint16_t x_pos,uint16_t y_pos,uint16_t width,uint16_t height);
void lcd_draw_bmp(char *ptr_bmp, uint8_t colour, uint16_t x_pos, uint16_t y_pos);
void lcd_write_char(uint8_t letter,  uint8_t font_red, uint8_t font_green, uint8_t font_blue, uint8_t back_red, uint8_t back_green, uint8_t back_blue,uint8_t size, uint16_t x_pos, uint16_t y_pos);
void lcd_print(uint8_t font_red, uint8_t font_green, uint8_t font_blue, uint8_t back_red, uint8_t back_green, uint8_t back_blue, uint8_t size, uint16_t x_pos, uint16_t y_pos,char *ptr_string,...);
char *flash2ram(char *ptr_string);
void lcd_init_scrollmode(void);
void lcd_scrollmode(uint16_t saddr);
void lcd_show_init_screen(void);
void lcd_print_string(uint8_t font_red, uint8_t font_green, uint8_t font_blue, uint8_t back_red, uint8_t back_green, uint8_t back_blue, uint8_t size, uint16_t x_pos, uint16_t y_pos,char *ptr_string);


#endif //LCD_H
