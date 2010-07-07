/*
	This is the Üeber-LCD

	(c) 2010 Jörg Schmidt

*/

//*****************************************************************************
//                    I N C L U D E S / D E F I N E S
//*****************************************************************************
#include "lcd.h"
#include "font.h"
#include <stdio.h>
#include <string.h>
//*****************************************************************************
//                    S T R I N G S  / V A R S
//*****************************************************************************
char txt_init1[] PROGMEM="Powered by";
char txt_init2[] PROGMEM="FAB";
char txt_init3[] PROGMEM="OS";


//*****************************************************************************
//                           F U N C T I O N S
//*****************************************************************************

void lcd_write_cmd(uint8_t cmd)
{
	LCD_DC_C;	//write cmd // this could eventually be done once instead of every time ? fixme
	LCD_DATA_PORT = cmd;		//set cmd
	LCD_WR_C;	//Toggle WR
	LCD_WR_S;	//Toggle WR		
}

void lcd_write_data(uint8_t data)
{
	LCD_DC_S;	//write data
	LCD_DATA_PORT = data;		//set data
	LCD_WR_C;	//Toggle WR
	LCD_WR_S;	//Toggle WR	
}

uint8_t lcd_read_data()
{
	uint8_t data;
	LCD_DATA_DDR = 0x00;		//port as input
	LCD_DC_S;	//read data
	LCD_RD_C;	//Toggle RD
	data = LCD_DATA_PIN;		//read data
	LCD_RD_S;	//Toggle RD
	LCD_DATA_DDR = 0xff;		//port as output
	return data;	
}


void lcd_init(void)
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
	//_delay_ms(1); // may be replaced by OS-wait, not using the delay lib? fixme
	//set 24bit colour mode
	lcd_write_cmd(SET_INTF_PX_F);	//set interface pixel format
	lcd_write_data(0x07);			//set 24bbp
	//memory write direction
	lcd_write_cmd(MEM_ACS_CNTRL);
	lcd_write_data(0x70);	
	//enable LCD
	lcd_write_cmd(DISP_SLEEP_OUT);	//sleep out
	lcd_write_cmd(IDLE_MODE_OFF);	//idle off
	lcd_write_cmd(NORM_MODE_ON);	//normal display mode on
	lcd_write_cmd(DISP_ON);			//lcd on
	//clear_lcd
	lcd_clear();
	//Enable Backlight
	LCD_LIGHT_DIR;
	LCD_LIGHT_ON;
}

void lcd_clear(void)
{
	uint16_t zeile,pos;
	//set column pointer
	lcd_write_cmd(COLUMN_ADR_SET);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x01);
	lcd_write_data(0x3F);

	//set page pointer
	lcd_write_cmd(PAGE_ADR_SET);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0xEF);
	//clear lcd
	lcd_write_cmd(MEM_WRITE);	//write memory
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

void lcd_draw_pixel(uint8_t red, uint8_t green, uint8_t blue, uint16_t x_pos, uint16_t y_pos )
{
	//set new pointer addr
	lcd_write_cmd(COLUMN_ADR_SET);
	lcd_write_data(HIGH(x_pos));
	lcd_write_data(LOW(x_pos));
	//set page pointer
	lcd_write_cmd(PAGE_ADR_SET);
	lcd_write_data((HIGH(y_pos)));
	lcd_write_data(LOW(y_pos));
	//write pixel
	lcd_write_cmd(MEM_WRITE);
	lcd_write_data(red);
	lcd_write_data(green);
	lcd_write_data(blue); 
}

void lcd_draw_line(uint8_t red, uint8_t green, uint8_t blue, uint16_t x_pos1,uint16_t y_pos1,uint16_t x_pos2,uint16_t y_pos2)
{
  //reset pointer
	//set column pointer
	lcd_write_cmd(COLUMN_ADR_SET);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x01);
	lcd_write_data(0x3F);

	//set page pointer
	lcd_write_cmd(PAGE_ADR_SET);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0xEF);

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

  lcd_draw_pixel(red, green, blue, x_pos1,y_pos1);

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
      lcd_draw_pixel(red, green, blue, x_pos1,y_pos1);
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
      lcd_draw_pixel(red, green, blue, x_pos1,y_pos1);
    }
  }
}

