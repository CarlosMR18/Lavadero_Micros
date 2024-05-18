#include "Parte_1.h"

	// FUNCIONES LAVADERO HORIZONTAL

void setup_LavHorizontal(){
	// Motor 3: Altura rodillo H
	setBit(REG_M3_en_DDR,PIN_M3_en_DDR); // Definir como salida
	setBit(REG_M3_di_DDR,PIN_M3_di_DDR);
	setBit(REG_M3_en_PORT,PIN_M3_en_PORT); // Subir rodillo
	setBit(REG_M3_di_PORT,PIN_M3_di_PORT);
	// Motor 4: Giro rodillo H
	setBit(REG_M4_en_DDR,PIN_M4_en_DDR); // Definir como salida
	setBit(REG_M4_di_DDR,PIN_M4_di_DDR);
	clearBit(REG_M4_en_PORT,PIN_M4_en_PORT); // Apagado de inicio
	setBit(REG_M4_di_PORT,PIN_M4_di_PORT); // Sentido giro -- COMPROBAR EN MAQUETA
}

	// Lavadero Horizontal - Altura
	
void up_LavHorizontal(){
	setBit(REG_M3_en_PORT,PIN_M3_en_PORT);
	setBit(REG_M3_di_PORT,PIN_M3_di_PORT); // COMPROBAR EN MAQUETA si es setBit() o clearBit()
}

void down_LavHorizontal(){
	setBit(REG_M3_en_PORT,PIN_M3_en_PORT);
	clearBit(REG_M3_di_PORT,PIN_M3_di_PORT); // COMPROBAR EN MAQUETA si es ~[setBit() o clearBit()]
}

void stop_AlturaH(){
	clearBit(REG_M3_en_PORT,PIN_M3_en_PORT);
}

	// Lavadero Horizontal - Giro
	
void on_LavHorizontal(){
	setBit(REG_M4_en_PORT,PIN_M4_en_PORT);
}

void off_LavHorizontal(){
	clearBit(REG_M4_en_PORT,PIN_M4_en_PORT);
}

void lavaderoHorizontal(){

	if (limit_switch_lavH == 1 && isBitSet(REG_M3_en_PORT,PIN_M3_en_PORT)){  // devuelve '1' si detecta fin de carrera Y si el motor esta encendido
		off_LavHorizontal(); //deja de girar el rodillo
		toggleBit(REG_M3_di_PORT,PIN_M3_di_PORT); // cambia el sentido del motor
		stop_AlturaH(); //se para el rodillo
	}
	
	if(aux_lavH){		//si los valores de los sensores son los mismos que en instante anterior (antirrebotes)
		if (so4==0 && so3!=0){		//detecta abajo pero no a los lados
				stop_AlturaH();
				on_LavHorizontal();
			} else if(so3==0){					//detecta alguno de los lados
				up_LavHorizontal();  //sube el rodillo
				on_LavHorizontal();  //empieza a girar el rodillo
			} else if(limit_switch_lavH==0){												//no detecta nada
				down_LavHorizontal();
				on_LavHorizontal();
			} else{
				off_LavHorizontal();
		}
	}
}

	// FUNCIONES SECADO
	
void setup_secado(){
	setBit(REG_M5_en_DDR,PIN_M5_en_DDR); // Definir como salida
	setBit(REG_M5_di_DDR,PIN_M5_di_DDR);
	setBit(REG_M5_en_PORT,PIN_M5_en_PORT); // Subir
	setBit(REG_M5_di_PORT,PIN_M5_di_PORT);
}

void up_secado(){
	setBit(REG_M5_en_PORT,PIN_M5_en_PORT);
	setBit(REG_M5_di_PORT,PIN_M5_di_PORT); // COMPROBAR EN MAQUETA si es setBit() o clearBit()
}

void down_secado(){
	setBit(REG_M5_en_PORT,PIN_M5_en_PORT);
	clearBit(REG_M5_di_PORT,PIN_M5_di_PORT); // COMPROBAR EN MAQUETA si es ~[setBit() o clearBit()]
}

void stop_secado(){
	clearBit(REG_M5_en_PORT,PIN_M5_en_PORT);
}

void secado(){
	
	if (limit_switch_secado == 1 && isBitSet(REG_M5_en_PORT,PIN_M5_en_PORT)){  // devuelve '1' si detecta fin de carrera Y si el motor esta encendido
		toggleBit(REG_M5_di_PORT,PIN_M5_di_PORT); // cambia el sentido del motor
		if (isBitSet(REG_M5_di_PORT,PIN_M5_di_PORT)){ //el rodillo esta abajo 
			up_secado(); //vuelvo a la posicion inicial(arriba)
		}else{ //el rodillo esta arriba
			stop_secado(); //me quedo en la posicion inicial(arriba)
		}
	}

	if(aux_secado){
		if (secado[1]==0 && (secado[0]==1 || secado[2]==1)){
			stop_secado();
		} else if(secado[0]==0 && secado[1]==0){
			up_secado();
		} else {
	           	down_secado();
	        }
	/*if (limit_switch_secado = 1 && isBitSet(REG_M5_en_PORT,PIN_M5_en_PORT)){
		stop_secado();
		toggleBit(REG_M5_di_PORT,PIN_M5_di_PORT);
	}
	
	if(aux_secado){
		// EQUIVALENTE A LAVADO_H
	}*/
}

void setup_Parte1(){
	cli();
	setup_LavHorizontal();
	setup_secado();
	sei();
}

void Parte_1(){
	lavaderoHorizontal();
	secado();
}
