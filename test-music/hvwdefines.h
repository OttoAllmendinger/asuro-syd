// hvwdefines
// build 004, engines forward/reverse swapped
// build 005, explanation comment added

/*

  Question:
  Why is this long include file needed?
	
  Answer:
  The Asuro programming examples use something like this:
    #define FWD  (1 << PB5)

  If I like to create a new program or modify any of my own existing programs
  if find it difficult to understand what all of this ment to be. So I need
  to re-read the Atmega8 pdf documentation

  I develooped my own way of setting, clearing and reading bits from various
  registers in such a way that I easily could understand my own programs at a
  later time when I all forgot about my program.
	
  -----o-----

  I use a lot of 'defines'. For example:
	
    #define	PB5_L		(0x20<<8)

  this converts PB5_L (a bit that must be cleared) into hex value 0x2000 (shifts it to the high byte part)

    #define	PB5_H		(0x20)

  this converts PB5_H (a bit that must be set    ) into hex value 0x0020 (low byte part)

  -----o-----

  Now my 'defined' SFRX function (defined near the end of this file) may be written like

    SFRX ( PORTB , PB4_L | PB5_H | PB1_L ) ;

  This function will set bit 4 and bit 1 to LOW and bit 5 to HIGH in port PORTB.

  This function is executed by the preprocessor and therefore will NOT create more 
  microcontroller code as when using the ASURO way of programming.

  -----o-----

  Port PB5 controls the right Asuro engine, when LOW it enables the right engine to
  move reverse and PB4 must be HIGH.

  #define PB5_L  (0x20<<8)         setting PortB bit5 low  is converted to hex 0x2000 (high byte part of 16-bit integer) (PORTB)
  #define	PB5_H  (0x20)            setting PortB bit5 high is converted to hex 0x0020 (low  byte part of 16-bit integer) (PORTB)

  #define PB5_I  PB5_L             setting a bit as (I)nput  equals setting that bit (in DDRB, not PORTB) to (L)ow
  #define PB5_O  PB5_H             setting a bit as (O)utput equals setting that bit (in DDRB, not PORTB) to (H)igh

  #define REV_RGT_L PB5_L            activate reversing right engine (remember that   active = LOW  in case of engine control) (PORTB)
  #define REV_RGT_H	PB5_H          inactivate reversing right engine (remember that inactive = HIGH in case of engine control) (PORTB)

  #define REV_RGT_I	PB5_L          setting right engine microcontroller pin as (I)nput  (DDRB)
  #define REV_RGT_O	PB5_H          setting right engine microcontroller pin as (O)utput (DDRB)

  #define REV_RGT_N	PB5_L          deselection (N)one/(N)eutral pull-up resistor at right engine microcontroller pin (PORTB)
  #define REV_RGT_P	PB5_H            selection (P)ull-up        pull-up resistor at right engine microcontroller pin (PORTB)

  if you like to use PORT bitnames you may use PB5_n bitnaming and if you like to use 
  port-related Asuro-names you may use REV_RGT_n bit naming. Both do the same.

  -----o-----

  If I want to switch left engine to (output and) forward I may program: (bit 5 must be low and bit 4 must be high)
	
    SFRX ( DDRB  , PB5_O | PB4_O ) ;          //switch ports pb4 and pb5 as outputs
    SFRX ( PORTB , PB5_L | PB4_H ) ;          //switch port pb5 low and pb4 high
	
  or
	
    SFRX ( DDRB  , REV_RGT_O | FWD_RGT_O ) ;  //swithc ports pb4 and pb5 as outputs
    SFRX ( PORTB , REV_RGT_L | FWD_RGT_H ) ;  //switch port pb5 low and pb4 high
	
  the bit position in the SFRX function is irrelevant, see below.

    SFRX ( PORTB , PB5_L | PB4_H ) ;          //switch port pb5 low and pb4 high
    SFRX ( PORTB , PB4_H | PB5_L ) ;          //switch port pb5 low and pb4 high

  -----o-----

  This way of programming is not watertide (I know) and may be confusing,

  for example:
	
    SFRX ( DDRB , REV_RGT_L ) ;
	
  will set Port PB5 to INPUT and not to LOW because REV_RGT_L is ment to be used
  with PORTB and not with DDRB!
  No compiler warning or whatsoever is given.

  -----o-----

*/

#ifndef _HVW_DEFINES_H_
#define _HVW_DEFINES_H_ 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>

#define  FALSE	0
#define  TRUE	1

/*
_BV(b) equals (1<<b), meaning, tranfer bitnumber to byte-with-bit-in-specified-byte-bit--position
Each byte is then transferred to the high part of an integer if it is a SET bit.
Each byte is then transferred to the low  part of an integer if it is a CLR bit.
Example:
	PUD			= 2
	_BV(PUD)		= 0x04
	PUD_L			= 0x0004	to be cleared
	PUD_H			= 0x0400	to be set
	The inline function will find the set-bits at NN position as 0xNN00
	The inline function will find the clr-bits at NN position as 0x00NN
*/	

#define RESERVED  0x00


