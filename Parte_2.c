#include "Parte_2.h"

// VARIABLES

// FUNCIONES (Inicialización, Uso...)

// FUNCIONES LUZ (L1)

void setup_luz(){
	setBit(REG_LED_DDR, PIN_L1_DDR); 	//Comentario Carlos (CC): debes de configurar solo tus pines. Si otra persona usa el puerto L, le estás cambiando la configuración de sus pines. puedes Usar macros de General.h (setBit, clrBit)
	setBit(REG_LED_PORT, PIN_L1_PORT);

	// cli();												//deshabilito las interrupciones globales
	// TCCR5A= 0x00;										//configurar CTC
	// TCCR5B = (1 << WGM52) | (1 << CS51) | (1 << CS50);	//Preescalador de 64  									//CC: No olvides "|", TCCR5B |= (1 << WGM52) | (1 << CS51) | (1 << CS50);
	// OCR1A =62500-1;										//configuro el numero de ciclos a contar
	// TIMSK5 = (1 << OCIE0A);								//habilito la mascara de la interrupcion 			//CC: No olvides "|", sería TIMSK5 |= (1 << OCIE0A);
	// TIFR5 = (1 << OCF5A);								//habilito la bandera de la interrupcion			//CC: TIFR5 |= (1 << OCF5A);
	// sei();												//hbailito las interrupciones globales
}

volatile uint8_t modo; //REVISAR
void control_L1 (uint8_t modo_parametro){ // Se usará en la integración							//CC: En setup_luz() configuras el timer5 cada 0.5s y en control_L1() no se usa el timer5. Consejo: Cada vez que entre al case reconfiguro el timer5 para cumplir con el parpadelo
 	//if (regModoL1 != modo){ //Solo actualizo cuando cambia el modo
 		if(modo_parametro == 0){
 			if(seconds()%10==0){
 				while(millis()%500!=0){
 					setBit(REG_LED_PORT, PIN_L1_PORT);	//PORTL= 0x02;
 				}
 				clearBit(REG_LED_PORT, PIN_L1_PORT);//PORTL= 0x00;
 			}
		}
 		else {
 			if(millis()%500==0){
				toggleBit(REG_LED_PORT, PIN_L1_PORT); 
 					// if(PINL0==1){
 					// 	clearBit(REG_LED_PORT, PIN_L1_PORT); //PORTL= 0x00;
 					// } else{
 					// 	setBit(REG_LED_PORT, PIN_L1_PORT);//PORTL=0x02;
 					// }
 			}
		}
 	
 	//regModoL1 = modo;
}



// FUNCIONES BARRERA

void setup_barrera(){
	setBit(REG_M1_en_DDR, PIN_M1_en_DDR);													//CC: debes de configurar solo tus pines. Si otra persona usa el puerto L, le estás cambiando la configuración de sus pines. puedes Usar macros de General.h (setBit, clrBit)
	clearBit(REG_M1_en_PORT, PIN_M1_en_PORT);
	clearBit(REG_SOB_DDR, PIN_SO1_DDR);											//CC: debes de configurar solo tus pines
	clearBit(REG_SOL_DDR, PIN_SO2_DDR); 
	DDRB &= ~(1<< DDB0); //PCINT0 como entrada
	cli();
	EIMSK = 0x08;
	EICRA = 11000000;
	PCICR |= setBit(REG_SOB_PIN, PIN_SO1_PIN);	// Habilito grupo de interrupciones en PORTB (por cambio de estado)
	PCMSK0 |= (1<<PCINT0);  //Habilito interrupción en pin PCINT0
	sei();			//Habilito interrupciones globales
	
	  
}

volatile int cuenta=0; //cuenta es una variable que cuenta los flancos que se producen en SW1
void barrera(){
	if(isClrSet(REG_SOL_PIN, PIN_SO2_PIN)==1){//si SO2 está a 0(cerrada) accionamos la barrera								//CC: PINL2 es una macro que contiene un "2". Usar is Usar macros de General.h ( isBitSet(Registro, Bit), isClrSet(Registro, Bit) )
		setBit(REG_M1_en_PORT, PIN_M1_en_PORT);
	}
	if(cuenta==3){//cuando se hayan producido 3 flancos de subida, parar la barrera
		clearBit(REG_M1_en_PORT, PIN_M1_en_PORT);
		cuenta=0;
		}
	if(isClrSet(REG_SOL_PIN, PIN_SO3_PIN)){//cuando SO3 detecte, el coche entre en el lavado horizontal, bajar la barrera de nuevo 
		if(isClrSet(REG_SOL_PIN, PIN_SO2_PIN)!=1){//mientras SO2 no esté a 1
			setBit(REG_M1_en_PORT, PIN_M1_en_PORT);//PORTK = 0x04;//cerrar la barrera
		}
	} 
}


//lavado vertical

void setup_lv(){
	setBit(REG_M2_en_DDR, PIN_M2_en_DDR); 
	clearBit(REG_M2_en_PORT, PIN_M2_en_PORT); //por defecto apagado
}
//lavado vertical
void lavadoV_on(){
	setBit(REG_M2_en_PORT, PIN_M2_en_PORT);		    
}

void lavadoV_off(){
	clearBit(REG_M2_en_PORT, PIN_M2_en_PORT);
}

ISR(PCINT0_vect){
	if(isClrSet(REG_SOB_PIN, PIN_SO1_PIN)== 1){ //si SO1 está a 0(detecta coche) accionar la barrera
		barrera();
		lavadoV_on();
		modo = 1;
	}
	// if(PIN_SW1_PIN & isBitSet(REG_SW_PIN, PIN_SW1_PIN)){//si se ha producido un flanco de subida, amentamos cuenta
	// 	cuenta++; 
	// }
	else {
		clearBit(REG_M1_en_PORT, PIN_M1_en_PORT);//PORTK = 0x00; //deshabilitar barrera
	}
}

ISR(INT3_VECT){
	cuenta++; 
}
void setup_Parte_2(){
	setup_barrera(); 
	setup_luz(); 
	setup_lv();
	modo = 0;
}


void Parte_2(){
		control_L1(modo); 
 	//if(PIN_SO1_PIN==0){
 		//barrera();//este no se si se puede quitar
		//lavadoV_on();//por establecer un criterio, llamamos a barrera a la vez que llamamos a lavado vertical
 	} 
	if(PIN_SO5_PIN==0){
		lavadoV_off(); 
	}
		
}


