#include "Parte_1.h"

	// FUNCIONES LAVADERO HORIZONTAL
// Filtrado rebotes
ISR(TIMER3_COMPA_vect){ // Milisegundos
    ms++;
    if(ms % Check_height_sensors == 0){   //cada cuanto chequeo los sensores de altura //#define macro en General.h
        so3p = so3;
	so4p = so4;
	so5p = so5;
	so7p = so7;
        so8p = so8;
        so9p = so9;

        //cargo los valores de los sensores de ese instante
	so3 = isBitSet(REG_SOB_PIN,PIN_SO3_PIN);
	so4 = isBitSet(REG_SOK_PIN,PIN_SO4_PIN);
	so5 = isBitSet(REG_SOK_PIN,PIN_SO5_PIN);
        limit_switch_lavH = isClrSet(REG_SW_PIN,PIN_SW2_PIN); // isClrSet porque SW2 '0' al detectar

	so7 = isBitSet(REG_SOB_PIN,PIN_SO7_PIN);
        so8 = isBitSet(REG_SOK_PIN,PIN_SO8_PIN);
        so9 = isBitSet(REG_SOK_PIN,PIN_SO9_PIN);
        limit_switch_sec = isClrSet(REG_SW_PIN,PIN_SW3_PIN); // isClrSet porque SW3 '0' al detectar

	if((so3p==so3) && (so4p==so4) && (so5p==so5)){  //Si los valores son los mismos que en instante anterior
		aux_lavH = 1;		  //muevo el rodillo
	}else{
		aux_lavH = 0;		  //no hago nada
	}

        if((so7p==so7) && (so8p==so8) && (so9p==so9)){  //Si los valores son los mismos que en instante anterior
            aux_sec = 1;          //muevo el secador
           }else{
            aux_sec = 0;          //no hago nada
        }

    }
}


void setup_LavHorizontal(){
	// Motor 3: Altura rodillo H
	setBit(REG_M3_en_DDR,PIN_M3_en_DDR); // Definir como salida
	setBit(REG_M3_di_DDR,PIN_M3_di_DDR);
	setBit(REG_M3_en_PORT,PIN_M3_en_PORT); // Subir rodillo
	clearBit(REG_M3_di_PORT,PIN_M3_di_PORT);
	
	// Motor 4: Giro rodillo 
	setBit(REG_M4_en_DDR,PIN_M4_en_DDR); // Definir como salida
	clearBit(REG_M4_en_PORT,PIN_M4_en_PORT); // Apagado de inicio
}

	// Lavadero Horizontal - Altura
	
void up_LavHorizontal(){
	setBit(REG_M3_en_PORT,PIN_M3_en_PORT);
	setBit(REG_M3_di_PORT,PIN_M3_di_PORT);
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
	// Motor 5: Altura secador
    setBit(REG_M5_en_DDR,PIN_M5_en_DDR);                    // Definir como salida    :: DDRD |= (1 << DDD7)
    setBit(REG_M5_di_DDR,PIN_M5_di_DDR);                    //                        :: DDRB |= (1 << DDB6)
    setBit(REG_M5_en_PORT,PIN_M5_en_PORT);                  // Subir secador          :: PORTD |= (1 << PD7)
    clearBit(REG_M5_di_PORT,PIN_M5_di_PORT);                //                        :: PORTB &= ~ (1 << PB6)

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

        if(aux_sec) {
            if( so8 && so7 && so9)                 // no detecta abajo
                down_secado();                    // baja
            else if( so8 || !(so7 && so9) )
                up_secado();
            else if( !(so8 || (so7 && so9)) )    // detecta abajo y en algun lateral
                up_secado();                    // sube
            else if( !so8 && so7 && so9 )        // detecta abajo pero no en los laterales
                stop_secado();                    // permanece quieto
            else
                stop_secado();
        }
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