//MCUCR
#define	SE_L      (0x80<<8)
#define	SE_H      (0x80)
#define	SM2_L     (0x40<<8)
#define	SM2_H     (0x40)
#define	SM1_L     (0x20<<8)
#define	SM1_H     (0x20)
#define	SM0_L     (0x10<<8)
#define	SM0_H     (0x10)
#define	ISC11_L   (0x08<<8)
#define	ISC11_H   (0x08)
#define	ISC10_L   (0x04<<8)
#define	ISC10_H   (0x04)
#define	ISC01_L   (0x02<<8)
#define	ISC01_H   (0x02)
#define	ISC00_L   (0x01<<8)
#define	ISC00_H   (0x01)

//GICR
#define	INT1_L		(0x80<<8)
#define	INT1_H		(0x80)
#define	INT0_L		(0x40<<8)
#define	INT0_H		(0x40)
//GIFR
#define	INTF1_L		(0x80<<8)
#define	INTF1_H		(0x80)
#define	INTF0_L		(0x40<<8)
#define	INTF0_H		(0x40)

//TCCR2, 0x25
#define	FOC2_L		(0x80<<8)
#define	FOC2_H		(0x80)
#define	WGM20_L		(0x40<<8)
#define	WGM20_H		(0x40)
#define	COM21_L		(0x20<<8)
#define	COM21_H		(0x20)
#define	COM20_L		(0x10<<8)
#define	COM20_H		(0x10)
#define	WGM21_L		(0x08<<8)
#define	WGM21_H		(0x08)
#define	CS22_L		(0x04<<8)
#define	CS22_H		(0x04)
#define	CS21_L		(0x02<<8)
#define	CS21_H		(0x02)
#define	CS20_L		(0x01<<8)
#define	CS20_H		(0x01)
//TIMSK
#define	OCIE2_L		(0x80<<8)
#define	OCIE2_H		(0x80)
#define	TOIE2_L		(0x40<<8)
#define	TOIE2_H		(0x40)
#define	TICIE1_L	(0x20<<8)
#define	TICIE1_H	(0x20)
#define	OCIE1A_L	(0x10<<8)
#define	OCIE1A_H	(0x10)
#define	OCIE1B_L	(0x08<<8)
#define	OCIE1B_H	(0x08)
#define	TOIE1_L		(0x04<<8)
#define	TOIE1_H		(0x04)
														//not available
														//not available
