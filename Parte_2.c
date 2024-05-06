#include "Parte_2.h"

// VARIABLES

// FUNCIONES (Inicialización, Uso...)
#define REG_M1_di_PORT	PORTL
		#define REG_M1_di_DDR	DDRL
		#define REG_M1_di_PIN	PINL
		
		#define PIN_M1_di_PORT	PK0
		#define PIN_M1_di_DDR  	DDK0
		#define PIN_M1_di_PIN  	PINK0
			// Enable
		#define REG_M1_en_PORT	PORTK
		#define REG_M1_en_DDR	DDRK
		#define REG_M1_en_PIN	PINK
		
		#define PIN_M1_en_PORT	PK2
		#define PIN_M1_en_DDR	DDK2
		#define PIN_M1_en_PIN	PINK2
		
		// Motor M2 => Rodillos verticales
			// Direction
		#define REG_M2_di_PORT	PORTK
		#define REG_M2_di_DDR	DDRK
		#define REG_M2_di_PIN	PINK
		
		#define PIN_M2_di_PORT	PK4
		#define PIN_M2_di_DDR  	DDK4
		#define PIN_M2_di_PIN  	PINK4
			// Enable
		#define REG_M2_en_PORT	PORTK
		#define REG_M2_en_DDR	DDRK
		#define REG_M2_en_PIN	PINK
		
		#define PIN_M2_en_PORT	PK6
		#define PIN_M2_en_DDR	DDK6
		#define PIN_M2_en_PIN	PINK6

// FUNCIONES LUZ (L1)

void setup_luz(){
	DDRL=10101010; 	//Comentario Carlos (CC): debes de configurar solo tus pines. Si otra persona usa el puerto L, le estás cambiando la configuración de sus pines. puedes Usar macros de General.h (setBit, clrBit)
	PORTL=0X00; 
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
	DDRK= 0X00; 													//CC: debes de configurar solo tus pines. Si otra persona usa el puerto L, le estás cambiando la configuración de sus pines. puedes Usar macros de General.h (setBit, clrBit)
	PORTK=0X00;													//CC: debes de configurar solo tus pines
	
	DDRB &= ~(1<< DDB0); //PCINT0 como entrada
	cli();
	PCICR |= (1<<PCIB0);	// Habilito grupo de interrupciones en PORTB (por cambio de estado)
	PCMSK0 |= (1<<PCINT0);  //Habilito interrupción en pin PCINT0
	sei();			//Habilito interrupciones globales
	
	  
}

void barrera(){
	if(PINL2==1){								//CC: PINL2 es una macro que contiene un "2". Usar is Usar macros de General.h ( isBitSet(Registro, Bit), isClrSet(Registro, Bit) )
		setbit(REG_M1_en_PORT, PIN_M1_en_PORT)//PORTK = 0x04; 
	}
	//delay_seconds(); //configurar segundos para el delay
}

ISR(PCINT0_vect){
	if(PINB0 != 1){ 
		barrera();
	}
	else {
		clearbit(REG_M1_en_PORT, PIN_M1_en_PORT);//PORTK = 0x00; //deshabilitar barrera
	}
}

int contador_ms;				//CC: puedes usar millis() 
int main(void)					//CC: PARTE_2 NO TIENE MAIN, PREPARAR UNA FUNCIÓN "setup_Parte_2()" que contenga todos los setup y "Parte_2()" PARA INCLUIRLO EN EL while del main;
{
	setup_barrera();
	setup_luz();
	/* Replace with your application code */
	while (1)
	{
		if(SO1==1){
			if(contador_ms==10){
				encendido==1;%si el sensor detecta se pone bandera a 1, bandera vale 0 al principio y cunado apaguemos.
				contador_ms==0; 
			} 
        	}
		contador_ms++; %para controlar cuanto tiempo despues entra el siguiente coche
        	control_L1(); 
	}
}



//barrera						
//si SO2==1, BARRERA CERRADA
//    si SO1 detecta un coche, EN=1¿CONTANDO PULSOS?

//lavado vertical
void lavadoV_on(){
	setbit(REG_M2_en_PORT, PIN_M2_en_PORT);		    
}

void lavadoV_off(){
	clearbit(REG_M2_en_PORT, PIN_M2_en_PORT);
}
