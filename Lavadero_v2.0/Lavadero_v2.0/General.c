#include "General.h"

// Variables globales
extern volatile uint8_t Stop = 0; // '1' Bloque maqueta

extern volatile uint32_t s = 0; // Cuenta segundos
extern volatile uint32_t ms = 0; // Cuenta milisegundos

extern volatile uint8_t NumCarLavado = 0; // Cuenta coches en zona lavado
extern volatile uint8_t NumCarSecado = 0; // Cuenta coches en zona secado

	// Variables globales - Parte 1
	
		//**** Comprobar si toca incluir ****
	
	// Variables globales - Parte 2
	
		//**** Comprobar si toca incluir ****
	
	// Variables globales - Parte 3
	extern volatile uint8_t EnableEntrance = 1; // '1' permite nuevas entradas de vehículos y '0' no permite
	extern volatile uint8_t enable_prove_new = 0; // '1' pendiente comprobar entradas de vehículos
	extern volatile uint32_t cnt_prove_new = 0; // Cuenta comprobar entradas de vehículos	
	
	
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////
//////		STOP	 ///////
////////////////////////////

void setStop(void){
	Stop = 1;
}

uint8_t getStop(void){
	return Stop;
}

////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////
////// FUNCIONES TEMPORALES ///////
///////////////////////////////////


void setupTimers(){
	cli();
	/*
	// TIMER 1 => Timer PWM : Modo PWM, Phase and Frequency Correct CON PREESCALADO DE 1024
	TCCR1A = 0b00101001;
	TCCR1B = 0b00010101;
	TIMSK1 = 0b00000010;
	OCR1A =  Freq_uC/5000;	// Frecuencia de 5KHz, 0.2ms el ciclo
	OCR1B = CICLO_TRABAJO - 1;
	//	OCR1C = VELOCIDAD_M6;		// Ciclo de trabajo de la cinta
	//TIMSK1 = (1 << OCIE1A);	// Se habilita la máscara que habilita recibir interrupciones por el timer
	//TIFR1 = (1 << OCF1A);		// Se deshabilitan las demás interrupciones cuando salte la interrupción del OCR1A
	*/
	
	// TIMER 1 => Timer PWM : Fast PWM con prrescalado de 1024 y 5KHz (0.2ms el ciclo)
	TCCR1A = 0b00101011;
	TCCR1B = 0b00010101;
	TIMSK1 = 0b00000000;
	OCR1A =  (Freq_uC/5000)-1;
	OCR1B = (CICLO_TRABAJO_SECADO * Freq_uC / 5000) - 1;
	OCR1C = (CICLO_TRABAJO_CINTA * Freq_uC / 5000) - 1;
	
	// TIMER 4 => Timer segundos : Modo CTC con preescalado 256
	TCCR4A = 0b00000000;
	TCCR4B = 0b00001100;
	TIMSK4 = 0b00000010;
	OCR4A =  (Freq_uC/256)-1;

	// TIMER 3 => Timer milisegundos :  Modo CTC con preescalado 8
	TCCR3A = 0b00000000;
	TCCR3B = 0b00001010;
	TIMSK3 = 0b00000010;
	OCR3A = 1000-1; // (Freq_uC*0.001/8)-1;
	
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

////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////
////// CONTADORES PRESENCIA COCHES ///////
//////////////////////////////////////////

//Consideramos dos zonas en la maqueta: Lavado y Secado

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
////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////
////// INICIALIZACIÓN COMÚN ///////
///////////////////////////////////

void setup_General(void){ //Incluir todas funciones inicialización
	setupTimers();
}

////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////
////// -------------------- ///////
///////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
