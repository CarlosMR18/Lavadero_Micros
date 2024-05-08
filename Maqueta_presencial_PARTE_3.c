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
		#define Tiempo_prove_new 1200 // Tiempo comprobación entrada nueva en ms(dos sensores distintos)
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
volatile uint8_t NumCar = 0; // Cuenta coches dentro del lavadero

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

uint8_t getNumCar(void){ // Devuelve número coches en lavadero
	return NumCar;
}

void incNumCar(void){ // Incrementa en uno el contador de coches en lavadero
	NumCar++;
}

void decNumCar(void){ // Decrementa en uno el contador de coches en lavadero
	NumCar--;
}

////////////////////////////

// Funciones de interrupción

ISR(TIMER1_COMPA_vect){ // Segundos
	s++;
}

ISR(TIMER3_COMPA_vect){ // Milisegundos
	ms++;
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
	if(isBitSet(REG_SOL_PIN, PIN_SO10_PIN)){ 
		// No hay coche liberado de la cinta
		clearBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Apagado
		setBit(REG_LED_PORT, PIN_L5_PORT);		// RED: Encendido
	}
	else {
		// Coche liberado de la cinta
		setBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Encendido
		clearBit(REG_LED_PORT, PIN_L5_PORT);// RED: Apagado
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

// Inicialización
void setup_Parte3(){
	cli();
	setupCinta(); // Cinta de arrastre
	setup_leds(); // LEDs (Estado maqueta y Semaforo)
	setup_ParadaEmergencia(); // Parada de emergencia
	sei();
}


int main(void) {
	setupTimers();
	setupCinta();
	setup_leds();
	//setup_ParadaEmergencia();
	while(1) {
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
	}

}




















 

