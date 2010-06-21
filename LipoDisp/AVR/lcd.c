//*****************************************************************************
//                    I N C L U D E S / D E F I N E S
//*****************************************************************************
#include "lcd.h"
#include "font.h"

//*****************************************************************************
//                    S T R I N G S  / V A R S
//*****************************************************************************
char txt_init1[] PROGMEM="Powered by";
char txt_init2[] PROGMEM="FAB";
char txt_init3[] PROGMEM="OS";

//*****************************************************************************
//                            P R O T O T Y P E S
//*****************************************************************************
void waitms(int ms);
void write_cmd(unsigned char cmd);
void write_data(unsigned char data);
unsigned char read_data();
void init_lcd(void);
void clear_lcd(void);
void draw_pixel(unsigned char red, unsigned char green, unsigned char blue,unsigned int x_pos,unsigned int y_pos );
void draw_line(unsigned char red, unsigned char green, unsigned char blue, unsigned int x_pos1,unsigned int y_pos1,unsigned int x_pos2,unsigned int y_pos2);
void draw_circle(unsigned char red, unsigned char green, unsigned char blue, unsigned int center_xpos, unsigned int center_ypos ,unsigned int radius);
void draw_filled_box(unsigned char red, unsigned char green, unsigned char blue, unsigned int x_pos,unsigned int y_pos,unsigned int width,unsigned int hight);
void draw_box(unsigned char red, unsigned char green, unsigned char blue, unsigned int x_pos,unsigned int y_pos,unsigned int width,unsigned int hight);
void draw_bmp(char *ptr_bmp, unsigned char colour, unsigned int x_pos, unsigned int y_pos);
void write_char(unsigned char letter, unsigned char red, unsigned char green, unsigned char blue, unsigned char size, unsigned int x_pos, unsigned int y_pos);
void write_ram_text(char *text, unsigned char red, unsigned char green, unsigned char blue, unsigned char size, unsigned int x_pos, unsigned int y_pos);
void write_flash_text(char *text, unsigned char red, unsigned char green, unsigned char blue, unsigned char size, unsigned int x_pos, unsigned int y_pos);
void show_init_screen(void);

//*****************************************************************************
//                           F U N C T I O N S
//*****************************************************************************
void waitms(int ms) {
  int i;
  for (i=0;i<ms;i++) _delay_ms(1);
}


void write_cmd(unsigned char cmd)
{
	LCD_DC_C;	//write cmd
	LCD_DATA_PORT = cmd;		//set cmd
	LCD_WR_C;	//Toggle WR
	LCD_WR_S;	//Toggle WR		
}

void write_data(unsigned char data)
{
	LCD_DC_S;	//write data
	LCD_DATA_PORT = data;		//set data
	LCD_WR_C;	//Toggle WR
	LCD_WR_S;	//Toggle WR	
}

unsigned char read_data()
{
	unsigned char data;
	LCD_DATA_DDR = 0x00;		//port as input
	LCD_DC_S;	//read data
	LCD_RD_C;	//Toggle RD
	data = LCD_DATA_PIN;		//read data
	LCD_RD_S;	//Toggle RD
	_delay_us(1);
	LCD_DATA_DDR = 0xff;		//port as output
	return data;	
}


void init_lcd(void)
{
	//lcd data port as output
	LCD_DATA_DDR = 0xff;
	//lcd cmd port as output except TE
    PORTB.DIRSET = (1<<1);
	PORTD.DIRSET = 0x0f;
	//init values
	LCD_WR_S;
	LCD_CS_C;
	LCD_RE_C;
	LCD_DC_C;
	LCD_RD_S;
	//Reset disable
	LCD_RE_S;
	_delay_ms(1);
	//set 24bit colour mode
	write_cmd(SET_INTF_PX_F);	//set interface pixel format
	write_data(0x07);			//set 24bbp
	//memory write direction
	write_cmd(MEM_ACS_CNTRL);
	write_data(0x70);	
	//enable LCD
	write_cmd(DISP_SLEEP_OUT);	//sleep out
	write_cmd(IDLE_MODE_OFF);	//idle off
	write_cmd(NORM_MODE_ON);	//normal display mode on
	write_cmd(DISP_ON);			//lcd on
	//clear_lcd
	clear_lcd();

	LCD_LIGHT_DIR;
	LCD_LIGHT_ON;
}

