


#include <avr/io.h>
#include <avr/interrupt.h>


// VARIABLES
volatile uint32_t s = 0; // Cuenta segundos
volatile uint32_t ms = 0; // Cuenta milisegundos

#define REG_M1_en_PORT	PORTL
#define REG_M1_en_DDR	DDRL
#define REG_M1_en_PIN	PINL

#define PIN_M1_en_PORT	PL2
#define PIN_M1_en_DDR	DDL2
#define PIN_M1_en_PIN	PINL2

#define REG_M2_en_PORT	PORTK
#define REG_M2_en_DDR	DDRK
#define REG_M2_en_PIN	PINK

#define PIN_M2_en_PORT	PK6
#define PIN_M2_en_DDR	DDK6
#define PIN_M2_en_PIN	PINK6

#define REG_SW_PORT PORTD
#define REG_SW_DDR  DDRD
#define REG_SW_PIN  PIND
// SW1 => INT3
#define PIN_SW1_PORT PD3
#define PIN_SW1_DDR  DDD3
#define PIN_SW1_PIN  PIND3

#define REG_LED_PORT	PORTL
#define REG_LED_DDR 	DDRL
#define REG_LED_PIN 	PINL
// L1
#define PIN_L1_PORT PL1
#define PIN_L1_DDR  DDL1
#define PIN_L1_PIN  PINL1

// REGISTROS S01-S06-S12 B
#define REG_SOB_PORT PORTB
#define REG_SOB_DDR  DDRB
#define REG_SOB_PIN  PINB
// REGISTROS S02-S03-S04-S010 L
#define REG_SOL_PORT PORTL
#define REG_SOL_DDR  DDRL
#define REG_SOL_PIN  PINL

//REGISTRO S05-S02
#define REG_SOK_PORT PORTK
#define REG_SOK_DDR  DDRK
#define REG_SOK_PIN  PINK

// SO1 => PCINT0
#define PIN_SO1_PORT PB0
#define PIN_SO1_DDR  DDB0
#define PIN_SO1_PIN  PINB0
// SO2
#define PIN_SO2_PORT PK2
#define PIN_SO2_DDR  DDK2
#define PIN_SO2_PIN  PINK2
// SO3
#define PIN_SO3_PORT	PB1
#define PIN_SO3_DDR  	DDB1
#define PIN_SO3_PIN  	PINB1
//SO5
#define PIN_SO5_PORT	PK1
#define PIN_SO5_DDR  	DDK1
#define PIN_SO5_PIN  	PINK1

// FUNCIONES (Inicialización, Uso...)
#define setBit(Registro,Bit)	(Registro |= (1 << Bit))	// pone a 1 el bit B del puerto P
#define clearBit(Registro,Bit)	(Registro &= ~(1 << Bit))	// pone a 0 el bit B del puerto P
#define toggleBit(Registro,Bit) (Registro ^= (1 << Bit))  	// cambia el valor del bit B del puerto P

#define isBitSet(Registro, Bit) (Registro & (1 << Bit))		// devuelve '1' si dicho bit es '1'
#define isClrSet(Registro, Bit) (!isBitSet(Registro, Bit))	// devuelve '1' si dicho bit es '0'

volatile uint8_t modo; //REVISAR
volatile uint8_t barrera_cerrada=0;
volatile int coche=0;
volatile int cuenta=0;

void delay_seconds(uint32_t ds){
	volatile uint32_t delay = s + ds;
	while( s < delay ){}
}


uint32_t millis(void){
	return ms;
}

uint32_t seconds(void){
	return s;
}


// FUNCIONES LUZ (L1)
void setup_luz(){
	setBit(REG_LED_DDR, PIN_L1_DDR); 	//Comentario Carlos (CC): debes de configurar solo tus pines. Si otra persona usa el puerto L, le estás cambiando la configuración de sus pines. puedes Usar macros de General.h (setBit, clrBit)
	setBit(REG_LED_PORT, PIN_L1_PORT);


}


void control_L1 (uint8_t modo_parametro){ // Se usará en la integración							//CC: En setup_luz() configuras el timer5 cada 0.5s y en control_L1() no se usa el timer5. Consejo: Cada vez que entre al case reconfiguro el timer5 para cumplir con el parpadelo
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
			
		}
	}
	
}



// FUNCIONES BARRERA

