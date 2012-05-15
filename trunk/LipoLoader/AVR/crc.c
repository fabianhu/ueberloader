#include <avr/io.h>

/// <summary>
/// CRC16S
///
/// The S stands for "Simple", because we use no lookup table here.
/// calculates the CRC16 Checksum of a given byte array
/// using the x^16+x^15+x^2+1 generator polynom (0xA001)
/// processing each byte without using a lookup table
/// </summary>
/// <param name="Data">a byte array of data</param>
/// <returns>2 byte CRC Checksum</returns>
uint16_t calcCRC16S ( uint8_t* c, uint8_t len )
{
	 uint16_t Polynom = 0xA001;
	 uint16_t Register = 0xFFFF;
	 uint16_t temp = 0x00;

  // loop through the entire array of bytes
  for ( int i = 0; i < len; i++ )
  {
    temp = c[i];

    // shift all 8 data bits once
    for ( int y = 0; y < 8; y++ )
    {
      if ( ( ( Register ^ temp ) & 0x01 ) == 0x01 )
      {
        Register >>= 1;
        Register ^= Polynom;
      }
      else
      {
        Register >>= 1;
      }
      temp >>= 1; // shift data 1 bit right, dividing it by 2

    } // end of inner for loop (bit shift loop)
  } // end of outer for loop (data loop)

  // now we have got our overall 2-byte CRC "Checksum" number
  return Register;
} // end of CRC16S Method


#define CRC16INIT 0xA001

uint16_t calcCRC16r(uint16_t crc, uint16_t c)
{ // CCITT 16 bit (X^16 + X^12 + X^5 + 1).
  crc  = (unsigned char)(crc >> 8) | (crc << 8);
  crc ^= c;
  crc ^= (unsigned char)(crc & 0xff) >> 4;
  crc ^= (crc << 8) << 4;
  crc ^= ((crc & 0xff) << 4) << 1;

  return (crc);
}

uint16_t calcCRC16(uint16_t* c, uint16_t len)
{
	uint16_t i;
	uint16_t crc=CRC16INIT;
	for(i=0;i<len;i++)
	{
		crc = calcCRC16r(crc,c[i]);
	}
	return crc;
}

uint8_t CRC8c(uint8_t input, uint8_t seed)
{
    uint8_t i, feedback;

    for (i=0; i<8; i++)
    {
        feedback = ((seed ^ input) & 0x01);
        if (!feedback) seed >>= 1;
        else
        {
            seed ^= 0x18;
            seed >>= 1;
            seed |= 0x80;
        }
        input >>= 1;
    }

    return seed;
}

uint8_t CRC8x(uint8_t *input,uint8_t len)
{
    uint8_t i, check;

    check=0;

    for (i=0; i<len; i++)
    {
        check = CRC8c(input[i], check);
    }

    return check;
}