void clear_lcd(void)
{
	unsigned int zeile,pos;
	//set column pointer
	write_cmd(COLUMN_ADR_SET);
	write_data(0x00);
	write_data(0x00);
	write_data(0x01);
	write_data(0x3F);

	//set page pointer
	write_cmd(PAGE_ADR_SET);
	write_data(0x00);
	write_data(0x00);
	write_data(0x00);
	write_data(0xEF);
	//clear lcd
	write_cmd(MEM_WRITE);	//write memory
	LCD_DC_S;	//write data
	LCD_DATA_PORT = 0x00;		//set data
	for (zeile=0;zeile<240;zeile++)
		{
		for (pos=0;pos<160;pos++)
			{
			//1 pixel
			LCD_WR_C;	//Toggle WR
			LCD_WR_S;	//Toggle WR
			LCD_WR_C;	//Toggle WR
			LCD_WR_S;	//Toggle WR
			LCD_WR_C;	//Toggle WR
			LCD_WR_S;	//Toggle WR
			
			//2 pixel
			LCD_WR_C;	//Toggle WR
			LCD_WR_S;	//Toggle WR
			LCD_WR_C;	//Toggle WR
			LCD_WR_S;	//Toggle WR
			LCD_WR_C;	//Toggle WR
			LCD_WR_S;	//Toggle WR
			}
		}	
}

void draw_pixel(unsigned char red, unsigned char green, unsigned char blue, unsigned int x_pos, unsigned int y_pos )
{
	//set new pointer addr
	write_cmd(COLUMN_ADR_SET);
	write_data((unsigned char)(x_pos>>8));
	write_data((unsigned char)x_pos);
	//set page pointer
	write_cmd(PAGE_ADR_SET);
	write_data((unsigned char)(y_pos>>8));
	write_data((unsigned char)y_pos);
	//write pixel
	write_cmd(MEM_WRITE);
	write_data(red);
	write_data(green);
	write_data(blue); 
}

void draw_line(unsigned char red, unsigned char green, unsigned char blue, unsigned int x_pos1,unsigned int y_pos1,unsigned int x_pos2,unsigned int y_pos2)
{
  int dx,dy,stepx,stepy,fraction;

  dy = y_pos2 - y_pos1;
  dx = x_pos2 - x_pos1;

  if (dy < 0)
  { dy = -dy; stepy = -1;}
  else
  { stepy = 1;}

  if (dx < 0)
  { dx = -dx; stepx = -1;  }
  else
  { stepx = 1;}

  dy <<= 1;
  dx <<= 1;

  draw_pixel(red, green, blue, x_pos1,y_pos1);

  if (dx > dy)
  {
    fraction = dy - (dx >> 1);
    while (x_pos1 != x_pos2)
    {
      if (fraction >= 0)
      {
        y_pos1 += stepy;
        fraction -= dx;
      }
      x_pos1 += stepx;
      fraction += dy;
      draw_pixel(red, green, blue, x_pos1,y_pos1);
    }
  }
  else
  {
    fraction = dx - (dy >> 1);
    while (y_pos1 != y_pos2)
    {
      if (fraction >= 0)
      {
        x_pos1 += stepx;
        fraction -= dy;
      }
      y_pos1 += stepy;
      fraction += dx;
      draw_pixel(red, green, blue, x_pos1,y_pos1);
    }
  }
}

void draw_circle(unsigned char red, unsigned char green, unsigned char blue, unsigned int center_xpos, unsigned int center_ypos ,unsigned int radius)
{
	int x, y, xchange, ychange, radiusError;
	x = radius;
	y = 0;
	xchange = 1 - 2 * radius;
	ychange = 1;
	radiusError = 0;
	while(x >= y)
  		{
  			draw_pixel(red, green, blue, center_xpos+x, center_ypos+y); 
  			draw_pixel(red, green, blue, center_xpos-x, center_ypos+y); 
  			draw_pixel(red, green, blue, center_xpos-x, center_ypos-y);
 			draw_pixel(red, green, blue, center_xpos+x, center_ypos-y); 
  			draw_pixel(red, green, blue, center_xpos+y, center_ypos+x); 
  			draw_pixel(red, green, blue, center_xpos-y, center_ypos+x); 
  			draw_pixel(red, green, blue, center_xpos-y, center_ypos-x); 
  			draw_pixel(red, green, blue, center_xpos+y, center_ypos-x); 
  			y++;
  			radiusError += ychange;
  			ychange += 2;
  			if ( 2*radiusError + xchange > 0 )
    			{
    				x--;
					radiusError += xchange;
					xchange += 2;
				}
  		}
}

