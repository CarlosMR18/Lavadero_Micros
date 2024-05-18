#include "General.h"

// Variables globales
volatile uint8_t Stop = 0; // '1' Bloque maqueta
volatile uint32_t s = 0; // Cuenta segundos
volatile uint32_t ms = 0; // Cuenta milisegundos
volatile uint8_t NumCarLavado = 0; // Cuenta coches en zona lavado
volatile uint8_t NumCarSecado = 0; // Cuenta coches en zona secado

volatile uint8_t EnableEntrance = 1; // '1' permite nuevas entradas de vehículos y '0' no permite
volatile uint8_t enable_prove_new = 0; // '1' pendiente comprobar entradas de vehículos 
volatile uint32_t cnt_prove_new = 0; // Cuenta tiempo para comprobar entradas de vehículos 

	// Variables globales - Lavadero Horizontal
volatile uint8_t so3 = 1, so4 = 1, so5 = 1;  //variables para ver el estado de los sensores
volatile uint8_t so3p = 1, so4p = 1, so5p = 1; //variables para ver el estado previo de los sensores
volatile uint8_t aux_lavH = 0;
volatile uint8_t limit_switch_lavH = 0; //'1' SW2 detecta rodillo abajo
	// Variables globales - Secado
volatile uint8_t secado[3] = {0,0,0}; // lav_H[0] para SO7, lav_H[1] para SO8, lav_H[2] para SO9
volatile uint8_t prev_secado[3] = {0,0,0};
volatile uint8_t aux_secado = 0;
volatile uint8_t limit_switch_secado = 0; //'1' SW3 detecta secador abajo

volatile uint8_t reg_SO1 = 1; // Almacena valor de SO1
volatile uint8_t reg_SO3 = 1; // Almacena valor de SO3
volatile uint8_t reg_SO6 = 1; // Almacena valor de SO6
volatile uint8_t reg_SO12 = 1; // Almacena valor de SO12

//////////////////////////////

// Funciones de Stop

void setStop(void){
	Stop = 1;
}
	
uint8_t getStop(void){
	return Stop;
}

//////////////////////////////

// Funciones temporales

void setupTimers(void){
	cli();
		// TIMER 1 => Timer segundos : Modo CTC (OCRnA) con preescalado 256
	TCCR1A = 0b00000000; //WGM10 y WGM11 == 0
	TCCR1B = 0b00001101; //WGM12(bit3) == 1  CS12 == 1(preescalado 256)(bit2)
	TIMSK1 = 0b00000010;
	OCR1A =  Freq_uC/256;
		// TIMER 3 => Timer milisegundos : Modo CTC (OCRnA) sin preescalado
	TCCR3A = 0b00000000;  //WGM30 y WGM31 == 0
	TCCR3B = 0b00001001;  //WGM32(bit3) == 1  CS30 == 1(no preescalado)(bit0)
	TIMSK3 = 0b00000010;
	OCR3A =	Freq_uC/1000;
	sei();
}

void delay_milliseconds(uint32_t dms){
	volatile uint32_t delay = ms + dms;
	while( ms < delay ){}
}
void delay_seconds(uint32_t ds){
	volatile uint32_t delay = s + ds;
	while( s < delay ){}
}

uint32_t millis(void){
	return ms;
}

uint32_t seconds(void){
	return s;
}

////////////////////////////

// Funciones globales

void setup_General(void){ //Incluir todas funciones inicialización
	setupTimers();
}

	// NumCarLavado
uint8_t getNumCarLavado(void){ // Devuelve número coches en lavadero
	return NumCarLavado;
}

void incNumCarLavado(void){ // Incrementa en uno el contador de coches en lavadero
	NumCarLavado++;
}

void decNumCarLavado(void){ // Decrementa en uno el contador de coches en lavadero
	if(NumCarLavado > 0) {
		NumCarLavado--;
	}
}
	
	// NumCarSecado
uint8_t getNumCarSecado(void){ // Devuelve número coches en lavadero
	return NumCarSecado;
}

void incNumCarSecado(void){ // Incrementa en uno el contador de coches en lavadero
	NumCarSecado++;
}

void decNumCarSecado(void){ // Decrementa en uno el contador de coches en lavadero
	if(NumCarSecado > 0) {
		NumCarSecado--;
	}
}

////////////////////////////

// Funciones de interrupción

ISR(TIMER1_COMPA_vect){ // Segundos
	s++;
	//Parte 3
	if ((s - cnt_prove_new) > Tiempo_prove_new && NumCarLavado > 0 && enable_prove_new == 1){ // ORIGINAL: enable_prove_new == 0 
		NumCarLavado--;
		EnableEntrance = 1;
		enable_prove_new = 0;
	}
}

