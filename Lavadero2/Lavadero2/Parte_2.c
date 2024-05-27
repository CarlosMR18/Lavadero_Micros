/*
 * Parte_2.c
 *
 * Created: 22/05/2024 16:51:05
 * Author : Grupo2Lavadero
 */ 

#include "Parte_2.h"


// VARIABLES GLOBALES

extern volatile uint32_t s;		// Cuenta segundos
extern volatile uint32_t ms;	// Cuenta milisegundos

extern volatile uint8_t NumCarLavado;	// Cuenta coches en zona lavado
extern volatile uint8_t NumCarSecado;	// Cuenta coches en zona secado

// VARIABLES BARRERA
volatile uint8_t modo_barrera = 0;
volatile uint32_t cnt_apertura_barrera = 0;	// En millisegundos

// VARIABLES LUZ (L1)
volatile uint8_t timer_ticks = 0;			// Para controlar parpadeo
volatile uint8_t aux_parpadeo_LED1 = 20;	// Inicializado en parpadeo sin coches

// VARIABLES LAVADO VERTICAL
volatile uint8_t modo_lavado = 0;


////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
////////////////////	 BARRERA	////////////////////
////////////////////////////////////////////////////////

void setup_barrera(){
	cli();
	setBit(REG_M1_en_DDR, PIN_M1_en_DDR);		// Como salida M1_en
	clearBit(REG_M1_en_PORT, PIN_M1_en_PORT);	// Inicialmente apagado								
	
	//SO2 [SOK] (PCINT18)
	clearBit(REG_SOB_DDR, PIN_SO1_DDR);			// Entrada
	
	//SW1 -> NO USO, CONTROLO CON TIEMPOS PARA APERTURA

	sei();
	
	// Dejo Barrera Cerrada de incicio
	while(isClrSet(REG_SOK_PIN,PIN_SO2_PIN) == 0){	// Mientras barrera no abajo, activo motor barrera
		setBit(REG_M1_en_PORT, PIN_M1_en_PORT);
	} clearBit(REG_M1_en_PORT, PIN_M1_en_PORT);		// Apago motor barrera
}

void openbarrera(){
	cnt_apertura_barrera = ms;
	setBit(REG_M1_en_PORT, PIN_M1_en_PORT);	// Encendido motor barrera
	modo_barrera = 1;
}

void closebarrera(){
	setBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Encendido motor barrera
	modo_barrera = 2;	// Cambia estado en barrera()
}

void stopbarrera(){
	modo_barrera = 0;	// Cambia estado en barrera()
}

void barrera(){		// Loop
	
	switch (modo_barrera){
		case 0:		// Barrera parada
			clearBit(REG_M1_en_PORT, PIN_M1_en_PORT);	// Apago motor barrera, nos aseguramos que se para
			break;
			
		case 1:		// Barrera Subir 
			if(ms - cnt_apertura_barrera > Check_apertura_barrera){		
				clearBit(REG_M1_en_PORT, PIN_M1_en_PORT);	// Apago motor barrera
				modo_barrera = 0;	// Cambio a modo 0 (Barrera parada)
			}
			break;
			
		case 2:		// Barrera Bajar
			if (isClrSet(REG_SOK_PIN,PIN_SO2_PIN) == 1){	// Detecto barrera cerrada
				clearBit(REG_M1_en_PORT, PIN_M1_en_PORT);	// Apago motor barrera
				modo_barrera = 0;
			}
			break;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
////////////////////	 LUZ (L1)	////////////////////
////////////////////////////////////////////////////////

void setup_luz(){
	cli();
	setBit(REG_LED_DDR, PIN_L1_DDR);		// Pin del LED como salida
	clearBit(REG_LED_PORT, PIN_L1_PORT);	// LED apagado inicialmente
	
	// Configurar Timer 5 para generar interrupción cada 0.5 segundo
	TCCR5B |= (1 << CS52) | (1 << WGM52);	// Prescaler de 256 y modo CTC
	TCNT5 = 0;					// Inicializar el contador
	OCR5A = 15625-1;			// Valor de comparación para 0.5 segundo
	TIMSK5 |= (1 << OCIE5A);	// Habilitar la interrupción por comparación
	TIFR5 |= (1<<OCF5A);
	sei();
}

void control_LED1(void){
	if (!(NumCarLavado == 0 && NumCarSecado == 0) && aux_parpadeo_LED1 == 20) timer_ticks = 0;		// Cambio modo
	else if ((NumCarLavado == 0 && NumCarSecado == 0) && aux_parpadeo_LED1 == 2) timer_ticks = 0;	// Cambio modo
	
	if (NumCarLavado == 0 && NumCarSecado == 0) { aux_parpadeo_LED1 = 20; }	// Parpadeo largo: Lavadero no funciona (sin coches)
	
	else aux_parpadeo_LED1 = 2;	// Parpadeo corto: Lavadero funciona (hay coches)
	
	
}

ISR(TIMER5_COMPA_vect) {	// Timer solo usado por Parte2 
	
	// Rutina de interrupción del Timer 5
	timer_ticks++;
	if (timer_ticks == aux_parpadeo_LED1) { // 10 o 0.5 segundos
		setBit(REG_LED_PORT,PIN_L1_PORT);	// Enciendo el LED
		timer_ticks = 0;					// Reiniciar el contador
	}
	else if (timer_ticks == 1) {	// 0.5 segundo
		clearBit(REG_LED_PORT,PIN_L1_PORT);	// Apago el LED
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
///////////////////	 LAVADO VERTICAL  //////////////////
////////////////////////////////////////////////////////

void setup_lv(){
	setBit (REG_M2_en_DDR, PIN_M2_en_DDR);
	clearBit(REG_M2_en_PORT, PIN_M2_en_PORT);	// Por defecto apagado
}

void lavadovertical(uint8_t modo_lavado){
	
	if (!modo_lavado){	// Para: No hay Coches
		clearBit(REG_M2_en_PORT, PIN_M2_en_PORT); // Apago motor lavado, nos aseguramos que para
	} 
	else{	// Activa: Hay Coches
		setBit(REG_M2_en_PORT, PIN_M2_en_PORT);
	}
}


//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////
//////////////////	 COMÚN 	//////////////////
//////////////////////////////////////////////

void setup_Parte2(void){
	setup_barrera();
	setup_luz();
	setup_lv();
}

void Parte_2(void){
	barrera();
	control_LED1();
	lavadovertical(NumCarLavado);	
}