void draw_filled_box(unsigned char red, unsigned char green, unsigned char blue, unsigned int x_pos,unsigned int y_pos,unsigned int width,unsigned int height)
{
	//plus_bmp minus_bmp ok_bmp cancel_bmp
	unsigned int i;

	//set new pointer addr
	write_cmd(COLUMN_ADR_SET);
	write_data((unsigned char)(x_pos>>8));
	write_data((unsigned char)x_pos);
	write_data((unsigned char)((x_pos+width-1)>>8));
	write_data((unsigned char)(x_pos+width-1));
	//set page pointer
	write_cmd(PAGE_ADR_SET);
	write_data((unsigned char)(y_pos>>8));
	write_data((unsigned char)y_pos);
	write_data((unsigned char)((y_pos+height-1)>>8));
	write_data((unsigned char)(y_pos+height-1));
	//write pixel
	write_cmd(MEM_WRITE);
	for (i=0;i<(3*width*height);i++)
		{
		write_data(red);
		write_data(green);
		write_data(blue);
		}
	//reset pointer
	//set column pointer
	write_cmd(COLUMN_ADR_SET);
	write_data(0x00);
	write_data(0x00);
	write_data(0x01);
	write_data(0x3F);

	//set page pointer
	write_cmd(PAGE_ADR_SET);
	write_data(0x00);
	write_data(0x00);
	write_data(0x00);
	write_data(0xEF);	
}

void draw_box(unsigned char red, unsigned char green, unsigned char blue,unsigned int x_pos,unsigned int y_pos,unsigned int width,unsigned int hight)
{
	unsigned int x;
	//obere Kante
	for(x=0;x<width;x++)
	{
		draw_pixel(red, green, blue, x_pos+x, y_pos);
	}
	//linke/rechte Kante
	for(x=0;x<hight;x++)
	{
		draw_pixel(red, green, blue, x_pos,y_pos+x);
		draw_pixel(red, green, blue, x_pos+width,y_pos+x);

	}
	//untere Kante
	for(x=0;x<=width;x++)
	{
		draw_pixel(red, green, blue, x_pos+x,y_pos+hight);
	}		
}

void draw_bmp(char *ptr_bmp, unsigned char colour, unsigned int x_pos, unsigned int y_pos)
{
	//plus_bmp minus_bmp ok_bmp cancel_bmp
	unsigned int width, height, i;
	unsigned char greyscale;

	width = pgm_read_byte(ptr_bmp++);
	height = pgm_read_byte(ptr_bmp++);
	greyscale = pgm_read_byte(ptr_bmp++);

	//set new pointer addr
	write_cmd(COLUMN_ADR_SET);
	write_data((unsigned char)(x_pos>>8));
	write_data((unsigned char)x_pos);
	write_data((unsigned char)((x_pos+width-1)>>8));
	write_data((unsigned char)(x_pos+width-1));
	//set page pointer
	write_cmd(PAGE_ADR_SET);
	write_data((unsigned char)(y_pos>>8));
	write_data((unsigned char)y_pos);
	write_data((unsigned char)((y_pos+height-1)>>8));
	write_data((unsigned char)(y_pos+height-1));
	//colour mode
	if(!greyscale)
		{
		//write pixel
		write_cmd(MEM_WRITE);
		for (i=0;i<(3*width*height);i++){write_data(pgm_read_byte(ptr_bmp++));}
		}
	else //monochrom
		{
		//write pixel
		write_cmd(MEM_WRITE);
		for (i=0;i<(width*height);i++)
			{
			switch( colour )
     			{
        		case 0 : //sw
					write_data(pgm_read_byte(ptr_bmp));
					write_data(pgm_read_byte(ptr_bmp));
					write_data(pgm_read_byte(ptr_bmp++));
					break;
        		case 1 : //red
					write_data(pgm_read_byte(ptr_bmp++));
					write_data(0);
					write_data(0);
					break;
        		case 2 : //green
					write_data(0);
					write_data(pgm_read_byte(ptr_bmp++));
					write_data(0);
					break;
        		case 3 : //blue
					write_data(0);
					write_data(0);
					write_data(pgm_read_byte(ptr_bmp++));
					break;
        		default  : //sw
					write_data(pgm_read_byte(ptr_bmp));
					write_data(pgm_read_byte(ptr_bmp));
					write_data(pgm_read_byte(ptr_bmp++));
				}
     		}		
		}
	//reset pointer
	//set column pointer
	write_cmd(COLUMN_ADR_SET);
	write_data(0x00);
	write_data(0x00);
	write_data(0x01);
	write_data(0x3F);

	//set page pointer
	write_cmd(PAGE_ADR_SET);
	write_data(0x00);
	write_data(0x00);
	write_data(0x00);
	write_data(0xEF);

		
}