void lcd_draw_circle(uint8_t red, uint8_t green, uint8_t blue, uint16_t center_xpos, uint16_t center_ypos ,uint16_t radius)
{
	//reset pointer
	//set column pointer
	lcd_write_cmd(COLUMN_ADR_SET);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x01);
	lcd_write_data(0x3F);

	//set page pointer
	lcd_write_cmd(PAGE_ADR_SET);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0xEF);

	int x, y, xchange, ychange, radiusError;
	x = radius;
	y = 0;
	xchange = 1 - 2 * radius;
	ychange = 1;
	radiusError = 0;
	while(x >= y)
  		{
  			lcd_draw_pixel(red, green, blue, center_xpos+x, center_ypos+y); 
  			lcd_draw_pixel(red, green, blue, center_xpos-x, center_ypos+y); 
  			lcd_draw_pixel(red, green, blue, center_xpos-x, center_ypos-y);
 			lcd_draw_pixel(red, green, blue, center_xpos+x, center_ypos-y); 
  			lcd_draw_pixel(red, green, blue, center_xpos+y, center_ypos+x); 
  			lcd_draw_pixel(red, green, blue, center_xpos-y, center_ypos+x); 
  			lcd_draw_pixel(red, green, blue, center_xpos-y, center_ypos-x); 
  			lcd_draw_pixel(red, green, blue, center_xpos+y, center_ypos-x); 
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

void lcd_draw_filled_box(uint8_t red, uint8_t green, uint8_t blue, uint16_t x_pos,uint16_t y_pos,uint16_t width,uint16_t height)
{
	//plus_bmp minus_bmp ok_bmp cancel_bmp
	uint16_t i;

	//set new pointer addr
	lcd_write_cmd(COLUMN_ADR_SET);
	lcd_write_data(HIGH(x_pos));
	lcd_write_data(LOW(x_pos));
	i= x_pos+width-1;
	lcd_write_data(HIGH(i));
	lcd_write_data(LOW(i));
	//set page pointer
	lcd_write_cmd(PAGE_ADR_SET);
	lcd_write_data(HIGH(y_pos));
	lcd_write_data(LOW(y_pos));
	i= y_pos+height-1;
	lcd_write_data(HIGH(i));
	lcd_write_data(LOW(i));
	//write pixel
	lcd_write_cmd(MEM_WRITE);
	for (i=0;i<(3*width*height);i++)
		{
		lcd_write_data(red);
		lcd_write_data(green);
		lcd_write_data(blue);
		}
}

void lcd_draw_box(uint8_t red, uint8_t green, uint8_t blue,uint16_t x_pos,uint16_t y_pos,uint16_t width,uint16_t hight)
{
	uint16_t x;
	//obere Kante
	for(x=0;x<width;x++)
	{
		lcd_draw_pixel(red, green, blue, x_pos+x, y_pos);
	}
	//linke/rechte Kante
	for(x=0;x<hight;x++)
	{
		lcd_draw_pixel(red, green, blue, x_pos,y_pos+x);
		lcd_draw_pixel(red, green, blue, x_pos+width,y_pos+x);

	}
	//untere Kante
	for(x=0;x<=width;x++)
	{
		lcd_draw_pixel(red, green, blue, x_pos+x,y_pos+hight);
	}		
}

void lcd_draw_bmp(char *ptr_bmp, uint8_t colour, uint16_t x_pos, uint16_t y_pos)
{
	//plus_bmp minus_bmp ok_bmp cancel_bmp
	uint16_t width, height, i;
	uint8_t greyscale;

	width = pgm_read_byte(ptr_bmp++);
	height = pgm_read_byte(ptr_bmp++);
	greyscale = pgm_read_byte(ptr_bmp++);

	//set new pointer addr
	lcd_write_cmd(COLUMN_ADR_SET);
	lcd_write_data(HIGH(x_pos));
	lcd_write_data(LOW(x_pos));
	i = x_pos+width-1;
	lcd_write_data(HIGH(i));
	lcd_write_data(LOW(i));
	//set page pointer
	lcd_write_cmd(PAGE_ADR_SET);
	lcd_write_data(HIGH(y_pos));
	lcd_write_data(LOW(y_pos));
	i= y_pos+height-1;
	lcd_write_data(HIGH(i));
	lcd_write_data(LOW(i));
	//colour mode
	if(!greyscale)
		{
		//write pixel
		lcd_write_cmd(MEM_WRITE);
		for (i=0;i<(3*width*height);i++){lcd_write_data(pgm_read_byte(ptr_bmp++));}
		}
	else //monochrom
		{
		//write pixel
		lcd_write_cmd(MEM_WRITE);
		for (i=0;i<(width*height);i++)
			{
			switch( colour )
     			{
        		case 0 : //sw
					lcd_write_data(pgm_read_byte(ptr_bmp));
					lcd_write_data(pgm_read_byte(ptr_bmp));
					lcd_write_data(pgm_read_byte(ptr_bmp++));
					break;
        		case 1 : //red
					lcd_write_data(pgm_read_byte(ptr_bmp++));
					lcd_write_data(0);
					lcd_write_data(0);
					break;
        		case 2 : //green
					lcd_write_data(0);
					lcd_write_data(pgm_read_byte(ptr_bmp++));
					lcd_write_data(0);
					break;
        		case 3 : //blue
					lcd_write_data(0);
					lcd_write_data(0);
					lcd_write_data(pgm_read_byte(ptr_bmp++));
					break;
        		default  : //sw
					lcd_write_data(pgm_read_byte(ptr_bmp));
					lcd_write_data(pgm_read_byte(ptr_bmp));
					lcd_write_data(pgm_read_byte(ptr_bmp++));
				}
     		}		
		}
		
}

void lcd_write_char(uint8_t letter, uint8_t font_red, uint8_t font_green, uint8_t font_blue,  uint8_t back_red, uint8_t back_green, uint8_t back_blue, uint8_t size, uint16_t x_pos, uint16_t y_pos)
{
		
	uint16_t i;
	uint8_t width, space, space_width, y_scale, x_scale, act_byte =0x00, bit_nr=0x00, act_row, y_offset;
	uint8_t *ptr_letter;

	space_width = 2; 	//distance between two letters

	//set pointer and get letter size
	ptr_letter = font[letter-32];			//font sign pointer
	width = pgm_read_byte(ptr_letter++);	//width of the letter in pixel
	y_offset = pgm_read_byte(ptr_letter++); //distance bewteen top of "letter-frame" and start of letter itself in pixel

	//Define a memory area where the MCU can access the lcd memory
	//Access to other memory areas are denied
	//set new pointer addr (width of the area)
	lcd_write_cmd(COLUMN_ADR_SET);
	lcd_write_data(HIGH(x_pos));
	lcd_write_data(LOW(x_pos));
	i = x_pos+(width+space_width)*size-1;
	lcd_write_data(HIGH(i));
	lcd_write_data(LOW(i));
	//set page pointer (height of the area)
	lcd_write_cmd(PAGE_ADR_SET);
	lcd_write_data(HIGH(y_pos));
	lcd_write_data(LOW(y_pos));
	i= y_pos+FONTHEIGHT*size-1;
	lcd_write_data(HIGH(i));
	lcd_write_data(LOW(i));
	//start writing to lcd ram
	lcd_write_cmd(MEM_WRITE);	
	
	//fill pixel offset field
	for(act_row=0;act_row<y_offset*size;act_row++)
		{
		for(bit_nr=0;bit_nr<(width+space_width)*size;bit_nr++)
			{
			lcd_write_data(back_red);
			lcd_write_data(back_green);
			lcd_write_data(back_blue);
			}
		}
	//draw sign
	for(act_row=0;act_row<FONTHEIGHT-y_offset;act_row++)
		{
		for(y_scale=0;y_scale<size;y_scale++)
			{
			ptr_letter = &font[letter-32][2+act_row*2];
			for(bit_nr=0;bit_nr<width;bit_nr++)
				{
				//load next byte
				if(bit_nr%8==0)
					{
					act_byte = pgm_read_byte(ptr_letter++);
					}
				for(x_scale=0;x_scale<size;x_scale++)
					{
					//draw pixel in fontcolour
					if( (1<<(7-bit_nr%8)) & act_byte)
						{
						lcd_write_data(font_red);
						lcd_write_data(font_green);
						lcd_write_data(font_blue);
						}
					//draw pixel in backgroundcolour
					else 
						{
						lcd_write_data(back_red);
						lcd_write_data(back_green);
						lcd_write_data(back_blue);
						}
					}
				}
				//write space between two letters
				for(space=0;space<space_width*size;space++)
					{//draw pixel in backgroundcolour
					lcd_write_data(back_red);
					lcd_write_data(back_green);
					lcd_write_data(back_blue);
					}
			} 
		}	
}

void lcd_show_init_screen(void)
{
	//Powered by
	lcd_print(WHITE, BLACK , 2, 30, 50,flash2ram(&txt_init1));
	//FAB
	lcd_print(YELLOW, BLACK , 3, 100, 100,flash2ram(&txt_init2));
	//OS
	lcd_print(RED, BLACK , 3, 185, 110,flash2ram(&txt_init3));
};



void itoa10(int value, char* result)
{
	char* pt1 = result;
	char* pt2 = result;
	char tmp_char;
	int tmp_value;

	// div by 10
	do {
		tmp_value = value;
		value /= 10;
		*pt1++ = '0'+abs(tmp_value - (value * 10));
	} while ( value );
	// negative?
	if (tmp_value < 0) *pt1++ = '-';
	*pt1-- = '\0';
	// revert
	while(pt2 < pt1) {
		tmp_char = *pt1;
		*pt1--= *pt2;
		*pt2++ = tmp_char;
	}
}

void itoa10ra(int value, char* result)
{
	char* pt1 = result+5;
	*(pt1+1) = '\0';
	int tmp_value;

	memset(result,' ',6);

	// div by 10
	do {
		tmp_value = value;
		value /= 10;
		*pt1-- = '0'+abs(tmp_value - value * 10);
	} while ( value );
	// negative?
	if (tmp_value < 0) *pt1++ = '-';
	
}

void lcd_print(uint8_t font_red,uint8_t font_green, uint8_t font_blue,uint8_t back_red,uint8_t back_green, uint8_t back_blue, uint8_t size, uint16_t x_pos, uint16_t y_pos,char *ptr_string,...)
{
    va_list specifier;                  //list of specifier ,...                
    va_start (specifier, ptr_string);   	//initialize specifier list       
    char act_sign;                      //actual sign of the string
	char pixel_width, *ptr_buf, buf[8]={};
	uint8_t tabwidth=0;
	uint16_t new_x_pos, new_y_pos;
	//get position
	new_x_pos=x_pos;
	new_y_pos=y_pos;
			
    	while (*ptr_string)
    	{
	  	//Get next sign
		act_sign = *ptr_string++;
		//get letter width
		pixel_width = pgm_read_byte(font[act_sign-32]);
		//check if line ends
		if(new_x_pos+pixel_width*size>319)
			{
			new_x_pos=0;
			new_y_pos+=FONTHEIGHT*size;
			}
        if (act_sign=='%')              //value-placeholder?
        {
            //Get next sign
			act_sign = *ptr_string++;

            switch (act_sign)
                {           
					case 'i': //lcd_print integerchar
						//convert uint16_t
						itoa10(va_arg(specifier,uint16_t),buf);
						//lcd_print value
						ptr_buf=buf;
						while(*ptr_buf)
						{
							//get letter width
							pixel_width = pgm_read_byte(font[*ptr_buf-32]);
							//check if line ends
							if(new_x_pos+pixel_width*size>319)
							{
								new_x_pos=0;
								new_y_pos+=FONTHEIGHT*size;
							}
							//Write sign
							lcd_write_char(*ptr_buf++, font_red, font_green, font_blue, back_red, back_green, back_blue, size, new_x_pos, new_y_pos);
							//adjust position
							new_x_pos+=size*(pixel_width+CHARSPACE);
						}
						break;
					case 'd': // fixed point; next char is: comma shift to !left! (%d1 = ####.# %d2= ###.## etc.)
						;
						uint8_t shift = (*ptr_string++)-48; // get shift amount
											//convert uint16_t
						itoa10(va_arg(specifier,uint16_t),buf);
						//lcd_print value
						ptr_buf=buf;
						while(*ptr_buf)
						{
							//get letter width
							pixel_width = pgm_read_byte(font[*ptr_buf-32]);
							//check if line ends
							if(new_x_pos+pixel_width*size>319)
							{
								new_x_pos=0;
								new_y_pos+=FONTHEIGHT*size;
							}
							//Write sign
							lcd_write_char(*ptr_buf++, font_red, font_green, font_blue, back_red, back_green, back_blue, size, new_x_pos, new_y_pos);
							//adjust position
							new_x_pos+=size*(pixel_width+CHARSPACE);
						}
						break;
					default:
						break;
					//fixme other "itoa's"
                }
        }
		else if (act_sign=='/')  //format sign, e.g. tab
		{
            //Get next sign
	        act_sign = *ptr_string++;

            switch (act_sign)
                {           
                case 't': //increase xpos
					tabwidth=TABSIZE-(new_x_pos%TABSIZE);
					lcd_draw_filled_box(back_red, back_green, back_blue, new_x_pos,new_y_pos, tabwidth, FONTHEIGHT*size);
                    new_x_pos+=tabwidth;
					break;
				case '/': //lcd_print '/'
					lcd_write_char(act_sign, font_red, font_green, font_blue, back_red, back_green, back_blue, size, new_x_pos, new_y_pos);
        			//adjust position
					new_x_pos+=size*(pixel_width+CHARSPACE);
					break;
				case '%': //lcd_print '%'
					lcd_write_char(act_sign, font_red, font_green, font_blue, back_red, back_green, back_blue, size, new_x_pos, new_y_pos);
        			//adjust position
					new_x_pos+=size*(pixel_width+CHARSPACE);
					break;
				case 'n': //create newline
					new_x_pos=0;
					new_y_pos+=FONTHEIGHT*size;
					break;
                //default: 
                }
        }
        else //write "normal" sign
        {
            //Write sign
			lcd_write_char(act_sign, font_red, font_green, font_blue, back_red, back_green, back_blue, size, new_x_pos, new_y_pos);
        	//adjust position
			new_x_pos+=size*(pixel_width+CHARSPACE);
		}
        
    }
  va_end(specifier); //close the specifier list     
}

char *flash2ram(char *ptr_string)
{
	static char returnstring[40]={}, *ptr_returnstring; // fixme anders, Speicher schonen!
	//copy string 
    ptr_returnstring=returnstring;
	while (pgm_read_byte(ptr_string))
    	{
	  	//Get next sign
        *ptr_returnstring++ = pgm_read_byte(ptr_string++);    
		}
	//terminate string
	*ptr_returnstring='\0';
	
	return returnstring;
 
}
