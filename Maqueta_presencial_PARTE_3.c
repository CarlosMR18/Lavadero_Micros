/*
 * Maqueta_presencial_PARTE_3.c
 *
 * Created: 08/05/2024 21:57:40
 * Author : usuario
 */ 


//	Librerias de trabajo
#include <avr/io.h>
#include <avr/interrupt.h>

#define setBit(Registro,Bit)	(Registro |= (1 << Bit))	// pone a 1 el bit B del puerto P
#define clearBit(Registro,Bit)	(Registro &= ~(1 << Bit))	// pone a 0 el bit B del puerto P
#define toggleBit(Registro,Bit) (Registro ^= (1 << Bit))  	// cambia el valor del bit B del puerto P

#define isBitSet(Registro, Bit) (Registro & (1 << Bit))		// devuelve '1' si dicho bit es '1'
#define isClrSet(Registro, Bit) (!isBitSet(Registro, Bit))	// devuelve '1' si dicho bit es '0'

// Caracteristicas del microprocesador
#define Freq_uC 8000000
#define Check_height_sensors 50 // MS en comprobar estado (Regular en maqueta)
#define Tiempo_prove_new 50 // Tiempo comprobación entrada nueva en s(dos sensores distintos)
// Estado de maqueta
#define MAQUETA_ON 1 //CYCLE_WORKING
#define MAQUETA_OFF 0 //CYCLE_STOPPED

// MOTORES

// Motor M1 => Barrera de entrada
// Direction
#define REG_M1_di_PORT	PORTL
#define REG_M1_di_DDR	DDRL
#define REG_M1_di_PIN	PINL

#define PIN_M1_di_PORT	PK0
#define PIN_M1_di_DDR  	DDK0
#define PIN_M1_di_PIN  	PINK0
// Enable
#define REG_M1_en_PORT	PORTK
#define REG_M1_en_DDR	DDRK
#define REG_M1_en_PIN	PINK

#define PIN_M1_en_PORT	PK2
#define PIN_M1_en_DDR	DDK2
#define PIN_M1_en_PIN	PINK2

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
#define REG_M3_di_PORT	PORTB
#define REG_M3_di_DDR	DDRB
#define REG_M3_di_PIN	PINB

#define PIN_M3_di_PORT	PB1
#define PIN_M3_di_DDR  	DDB1
#define PIN_M3_di_PIN  	PINB1
// Enable
#define REG_M3_en_PORT	PORTB
#define REG_M3_en_DDR	DDRB
#define REG_M3_en_PIN	PINB

#define PIN_M3_en_PORT	PB3
#define PIN_M3_en_DDR	DDB3
#define PIN_M3_en_PIN	PINB3

// Motor M4 => Rodillo horizontal giro
// Direction
#define REG_M4_di_PORT	PORTK
#define REG_M4_di_DDR	DDRK
#define REG_M4_di_PIN	PINK

#define PIN_M4_di_PORT	PK5
#define PIN_M4_di_DDR  	DDK5
#define PIN_M4_di_PIN  	PINK5
// Enable
#define REG_M4_en_PORT	PORTK
#define REG_M4_en_DDR	DDRK
#define REG_M4_en_PIN	PINK

#define PIN_M4_en_PORT	PK7
#define PIN_M4_en_DDR	DDK7
#define PIN_M4_en_PIN	PINK7

// Motor M5 => Ventilador de secado
// Direction
#define REG_M5_di_PORT	PORTK
#define REG_M5_di_DDR	DDRK
#define REG_M5_di_PIN	PINK

#define PIN_M5_di_PORT	PK1
#define PIN_M5_di_DDR  	DDK1
#define PIN_M5_di_PIN  	PINK1
// Enable
#define REG_M5_en_PORT	PORTK
#define REG_M5_en_DDR	DDRK
#define REG_M5_en_PIN	PINK

#define PIN_M5_en_PORT	PK3
#define PIN_M5_en_DDR	DDK3
#define PIN_M5_en_PIN	PINK3

// Motor M6 => Cinta de arrastre
// Direction => PCINT5
#define REG_M6_di_PORT	PORTB
#define REG_M6_di_DDR	DDRB
#define REG_M6_di_PIN	PINB