void write_char(unsigned char letter, unsigned char red, unsigned char green, unsigned char blue, unsigned char size, unsigned int x_pos, unsigned int y_pos)
{
	volatile unsigned char act_byte =0x00, bit_nr=0x00, act_row, pixel_width, y_offset;
	volatile unsigned char x_scale, y_scale;
	volatile char *ptr_letter; 
	volatile unsigned int new_x_pos, new_y_pos;
	
	//set pointer and get letter size
	ptr_letter = font[letter-32];
	pixel_width = pgm_read_byte(ptr_letter++);
	y_offset = size*pgm_read_byte(ptr_letter++);
	//set x pos
	new_x_pos = x_pos;
	//draw letter
	for(act_row=0;act_row<pixel_width;act_row++)
		{
		for(x_scale=0;x_scale<size;x_scale++)
			{
			ptr_letter = &font[letter-32][2+act_row*2];
			//set y pos
			new_y_pos = y_pos+y_offset;
			//draw column
			for(bit_nr=0;bit_nr<16;bit_nr++)
				{
				//load next byte
				if(bit_nr%8==0)
					{
					act_byte = pgm_read_byte(ptr_letter++);
					}
				for(y_scale=0;y_scale<size;y_scale++)
					{
					//draw bit
					if( (1<<(7-bit_nr%8)) & act_byte)
						{draw_pixel(red, green, blue, new_x_pos, new_y_pos++);}
					else 
						{new_y_pos++;}
					}
				}
			//set new x pos
			new_x_pos++;
			}
		}
}

void write_ram_text(char *text, unsigned char red, unsigned char green, unsigned char blue, unsigned char size, unsigned int x_pos, unsigned int y_pos)
{
	unsigned int new_x_pos, new_y_pos;
	char *ptr_letter, pixel_width; 
	new_x_pos=x_pos;
	new_y_pos=y_pos;
	while(*text!=0x00)
		{
		//get letter width
		ptr_letter = font[*text-32];
		pixel_width = pgm_read_byte(ptr_letter);
		//end of line?
		if(new_x_pos > 320-pixel_width) 
			{
			new_y_pos+= size*16;
			new_x_pos=x_pos;
			}
		//write text
		if(*text!=' ') 
			{
			write_char(*text, red, green, blue, size, new_x_pos, new_y_pos);
			new_x_pos+=size*pixel_width+size*2;
			}
		//space
		else
			{
			new_x_pos+=size*4;
			}
		//next letter
		text++;
		}
}

void write_flash_text(char *text, unsigned char red, unsigned char green, unsigned char blue, unsigned char size, unsigned int x_pos, unsigned int y_pos)
{
	unsigned int new_x_pos, new_y_pos;
	char *ptr_letter, pixel_width; 
	new_x_pos=x_pos;
	new_y_pos=y_pos;
	while(pgm_read_byte(text)!=0x00)
		{
		//get letter width
		ptr_letter = font[pgm_read_byte(text)-32];
		pixel_width = pgm_read_byte(ptr_letter);
		//end of line?
		if(new_x_pos > 320-pixel_width) 
			{
			new_y_pos+= size*16;
			new_x_pos=x_pos;
			}
		//write text
		if(pgm_read_byte(text)!=' ') 
			{
			write_char(pgm_read_byte(text), red, green, blue, size, new_x_pos, new_y_pos);
			new_x_pos+=size*pixel_width+size*2;
			}
		//space
		else
			{
			new_x_pos+=size*4;
			}
		//next letter
		text++;
		}
}

void show_init_screen(void)
{
	//Powered by
	write_flash_text(txt_init1, 255, 255, 255, 2, 30, 50);
	//FAB
	write_flash_text(txt_init2, 255, 255, 0, 3, 80, 100);
	//OS
	write_flash_text(txt_init3, 255, 0, 0, 4, 165, 110);
};
