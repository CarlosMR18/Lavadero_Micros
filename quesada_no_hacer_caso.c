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
#define REG_M3_di_PORT	PORTL
#define REG_M3_di_DDR	DDRL
#define REG_M3_di_PIN	PINL

#define PIN_M3_di_PORT	PL4
#define PIN_M3_di_DDR	DDL4
#define PIN_M3_di_PIN	PINL4
//////////////
#define REG_M3_en_PORT	PORTD
#define REG_M3_en_DDR	DDRD
#define REG_M3_en_PIN	PIND

#define PIN_M3_en_PORT	PD4
#define PIN_M3_en_DDR	DDD4
#define PIN_M3_en_PIN	PIND4
/////////////
#define REG_M4_en_PORT	PORTD
#define REG_M4_en_DDR	DDRD
#define REG_M4_en_PIN	PIND

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
#define REG_SOK_PORT PORTK
#define REG_SOK_DDR  DDRK
#define REG_SOK_PIN  PINK
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
volatile uint8_t limit_switch_lavH = 0; //'1' SW2 detecta rodillo en un extremo

void setupTimers(void){
	cli();
	// TIMER 4 => Timer segundos : Modo CTC (ICRn) con preescalado 256
	TCCR4A = 0b00000000;
	TCCR4B = 0b00001101;
	TIMSK4 = 0b00000010;
	OCR4A =  Freq_uC/256;
	// TIMER 3 => Timer milisegundos :  Modo CTC (ICRn) sin preescalado
	TCCR3A = 0b00000000; //WGM30 y WGM31 == 0
	TCCR3B = 0b00001101; //WGM32(bit3) y WGM33(bit2) == 1  CS30 == 1(no preescalado)(bit0)
	TIMSK3 = 0b00000010;
	OCR3A =	Freq_uC/1000;
	sei();
}

// Filtrado rebotes
ISR(TIMER3_COMPA_vect){ // Milisegundos
	ms++;
	if(ms % Check_height_sensors == 0){   //cada cuanto chequeo los sensores de altura //#define macro en General.h
		for (int i = 0; i < 3; i++) {	  
			prev_lav_H[i] = lav_H[i];     //guardo los valores del instante anterior
		}

		//cargo los valores de los sensores de ese instante
		lav_H[0] = isBitSet(REG_SOB_PIN,PIN_SO3_PIN);
		lav_H[1] = isBitSet(REG_SOK_PIN,PIN_SO4_PIN);
		lav_H[2] = isBitSet(REG_SOK_PIN,PIN_SO5_PIN);
		limit_switch_lavH = isClrSet(REG_SW_PIN,PIN_SW2_PIN); // isClrSet porque SW2 '0' al detectar
		
		
		if(prev_lav_H == lav_H){  //Si los valores son los mismos que en instante anterior
			aux_lavH = 1;		  //muevo el rodillo
			}else{
			aux_lavH = 0;		  //no hago nada
		}
		
	}
}

void setup_LavHorizontal(){
	// Motor 3: Altura rodillo H
	cli();
	setBit(REG_M3_en_DDR,PIN_M3_en_DDR); // Definir como salida
	setBit(REG_M3_di_DDR,PIN_M3_di_DDR);
	setBit(REG_M3_en_PORT,PIN_M3_en_PORT); // Subir rodillo
	clearBit(REG_M3_di_PORT,PIN_M3_di_PORT);
	// Motor 4: Giro rodillo 
	setBit(REG_M4_en_DDR,PIN_M4_en_DDR); // Definir como salida
	clearBit(REG_M4_en_PORT,PIN_M4_en_PORT); // Apagado de inicio
	sei();
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
	if (limit_switch_lavH == 1 && isBitSet(REG_M3_en_PORT,PIN_M3_en_PORT)) {  // devuelve '1' si detecta fin de carrera Y si el motor esta encendido
		off_LavHorizontal();							//deja de girar el rodillo
		toggleBit(REG_M3_di_PORT,PIN_M3_di_PORT);		// cambia el sentido del motor
		if (isBitSet(REG_M3_di_PORT,PIN_M3_di_PORT)){	//el rodillo esta abajo
			stop_AlturaH();			//me quedo en la posicion inicial(abajo) 
			}else{					//el rodillo esta arriba
			down_LavHorizontal();	//vuelvo a la posicion inicial(abajo)
		}
	}

	if(aux_lavH){		//si los valores de los sensores son los mismos que en instante anterior (antirrebotes)
		if (lav_H[1]==0 && (lav_H[0]==1 || lav_H[2]==1)){		//detecta abajo pero no a los lados
			stop_AlturaH();
		} else if(lav_H[0]==0 || lav_H[2]==0){ //detecta alguno de los lados 
			up_LavHorizontal();  //sube el rodillo
			on_LavHorizontal();  //empieza a girar el rodillo
		} else {												//no detecta nada 
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
