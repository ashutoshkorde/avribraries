/* Optimized AVR305 half-duplex serial uart implementation
 * timing for 81N, 115.2kbps @8Mhz = 69.4 cycles/bit
 * and @16Mhz = 138.9 cycles/bit
 * @author: Ralph Doncaster
 * @version: $Id$
	http://nerdralph.blogspot.com/2013/12/writing-avr-assembler-code-with-arduino.html
 */

#include <avr/io.h>

; correct for avr/io.h 0x20 port offset for io instructions
#ifdef __AVR_ATtiny85__
#define UART_PORT_OFFSET (PORTB - 0x20)
#define UART_TX_PIN 2
#else
#define UART_PORT_OFFSET (PORTD - 0x20)
#define UART_TX_PIN 1
#endif

#define bitcnt r18
#define delayArg r19

.global TxTimedByte
; transmit byte in r24 with bit delay in r22 - 15 instructions
; calling code must set Tx line to idle state (high) or 1st byte may be lost
; i.e. PORTB |= (1<<UART_Tx)
; each loop takes 10 cycles on a standard AVR
TxTimedByte:
	cli
        sbi UART_PORT_OFFSET-1, UART_TX_PIN  	            ; set Tx line to output
        ldi bitcnt, 10                              ; 1 start + 8 bit + 1 stop
        com r24	                                    ; invert and set carry
TxLoop:
        ; 9 cycle loop + delay
        brcc tx1
        cbi UART_PORT_OFFSET, UART_TX_PIN                  ; transmit a 0
tx1:
        brcs TxDone
        sbi UART_PORT_OFFSET, UART_TX_PIN                  ; transmit a 1
TxDone:
	mov delayArg, r22
TxDelay:
; delay (3 cycle * delayArg) -1
        dec delayArg
        brne TxDelay
        lsr r24
        dec bitcnt
        brne TxLoop
	reti				; return and enable interrupts

