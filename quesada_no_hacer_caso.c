#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>


// Definicion de macros de trabajo
#define setBit(Registro,Bit)	(Registro |= (1 << Bit))	// pone a 1 el bit B del puerto P
#define clearBit(Registro,Bit)	(Registro &= ~(1 << Bit))	// pone a 0 el bit B del puerto P
#define toggleBit(Registro,Bit) (Registro ^= (1 << Bit))  	// cambia el valor del bit B del puerto P
#define isBitSet(Registro, Bit) (Registro & (1 << Bit))		// devuelve '1' si dicho bit es '1'
#define isClrSet(Registro, Bit) (!isBitSet(Registro, Bit))	// devuelve '1' si dicho bit es '0'

// Caracteristicas del microprocesador
#define Freq_uC 8000000
#define Check_height_sensors 50 // MS en comprobar estado (Regular en maqueta)
#define Tiempo_prove_new 1200 // Tiempo comprobación entrada nueva en ms(dos sensores distintos)

//////////////
#define REG_M3_di_PORT	PORTB
#define REG_M3_di_DDR	DDRB
#define REG_M3_di_PIN	PINB

#define PIN_M3_di_PORT	PL4
#define PIN_M3_di_DDR	DDL4
#define PIN_M3_di_PIN	PINL4
//////////////
#define REG_M3_en_PORT	PORTB
#define REG_M3_en_DDR	DDRB
#define REG_M3_en_PIN	PINB

#define PIN_M3_en_PORT	PD4
#define PIN_M3_en_DDR	DDD4
#define PIN_M3_en_PIN	PIND4
/////////////
#define REG_M4_en_PORT	PORTK
#define REG_M4_en_DDR	DDRK
#define REG_M4_en_PIN	PINK

#define PIN_M4_en_PORT PD6
#define PIN_M4_en_DDR  DDD6
#define PIN_M4_en_PIN  PIND6
////////////
#define REG_SW_PORT PORTD
#define REG_SW_DDR  DDRD
#define REG_SW_PIN  PIND
////////////
#define PIN_SW2_PORT PD1
#define PIN_SW2_DDR  DDD1
#define PIN_SW2_PIN  PIND1
////////////
// REGISTROS S01-S05-S06-S12 B
#define REG_SOB_PORT PORTB
#define REG_SOB_DDR  DDRB
#define REG_SOB_PIN  PINB
// REGISTROS S02-S03-S04-S010 L
#define REG_SOL_PORT PORTL
#define REG_SOL_DDR  DDRL
#define REG_SOL_PIN  PINL
/////////////
#define PIN_SO3_PORT	PB1
#define PIN_SO3_DDR  	DDB1
#define PIN_SO3_PIN  	PINB1
///////////
#define PIN_SO4_PORT	PK5
#define PIN_SO4_DDR  	DDK5
#define PIN_SO4_PIN  	PINK5
////////////
#define PIN_SO5_PORT	PK1
#define PIN_SO5_DDR  	DDK1
#define PIN_SO5_PIN  	PINK1
///////////

volatile uint32_t ms = 0;
// Variables globales - Lavadero Horizontal
volatile uint8_t lav_H[3] = {0,0,0}; // lav_H[0] para SO3, lav_H[1] para SO4, lav_H[2] para SO5
volatile uint8_t prev_lav_H[3] = {0,0,0};
volatile uint8_t aux_lavH = 0;
volatile uint8_t limit_switch_lavH = 0; //'1' SW2 detecta rodillo abajo

void setupTimers(void){
	cli();
	// TIMER 1 => Timer segundos : Modo CTC (ICRn) con preescalado 256
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001101;
	TIMSK1 = 0b00000010;
	OCR1A =  Freq_uC/256;
	// TIMER 3 => Timer milisegundos :  Modo CTC (ICRn) sin preescalado
	TCCR3A = 0b00000000;
	TCCR3B = 0b00001001;
	TIMSK3 = 0b00000010;
	OCR3A =	Freq_uC/1000;
	sei();
}