void setup_barrera(){
	setBit(REG_M1_en_DDR, PIN_M1_en_DDR);													//CC: debes de configurar solo tus pines. Si otra persona usa el puerto L, le estás cambiando la configuración de sus pines. puedes Usar macros de General.h (setBit, clrBit)
	clearBit(REG_M1_en_PORT, PIN_M1_en_PORT);
	clearBit(REG_SOB_DDR, PIN_SO1_DDR);											//CC: debes de configurar solo tus pines
	clearBit(REG_SOK_DDR, PIN_SO2_DDR);
	DDRB &= ~(1<< DDB0); //PCINT0 como entrada
	cli();
	EIMSK = 0x08;
	EICRA = 0xC0;
	//PCICR |= setBit(REG_SOB_PIN, PIN_SO1_PIN);	// Habilito grupo de interrupciones en PORTB (por cambio de estado)
	PCMSK0 |= (1<<PCINT0);  //Habilito interrupción en pin PCINT0
	PCMSK2 |= (1<<PCINT18);
	PCICR |= (1<< PCIE0) | (1<< PCIE2);
	//PCIFR |= (1<< PCIF2) & (1 << PCIF0);
	sei();			//Habilito interrupciones globales
	while(isBitSet(REG_SOK_PIN, PIN_SO2_PIN)==1){
		setBit(REG_M1_en_PORT, PIN_M1_en_PORT);
	}
	clearBit(REG_M1_en_PORT, PIN_M1_en_PORT);
	barrera_cerrada=1;
}

//cuenta es una variable que cuenta los flancos que se producen en SW1
// void barrera(){
// 	if((coche==1) && (millis()%500==0)){
// 		if(isClrSet(REG_SOL_PIN, PIN_SO2_PIN)==1){//si SO2 está a 0(cerrada) accionamos la barrera								//CC: PINL2 es una macro que contiene un "2". Usar is Usar macros de General.h ( isBitSet(Registro, Bit), isClrSet(Registro, Bit) )
// 			setBit(REG_M1_en_PORT, PIN_M1_en_PORT);
// 		}
// 		if(cuenta==3){//cuando se hayan producido 3 flancos de subida, parar la barrera
// 			clearBit(REG_M1_en_PORT, PIN_M1_en_PORT);
// 			cuenta=0;
// 		}
// 	}
//
// 	if(flanco de subida s03){//cuando SO3 detecte, el coche entre en el lavado horizontal, bajar la barrera de nuevo//poner una bandera para cuadno SO3 detecte el culo del coche
// 		if(isClrSet(REG_SOL_PIN, PIN_SO2_PIN)!=1){//mientras SO2 no esté a 1
// 			setBit(REG_M1_en_PORT, PIN_M1_en_PORT);//PORTK = 0x04;//cerrar la barrera
// 		}
// 	}
// }

void openbarrera(){
	if(barrera_cerrada==1){
		//while(cuenta<3){
		setBit(REG_M1_en_PORT, PIN_M1_en_PORT);
		//}
	}
}

void closebarrera(){
	while(barrera_cerrada!=1){
		setBit(REG_M1_en_PORT, PIN_M1_en_PORT);
	}
}

void stopbarrera(){
	clearBit(REG_M1_en_PORT, PIN_M1_en_PORT);
}


ISR(PCINT2_vect){
	if(isClrSet(REG_SOK_PORT, PIN_SO2_PIN)==1){//¿FLANCO DE BAJADA?
		barrera_cerrada=1;
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
	if( isClrSet(REG_SOB_PIN, PIN_SO1_PIN)){ //si SO1 está a 0(detecta coche) accionar la barrera
		coche=1;
	}
}

// ISR(INT3_vect){
// 	cuenta++;
// }
void setup_Parte_2(){
	setup_barrera();
	setup_luz();
	setup_lv();
	modo = 0;
}


void Parte_2(){
	control_L1(modo);
	if(coche==1){
		modo=1;
		openbarrera();//este no se si se puede quitar
		delay_seconds(2);
		//while(seconds()%1!=1){
		stopbarrera();
		//}
		delay_seconds(2);
		closebarrera();
		//lavadoV_on();//por establecer un criterio, llamamos a barrera a la vez que llamamos a lavado vertical
		
	}
	// 	if(PIN_SO5_PIN==0){
	// 		lavadoV_off();
	// 	}

}


int main(){
	setup_Parte_2();
	while(1){
		cli();
		
		/*
		//Parte_2();
		openbarrera();//este no se si se puede quitar
		delay_seconds(2);
		//while(seconds()%1!=1){
		stopbarrera();
		//}
		delay_seconds(2);
		closebarrera();*/
	}
	return 0;
}