#define	TOIE0_L		(0x01<<8)
#define	TOIE0_H		(0x01)
//SPMCR
#define	SPMIE_L		(0x80<<8)
#define	SPMIE_H		(0x80)
#define	RWWSB_L		(0x40<<8)
#define	RWWSB_H		(0x40)
#define	NONE5_L		(0x20<<8)
#define	NONE5_H		(0x20)
#define	RWWSRE_L	(0x10<<8)
#define	RWWSRE_H	(0x10)
#define	BLBSET_L	(0x08<<8)
#define	BLBSET_H	(0x08)
#define	PGWRT_L		(0x04<<8)
#define	PGWRT_H		(0x04)
#define	PGERS_L		(0x02<<8)
#define	PGERS_H		(0x02)
#define	SPMEM_L		(0x01<<8)
#define	SPMEM_H		(0x01)
//SFIOR
#define ACME_L		(0x08<<8)
#define ACME_H		(0x08)
#define PUD_L		(0x04<<8)
#define PUD_H		(0x04)
#define PSR2_L		(0x02<<8)
#define PSR2_H		(0x02)
#define PSR10_L		(0x01<<8)
#define PSR10_H		(0x01)
//SREG
#define GIE_L   (0x80<<8)
#define GIE_H   (0x80)
#define BCS_L   (0x40<<8)
#define BCS_H   (0x40)
#define HCF_L   (0x20<<8)
#define HCF_H   (0x20)
#define SB_L    (0x10<<8)
#define SB_H    (0x10)
#define TCOF_L  (0x08<<8)
#define TCOF_H  (0x08)
#define NF_L    (0x04<<8)
#define NF_H    (0x04)
#define ZF_L    (0x02<<8)
#define ZF_H    (0x02)
#define CF_L    (0x01<<8)
#define CF_H    (0x01)
//ACSR
#define ACD_L		(0x80<<8)
#define ACD_H		(0x80)
#define ACBG_L		(0x40<<8)
#define ACBG_H		(0x40)
#define ACO_L		(0x20<<8)
#define ACO_H		(0x20)
#define ACI_L		(0x10<<8)
#define ACI_H		(0x10)
#define ACIE_L		(0x08<<8)
#define ACIE_H		(0x08)
#define ACIC_L		(0x04<<8)
#define ACIC_H		(0x04)
#define ACIS1_L		(0x02<<8)
#define ACIS1_H		(0x02)
#define ACIS0_L		(0x01<<8)
#define ACIS0_H		(0x01)
//ADMUX
#define REFS1_L		(0x80<<8)
#define REFS1_H		(0x80)
#define REFS0_L		(0x40<<8)
#define REFS0_H		(0x40)
#define ADLAR_L		(0x20<<8)
#define ADLAR_H		(0x20)
#define NONE4_L		(0x10<<8)
#define NONE4_H		(0x10)
#define MUX3_L		(0x08<<8)
#define MUX3_H		(0x08)
#define MUX2_L		(0x04<<8)
#define MUX2_H		(0x04)
#define MUX1_L		(0x02<<8)
#define MUX1_H		(0x02)
#define MUX0_L		(0x01<<8)
#define MUX0_H		(0x01)
//ADCSRA
#define ADEN_L		(0x80<<8)
#define ADEN_H		(0x80)
#define ADSC_L		(0x40<<8)
#define ADSC_H		(0x40)
#define ADFR_L		(0x20<<8)
#define ADFR_H		(0x20)
#define ADIF_L		(0x10<<8)
#define ADIF_H		(0x10)
#define ADIE_L		(0x08<<8)
#define ADIE_H		(0x08)
#define ADPS2_L		(0x04<<8)
#define ADPS2_H		(0x04)
#define ADPS1_L		(0x02<<8)
#define ADPS1_H		(0x02)
#define ADPS0_L		(0x01<<8)
#define ADPS0_H		(0x01)
//TCCR0
#define	CS02_L		(0x04<<8)
#define	CS02_H		(0x04)
#define	CS01_L		(0x02<<8)
#define	CS01_H		(0x02)
#define	CS00_L		(0x01<<8)
#define	CS00_H		(0x01)
//TCCR1A
#define	COM1A1_L	(0x80<<8)
#define	COM1A1_H	(0x80)
#define	COM1A0_L	(0x40<<8)
#define	COM1A0_H	(0x40)
#define	COM1B1_L	(0x20<<8)
#define	COM1B1_H	(0x20)
#define	COM1B0_L	(0x10<<8)
#define	COM1B0_H	(0x10)
#define	FOC1A_L		(0x08<<8)
#define	FOC1A_H		(0x08)
#define	FOC1B_L		(0x04<<8)
#define	FOC1B_H		(0x04)
#define	WGM11_L		(0x02<<8)
#define	WGM11_H		(0x02)
#define	WGM10_L		(0x01<<8)
#define	WGM10_H		(0x01)
//TCCR1B
#define	ICNC1_L		(0x80<<8)
#define	ICNC1_H		(0x80)
#define	ICES1_L		(0x40<<8)
#define	ICES1_H		(0x40)
#define	RSRVD5_L	(0x20<<8)
#define	RSRVD5_H	(0x20)
#define	WGM13_L		(0x10<<8)
#define	WGM13_H		(0x10)
#define	WGM12_L		(0x08<<8)
#define	WGM12_H		(0x08)
#define	CS12_L		(0x04<<8)
#define	CS12_H		(0x04)
#define	CS11_L		(0x02<<8)
#define	CS11_H		(0x02)
#define	CS10_L		(0x01<<8)
#define	CS10_H		(0x01)
//TCCR2
#define	FOC2_L		(0x80<<8)
#define	FOC2_H		(0x80)
#define	WGM20_L		(0x40<<8)
#define	WGM20_H		(0x40)
#define	COM21_L		(0x20<<8)
#define	COM21_H		(0x20)
#define	COM20_L		(0x10<<8)
#define	COM20_H		(0x10)
#define	WGM21_L		(0x08<<8)
#define	WGM21_H		(0x08)
#define	CS22_L		(0x04<<8)
#define	CS22_H		(0x04)
#define	CS21_L		(0x02<<8)
#define	CS21_H		(0x02)
#define	CS20_L		(0x01<<8)
#define	CS20_H		(0x01)
//UCSRA
#define	RXC_L     	(0x80<<8)
#define	RXC_H     	(0x80)
#define	TXC_L     	(0x40<<8)
#define	TXC_H     	(0x40)
#define	UDRE_L    	(0x20<<8)
#define	UDRE_H    	(0x20)
#define	FE_L      	(0x10<<8)
#define	FE_H      	(0x10)
#define	DOR_L     	(0x08<<8)
#define	DOR_H     	(0x08)
#define	PE_L      	(0x04<<8)
#define	PE_H      	(0x04)
#define	U2X_L     	(0x02<<8)
#define	U2X_H     	(0x02)
#define	MPCM_L    	(0x01<<8)
#define	MPCM_H    	(0x01)
//UCSRB
#define RXCIE_L   	(0x80<<8)
#define RXCIE_H   	(0x80)
#define TXCIE_L   	(0x40<<8)
#define TXCIE_H   	(0x40)
#define UDRIE_L   	(0x20<<8)
#define UDRIE_H   	(0x20)
#define RXEN_L    	(0x10<<8)
#define RXEN_H    	(0x10)
#define TXEN_L    	(0x08<<8)
#define TXEN_H    	(0x08)
#define UCSZ2_L   	(0x04<<8)
#define UCSZ2_H   	(0x04)
#define RXB8_L    	(0x02<<8)
#define RXB8_H    	(0x02)
#define TXB8_L    	(0x01<<8)
#define TXB8_H    	(0x01)
//UCSRC
#define URSEL_L   	(0x80<<8)
#define URSEL_H   	(0x80)
#define UMSEL_L   	(0x40<<8)
#define UMSEL_H   	(0x40)
#define UPM1_L    	(0x20<<8)
#define UPM1_H    	(0x20)
#define UPM0_L    	(0x10<<8)
#define UPM0_H    	(0x10)
#define USBS_L    	(0x08<<8)
#define USBS_H    	(0x08)
#define UCSZ1_L   	(0x04<<8)
#define UCSZ1_H   	(0x04)
#define UCSZ0_L   	(0x02<<8)
#define UCSZ0_H   	(0x02)
#define UCPOL_L   	(0x01<<8)
#define UCPOL_H   	(0x01)
//UBRRH
//#define URSEL_L	(0x80<<8)  allready defined with UCSRC
//#define URSEL_H	(0x80)     allready defined with UCSRC
#define UBRR11_L  	(0x08<<8)
#define UBRR11_H  	(0x08)
#define UBRR10_L  	(0x04<<8)
#define UBRR10_H  	(0x04)
#define UBRR09_L  	(0x02<<8)
#define UBRR09_H  	(0x02)
#define UBRR08_L  	(0x01<<8)
#define UBRR08_H  	(0x01)
//UBRRL
#define UBRR07_L  	(0x80<<8)
#define UBRR07_H  	(0x80)
#define UBRR06_L  	(0x40<<8)
#define UBRR06_H  	(0x40)
#define UBRR05_L  	(0x20<<8)
#define UBRR05_H  	(0x20)
#define UBRR04_L  	(0x10<<8)
#define UBRR04_H  	(0x10)
#define UBRR03_L  	(0x08<<8)
#define UBRR03_H  	(0x08)
#define UBRR02_L  	(0x04<<8)
#define UBRR02_H  	(0x04)
#define UBRR01_L  	(0x02<<8)
#define UBRR01_H  	(0x02)
#define UBRR00_L  	(0x01<<8)
#define UBRR00_H  	(0x01)
//TIFR
#define OCF2_L    	(0x80<<8)
#define OCF2_H    	(0x80)
#define TOV2_L    	(0x40<<8)
#define TOV2_H    	(0x40)
#define ICF1_L    	(0x20<<8)
#define ICF1_H    	(0x20)
#define OCF1A_L   	(0x10<<8)
#define OCF1A_H   	(0x10)
#define OCF1B_L   	(0x08<<8)
#define OCF1B_H   	(0x08)
#define TOV1_L    	(0x04<<8)
#define TOV1_H    	(0x04)
#define TIFRB1_L  	(0x02<<8)
#define TIFRB1_H  	(0x02)
#define TOV0_L    	(0x01<<8)
#define TOV0_H    	(0x01)