ISR(TIMER3_COMPA_vect){ // Milisegundos
	ms++;
	if(ms % Check_height_sensors == 0){ //#define macro en General.h
		//prev_lav_H = lav_H;
		//memcpy(prev_lav_H, lav_H, sizeof(lav_H));
		for (int i = 0; i < 3; i++) {
			prev_lav_H[i] = lav_H[i];
		}


		lav_H[0] = isBitSet(REG_SOL_PIN,PIN_SO3_PIN);
		lav_H[1] = isBitSet(REG_SOL_PIN,PIN_SO4_PIN);
		lav_H[2] = isBitSet(REG_SOB_PIN,PIN_SO5_PIN);
		limit_switch_lavH = isClrSet(REG_SW_PIN,PIN_SW2_PIN); // isClrSet porque SW2 '0' al detectar
		
		
		if(prev_lav_H == lav_H){ // Filtrado rebotes
			aux_lavH = 1;
		}
		
	}
}

	void setup_LavHorizontal(){
		// Motor 3: Altura rodillo H
		setBit(REG_M3_en_DDR,PIN_M3_en_DDR); // Definir como salida
		setBit(REG_M3_di_DDR,PIN_M3_di_DDR);
		setBit(REG_M3_en_PORT,PIN_M3_en_PORT); // Subir rodillo
		setBit(REG_M3_di_PORT,PIN_M3_di_PORT);
		// Motor 4: Giro rodillo H
		setBit(REG_M4_en_DDR,PIN_M4_en_DDR); // Definir como salida
		//setBit(REG_M4_di_DDR,PIN_M4_di_DDR);
		clearBit(REG_M4_en_PORT,PIN_M4_en_PORT); // Apagado de inicio
		//setBit(REG_M4_di_PORT,PIN_M4_di_PORT); // Sentido giro -- COMPROBAR EN MAQUETA
	}

	// Lavadero Horizontal - Altura

	void up_LavHorizontal(){
		setBit(REG_M3_en_PORT,PIN_M3_en_PORT);
		setBit(REG_M3_di_PORT,PIN_M3_di_PORT); // COMPROBAR EN MAQUETA si es setBit() o clearBit()
	}

	void down_LavHorizontal(){
		setBit(REG_M3_en_PORT,PIN_M3_en_PORT);
		clearBit(REG_M3_di_PORT,PIN_M3_di_PORT); // COMPROBAR EN MAQUETA si es ~[setBit() o clearBit()]
	}

	void stop_AlturaH(){
		clearBit(REG_M3_en_PORT,PIN_M3_en_PORT);
	}

	// Lavadero Horizontal - Giro

	void on_LavHorizontal(){
		setBit(REG_M4_en_PORT,PIN_M4_en_PORT);
	}

	void off_LavHorizontal(){
		clearBit(REG_M4_en_PORT,PIN_M4_en_PORT);
	}

	void lavaderoHorizontal(){
		if (limit_switch_lavH == 1 && isBitSet(REG_M3_en_PORT,PIN_M3_en_PORT)){  // devuelve '1' si detecta fin de carrera Y si el motor esta encendido
			clearBit(REG_M4_en_PORT,PIN_M4_en_PORT); //deja de girar el rodillo
			toggleBit(REG_M3_di_PORT,PIN_M3_di_PORT); // cambia el sentido del motor
			if (isBitSet(REG_M3_di_PORT,PIN_M3_di_PORT)){ //el rodillo esta abajo
				up_LavHorizontal(); //vuelvo a la posicion inicial(arriba)
				}else{ //el rodillo esta arriba
				stop_AlturaH(); //me quedo en la posicion inicial(arriba)
			}
		}

		if(aux_lavH){
			if (lav_H[1]==0 && (lav_H[0]==1 || lav_H[2]==1)){
				stop_AlturaH();
				} else if(lav_H[0]==0 && lav_H[1]==0){ //para direccion contraria: (lav_H[0]==0 || lav_H[2]==0) && lav_H[1]==0 
				up_LavHorizontal();
				} else {
				down_LavHorizontal();
			}
		}
	}
	
volatile uint8_t Stop = 0;
void setStop(void){
	Stop = 1;
}

uint8_t getStop(void){
	return Stop;
}

int main(void){
	setupTimers();
	setup_LavHorizontal();
	
	while(1) {
		if(!getStop()){
			lavaderoHorizontal();
		}
	}
	return 0;
}
	
//aplicar PWM al secado para el secado-->mas despacio
//mirar señales osciloscopio--> rebotes
	
