#include "Parte_3.h"

// VARIABLES


//////////////////////////////////////////////////////
// FUNCIONES (Inicialización, Uso...)

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
	if(isBitSet(REG_SOL_PORT, PIN_SO10_PIN)){ 
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