#define DDRGB       DDRB
#define DDRGC       DDRC
#define DDRGD       DDRD

#define PINRB       PINB
#define PINRC       PINC
#define PINRD       PIND

//PORTB
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|  PB7  |  PB6  |  PB5  |  PB4  |  PB3  |  PB2  |  PB1  |  PB0  | Port name
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|   7   |   6   |   5   |   4   |   3   |    2  |    1  |   0   | bit number
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|  10   |   9   |  19   |  18   |  17   |   16  |   15  |  14   | pin number
//+-------+-------+-------+-------+-------+-------+-------+-------+
//| XTAL2 | XTAL1 |REV_RGT|FWD_RGT|IR_TXD1|SPD_RGT|SPD_LFT|SLD_GRN| signal name
//+-------+-------+-------+-------+-------+-------+-------+-------+
#define	PB7_L		(0x80<<8)
#define	PB7_H		(0x80)
#define	PB6_L		(0x40<<8)
#define	PB6_H		(0x40)
#define	PB5_L		(0x20<<8)
#define	PB5_H		(0x20)
#define	PB4_L		(0x10<<8)
#define	PB4_H		(0x10)
#define	PB3_L		(0x08<<8)
#define	PB3_H		(0x08)
#define	PB2_L		(0x04<<8)
#define	PB2_H		(0x04)
#define	PB1_L		(0x02<<8)
#define	PB1_H		(0x02)
#define	PB0_L		(0x01<<8)
#define	PB0_H		(0x01)

#define PB7_I     	PB7_L
#define PB7_O     	PB7_H
#define PB6_I     	PB6_L
#define PB6_O     	PB6_H
#define PB5_I     	PB5_L
#define PB5_O     	PB5_H
#define PB4_I     	PB4_L
#define PB4_O     	PB4_H
#define PB3_I     	PB3_L
#define PB3_O     	PB3_H
#define PB2_I     	PB2_L
#define PB2_O     	PB2_H
#define PB1_I     	PB1_L
#define PB1_O     	PB1_H
#define PB0_I     	PB0_L
#define PB0_O     	PB0_H

#define REV_RGT     PB5_H
#define FWD_RGT     PB4_H
#define IR_TXD1     PB3_H
#define IR1_TXD     PB3_H
#define SPD_RGT     PB2_H
#define SPD_LFT     PB1_H
#define SLD_GRN     PB0_H

#define REV_RGT_L	PB5_L	//the WHEEL (not the ENGINE) is turning reverse if REV signal is LOW
#define REV_RGT_H	PB5_H
#define FWD_RGT_L	PB4_L
#define FWD_RGT_H	PB4_H
#define IR_TXD1_L	PB3_L
#define IR_TXD1_H	PB3_H
#define IR1_TXD_L	PB3_L
#define IR1_TXD_H	PB3_H
#define SPD_RGT_L	PB2_L
#define SPD_RGT_H	PB2_H
#define SPD_LFT_L	PB1_L
#define SPD_LFT_H	PB1_H
#define SLD_GRN_L	PB0_L
#define SLD_GRN_H	PB0_H

#define REV_RGT_I	PB5_L
#define REV_RGT_O	PB5_H
#define FWD_RGT_I	PB4_L
#define FWD_RGT_O	PB4_H
#define IR_TXD1_I	PB3_L
#define IR_TXD1_O	PB3_H
#define IR1_TXD_I	PB3_L
#define IR1_TXD_O	PB3_H
#define SPD_RGT_I	PB2_L
#define SPD_RGT_O	PB2_H
#define SPD_LFT_I	PB1_L
#define SPD_LFT_O	PB1_H
#define SLD_GRN_I	PB0_L
#define SLD_GRN_O	PB0_H

