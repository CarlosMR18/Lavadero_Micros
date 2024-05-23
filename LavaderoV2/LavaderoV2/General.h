
//	Librerias de trabajo
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

//	Construccion de libreria
#ifndef GENERAL_H_
#define GENERAL_H_

// Definicion de macros de trabajo 

#define setBit(Registro,Bit)	(Registro |= (1 << Bit))	// pone a 1 el bit B del puerto P
#define clearBit(Registro,Bit)	(Registro &= ~(1 << Bit))	// pone a 0 el bit B del puerto P
#define toggleBit(Registro,Bit) (Registro ^= (1 << Bit))  	// cambia el valor del bit B del puerto P

#define isBitSet(Registro, Bit) (Registro & (1 << Bit))		// devuelve '1' si dicho bit es '1'
#define isClrSet(Registro, Bit) (!isBitSet(Registro, Bit))	// devuelve '1' si dicho bit es '0'

// Redefinicion de constantes

// Caracteristicas del microprocesador
#define Freq_uC 8000000

// MACROS PARTE 1
#define CICLO_TRABAJO_SECADO 0.7
#define Check_height_sensors 30 // MS en comprobar estado 

// MACROS PARTE 2
#define Check_apertura_barrera 1600


// MACROS PARTE 3
#define CICLO_TRABAJO_CINTA 0.4
#define Tiempo_prove_new 56 // Tiempo comprobación entrada nueva en s(dos sensores distintos)


		// MOTORES
		
		// Motor M1 => Barrera de entrada
		// Direction
		#define REG_M1_di_PORT	PORTK
		#define REG_M1_di_DDR	DDRK
		#define REG_M1_di_PIN	PINK
		
		#define PIN_M1_di_PORT	PK0
		#define PIN_M1_di_DDR  	DDK0
		#define PIN_M1_di_PIN  	PINK0
		// Enable
		#define REG_M1_en_PORT	PORTL
		#define REG_M1_en_DDR	DDRL
		#define REG_M1_en_PIN	PINL
		
		#define PIN_M1_en_PORT	PL2
		#define PIN_M1_en_DDR	DDL2
		#define PIN_M1_en_PIN	PINL2
		
		// Motor M2 => Rodillos verticales
		// Direction
		#define REG_M2_di_PORT	PORTK
		#define REG_M2_di_DDR	DDRK
		#define REG_M2_di_PIN	PINK
		
		#define PIN_M2_di_PORT	PK4
		#define PIN_M2_di_DDR  	DDK4
		#define PIN_M2_di_PIN  	PINK4
		// Enable
		#define REG_M2_en_PORT	PORTK
		#define REG_M2_en_DDR	DDRK
		#define REG_M2_en_PIN	PINK
		
		#define PIN_M2_en_PORT	PK6
		#define PIN_M2_en_DDR	DDK6
		#define PIN_M2_en_PIN	PINK6
		
		// Motor M3 => Rodillo horizontal altura
		// Direction => PCINT1
		#define REG_M3_di_PORT	PORTL
		#define REG_M3_di_DDR	DDRL
		#define REG_M3_di_PIN	PINL

		#define PIN_M3_di_PORT	PL4
		#define PIN_M3_di_DDR	DDL4
		#define PIN_M3_di_PIN	PINL4
		// Enable
		#define REG_M3_en_PORT	PORTD
		#define REG_M3_en_DDR	DDRD
		#define REG_M3_en_PIN	PIND
		
		#define PIN_M3_en_PORT	PD4
		#define PIN_M3_en_DDR	DDD4
		#define PIN_M3_en_PIN	PIND4
		
		// Motor M4 => Rodillo horizontal giro
		// Direction
		#define REG_M4_di_PORT	PORTL
		#define REG_M4_di_DDR	DDRL
		#define REG_M4_di_PIN	PINL
		
		#define PIN_M4_di_PORT	PL6
		#define PIN_M4_di_DDR  	DDL6
		#define PIN_M4_di_PIN  	PINL6
		// Enable
		#define REG_M4_en_PORT	PORTD
		#define REG_M4_en_DDR	DDRD
		#define REG_M4_en_PIN	PIND
		
		#define PIN_M4_en_PORT PD6
		#define PIN_M4_en_DDR  DDD6
		#define PIN_M4_en_PIN  PIND6
		
		// Motor M5 => Ventilador de secado
		// Direction
		#define REG_M5_di_PORT	PORTD
		#define REG_M5_di_DDR	DDRD
		#define REG_M5_di_PIN	PIND
		
		#define PIN_M5_di_PORT	PD7
		#define PIN_M5_di_DDR	DDD7
		#define PIN_M5_di_PIN	PIND7
		
		// Enable		
		#define REG_M5_en_PORT	PORTB
		#define REG_M5_en_DDR	DDRB
		#define REG_M5_en_PIN	PINB
		
		#define PIN_M5_en_PORT	PB6
		#define PIN_M5_en_DDR  	DDB6
		#define PIN_M5_en_PIN  	PINB6
		
		// Motor M6 => Cinta de arrastre
		// Direction => PCINT5
		#define REG_M6_di_PORT	PORTL
		#define REG_M6_di_DDR	DDRL
		#define REG_M6_di_PIN	PINL
		
		#define PIN_M6_di_PORT	PL0
		#define PIN_M6_di_DDR  	DDL0
		#define PIN_M6_di_PIN  	PINL0
		// Enable
		#define REG_M6_en_PORT	PORTB
		#define REG_M6_en_DDR	DDRB
		#define REG_M6_en_PIN	PINB
		
		#define PIN_M6_en_PORT	PB7
		#define PIN_M6_en_DDR	DDB7
		#define PIN_M6_en_PIN	PINB7
		
		// SWITCHES
		#define REG_SW_PORT PORTD
		#define REG_SW_DDR  DDRD
		#define REG_SW_PIN  PIND
		// SW1 => INT3
		#define PIN_SW1_PORT PD3
		#define PIN_SW1_DDR  DDD3
		#define PIN_SW1_PIN  PIND3
		// SW2 => INT1
		#define PIN_SW2_PORT PD1
		#define PIN_SW2_DDR  DDD1
		#define PIN_SW2_PIN  PIND1
		// SW3 => INT2
		#define PIN_SW3_PORT PD2
		#define PIN_SW3_DDR  DDD2
		#define PIN_SW3_PIN  PIND2
		// SW4 => INT0
		#define PIN_SW4_PORT PD0
		#define PIN_SW4_DDR  DDD0
		#define PIN_SW4_PIN  PIND0
		
		// LED
		#define REG_LED_PORT	PORTL
		#define REG_LED_DDR 	DDRL
		#define REG_LED_PIN 	PINL
		// L1
		#define PIN_L1_PORT PL1
		#define PIN_L1_DDR  DDL1
		#define PIN_L1_PIN  PINL1
		// L4
		#define PIN_L4_PORT PL3
		#define PIN_L4_DDR  DDL3
		#define PIN_L4_PIN  PINL3
		// L5
		#define PIN_L5_PORT PL5
		#define PIN_L5_DDR  DDL5
		#define PIN_L5_PIN  PINL5
		// L6
		#define PIN_L6_PORT PL7
		#define PIN_L6_DDR  DDL7
		#define PIN_L6_PIN  PINL7
		
		// SENSORES
		// REGISTROS S01-S03-S06-S07-S010-S12 B
		#define REG_SOB_PORT PORTB
		#define REG_SOB_DDR  DDRB
		#define REG_SOB_PIN  PINB
		// REGISTROS S011 D
		#define REG_SOD_PORT PORTD
		#define REG_SOD_DDR  DDRD
		#define REG_SOD_PIN  PIND
		// REGISTROS S02-S04-S05-S08-S09 K
		#define REG_SOK_PORT PORTK
		#define REG_SOK_DDR  DDRK
		#define REG_SOK_PIN  PINK
		// SO1 => PCINT0
		#define PIN_SO1_PORT PB0
		#define PIN_SO1_DDR  DDB0
		#define PIN_SO1_PIN  PINB0
		// SO2 => PCINT18
		#define PIN_SO2_PORT PK2
		#define PIN_SO2_DDR  DDK2
		#define PIN_SO2_PIN  PINK2
		// SO3 => PCINT1
		#define PIN_SO3_PORT	PB1
		#define PIN_SO3_DDR  	DDB1
		#define PIN_SO3_PIN  	PINB1
		// SO4 => PCINT21
		#define PIN_SO4_PORT	PK5
		#define PIN_SO4_DDR  	DDK5
		#define PIN_SO4_PIN  	PINK5
		// SO5 => PCINT17
		#define PIN_SO5_PORT	PK1
		#define PIN_SO5_DDR  	DDK1
		#define PIN_SO5_PIN  	PINK1
		// SO6 => PCINT4
		#define PIN_SO6_PORT PB4
		#define PIN_SO6_DDR  DDB4
		#define PIN_SO6_PIN  PINB4
		// SO7 => PCINT3
		#define PIN_SO7_PORT PB3
		#define PIN_SO7_DDR  DDB3
		#define PIN_SO7_PIN  PINB3
		// SO8 => PCINT23
		#define PIN_SO8_PORT PK7
		#define PIN_SO8_DDR  DDK7
		#define PIN_SO8_PIN  PINK7
		// SO9 => PCINT19
		#define PIN_SO9_PORT PK3
		#define PIN_SO9_DDR  DDK3
		#define PIN_SO9_PIN  PINK3
		// SO10
		#define PIN_SO10_PORT PB5
		#define PIN_SO10_DDR  DDB5
		#define PIN_SO10_PIN  PINB5
		// SO11
		#define PIN_SO11_PORT PD5
		#define PIN_SO11_DDR  DDD5
		#define PIN_SO11_PIN  PIND5
		// SO12 => PCINT2
		#define PIN_SO12_PORT PB2
		#define PIN_SO12_DDR  DDB2
		#define PIN_SO12_PIN  PINB2
		
		


// FUNCIONES COMUNES
	// Funciones de Stop
	void setStop(void);
	uint8_t getStop(void);
	
	// Funciones temporales
	void setupTimers(void);
	void setupVariablesGeneral(void);
	void delay_milliseconds(uint32_t);
	void delay_seconds(uint32_t);	
	uint32_t millis(void);
	uint32_t seconds(void);
	
	// NumCarLavado
	uint8_t getNumCarLavado(void);
	void incNumCarLavado(void);
	void decNumCarLavado(void);
	// NumCarSecado
	uint8_t getNumCarSecado(void);
	void incNumCarSecado(void);
	void decNumCarSecado(void);

	// Inicialización Común
	void setup_General(void);

#endif /* GENERAL_H_ */