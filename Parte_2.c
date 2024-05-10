#include "Parte_2.h"

// VARIABLES

// FUNCIONES (Inicialización, Uso...)

// FUNCIONES LUZ (L1)

void setup_luz(){
	setbit(REG_LED_DDR, PIN_LED_DDR); 	//Comentario Carlos (CC): debes de configurar solo tus pines. Si otra persona usa el puerto L, le estás cambiando la configuración de sus pines. puedes Usar macros de General.h (setBit, clrBit)
	setbit(REG_LED_PORT, PIN_LED_PORT);
	set(
	// cli();												//deshabilito las interrupciones globales
	// TCCR5A= 0x00;										//configurar CTC
	// TCCR5B = (1 << WGM52) | (1 << CS51) | (1 << CS50);	//Preescalador de 64  									//CC: No olvides "|", TCCR5B |= (1 << WGM52) | (1 << CS51) | (1 << CS50);
	// OCR1A =62500-1;										//configuro el numero de ciclos a contar
	// TIMSK5 = (1 << OCIE0A);								//habilito la mascara de la interrupcion 			//CC: No olvides "|", sería TIMSK5 |= (1 << OCIE0A);
	// TIFR5 = (1 << OCF5A);								//habilito la bandera de la interrupcion			//CC: TIFR5 |= (1 << OCF5A);
	// sei();												//hbailito las interrupciones globales
}


void control_L1 (uint8_t modo){ // Se usará en la integración							//CC: En setup_luz() configuras el timer5 cada 0.5s y en control_L1() no se usa el timer5. Consejo: Cada vez que entre al case reconfiguro el timer5 para cumplir con el parpadelo
	if (regModoL1 != modo){ //Solo actualizo cuando cambia el modo
		switch(modo){
			case 0:
				if(s%10==0){
					while(ms%500!=0){
						setbit(REG_LED_PORT, PIN_L1_PORT);	//PORTL= 0x02;
					}
					clearbit(REG_LED_PORT, PIN_L1_PORT);//PORTL= 0x00;
				}
			break;
			default:
				if(ms%500==0){
					if(PINL0==1){
						clearbit(REG_LED_PORT, PIN_L1_PORT); //PORTL= 0x00;
					} else{
						setbit(REG_LED_PORT, PIN_L1_PORT);//PORTL=0x02;
					}
				}
		}
	}
	regModoL1 = modo;
}


// FUNCIONES BARRERA

void setup_barrera(){
	setbit(REG_M1_en_DDR, PIN_M1_en_DDR);													//CC: debes de configurar solo tus pines. Si otra persona usa el puerto L, le estás cambiando la configuración de sus pines. puedes Usar macros de General.h (setBit, clrBit)
	clearbit(REG_M1_en_PORT, PIN_M1_en_PORT);
	clearbit(REG_SOB_DDR, PIN_SO1_DDR);											//CC: debes de configurar solo tus pines
	clearbit(REG_SOL_DDR, PIN_SO2_DDR); 
	DDRB &= ~(1<< DDB0); //PCINT0 como entrada
	cli();
	PCICR |= (1<<PCIB0);	// Habilito grupo de interrupciones en PORTB (por cambio de estado)
	PCMSK0 |= (1<<PCINT0);  //Habilito interrupción en pin PCINT0
	sei();			//Habilito interrupciones globales
	
	  
}

void barrera(){
	if(isClrSet(REG_SOL_PIN, PIN_SO2_PIN)==1){								//CC: PINL2 es una macro que contiene un "2". Usar is Usar macros de General.h ( isBitSet(Registro, Bit), isClrSet(Registro, Bit) )
		setbit(REG_M1_en_PORT, PIN_M1_en_PORT)//PORTK = 0x04; 
	}
}

ISR(PCINT0_vect){
	if(isClrSet(REG_SOB_PIN, PIN_SO1_PIN)== 1){ 
		barrera();
	}
	else {
		clearbit(REG_M1_en_PORT, PIN_M1_en_PORT);//PORTK = 0x00; //deshabilitar barrera
	}
}

//lavado vertical

void setup_lv(){
	setbit(REG_M2_en_DDR, PIN_M2_en_DDR); 
	clearbit(REG_M2_en_PORT, PIN_M2_en_PORT); //por defecto apagado
}
//lavado vertical
void lavadoV_on(){
	setbit(REG_M2_en_PORT, PIN_M2_en_PORT);		    
}

void lavadoV_off(){
	clearbit(REG_M2_en_PORT, PIN_M2_en_PORT);
}

void setup_Parte_2(){
	setup_barrera(); 
	setup_luz(); 
	setup_lv();
}

void Parte_2(){
	while(1){
		control_L1(); 
	}
	if(PIN_SO1_PIN==0){
		barrera();
	}
	delay_ms(100);
	lavadoV_on(); 
	if(PIN_S05_PIN==0){
		lavadoV_off(); 
	}
		
}

int contador_ms;				//CC: puedes usar millis() 
int main(void)					//CC: PARTE_2 NO TIENE MAIN, PREPARAR UNA FUNCIÓN "setup_Parte_2()" que contenga todos los setup y "Parte_2()" PARA INCLUIRLO EN EL while del main;
{
	setup_Parte_2();
	/* Replace with your application code */
	while (1)
	{
		parte2(); 
		// if(SO1==1){
		// 	if(contador_ms==10){
		// 		encendido==1;%si el sensor detecta se pone bandera a 1, bandera vale 0 al principio y cunado apaguemos.
		// 		contador_ms==0; 
		// 	} 
  //       	}
		// contador_ms++; %para controlar cuanto tiempo despues entra el siguiente coche
  //       	control_L1(); 
	}
}
