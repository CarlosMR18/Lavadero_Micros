#include <avr/io.h>
#include <avr/interrupt.h>

#define REG_M1_en_PORT	PORTL
#define REG_M1_en_DDR	DDRL
#define REG_M1_en_PIN	PINL

#define PIN_M1_en_PORT	PL2
#define PIN_M1_en_DDR	DDL2
#define PIN_M1_en_PIN	PINL2

#define REG_M2_en_PORT	PORTK
#define REG_M2_en_DDR	DDRK
#define REG_M2_en_PIN	PINK

#define PIN_M2_en_PORT	PK6
#define PIN_M2_en_DDR	DDK6
#define PIN_M2_en_PIN	PINK6

#define REG_SW_PORT PORTD
#define REG_SW_DDR  DDRD
#define REG_SW_PIN  PIND
// SW1 => INT3
#define PIN_SW1_PORT PD3
#define PIN_SW1_DDR  DDD3
#define PIN_SW1_PIN  PIND3

#define REG_LED_PORT	PORTL
#define REG_LED_DDR 	DDRL
#define REG_LED_PIN 	PINL
// L1
#define PIN_L1_PORT PL1
#define PIN_L1_DDR  DDL1
#define PIN_L1_PIN  PINL1

// REGISTROS S01-S06-S12 B
#define REG_SOB_PORT PORTB
#define REG_SOB_DDR  DDRB
#define REG_SOB_PIN  PINB
// REGISTROS S02-S03-S04-S010 L
#define REG_SOL_PORT PORTL
#define REG_SOL_DDR  DDRL
#define REG_SOL_PIN  PINL

//REGISTRO S05-S02
#define REG_SOK_PORT PORTK
#define REG_SOK_DDR  DDRK
#define REG_SOK_PIN  PINK

// SO1 => PCINT0
#define PIN_SO1_PORT PB0
#define PIN_SO1_DDR  DDB0
#define PIN_SO1_PIN  PINB0
// SO2
#define PIN_SO2_PORT PK2
#define PIN_SO2_DDR  DDK2
#define PIN_SO2_PIN  PINK2
// SO3
#define PIN_SO3_PORT	PB1
#define PIN_SO3_DDR  	DDB1
#define PIN_SO3_PIN  	PINB1
//SO5
#define PIN_SO5_PORT	PK1
#define PIN_SO5_DDR  	DDK1
#define PIN_SO5_PIN  	PINK1

////////////////////////////////////////////////////////////////
////////////////////	 GENERAL - MACROS	////////////////////
////////////////////////////////////////////////////////////////
#define setBit(Registro,Bit)	(Registro |= (1 << Bit))	// pone a 1 el bit B del puerto P
#define clearBit(Registro,Bit)	(Registro &= ~(1 << Bit))	// pone a 0 el bit B del puerto P
#define toggleBit(Registro,Bit) (Registro ^= (1 << Bit))  	// cambia el valor del bit B del puerto P

#define isBitSet(Registro, Bit) (Registro & (1 << Bit))		// devuelve '1' si dicho bit es '1'
#define isClrSet(Registro, Bit) (!isBitSet(Registro, Bit))	// devuelve '1' si dicho bit es '0'

#define Freq_uC 8000000

////////////////////////////////////////////////////////////////
////////////////////	 GENERAL - TIEMPO	////////////////////
////////////////////////////////////////////////////////////////

volatile uint32_t s = 0; // Cuenta segundos
volatile uint32_t ms = 0; // Cuenta milisegundos
volatile uint32_t s_cnt = 0; // Cuenta segundos
volatile uint32_t ms_cnt = 0; // Cuenta milisegundos


