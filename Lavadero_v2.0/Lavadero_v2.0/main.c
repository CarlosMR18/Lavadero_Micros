/*
 * Maqueta_presencial_PARTE_3.c
 *
 * Created: 08/05/2024 21:57:40
 * Author : usuario
 */ 

#include "Integration.h"


// Funciones de interrupción
/*
ISR(TIMER1_COMPA_vect){ // Segundos
	s++;
	if(enable_prove_new == 1) {		//--------------------------------------------------------------------------------------------------------NUEVO LEYRE
		cnt_prove_new++;
	}
	if (cnt_prove_new > Tiempo_prove_new && NumCarLavado > 0 && enable_prove_new == 0){
		NumCarLavado--;
		EnableEntrance = 1;
		enable_prove_new = 0;
	}
}*/
/*
ISR(TIMER3_COMPA_vect){ // Milisegundos
	ms++;
}
*/
// IDK
/*
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
		//moveCinta(); //--------------------------------------------------------------------------------------------------------------- Mejor ponerlo por consulta periódica en main???
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
	
}*/





/*
// Inicialización
void setup_Parte3(){
	cli();
	setupCinta(); // Cinta de arrastre
	setup_leds(); // LEDs (Estado maqueta y Semaforo)
	setup_ParadaEmergencia(); // Parada de emergencia
	setup_sensores_parte3();
	sei();
}
*/

int main(void) {
	setup_General();
	setup_Parte1();
	setup_Parte2();
	setup_Parte3();
	setup_Integration();

	while(1) {
		Parte_1();
		Parte_2();
		Parte_3();
		/*
		if (getNumCarLavado() == 1 && getNumCarSecado() == 0){	// Ningún vehículo
			setBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Encendido
			clearBit(REG_LED_PORT, PIN_L5_PORT);	// RED: Apagado
		}
		else if (getNumCarLavado() == 0 && getNumCarSecado() == 1){	// Ningún vehículo
			clearBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Apagado
			setBit(REG_LED_PORT, PIN_L5_PORT);	// RED: Encendido
		}
		else if (getNumCarLavado() == 0 && getNumCarSecado() == 0){	// Ningún vehículo
			clearBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Apagado
			clearBit(REG_LED_PORT, PIN_L5_PORT);	// RED: Apagado
		} else {
			setBit(REG_LED_PORT, PIN_L4_PORT);	// GREEN: Encendido
			setBit(REG_LED_PORT, PIN_L5_PORT);	// RED: Encendido
		}
		
	//	if (isClrSet(REG_SOB_PIN,PIN_SO1_PIN) && reg_SO1 == 1 && EnableEntrance == 1){ // Flanco bajada y entrada habilitada
	//		NumCarLavado++;		// Ha entrado coche
	//	}*/
		
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