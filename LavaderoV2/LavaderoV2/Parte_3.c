#include "Parte_3.h"

// Variables globales
extern volatile uint8_t Stop; // '1' Bloque maqueta

extern volatile uint32_t s; // Cuenta segundos
extern volatile uint32_t ms; // Cuenta milisegundos

extern volatile uint8_t NumCarLavado; // Cuenta coches en zona lavado
extern volatile uint8_t NumCarSecado; // Cuenta coches en zona secado


////////////////////////////////
////// CINTA DE ARRASTRE ///////
////////////////////////////////

void setupCinta(){	//Inicialización
	setBit(REG_M6_di_DDR, PIN_M6_di_DDR); //Configurar el pin de dirección M6 como salida
	setBit(REG_M6_en_DDR, PIN_M6_en_DDR); //Configurar el pin M6 en como salida
	setBit(REG_M6_di_PORT, PIN_M6_di_PORT); //Ajustar el sentido de giro de M6 a la derecha
}

void moveCinta(){
	setBit(REG_M6_en_PORT, PIN_M6_en_PORT);
	setBit(TCCR1A, COM1C1);
}

void stopCinta(){
	clearBit(REG_M6_en_PORT, PIN_M6_en_PORT);
	clearBit(TCCR1A, COM1C1);
}

void cinta(){	//Loop
	if (NumCarLavado == 0 && NumCarSecado == 0){	// Ningún vehículo
		stopCinta();
	}
	else{	// Algún vehículo
		moveCinta();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////
//////////	SEMÁFORO LEDs	 //////////
///////////////////////////////////////

void setup_semaforo(){ //Inicialización Semáforo: LED4 y LED5
	setBit(REG_LED_DDR, PIN_L4_DDR);	// GREEN: Establecer LED 4 pin como salida
	setBit(	REG_LED_DDR, PIN_L5_DDR);	// RED: Establecer LED 5 pin como salida
}

void off_Semaforo() {
	clearBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Apagado
	setBit(REG_LED_PORT, PIN_L5_PORT);		// RED: Encendido
}

void semaforo(){ //LED_4_5
	if (NumCarLavado == 0 && NumCarSecado == 0){	// Ningún vehículo
		clearBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Apagado
		clearBit(REG_LED_PORT, PIN_L5_PORT);	// RED: Apagado
	}
	else{	// Algún vehículo
		
		if(isBitSet(REG_SOB_PIN, PIN_SO10_PIN) && isClrSet(REG_SOD_PIN, PIN_SO11_PIN) && isClrSet(REG_SOB_PIN, PIN_SO12_PIN)){
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

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////
///////		 COMÚN		///////
///////////////////////////////

void setup_Parte3(){
	cli();
	setupCinta();
	setup_semaforo();
	setup_ParadaEmergencia();
	sei();
}

void Parte_3(){	//loop
	cinta();
	semaforo();
}