void setupTimers(void){
	cli();
	// TIMER 1 -> Timer para interrupción cada 2 segundos 
	TCCR1B |= (1<<WGM12) 	//Modo CTC (OCRnA)
	OCR1B = 62499; 		//Valor comparación para contar 2 segundos
	TCCR1B |= (1<<CS12); 	//Preescalado de 256
	TIMSK1 |= (1<<OCIE1A); 	//Interrupción por comparación
	
	// TIMER 4 => Timer segundos : Modo CTC (ICRn) con preescalado 256
	TCCR4A = 0b00000000;                    //WGM40 y WGM41 == 0
	TCCR4B = 0b00001101;                    //WGM42(bit3) == 1  CS42 == 1(preescalado 256)(bit2)
	TIMSK4 = 0b00000010;
	OCR4A =  Freq_uC/256;

	// TIMER 3 => Timer milisegundos :  Modo CTC (OCRnA) sin preescalado
	TCCR3A = 0b00000000;                    //WGM30 y WGM31 == 0
	TCCR3B = 0b00001001;                    //WGM32(bit3) == 1  CS30 == 1(no preescalado)(bit0)
	TIMSK3 = 0b00000010;
	OCR3A = Freq_uC/1000;
}

	void delay_Ms(int ms){
		for(int j=0; j<ms; j++){
			for(volatile int i=0; i<444; i++);
		}
	}
	
void delay_milliseconds(uint32_t dms){
	volatile uint32_t delay = ms + dms;
	while( ms < delay ){}
}

void delay_seconds(uint32_t ds){
	volatile uint32_t delay = s + ds;
	while( s < delay );
}

uint32_t millis(void){
	cli(); 
	ms=ms_cnt; 
	sei(); 
	return ms;
}

uint32_t seconds(void){
	cli(); 
	s=s_cnt; 
	sei(); 
	return s;
}

// Funciones de interrupción

volatile uint8_t b_flag=0;
ISR(TIMER1_COMPA_vect){ // Segundos
	b_flag = 1;
}

ISR(TIMER4_COMPA_vect){ // Segundos
	s_cnt++;
}

ISR(TIMER3_COMPA_vect){ // Milisegundos
	ms_cnt++;
}

////////////////////////////////////////////////////////
////////////////////	 BARRERA	////////////////////
////////////////////////////////////////////////////////

// VARIABLES BARRERA
volatile uint8_t reg_SO1 = 1; // Almacena valor de SO1
volatile uint8_t reg_SO2 = 1; // Almacena valor de SO2
volatile uint8_t barrera_cerrada=0;
volatile uint8_t modo_barrera = 0;
volatile uint8_t cnt_apertura_barrera = 0;
volatile uint8_t cuenta=0; 

// FUNCIONES BARRERA


void setup_barrera(){
	cli();
	setBit(REG_M1_en_DDR, PIN_M1_en_DDR); // Como salida M1_en
	clearBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Inicialmente apagado
	
	//SO1 [SOB] (PCINT0)
	clearBit(REG_SOB_DDR, PIN_SO1_DDR); // Entrada
	// Configuro interrupción por cambio PCINT0
	setBit(PCMSK0, PCINT0); // Habilito mascara interrupción
	setBit(PCICR, PCIE0); // Habilito registro interrupción
	setBit(PCIFR, PCIF0); // Borro bandera
	
	//SO2 [SOK] (PCINT18)
	clearBit(REG_SOK_DDR, PIN_SO2_DDR); // Entrada
	// Configuro interrupción por cambio PCINT18
	setBit(PCMSK2, PCINT18); // Habilito mascara interrupción
	setBit(PCICR, PCIE2); // Habilito registro interrupción
	setBit(PCIFR, PCIF2); // Borro bandera

	//SW1 -> NO USO, CONTROLO CON TIEMPOS PARA APERTURA
	clearBit(REG_SW_DDR, PIN_SW1_DDR);
	
	EIMSK |= (1<<INT3) ;
	EICRA |= (1<<ISC31); //|(1<< ISC30);
	EIFR |= (1<<INTF3); 

	//Empezar con barrera cerrada
	if(isClrSet(REG_SOK_PIN,PIN_SO2_PIN)== 0){
		setBit(REG_M1_en_PORT, PIN_M1_en_PORT); 
		while (isClrSet(PINK,PIN_SO2_PIN)== 0);
		barrera_cerrada=1;
	}
	
	sei();
}