#define REV_RGT_N	PB5_L
#define REV_RGT_P	PB5_H
#define FWD_RGT_N	PB4_L
#define FWD_RGT_P	PB4_H
#define IR_TXD1_N	PB3_L
#define IR_TXD1_P	PB3_H
#define IR1_TXD_N	PB3_L
#define IR1_TXD_P	PB3_H
#define SPD_RGT_N	PB2_L
#define SPD_RGT_P	PB2_H
#define SPD_LFT_N	PB1_L
#define SPD_LFT_P	PB1_H
#define SLD_GRN_N	PB0_L
#define SLD_GRN_P	PB0_H

#define	OC2_I		PB3_L
#define	OC2_O		PB3_H
#define	OC1B_L		PB2_L
#define	OC1B_H		PB2_H
#define	OC1A_L		PB1_L
#define	OC1A_H		PB1_H

//PORTC
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|  PC7  |  PC6  |  PC5  |  PC4  |  PC3  |  PC2  |  PC1  |  PC0  | port name
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|   7   |   6   |   5   |   4   |   3   |    2  |    1  |   0   | bit number
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|   -   |   1   |  28   |  27   |  26   |   25  |   24  |  23   | pin number
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|   -   | RESET |VPL_SNS|SWI_JNC|TRS_LFT|TRS_RGT|BOD_LFT|BOD_RGT| signal name (hvw)
//+-------+-------+-------+-------+-------+-------+-------+-------+
#define	PC7_L		(0x80<<8)
#define	PC7_H		(0x80)
#define	PC6_L		(0x40<<8)
#define	PC6_H		(0x40)
#define	PC5_L		(0x20<<8)
#define	PC5_H		(0x20)
#define	PC4_L		(0x10<<8)
#define	PC4_H		(0x10)
#define	PC3_L		(0x08<<8)
#define	PC3_H		(0x08)
#define	PC2_L		(0x04<<8)
#define	PC2_H		(0x04)
#define	PC1_L		(0x02<<8)
#define	PC1_H		(0x02)
#define	PC0_L		(0x01<<8)
#define	PC0_H		(0x01)

#define PC7_I		PC7_L
#define PC7_O		PC7_H
#define PC6_I		PC6_L
#define PC6_O		PC6_H
#define PC5_I		PC5_L
#define PC5_O		PC5_H
#define PC4_I		PC4_L
#define PC4_O		PC4_H
#define PC3_I		PC3_L
#define PC3_O		PC3_H
#define PC2_I		PC2_L
#define PC2_O		PC2_H
#define PC1_I		PC1_L
#define PC1_O		PC1_H
#define PC0_I		PC0_L
#define PC0_O		PC0_H

#define VPL_SNS	PC5_H
#define SWI_JNC	PC4_H
#define TRS_LFT	PC3_H
#define TRS_RGT	PC2_H
#define BOD_LFT	PC1_H
#define BOD_RGT	PC0_H

#define VPL_SNS_L	PC5_L
#define VPL_SNS_H	PC5_H
#define SWI_JNC_L	PC4_L
#define SWI_JNC_H	PC4_H
#define TRS_LFT_L	PC3_L
#define TRS_LFT_H	PC3_H
#define TRS_RGT_L	PC2_L
#define TRS_RGT_H	PC2_H
#define BOD_LFT_L	PC1_L
#define BOD_LFT_H	PC1_H
#define BOD_RGT_L	PC0_L
#define BOD_RGT_H	PC0_H

#define VPL_SNS_I	PC5_L
#define VPL_SNS_O	PC5_H
#define SWI_JNC_I	PC4_L
#define SWI_JNC_O	PC4_H
#define TRS_LFT_I	PC3_L
#define TRS_LFT_O	PC3_H
#define TRS_RGT_I	PC2_L
#define TRS_RGT_O	PC2_H
#define BOD_LFT_I	PC1_L
#define BOD_LFT_O	PC1_H
#define BOD_RGT_I	PC0_L
#define BOD_RGT_O	PC0_H

#define VPL_SNS_N	PC5_L
#define VPL_SNS_P	PC5_H
#define SWI_JNC_N	PC4_L
#define SWI_JNC_P	PC4_H
#define TRS_LFT_N	PC3_L
#define TRS_LFT_P	PC3_H
#define TRS_RGT_N	PC2_L
#define TRS_RGT_P	PC2_H
#define BOD_LFT_N	PC1_L
#define BOD_LFT_P	PC1_H
#define BOD_RGT_N	PC0_L
#define BOD_RGT_P	PC0_H