ISR(TIMER3_COMPA_vect){ // Milisegundos
	ms++;
	//Parte 3
	if (cnt_prove_new > Tiempo_prove_new && NumCarLavado > 0 && enable_prove_new == 0){
		NumCarLavado--;
		EnableEntrance = 1;
		enable_prove_new = 0;
	}
	// Parte 1
	if(ms % Check_height_sensors == 0){   //cada cuanto chequeo los sensores de altura //#define macro en General.h
		so3p =so3;
		so4p =so4;
		so5p =so5;

		//cargo los valores de los sensores de ese instante
		so3 = isBitSet(REG_SOB_PIN,PIN_SO3_PIN);
		so4 = isBitSet(REG_SOK_PIN,PIN_SO4_PIN);
		so5 = isBitSet(REG_SOK_PIN,PIN_SO5_PIN);
		limit_switch_lavH = isClrSet(REG_SW_PIN,PIN_SW2_PIN); // isClrSet porque SW2 '0' al detectar
		
		
		if((so3p==so3) && (so4p==so4) && (so5p==so5)){  //Si los valores son los mismos que en instante anterior
			aux_lavH = 1;		  //muevo el rodillo
			}else{
			aux_lavH = 0;		  //no hago nada
		}
		
	}
	
	if (enable_prove_new && (ms-cnt_prove_new > Tiempo_prove_new)){
		enable_prove_new = 0;
	}
	
}
// Sensores Parte3 --PENSAR DONDE INCLUIR ESTA INTERRUPCIÓN
ISR(PCINT0_vect){
	//SO1 [SOB] (PCINT0)
	if (isBitSet(REG_SOB_PIN,PIN_SO1_PIN) && reg_SO1 == 0){ // Flanco subida
		//closeBarrera(); // Pendiente Parte2
		reg_SO1 = 1; // Actualizo registro SO1 con valor actual
	}	
	else if (isClrSet(REG_SOB_PIN,PIN_SO1_PIN) && (reg_SO1 == 1) && (EnableEntrance == 1)){ // Flanco bajada y entrada habilitada
		NumCarLavado++;		// Ha entrado coche
		enable_prove_new = 1; // Activo comprobación entrada
		cnt_prove_new = s; // No usamos contador propio, comparamos (s - cnt_prove_new) > Tiempo_prove_new
		//moveCinta();		SOBRA, AL HACER NumCarLavado++ LA FUNCIÓN cINTA() ACTIVA EL MOVIMIENTO
		//openBarrera(); // Pendiente Parte2
		//on_LavHorizontal(); // Pendiente Parte1
		//on_LavVertical(); // Pendiente Parte2
		
		reg_SO1 = 0; // Actualizo registro SO1 con valor actual
	}
	
	//SO3 [SOL] *PB1*(*PCINT1*) -- MODIFICACIÓN PROPUESTA POR NACHO :) --
	else if (isClrSet(REG_SOL_PIN,PIN_SO3_PIN) && reg_SO3 == 1 && enable_prove_new == 1 && cnt_prove_new < Tiempo_prove_new){ // Flanco bajada y entrada habilitada
		enable_prove_new = 0;
		
		reg_SO3 = 0; // Actualizo registro SO3 con valor actual
	} 
	
	else if (isBitSet(REG_SOL_PIN,PIN_SO3_PIN) && reg_SO3 == 0 ){ // Flanco subida 
		reg_SO3 = 1; // Actualizo registro SO3 con valor actual
	} 
	
	//SO6 [SOB] (PCINT4)
	else if (isBitSet(REG_SOB_PIN,PIN_SO6_PIN) && reg_SO6 == 0){ // Flanco subida - Paso del culo
		EnableEntrance = 1;
		decNumCarLavado();
		
		reg_SO6 = 1; // Actualizo registro SO6 con valor actual
	}
	
	else if (isClrSet(REG_SOB_PIN,PIN_SO6_PIN) && reg_SO6 == 1){ // Flanco bajada - Paso del morro
		incNumCarSecado();
		reg_SO6 = 0;
	}
	
	//SO12 [SOB] PB2 (PCINT2)
	else if (isBitSet(REG_SOB_PIN,PIN_SO12_PIN) && reg_SO12 == 0){ // Flanco subida
		decNumCarSecado();
	
		reg_SO12 = 1; // Actualizo registro SO12 con valor actual
	}
	
	else if (isClrSet(REG_SOB_PIN,PIN_SO12_PIN) && reg_SO12 == 1){ // Flanco bajada
		reg_SO12 = 0; // Actualizo registro SO12 con valor actual
	}
	
}