#define PIN_M6_di_PORT	PB5
#define PIN_M6_di_DDR  	DDB5
#define PIN_M6_di_PIN  	PINB5
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
// REGISTROS S01-S05-S06-S12 B
#define REG_SOB_PORT PORTB
#define REG_SOB_DDR  DDRB
#define REG_SOB_PIN  PINB
// REGISTROS S02-S03-S04-S010 L
#define REG_SOL_PORT PORTL
#define REG_SOL_DDR  DDRL
#define REG_SOL_PIN  PINL
// REGISTROS S07-S08-S09-2011 D
#define REG_SOD_PORT PORTD
#define REG_SOD_DDR  DDRD
#define REG_SOD_PIN  PIND
// SO1 => PCINT0
#define PIN_SO1_PORT PB0
#define PIN_SO1_DDR  DDB0
#define PIN_SO1_PIN  PINB0
// SO2
#define PIN_SO2_PORT PL2
#define PIN_SO2_DDR  DDL2
#define PIN_SO2_PIN  PINL2
// SO3
#define PIN_SO3_PORT PL4
#define PIN_SO3_DDR  DDL4
#define PIN_SO3_PIN  PINL4
// SO4
#define PIN_SO4_PORT PL6
#define PIN_SO4_DDR  DDL6
#define PIN_SO4_PIN  PINL6
// SO5 => PCINT6
#define PIN_SO5_PORT PB6
#define PIN_SO5_DDR  DDB6
#define PIN_SO5_PIN  PINB6

// SO6 => PCINT4
#define PIN_SO6_PORT PB4
#define PIN_SO6_DDR  DDB4
#define PIN_SO6_PIN  PINB4
// SO7
#define PIN_SO7_PORT PD4
#define PIN_SO7_DDR  DDD4
#define PIN_SO7_PIN  PIND4
// SO8
#define PIN_SO8_PORT PD6
#define PIN_SO8_DDR  DDD6
#define PIN_SO8_PIN  PIND6
// SO9
#define PIN_SO9_PORT PD7
#define PIN_SO9_DDR  DDD7
#define PIN_SO9_PIN  PIND7
// SO10
#define PIN_SO10_PORT PL0
#define PIN_SO10_DDR  DDL0
#define PIN_SO10_PIN  PINL0
// SO11
#define PIN_SO11_PORT PD5
#define PIN_SO11_DDR  DDD5
#define PIN_SO11_PIN  PIND5
// SO12 => PCINT2
#define PIN_SO12_PORT PB2
#define PIN_SO12_DDR  DDB2
#define PIN_SO12_PIN  PINB2

// Variables globales
volatile uint8_t Stop = 0; // '1' Bloque maqueta
volatile uint32_t s = 0; // Cuenta segundos
volatile uint32_t ms = 0; // Cuenta milisegundos
//volatile uint8_t NumCar = 0; // Cuenta coches dentro del lavadero -- ELIMINAR EN GITHUB
volatile uint8_t NumCarLavado = 0; // Cuenta coches en zona lavado
volatile uint8_t NumCarSecado = 0; // Cuenta coches en zona secado

volatile uint8_t EnableEntrance = 1; // '1' permite nuevas entradas de vehículos y '0' no permite
volatile uint8_t enable_prove_new = 0; // '1' pendiente comprobar entradas de vehículos
volatile uint32_t cnt_prove_new = 0; // Cuenta comprobar entradas de vehículos

// Variables globales - Lavadero Horizontal
volatile uint8_t lav_H[3] = {0,0,0}; // lav_H[0] para SO3, lav_H[1] para SO4, lav_H[2] para SO5
volatile uint8_t prev_lav_H[3] = {0,0,0};
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

/* Puede ser interesante
volatile uint8_t estLavV = 0; // '1' activado Lavadero Vertical y '0' desactivado
volatile uint8_t estLavH = 0; // '1' activado Lavadero Vertical y '0' desactivado
volatile uint8_t estSec  = 0; // '1' activado Secadero y '0' desctivado
*/

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
	// TIMER 1 => Timer PWM : Modo PWM, Phase and Frequency Correct sin preescalado
	TCCR1A = 0b00001001;	// (1 << WGM10) | (1 << COM1C1)
	TCCR1B = 0b00010001;	// (1 << WGM13) | (1 << CS10);
	TIMSK1 = 0b00000010;
	OCR1A =  Freq_uC/5000;	// Frecuencia de 5KHz, 0.2ms el ciclo
	OCR1C = 1280 - 1;		// Ciclo de trabajo del 80%
	//TIMSK1 = (1 << OCIE1A);	// Se habilita la máscara que habilita recibir interrupciones por el timer
	//TIFR1 = (1 << OCF1A);		// Se deshabilitan las demás interrupciones cuando salte la interrupción del OCR1A
	
	// TIMER 3 => Timer milisegundos :  Modo CTC (ICRn) sin preescalado
	TCCR3A = 0b00000000;
	TCCR3B = 0b00001001;
	TIMSK3 = 0b00000010;
	OCR3A =	Freq_uC/1000;
	// TIMER 4 => Timer segundos : Modo CTC (ICRn) con preescalado 256
	TCCR4A = 0b00000000;
	TCCR4B = 0b00001101;
	TIMSK4 = 0b00000010;
	OCR4A =  Freq_uC/256;
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