/*
#define VPS_L		(0x20<<8)
#define VPS_H		(0x20)
#define SWS_L		(0x10<<8)
#define SWS_H		(0x10)
#define TSL_L		(0x08<<8)
#define TSL_H		(0x08)
#define TSR_L		(0x04<<8)
#define TSR_H		(0x04)
#define OML_L		(0x02<<8)		//low
#define OML_H		(0x02)			//high
#define OML_I		OML_L				//input
#define OML_O		OML_H				//output
#define OML_N		OML_L				//no pull_up
#define OML_P		OML_H				//pull_up
#define OMR_L		(0x01<<8)
#define OMR_H		(0x01)
#define OMR_I		OMR_L
#define OMR_O		OMR_H
#define OMR_N		OMR_L
#define OMR_P		OMR_H
*/
/*
//PORTD
#define OMD_L		(0x80<<8)			//odo meter driver
#define OMD_H		(0x80)
#define OMD_I		OMD_L
#define OMD_O		OMD_H
#define	TLD_L		(0x40<<8)			//tracking light driver
#define	TLD_H		(0x40)	
#define	TLD_I		TLD_L
#define	TLD_O		TLD_H
#define	TLD_N		TLD_L
#define	TLD_P		TLD_H
#define	SLR_L		(0x04<<8)			//system led red
#define	SLR_H		(0x04)
*/
//PORTD
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|  PD7  |  PD6  |  PD5  |  PD4  |  PD3  |  PD2  |  PD1  |  PD0  | port name
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|   7   |   6   |   5   |   4   |   3   |    2  |    1  |   0   | bit number
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|  13   |  12   |  11   |   6   |   5   |    4  |    3  |   2   | pin number
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|BOD_COM|TRK_LED|REV_LFT|FWD_LFT|SWITCH |SLD_RED|IR_TXD2|IR_RXD | signal name (hvw)
//+-------+-------+-------+-------+-------+-------+-------+-------+
#define	PD7_L		(0x80<<8)
#define	PD7_H		(0x80)
#define	PD6_L		(0x40<<8)
#define	PD6_H		(0x40)
#define	PD5_L		(0x20<<8)
#define	PD5_H		(0x20)
#define	PD4_L		(0x10<<8)
#define	PD4_H		(0x10)
#define	PD3_L		(0x08<<8)
#define	PD3_H		(0x08)
#define	PD2_L		(0x04<<8)
#define	PD2_H		(0x04)
#define	PD1_L		(0x02<<8)
#define	PD1_H		(0x02)
#define	PD0_L		(0x01<<8)
#define	PD0_H		(0x01)

#define PD7_I		PD7_L
#define PD7_O		PD7_H
#define PD6_I		PD6_L
#define PD6_O		PD6_H
#define PD5_I		PD5_L
#define PD5_O		PD5_H
#define PD4_I		PD4_L
#define PD4_O		PD4_H
#define PD3_I		PD3_L
#define PD3_O		PD3_H
#define PD2_I		PD2_L
#define PD2_O		PD2_H
#define PD1_I		PD1_L
#define PD1_O		PD1_H
#define PD0_I		PD0_L
#define PD0_O		PD0_H

#define BOD_COM   PD7_H
#define TRK_LED   PD6_H
#define REV_LFT   PD5_H
#define FWD_LFT   PD4_H
#define SWI_IAC   PD3_H
#define SLD_RED   PD2_H
#define IR_TXD2   PD1_H
#define IR2_TXD   PD1_H
#define IR_RXD    PD0_H

#define BOD_COM_L	PD7_L
#define BOD_COM_H	PD7_H
#define TRK_LED_L	PD6_L
#define TRK_LED_H	PD6_H
#define REV_LFT_L	PD5_L	//remember: signal is active when LOW
#define REV_LFT_H	PD5_H
#define FWD_LFT_L	PD4_L	//remember: signal is active when LOW
#define FWD_LFT_H	PD4_H
#define SWI_IAC_L	PD3_L
#define SWI_IAC_H	PD3_H
#define SLD_RED_L	PD2_L
#define SLD_RED_H	PD2_H
#define IR_TXD2_L	PD1_L
#define IR_TXD2_H	PD1_H
#define IR2_TXD_L	PD1_L
#define IR2_TXD_H	PD1_H
#define IR_RXD_L 	PD0_L
#define IR_RXD_H 	PD0_H
#define BOD_COM_I	PD7_L
#define BOD_COM_O	PD7_H
#define TRK_LED_I	PD6_L
#define TRK_LED_O	PD6_H
#define REV_LFT_I	PD5_L
#define REV_LFT_O	PD5_H
#define FWD_LFT_I	PD4_L
#define FWD_LFT_O	PD4_H
#define SWI_IAC_I	PD3_L
#define SWI_IAC_O	PD3_H
#define SLD_RED_I	PD2_L
#define SLD_RED_O	PD2_H
#define IR_TXD2_I	PD1_L
#define IR_TXD2_O	PD1_H
#define IR2_TXD_I	PD1_L
#define IR2_TXD_O	PD1_H
#define IR_RXD_I	PD0_L
#define IR_RXD_O	PD0_H

#define BOD_COM_N	PD7_L
#define BOD_COM_P	PD7_H
#define TRK_LED_N	PD6_L
#define TRK_LED_P	PD6_H
#define REV_LFT_N	PD5_L
#define REV_LFT_P	PD5_H
#define FWD_LFT_N	PD4_L
#define FWD_LFT_P	PD4_H
#define SWI_IAC_N	PD3_L
#define SWI_IAC_P	PD3_H
#define SLD_RED_N	PD2_L
#define SLD_RED_P	PD2_H
#define IR_TXD2_N	PD1_L
#define IR_TXD2_P	PD1_H
#define IR2_TXD_N	PD1_L
#define IR2_TXD_P	PD1_H
#define IR_RXD_N	PD0_L
#define IR_RXD_P	PD0_H