ISR(INT3_vect){
	cuenta++;
}

void openbarrera(){
	cnt_apertura_barrera = millis();
	modo_barrera = 1;	// Para barrera()
}

void closebarrera(){
	modo_barrera = 2;	// Para barrera()
}

void stopbarrera(){
	modo_barrera = 0;	// Para barrera()
}

volatile uint32_t segundos=0;
volatile uint8_t bandera_coche=0; 

// barrera(): Incluir en el WHILE DEL MAIN
void barrera(){		// En WHILE del MAIN
	
	switch (modo_barrera){
		case 0:		//Barrera parada
			clearBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Apago motor barrera, nos aseguramos que para
			break;
			
		case 1:		//Barrera Subir 
			
			if(cuenta<2){
				setBit(REG_M1_en_PORT, PIN_M1_en_PORT);
			}else if(cuenta==2){
				clearBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Apago motor barrera
			}

			//OPCIÓN CON TIMER1
			//if(b_flag = 0){
			//	setBit(REG_M1_en_PORT, PIN_M1_en_PORT);
			//}else if(b_flag = 1){
			//	clearBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Apago motor barrera
			//}
			//TCNT1 = 0;
			
			modo_barrera = 0;  // Cambio a modo 0 (Barrera parada)
			bandera_coche=0; //necesaria para probar nosotras
			break;
			
		case 2:		//Barrera Bajar
			setBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Encendido
			if (barrera_cerrada==1){
				clearBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Apago motor barrera
				modo_barrera = 0;
			}
			break;
	}
}

ISR(PCINT0_vect){
	if (isBitSet(PINB,PIN_SO1_PIN)  && reg_SO1 == 0){ // Flanco subida - Deja de detectar (No abajo)
		bandera_coche=2; 
		reg_SO1 = 1;			// Actualizo registro SO1 con valor actual
	}
	else if ((isClrSet(PINB,PIN_SO1_PIN)&& (~PIN_SO1_PIN) )&& (reg_SO1 == 1)){ // Flanco bajada y entrada habilitada - Empieza a detectar (Abajo)
		bandera_coche=1;  
		reg_SO1=0; 
		//modo_led1=1; 			// Actualizo registro SO1 con valor actual
	}
}

ISR(PCINT2_vect){
	//SO2 [SOK] (PCINT18) - Sensor posición barrera
	if (isBitSet(PINK,PIN_SO2_PIN) && reg_SO2 == 0){ // Flanco subida - Deja de detectar (No abajo)
		barrera_cerrada = 0;
		reg_SO2 = 1;			// Actualizo registro SO1 con valor actual
	}
	else if(isClrSet(PINK,PIN_SO2_PIN) && reg_SO2 == 1){ // Flanco bajada y entrada habilitada - Empieza a detectar (Abajo)
		barrera_cerrada = 1;
		reg_SO2 = 0;			// Actualizo registro SO2 con valor actual
		//clearBit(REG_M1_en_PORT, PIN_M1_en_PORT); 
	}
}



////////////////////////////////////////////////////////
////////////////////	 LUZ (L1)	////////////////////
////////////////////////////////////////////////////////