ISR(TIMER4_COMPA_vect){ // Segundos
	s++;
	if (cnt_prove_new > Tiempo_prove_new && NumCarLavado > 0 && enable_prove_new == 0){
		NumCarLavado--;
		EnableEntrance = 1;
		enable_prove_new = 0;
	}
}

ISR(TIMER3_COMPA_vect){ // Milisegundos
	ms++;
	if(enable_prove_new == 1) {		//--------------------------------------------------------------------------------------------------------NUEVO LEYRE
		cnt_prove_new++;
	}
}

// IDK
ISR(PCINT0_vect){
	//SO1 [SOB] (PCINT0)
	if (isBitSet(REG_SOB_PIN,PIN_SO1_PIN) && reg_SO1 == 0){ // Flanco subida
		//closeBarrera(); // Pendiente Parte2
		reg_SO1 = 1; // Actualizo registro SO1 con valor actual
	}	
	else if (isClrSet(REG_SOB_PIN,PIN_SO1_PIN) && (reg_SO1 == 1) && (EnableEntrance == 1)){ // Flanco bajada y entrada habilitada
		NumCarLavado++;		// Ha entrado coche
		enable_prove_new = 1; // Activo comprobación entrada
		cnt_prove_new = 0;
		moveCinta(); //--------------------------------------------------------------------------------------------------------------- Mejor ponerlo por consulta periódica en main???
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

///////////////////////////
///////   PARTE 3   ///////
///////////////////////////

// Cinta de arrastre  (M6)
void setupCinta(){
	setBit(REG_M6_di_DDR, PIN_M6_di_DDR); //Configurar el pin de dirección M6 como salida
	setBit(REG_M6_en_DDR, PIN_M6_en_DDR); //Configurar el pin M6 en como salida
	setBit(REG_M6_di_PORT, PIN_M6_di_PORT); //Ajustar el sentido de giro de M6 a la derecha
}

void moveCinta(){
	setBit(REG_M6_en_PORT, PIN_M6_en_PORT);
}

void stopCinta(){
	clearBit(REG_M6_en_PORT, PIN_M6_en_PORT);
}

void cinta(){
	if (NumCarLavado == 0 && NumCarSecado == 0){	// Ningún vehículo
		stopCinta();
	}
	else{	// Algún vehículo
		moveCinta();
	}
}

// LEDs (Estado maqueta y Semaforo)
void setup_leds(){ //setup_L1
	// Semáforo: LED4 y LED5
	setBit(REG_LED_DDR, PIN_L4_DDR);	// GREEN: Establecer LED 4 pin como salida
	setBit(	REG_LED_DDR, PIN_L5_DDR);	// RED: Establecer LED 5 pin como salida
	// Estado maqueta: LED1
	setBit(REG_LED_DDR, PIN_L1_DDR); //Set the led 1 pin as an output
}

// Semaforo [LED4 y LED5]
void semaforo(){ //LED_4_5
	if (NumCarLavado == 0 && NumCarSecado == 0){	// Ningún vehículo
		clearBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Apagado
		clearBit(REG_LED_PORT, PIN_L5_PORT);	// RED: Apagado
	} 
	else{	// Algún vehículo
		
		if(isBitSet(REG_SOL_PIN, PIN_SO10_PIN) && isClrSet(REG_SOD_PIN, PIN_SO11_PIN) && isClrSet(REG_SOB_PIN, PIN_SO12_PIN)){
			// No hay coche liberado de la cinta (SO10 && not_SO11 && not_SO12)
			clearBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Apagado
			setBit(REG_LED_PORT, PIN_L5_PORT);		// RED: Encendido
		}
		else {
			// Coche liberado de la cinta
			setBit(REG_LED_PORT, PIN_L4_PORT);		// GREEN: Encendido
			clearBit(REG_LED_PORT, PIN_L5_PORT);	// RED: Apagado
		}
	}
}

// Estado maqueta [LED1]
void encendido_L1(){
	setBit(REG_LED_PORT, PIN_L1_PORT);
}

void apagado_L1(){
	clearBit(REG_LED_PORT, PIN_L1_PORT);
}

void LED_1(){
	// Pendiente (Consulta periódica)
}


// Parada de emergencia
void setup_ParadaEmergencia(){ // SW4 al soltar boton
	
	setBit(EICRA,ISC00); // Interrupción flanco subida
	setBit(EICRA,ISC01);
	
	setBit(EIMSK,INT0); // Activo mascara interrupción
}

void ParadaEmergencia() { // PENSAR DONDE PONERLO
	setStop();
	
	/*Pendiente de partes previas 1, 2 y 3*/
}

 // Sensores: SO1, SO3, SO6, SO10, SO11 y SO12
 void setup_sensores_parte3(){
	cli();
	//SO1 [SOB] (PCINT0)
	clearBit(REG_SOB_DDR, PIN_SO1_DDR); // Entrada
		// Configuro interrupción por cambio PCINT0
	setBit(PCMSK0, PCINT0); // Habilito mascara interrupción 
	setBit(PCICR, PCIE0); // Habilito registro interrupción
	setBit(PCIFR, PCIF0); // Borro bandera
	
	//SO3 [SOL] *PB1*(*PCINT1*) -- MODIFICACIÓN PROPUESTA POR NACHO :) -- 
	clearBit(REG_SOL_DDR, PIN_SO3_DDR); // Entrada
		// Configuro interrupción por cambio *PCINT1*
	/*
	setBit(PCMSK0, PCINT1); // Habilito mascara interrupción
	setBit(PCICR, PCIE0); // Habilito registro interrupción
	setBit(PCIFR, PCIF0); // Borro bandera
	*/
	
	//SO6 [SOB] (PCINT4)
	clearBit(REG_SOB_DDR, PIN_SO6_DDR); // Entrada
		// Configuro interrupción por cambio PCINT4
	setBit(PCMSK0, PCINT4); // Habilito mascara interrupción
	setBit(PCICR, PCIE0); // Habilito registro interrupción
	setBit(PCIFR, PCIF0); // Borro bandera
	
	//SO10 [SOL] PL0 
	clearBit(REG_SOL_DDR, PIN_SO10_DDR); // Entrada
		
	//SO11 [SOD] PD5 
	clearBit(REG_SOD_DDR, PIN_SO11_DDR); // Entrada
		
	//SO12 [SOB] PB2 (PCINT2)
	clearBit(REG_SOB_DDR, PIN_SO12_DDR); // Entrada
		// Configuro interrupción por cambio PCINT4
	setBit(PCMSK0, PCINT2); // Habilito mascara interrupción
	setBit(PCICR, PCIE0); // Habilito registro interrupción
	setBit(PCIFR, PCIF0); // Borro bandera
	sei();	
}


// Inicialización
void setup_Parte3(){
	cli();
	setupCinta(); // Cinta de arrastre
	setup_leds(); // LEDs (Estado maqueta y Semaforo)
	setup_ParadaEmergencia(); // Parada de emergencia
	setup_sensores_parte3();
	sei();
}


int main(void) {
	setup_Parte3();
	setup_General();
	//setup_ParadaEmergencia();
	while(1) {
		cinta();
		//semaforo();
		
		if (NumCarLavado == 1 && NumCarSecado == 0){	// Ningún vehículo
			setBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Encendido
			clearBit(REG_LED_PORT, PIN_L5_PORT);	// RED: Apagado
		}
		else if (NumCarLavado == 0 && NumCarSecado == 1){	// Ningún vehículo
			clearBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Apagado
			setBit(REG_LED_PORT, PIN_L5_PORT);	// RED: Encendido
		}
		else if (NumCarLavado == 0 && NumCarSecado == 0){	// Ningún vehículo
			clearBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Apagado
			clearBit(REG_LED_PORT, PIN_L5_PORT);	// RED: Apagado
		} else {
			setBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Encendido
			setBit(REG_LED_PORT, PIN_L5_PORT);	// RED: Encendido
		}
		
	//	if (isClrSet(REG_SOB_PIN,PIN_SO1_PIN) && reg_SO1 == 1 && EnableEntrance == 1){ // Flanco bajada y entrada habilitada
	//		NumCarLavado++;		// Ha entrado coche
	//	}
		
		/*
		// Cinta
		moveCinta();
		delay_milliseconds(5000);
		stopCinta();
		delay_milliseconds(2000);
		moveCinta();
		delay_milliseconds(5000);
		stopCinta();
		
		// SEMÁFORO
		// No hay coche liberado de la cinta
		clearBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Apagado
		setBit(REG_LED_PORT, PIN_L5_PORT);		// RED: Encendido
		delay_milliseconds(2000);
		// Coche liberado de la cinta
		setBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Encendido
		clearBit(REG_LED_PORT, PIN_L5_PORT);// RED: Apagado
		delay_milliseconds(2000);
		// No hay coche liberado de la cinta
		clearBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Apagado
		setBit(REG_LED_PORT, PIN_L5_PORT);		// RED: Encendido
		delay_milliseconds(2000);
		// Coche liberado de la cinta
		setBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Encendido
		clearBit(REG_LED_PORT, PIN_L5_PORT);// RED: Apagado
		delay_milliseconds(2000);
		*/
	}
	
}