#define BIT_RATE_2400_BAUD     207
#define BIT_RATE_4800_BAUD     103
#define BIT_RATE_9600_BAUD     51
#define BIT_RATE_14400_BAUD    34
#define BIT_RATE_19200_BAUD    25
#define BIT_RATE_28800_BAUD    16
#define BIT_RATE_38400_BAUD    12
#define BIT_RATE_57600_BAUD    8
#define BIT_RATE_76800_BAUD    6
#define BIT_RATE_115200_BAUD   3
#define BIT_RATE_230400_BAUD   1
#define BIT_RATE_250000_BAUD   1
#define BIT_RATE_500000_BAUD   0

//PIND  Remember: reading normal pin status use PIND not PORTD
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|  PD7  |  PD6  |  PD5  |  PD4  |  PD3  |  PD2  |  PD1  |  PD0  | port name
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|   7   |   6   |   5   |   4   |   3   |    2  |    1  |   0   | bit number
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|  13   |  12   |  11   |   6   |   5   |    4  |    3  |   2   | pin number
//+-------+-------+-------+-------+-------+-------+-------+-------+
//|BOD_COM|TRK_LED|REV_LFT|FWD_LFT|SWITCH |SLD_RED|IR_TXD2|IR_RXD | signal name (hvw)
//+-------+-------+-------+-------+-------+-------+-------+-------+
#define PIND7_L		PD7_L
#define PIND7_H		PD7_H
#define PIND6_L		PD6_L
#define PIND6_H		PD6_H
#define PIND5_L		PD5_L
#define PIND5_H		PD5_H
#define PIND4_L		PD4_L
#define PIND4_H		PD4_H
#define PIND3_L		PD3_L
#define PIND3_H		PD3_H
#define PIND2_L		PD2_L
#define PIND2_H		PD2_H
#define PIND1_L		PD1_L
#define PIND1_H		PD1_H
#define PIND0_L		PD0_L
#define PIND0_H		PD0_H



#define SFR_LEFT_ENGINE_SPEED OCR1A
#define SFR_RGHT_ENGINE_SPEED OCR1B


//Set and Clear Special Function Register bits
#define SFRX(Sfr,Msk) (Sfr=((Sfr & (~ ((Msk)>>8)) ) |(Msk)))
// - a Mask (Msk) is 16 bits wide
// - the left  8 bits represents the bits to be cleared (each 1-bit means that corresponding registerbit must be cleared)
// - the right 8 bits represents the bits to be set     (each 1-bit means that corresponding registerbit must be set    )
// a Msk example:
// 7654 3210 7654 3210  (bitnumbers)
// 0010 0000 0000 0100
//   x                  bit 6 must be cleared
//                 x    bit 2 must be set
// p = previous result
// ((Msk)>>8)       => shift clear-bits from the high-byte to the low-byte (1-bits means that corresponding bit must be cleared)
// ~(p)             => invert clear-bits, meaning, bits that must be cleared are now '0' and must not be cleared are now '1' in low-byte
// Sfr&p            => get register data and 'and' it with 'p' so:
//                     1. reg 0 and clr 0 => reg 0 (a cleared bit that must be cleared stays cleared)
//                     2. reg 1 and clr 0 => reg 0 (a set bit that must be cleared is cleared)
//                     3. reg 0 and clr 1 => reg 0 (an cleared bit that must not be cleared stays cleared)
//                     4. reg 1 and clr 1 => reg 1 (an set bit that must not be cleared stays set)
// p|(Msk)          => to-be-set bits in low-byte are just prepaired to be set in register
// Sfr=p            => execute settings







/*

//Set and Clear Special Function Register bits
#define SFRX(Sfr,Msk) (Sfr=((Sfr & (~ ((Msk)>>8)) ) |(Msk)))
// - a Mask (Msk) is 16 bits wide
// - the left  8 bits represents the bits to be cleared (each 1-bit means that corresponding registerbit must be cleared)
// - the right 8 bits represents the bits to be set     (each 1-bit means that corresponding registerbit must be set    )
// a Msk example:
// 7654 3210 7654 3210  (bitnumbers)
// 0010 0000 0000 0100
//   x                  bit 6 must be cleared
//                 x    bit 2 must be set
// p = previous result

// ((Msk)>>8)       => shift clear-bits from the high-byte to the low-byte (1-bits means that corresponding bit must be cleared)

// ~(p)             => invert clear-bits, meaning, bits that must be cleared are now '0' and must not be cleared are now '1' in low-byte

// Sfr&p            => get register data and 'and' it with 'p' so:

//                     1. reg 0 and clr 0 => reg 0 (a reg '0' bit that must be cleared stays cleared)

//                     2. reg 1 and clr 0 => reg 0 (a reg '1' bit that must be cleared is cleared)

//                     3. reg 0 and clr 1 => reg 0 (a reg '0' bit that must not be cleared stays cleared)

//                     4. reg 1 and clr 1 => reg 1 (a reg '1' bit that must not be cleared stays set)

// p|(Msk)          => to-be-set bits in low-byte are just prepaired to be set in register

// Sfr=p            => execute settings



*/















//Write Special Function Register byte (arguments with _L will be ignored!)
//#define SFRW(Sfr,Msk) (Sfr=(Msk))

