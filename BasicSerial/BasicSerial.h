/* Optimized AVR305 half-duplex serial uart implementation
 * timing for 81N, 115.2kbps @8Mhz = 69.4 cycles/bit
 * and @16Mhz = 138.9 cycles/bit
 * @author: Ralph Doncaster
 * @version: $Id$
 */

//#define BAUD_RATE 115200

#ifndef BAUD_RATE
	#error Define 'BAUD_RATE'
#endif

#ifdef F_CPU
  #define TXDELAY (((F_CPU/BAUD_RATE)-9)/3)
#else
  #error CPU frequency F_CPU undefined
#endif

//#define Serial.begin

#if TXDELAY > 255
  #error low baud rates unsupported - use higher BAUD_RATE
#endif

extern "C" {
void TxTimedByte(char, char);
void TxTimedStringLn(const char* str, char txDelay);
void TxTimedString(const char* str, char txDelay);
void TxTimedFloat(float f, char txDelay);
void TxTimedLong(long l, char txDelay);
}

#ifdef _LOG_
#define TxByte(C) TxTimedByte(C , TXDELAY)
#define TxLong(C) TxTimedLong(C , TXDELAY)
#define TxString(C) TxTimedString(C, TXDELAY)
#define TxStringLn(C) TxTimedStringLn(C, TXDELAY)
#define TxFloat(C) TxTimedFloat(C, TXDELAY)
#else
#define TxByte(C) 
#define TxLong(C)
#define TxString(C) 
#define TxStringLn(C) 
#define TxFloat(C)
#endif