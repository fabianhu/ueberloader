
#include <avr/io.h>

char *itoa(int16_t _Value)
{
	int16_t remainder, factor;
	uint8_t flag = 0;
	uint8_t index;
	char buffer[8];


	index = 0;
	if (_Value < 0)
	{
		 buffer[index++] = '-';
		 _Value = -_Value;
	}
	if (_Value == 0)
	{
		 buffer[index++] = '0';
	}
	else
	{
		 factor = 10000;
		 while (factor > 0)
		 {
				 remainder = _Value / factor;
				 if (flag || remainder > 0)
				 {
						 buffer[index++] = '0' + remainder;
						 flag = 1;
				 }
				 _Value -= remainder * factor;
				 factor = factor / 10;
		 }
	}
	buffer[index] = 0;
	return(buffer);
}


const char* itoa(int val, int padding)


{
  if(padding > 29)
    padding = 29;
  static char buf[30];
  char* ptr = buf+29;
  bool neg = (val < 0);
  if(neg) {
    val = -val;
    --padding;
  }
  *ptr-- = 0;
  do {
    *ptr-- = (val % 10) + '0';
    val /= 10;
    --padding;
  } while(val);
  for(; padding > 0; --padding)
    *ptr-- = '0';
  if(neg)
    *ptr-- = '-';
  return ++ptr;
}

char* itoa (int n)
{
	int i=0,j;
	char s[17];
	char u[17];
	do
	{
		s[i++]=(char)( n%10 + 48 );
		n-=n%10;
	} while((n/=10)>0);
	for (j=0;j<i;j++)
		u[i-1-j]=s[j];
	u[j]='\0';
	return u;
}

void itoa (char *buf, int base, int d)
{
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;
	// If %d is specified and D is minus, put `-' in the head.
	if (base == 'd' && d < 0)
	{
		*p++ = '-';
		buf++;
		ud = -d;
	}
	else if (base == 'x')
		divisor = 16;
	// Divide US by divisor until UD == 0.
	do
	{
		int remainder = ud % divisor;

		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	} while (ud /= divisor);
	// hex values
	if(base == 'x')
	{
		*p++ = 'x';
		*p++ = '0';
	}
	// Reverse it
	*p = 0;
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}

void IntToStr( int num)
{

bool bNegative =false;

if( num < 0)

bNegative =true;

char * str[11];

int i =0;

while( num != 0) {

str[i++] = num % 10 + ’0';

num /=10;

}

i–; int j =0;

while( j

char ch = str[j];

str[j] = str[i];

str[i] = ch;

j++; i–;

}

}

char* itoa(int x) {
    int i = x < 0 ? 3 : 2;
    int t = abs(x);
    while (t = t / 10) ++i;
    char* s = new char[i];
    s[--i] = '\0';
    t = abs(x);
    while (i) s[--i] = '0' + (t % 10), t = t / 10;
    if (x < 0) s[0] = '-';
    return s;
}

#define INT_DIGITS 19		// enough for 64 bit integer

char *itoa(i)
     int i;
{
  // Room for INT_DIGITS digits, - and '\0'
  static char buf[INT_DIGITS + 2];
  char *p = buf + INT_DIGITS + 1;	// points to terminating '\0'
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
    return p;
  }
  else {			// i < 0
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }
  return p;
}
