/*
 * Parte_1.c
 *
 * Created: 22/05/2024 16:51:05
 * Author : Grupo2Lavadero
 */ 

#include "Parte_1.h"

// FUNCIÓN GLOBAL

// Variables globales - Parte 1
extern volatile uint8_t enable_check_parte1;
extern volatile uint8_t NumCarLavado;	// Cuenta coches en zona lavado

	// Variables Lavadero Horizontal
	volatile uint8_t aux_lavH = 0;
		// Variables globales - Lavadero Horizontal
	extern volatile uint8_t so3;
	extern volatile uint8_t so4;
	extern volatile uint8_t so5;
	extern volatile uint8_t so3p;
	extern volatile uint8_t so4p;
	extern volatile uint8_t so5p;
	extern volatile uint8_t limit_switch_lavH;	//'1' SW2 detecta rodillo en un extremo

	// Variables Secado
	volatile uint8_t aux_sec = 0;

		// Variables globales - Secado
	extern volatile uint8_t so7;
	extern volatile uint8_t so8;
	extern volatile uint8_t so9;
	extern volatile uint8_t so7p;
	extern volatile uint8_t so8p;
	extern volatile uint8_t so9p;
	extern volatile uint8_t aux_se;
	extern volatile uint8_t limit_switch_sec;	//'1' SW3 detecta rodillo en un extremo


////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////
////// LAVADERO HORIZONTAL ///////
//////////////////////////////////

void setup_LavHorizontal(){	// Inicialización
	// Motor 3: Altura rodillo H
	setBit(REG_M3_en_DDR,PIN_M3_en_DDR);	// Definir como salida
	setBit(REG_M3_di_DDR,PIN_M3_di_DDR);
	setBit(REG_M3_en_PORT,PIN_M3_en_PORT);	// Subir rodillo
	clearBit(REG_M3_di_PORT,PIN_M3_di_PORT);
	
	// Motor 4: Giro rodillo
	setBit(REG_M4_en_DDR,PIN_M4_en_DDR);		// Definir como salida
	clearBit(REG_M4_en_PORT,PIN_M4_en_PORT);	// Apagado de inicio
}


	// LAVADERO HORIZONTAL - ALTURA

void up_LavHorizontal(){
	setBit(REG_M3_en_PORT,PIN_M3_en_PORT);
	setBit(REG_M3_di_PORT,PIN_M3_di_PORT);
}

void down_LavHorizontal(){
	setBit(REG_M3_en_PORT,PIN_M3_en_PORT);
	clearBit(REG_M3_di_PORT,PIN_M3_di_PORT);
}

void stop_AlturaH(){
	clearBit(REG_M3_en_PORT,PIN_M3_en_PORT);
}


	// LAVADERO HORIZONTAL - GIRO

void on_LavHorizontal(){
	setBit(REG_M4_en_PORT,PIN_M4_en_PORT);
}

void off_LavHorizontal(){
	clearBit(REG_M4_en_PORT,PIN_M4_en_PORT);
}

void lavaderoHorizontal(){
		
	if((so3p==so3) && (so4p==so4) && (so5p==so5)){  // Si los valores son los mismos que en instante anterior
		aux_lavH = 1;	// Muevo el rodillo
	}
	else{
		aux_lavH = 0;	// No hago nada
	}
	
	if (limit_switch_lavH == 1 && isBitSet(REG_M3_en_PORT,PIN_M3_en_PORT)){		// Devuelve '1' si detecta fin de carrera Y si el motor esta encendido
		off_LavHorizontal();						// Deja de girar el rodillo
		toggleBit(REG_M3_di_PORT,PIN_M3_di_PORT);	// Cambia el sentido del motor
		stop_AlturaH();								// Se para el rodillo
	}
	
	if(aux_lavH){	// Si los valores de los sensores son los mismos que en instante anterior (antirrebotes)
		if ((so4==0||so5==0) && so3!=0){	// Detecta abajo pero no a los lados
			stop_AlturaH();
			on_LavHorizontal();
		} else if(so3==0){	// Detecta alguno de los lados
			up_LavHorizontal();	// Sube el rodillo
			on_LavHorizontal();	// Empieza a girar el rodillo
		} else if(limit_switch_lavH==0){	// No detecta nada
			down_LavHorizontal();
			on_LavHorizontal();
		} else{
			off_LavHorizontal();
		}
	}
	so3p = so3;
	so4p = so4;
	so5p = so5;
}


////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////
////// FUNCIONES SECADO ///////
///////////////////////////////

void setup_secado(){
	// Motor 5: Altura secador
	setBit(REG_M5_en_DDR,PIN_M5_en_DDR);
	setBit(REG_M5_di_DDR,PIN_M5_di_DDR);
	setBit(REG_M5_en_PORT,PIN_M5_en_PORT);
	clearBit(REG_M5_di_PORT,PIN_M5_di_PORT);
}

void up_secado(){
	setBit(REG_M5_en_PORT,PIN_M5_en_PORT);
	setBit(REG_M5_di_PORT,PIN_M5_di_PORT);
}

void down_secado(){
	setBit(REG_M5_en_PORT,PIN_M5_en_PORT);
	clearBit(REG_M5_di_PORT,PIN_M5_di_PORT);
}

void stop_secado(){
	clearBit(REG_M5_en_PORT,PIN_M5_en_PORT);
}

void secado(){

	if((so7p==so7) && (so8p==so8) && (so9p==so9)){  // Si los valores son los mismos que en instante anterior
		aux_sec = 1;	// Muevo el secador
	}
	else{
		aux_sec = 0;	// No hago nada
	}
	
	if(aux_sec) {
		if( so8 && so7 && so9)	// No detecta abajo
			down_secado();	// Baja
		else if( so8 || !(so7 && so9) )
			up_secado();
		else if( !(so8 || (so7 && so9)) )	// Detecta abajo y en algun lateral
			up_secado();	// Sube
		else if( !so8 && so7 && so9 )	// Detecta abajo pero no en los laterales
			stop_secado();	// Permanece quieto
		else
			stop_secado();
	}
	so7p = so7;
	so8p = so8;
	so9p = so9;
}


////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////
////// COMÚN ///////
////////////////////

void setup_Parte1(){
	cli();
	setup_LavHorizontal();
	setup_secado();
	sei();
}

void Parte_1(){
	if (enable_check_parte1){
		lavaderoHorizontal();
		secado();
		enable_check_parte1 = 0;
	}
	if (NumCarLavado){
		on_LavHorizontal();
	} 	else off_LavHorizontal();
}