// VARIABLES LUZ (L1)
volatile uint8_t timer_ticks = 0; // Para controlar parpadeo
volatile uint8_t aux_parpadeo_LED1 = 20; // Inicializado a parpadeo sin coches
volatile uint8_t modo_led1 = 0; //AUXILIAR - Cumple la funcionalidad de variable de integración
// FUNCIONES LUZ (L1)
void setup_luz(){
	cli();
	//modo_led1=0; 
	setBit(REG_LED_DDR, PIN_L1_DDR); // Pin del LED como salida
	clearBit(REG_LED_PORT, PIN_L1_PORT); //LED apagado inicialmente
	// Configurar Timer 5 para generar interrupción cada 0.5 segundo
	TCCR5A=0x00; //configurar CTC
	TCCR5B |= (1<< WGM52)| (1 << CS52) | (1 << CS50); // Prescaler de 1024
	TCNT5 = 0; // Inicializar el contador
	OCR5A = 3905; // Valor de comparación para 0.5 segundo
	TIMSK5 |= (1 << OCIE5A); // Habilitar la interrupción por comparación
	TIFR5 |= (1<< OCF5A);
	sei();
}

void control_LED1(uint8_t modo_led1){
	if (modo_led1 == 0) aux_parpadeo_LED1 = 20; //Lavadero no funciona (sin coches)
	else aux_parpadeo_LED1 = 2; // Lavadero funciona (hay coches)
}

ISR(TIMER5_COMPA_vect) {
	// Rutina de interrupción del Timer 5
	timer_ticks++;
	if (timer_ticks == aux_parpadeo_LED1) { // 10 segundos
		setBit(REG_LED_PORT,PIN_L1_PORT); // Enciendo el LED
		timer_ticks = 0; // Reiniciar el contador
	}
	else if (timer_ticks == 1) { // 0.5 segundo
		clearBit(REG_LED_PORT,PIN_L1_PORT); // Apago el LED
	}
}

/////////LAVADO/////////

void setup_lv(){
	setBit (REG_M2_en_DDR, PIN_M2_en_DDR);
	clearBit(REG_M2_en_PORT, PIN_M2_en_PORT); //por defecto apagado
}
volatile uint32_t modo_lavado=0; 
void lavadoV_on(){
	modo_lavado=1; 
	//setBit(REG_M2_en_PORT, PIN_M2_en_PORT);
}

void lavadoV_off(){
	modo_lavado=0; 
}

void lavadovertical(){
	switch (modo_lavado){
		case 0:		//lavado apagado
			clearBit(REG_M2_en_PORT, PIN_M2_en_PORT); // Apago motor lavado, nos aseguramos que para
		break;
		
		case 1:		//lavado encencido
			setBit(REG_M2_en_PORT, PIN_M2_en_PORT);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////

int main(){
	setupTimers(); 
	setup_barrera();
	setup_luz();
	setup_lv();
	sei();
	while(1){
		// Prueba 1 Barrera
		static uint8_t enable_aux = 1; // Variable estática auxiliar
		if(bandera_coche == 1){
			openbarrera();
			lavadoV_on(); 		// CAMBIO MODO DE MAQUINA DE ESTADOS "ABRIR" - Solo se ejecuta una vez
			enable_aux = 0;
			
		} 
		
				// MAQUINA DE ESTADOS: MIRAR COMO FUNCIONA - Necesita su ejecución de forma cíclica
		
		
		// Prueba 2 Barrera
		
		if(bandera_coche == 2){
			closebarrera();		// CAMBIO MODO DE MAQUINA DE ESTADOS "CERRAR" - Solo se ejecuta una vez
			enable_aux = 0;
			lavadoV_off(); 
		}
		
		
	
		
		//Prueba LED1
//  		modo_led1 = 1; //Parpadeo corto
//  		control_LED1(modo_led1);
// 		delay_seconds(2);
		/*
		modo_led1 = 0; //Parpadeo largo
		control_LED1(modo_led1);
		delay_seconds(20);
		setup(){
			modo_leds1=0; 
		}
		while(1)
		if(SO1 detecta coche){
			modo_led1=1; 
			
		}
		control_LED1(modo_led1); 
		cuando deje de funcionar la maqueta(modo_led1=0)
		*/
		
		control_LED1(modo_led1); 
		barrera(); 
		lavadovertical(); 
	}
}