//Write Special Function Register byte, 'set' has priority over 'clr'
#define SFRW(Sfr,Msk) (Sfr=( ( ((Msk)^(Msk)) >>8 ) | ( (Msk) & 0xff ) ) )




//Test on mask bits 1 being set and mask bits 0 being cleared (do not test unmasked bits)
//	(Msk)>>8)				=> shift 'clr' mask 8 bits to the right
//	| ((Msk)&0xFF) 	=> 'Or' shifted 'clr' mask with 'set' mask: now all 'clr' and 'set' bits are set to '1'
//  & Sfr						=> Retrieve all 'clr' and 'set' bits from Sfr
//  ^((Msk&0xFF))		=> Get 'set' bitmask, force all 'set' masked '1' bits from Sfr to go '0', leaving errornous bits resulting as '1'
//  !               => invert (any '1' bit is error = boolean pass) to (any '1' bit is error = boolean fail)
#define SFRQ(Sfr,Msk)  (!((((((Msk)>>8)|((Msk)&0xFF)) ) & Sfr)^((Msk)&0xFF))) //werkt!	


//toggle bits in register
#define SFRT(Sfr,Msk) (Sfr=(Sfr^Msk))

//SFRS (pecial Function Register bit Set)  may only be used with _H arguments!
#define SFRS(Sfr,Msk) (Sfr&Msk)

//check for bit clear, use _H bits!!!
#define SFRC(Sfr,Msk) (!(Sfr&Msk))





//================================================================================


//#define SFRQ(Sfr,Msk)     (((Msk)^Sfr) | ((~((Msk)>>8))&Sfr))


#define	SET_TRACK_LED_OUTPUT    (SFRX(DDRD, TRK_LED_O))
#define	SET_TRACK_LED_ON        (SFRX(PORTD,TRK_LED_H))
#define	SET_TRACK_LED_OFF       (SFRX(PORTD,TRK_LED_L))
#define CHK_TRACK_LED_ON        (SFRS(PORTD,TRK_LED_H))

#define SET_LEFT_BRAKE_LED_ON   (SFRX(PORTC,BOD_LFT_H))
#define SET_LEFT_BRAKE_LED_OFF  (SFRX(PORTC,BOD_LFT_L))
#define SET_RGHT_BRAKE_LED_ON   (SFRX(PORTC,BOD_RGT_H))
#define SET_RGHT_BRAKE_LED_OFF  (SFRX(PORTC,BOD_RGT_L))

#define CHK_LEFT_BRAKE_LED_ON   ( SFRS(PORTC,BOD_LFT_H))
#define CHK_LEFT_BRAKE_LED_OFF  (!SFRS(PORTC,BOD_LFT_H))
#define CHK_RGHT_BRAKE_LED_ON   ( SFRS(PORTC,BOD_RGT_H))
#define CHK_RGHT_BRAKE_LED_OFF  (!SFRS(PORTC,BOD_RGT_H))

#define	SFR_TST(Sfr,Msk)        (Sfr&Msk)

#define	TST_TRACK_LED_ON        (SFRS(PORTD,PD6_H))


#define SET_RED_SYSTM_LED_ON		(SFRX(PORTD,SLD_RED_H))
#define SET_RED_SYSTM_LED_OFF		(SFRX(PORTD,SLD_RED_L))
#define SET_GRN_SYSTM_LED_ON		(SFRX(PORTB,SLD_GRN_H))
#define SET_GRN_SYSTM_LED_OFF		(SFRX(PORTB,SLD_GRN_L))

//Set and Clear Bits in Byte
#define BYTE_BIT(Byt,Bit)		(Byt=(Byt&~((Bit)>>8))|(Bit))


#define SET_BYTE_BITS(Byt,Bit)		(Byt=(Byt|Bit))
#define SET_BYTE_BIT(Byt,Bit)		(Byt=(Byt|Bit))
#define CLR_BYTE_BITS(Byt,Bit)		(Byt=(Byt&~(Bit)))
#define CLR_BYTE_BIT(Byt,Bit)		(Byt=(Byt&~(Bit)))
#define CHK_BYTE_BIT_SET(Byt,Bit)	(Byt&Bit)
#define CHK_BYTE_BIT_CLR(Byt,Bit)	(~(Byt&Bit))	// Byt&(Bit) selects bit, 


#define byte unsigned char


#define SFR_ADMUX_MUX_ADC_LEFT_TRACK_SENSOR (MUX2_H|MUX1_H)	//mux3
#define SFR_ADMUX_MUX_ADC_RGHT_TRACK_SENSOR (MUX1_H)				//mux2


#define SET_LEFT_ENGINE_DIRECTION_TO_FORWARD SFRX(PORTD,PD4_L|PD5_H)
#define SET_LEFT_ENGINE_DIRECTION_TO_REVERSE SFRX(PORTD,PD4_H|PD5_L)
#define SET_RGHT_ENGINE_DIRECTION_TO_FORWARD SFRX(PORTB,PB4_L|PB5_H)
#define SET_RGHT_ENGINE_DIRECTION_TO_REVERSE SFRX(PORTB,PB4_H|PB5_L)
#define SET_LEFT_ENGINE_SPEED(LftEngSpd) (OCR1A=LftEngSpd)
#define SET_RGHT_ENGINE_SPEED(RgtEngSpd) (OCR1B=RgtEngSpd)



#endif //_MY_DEFINES_H_
