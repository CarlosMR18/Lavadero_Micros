
#include "Parte_2.h"


// VARIABLES GLOBALES
extern volatile uint8_t NumCarLavado; // Cuenta coches en zona lavado
extern volatile uint8_t NumCarSecado; // Cuenta coches en zona secado

// VARIABLES BARRERA
// volatile uint8_t reg_SO1 = 1; // Almacena valor de SO1
// volatile uint8_t reg_SO2 = 1; // Almacena valor de SO2
// volatile uint8_t barrera_cerrada=0; // NO SE USA, SOBRA
volatile uint8_t modo_barrera = 0;
volatile uint32_t cnt_apertura_barrera = 0; // En millisegundos

// VARIABLES LUZ (L1)
volatile uint8_t timer_ticks = 0; // Para controlar parpadeo
volatile uint8_t aux_parpadeo_LED1 = 20; // Inicializado en parpadeo sin coches

// VARIABLES LAVADO VERTICAL
volatile uint8_t modo_lavado = 0;

////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
////////////////////	 BARRERA	////////////////////
////////////////////////////////////////////////////////


void setup_barrera(){		//************ REVISAR MACROS ************
	cli();
	setBit(REG_M1_en_DDR, PIN_M1_en_DDR); // Como salida M1_en
	clearBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Inicialmente apagado								
	//clearBit(REG_M1_di_DDR, PIN_M1_di_DDR);	// Sentido giro
	
	//SO2 [SOK] (PCINT18)
	clearBit(REG_SOB_DDR, PIN_SO1_DDR); // Entrada
	// Configuro interrupción por cambio PCINT18
	setBit(PCMSK2, PCINT18); // Habilito mascara interrupción
	setBit(PCICR, PCIE2); // Habilito registro interrupción
	setBit(PCIFR, PCIF2); // Borro bandera
	
	//SW1 -> NO USO, CONTROLO CON TIEMPOS PARA APERTURA

	sei();
	
	// Dejo Barrera Cerrada de incicio
	while(isClrSet(REG_SOK_PIN,PIN_SO2_PIN) == 0){		// Mientras barrera no abajo, activo motor barrera
		setBit(REG_M1_en_PORT, PIN_M1_en_PORT);
	} clearBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Apago motor barrera
}

void openbarrera(){
	cnt_apertura_barrera = millis();
	setBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Encendido motor barrera
	modo_barrera = 1;	// Cambia estado en barrera()
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
		case 0:		//Barrera parada
			clearBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Apago motor barrera, nos aseguramos que se para
			break;
			
		case 1:		//Barrera Subir 
			// setBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Encendido -- Ya uncluido en openbarrera();
			if(millis() - cnt_apertura_barrera > 1600){		//Usar macro #define Check_apertura_barrera 1200 en Parte_2.h
				clearBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Apago motor barrera
				modo_barrera = 0; // Cambio a modo 0 (Barrera parada)
			}
			break;
			
		case 2:		//Barrera Bajar
			// setBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Encendido -- Ya uncluido en closebarrera();
			if (isClrSet(REG_SOK_PIN,PIN_SO2_PIN) == 1){ // Detecto barrera cerrada
				clearBit(REG_M1_en_PORT, PIN_M1_en_PORT); // Apago motor barrera
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
	setBit(REG_LED_DDR, PIN_L1_DDR); // Pin del LED como salida
	clearBit(REG_LED_PORT, PIN_L1_PORT); //LED apagado inicialmente
	// Configurar Timer 5 para generar interrupción cada 0.5 segundo
	TCCR5B |= (1 << CS52) | (1 << CS50); // Prescaler de 1024
	TCNT5 = 0; // Inicializar el contador
	OCR5A = 3905; // Valor de comparación para 0.5 segundo
	TIMSK5 |= (1 << OCIE5A); // Habilitar la interrupción por comparación
	sei();
}

void control_LED1(void){
	if (NumCarLavado == 0 && NumCarSecado == 0) aux_parpadeo_LED1 = 20; // Parpadeo largo: Lavadero no funciona (sin coches)
	else aux_parpadeo_LED1 = 2; // Parpadeo corto: Lavadero funciona (hay coches)
}

ISR(TIMER5_COMPA_vect) {	//Timer solo usado por Parte2 
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

////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
///////////////////	 LAVADO VERTICAL  //////////////////
////////////////////////////////////////////////////////


void setup_lv(){
	setBit (REG_M2_en_DDR, PIN_M2_en_DDR);
	clearBit(REG_M2_en_PORT, PIN_M2_en_PORT); //por defecto apagado
}

void lavadoV_on(){
	modo_lavado=1;
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
	lavadovertical();	
}

