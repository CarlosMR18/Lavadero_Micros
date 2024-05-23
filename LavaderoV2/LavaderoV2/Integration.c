#include "Integration.h"

// Variables globales
extern volatile uint8_t Stop; // '1' Bloque maqueta

extern volatile uint32_t s; // Cuenta segundos
extern volatile uint32_t ms; // Cuenta milisegundos

extern volatile uint8_t NumCarLavado; // Cuenta coches en zona lavado
extern volatile uint8_t NumCarSecado; // Cuenta coches en zona secado
//volatile uint8_t WrongExitCar = 0;		// Un coche ha entrado por la salida


// Variables globales - Parte 1
volatile uint8_t enable_check_parte1 = 0;
	// Variables globales - Lavadero Horizontal
volatile uint8_t so3 = 1;
volatile uint8_t so4 = 1;
volatile uint8_t so5 = 1;
volatile uint8_t so3p = 1;
volatile uint8_t so4p = 1;
volatile uint8_t so5p = 1;

volatile uint8_t limit_switch_lavH = 0; //'1' SW2 detecta rodillo en un extremo

	// Variables globales - Secado
volatile uint8_t so7 = 1;
volatile uint8_t so8 = 1;
volatile uint8_t so9 = 1;
volatile uint8_t so7p = 1;
volatile uint8_t so8p = 1;
volatile uint8_t so9p = 1;

volatile uint8_t limit_switch_sec = 0;        //'1' SW3 detecta rodillo en un extremo

// Variables globales - Parte 2


// Variables globales - Parte 3
volatile uint8_t EnableEntrance; // '1' permite nuevas entradas de vehículos y '0' no permite
volatile uint8_t enable_prove_new; // '1' pendiente comprobar entradas de vehículos
volatile uint32_t cnt_prove_new; // Cuenta comprobar entradas de vehículos

// Variables integración
volatile uint8_t reg_SO1 = 1; // Almacena valor de SO1
volatile uint8_t reg_SO3 = 1; // Almacena valor de SO3
volatile uint8_t reg_SO6 = 1; // Almacena valor de SO6
volatile uint8_t reg_SO10 = 1; // Almacena valor de SO10
volatile uint8_t reg_SO12 = 1; // Almacena valor de SO12

// Funciones

	// Sensores: SO1, SO3, SO6, SO10, SO11 y SO12	 *********	Comprobar si faltan	*********

 void setup_sensores_integration(){
	 /*
	 cli();
	 //SO1 [SOB] PB0(PCINT0)
	 clearBit(REG_SOB_DDR, PIN_SO1_DDR); // Entrada
	 // Configuro interrupción por cambio PCINT0
	 setBit(PCMSK0, PCINT0); // Habilito mascara interrupción
	 setBit(PCICR, PCIE0); // Habilito registro interrupción
	 setBit(PCIFR, PCIF0); // Borro bandera

	 //SO3 [SOB] PB1(PCINT1)
	 clearBit(REG_SOB_DDR, PIN_SO3_DDR); // Entrada
	 // Configura interrupción por cambio PCINT1 junto PCINT0
	 setBit(PCMSK0, PCINT1); // Habilito mascara interrupción
	 
	 //SO6 [SOB] (PCINT4)
	 clearBit(REG_SOB_DDR, PIN_SO6_DDR); // Entrada
	 // Configura interrupción por cambio PCINT4 junto PCINT0
	 setBit(PCMSK0, PCINT4); // Habilito mascara interrupción

	 //SO10 [SOB] PB5
	 clearBit(REG_SOB_DDR, PIN_SO10_DDR); // Entrada

	 //SO11 [SOD] PD5
	 clearBit(REG_SOD_DDR, PIN_SO11_DDR); // Entrada

	
	 
	 sei();*/
	 
	 cli();
	 //SO1 [SOB] PB0(PCINT0)
	 clearBit(REG_SOB_DDR, PIN_SO1_DDR); // Entrada
	 // Configuro interrupción por cambio PCINT0
	 setBit(PCMSK0, PCINT0); // Habilito mascara interrupción
	 setBit(PCICR, PCIE0); // Habilito registro interrupción
	 setBit(PCIFR, PCIF0); // Borro bandera

	 //SO3 [SOB] PB1(PCINT1)
	 clearBit(REG_SOB_DDR, PIN_SO3_DDR); // Entrada
	 // Configura interrupción por cambio PCINT1 junto PCINT0
	 setBit(PCMSK0, PCINT1); // Habilito mascara interrupción
	 
	 //SO6 [SOB] (PCINT4)
	 clearBit(REG_SOB_DDR, PIN_SO6_DDR); // Entrada
	 // Configura interrupción por cambio PCINT4 junto PCINT0
	 setBit(PCMSK0, PCINT4); // Habilito mascara interrupción

	 //SO10 [SOB] PB5
	 clearBit(REG_SOB_DDR, PIN_SO10_DDR); // Entrada
	 // Configura interrupción por cambio PCINT5 junto PCINT0
	 setBit(PCMSK0, PCINT5); // Habilito mascara interrupción

	 //SO12 [SOB] PB2 (PCINT2)
	 clearBit(REG_SOB_DDR, PIN_SO12_DDR); // Entrada
	 // Configura interrupción por cambio PCINT2 junto PCINT0
	 setBit(PCMSK0, PCINT2); // Habilito mascara interrupción
	 
	  //SO12 [SOB] PB2 (PCINT2)
	  clearBit(REG_SOB_DDR, PIN_SO12_DDR); // Entrada
	  // Configura interrupción por cambio PCINT2 junto PCINT0
	  setBit(PCMSK0, PCINT2); // Habilito mascara interrupción
	 
	 sei();
	 
 }
 
  ////////////////////////////////////////////////////////////////////////////////////////////////

  ///////////////////
  ////// COMÚN///////
  ///////////////////
  
   void setup_Integration(){
	   setup_sensores_integration();
   }
 
 
 ////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

 ////////////////////////////////////////////////
 ////// INTERRUPCIONES TIMERS INTEGRACIÓN ///////
 ////////////////////////////////////////////////
 
 ISR(TIMER4_COMPA_vect){ // Segundos
	 s++;
	 
	 if ((s - cnt_prove_new > Tiempo_prove_new) && NumCarLavado > 0 && enable_prove_new == 1){
		 NumCarLavado = 0;
		 EnableEntrance = 1;
		 enable_prove_new = 0;
	 }
 }

 ISR(TIMER3_COMPA_vect){ // Milisegundos
	 ms++;
	 
	 if(ms % Check_height_sensors == 0){
		enable_check_parte1 = 1;
		
		// Actualizo señales - Lavadero Horizontal
		so3 = isBitSet(REG_SOB_PIN,PIN_SO3_PIN);
		so4 = isBitSet(REG_SOK_PIN,PIN_SO4_PIN);
		so5 = isBitSet(REG_SOK_PIN,PIN_SO5_PIN);
		limit_switch_lavH = isClrSet(REG_SW_PIN,PIN_SW2_PIN); // isClrSet porque SW2 '0' al detectar
		 
		// Actualizo señales - Secado
		so7 = isBitSet(REG_SOB_PIN,PIN_SO7_PIN);
		so8 = isBitSet(REG_SOK_PIN,PIN_SO8_PIN);
		so9 = isBitSet(REG_SOK_PIN,PIN_SO9_PIN);
		limit_switch_sec = isClrSet(REG_SW_PIN,PIN_SW3_PIN); // isClrSet porque SW3 '0' al detectar
	 }
 }

 ////////////////////////////////////////////////////////////////////////////////////////////////

 //////////////////////////////////////
 ////// INTERRUPCIONES SENSORES ///////
 //////////////////////////////////////
 
 ISR(INT0_vect){ //Parada de emergencia
	 // SW4 al soltar boton [PD0] (INT0)
	 ParadaEmergencia();
	 
 }
 
 ISR(PCINT0_vect){
	 
	 //SO1 [SOB] (PCINT0)
	 if (isBitSet(REG_SOB_PIN,PIN_SO1_PIN) && reg_SO1 == 0){ // Flanco subida
		 reg_SO1 = 1; // Actualizo registro SO1 con valor actual
	
	 }
	 else if (isClrSet(REG_SOB_PIN,PIN_SO1_PIN) && (reg_SO1 == 1) && (EnableEntrance == 1)){ // Flanco bajada y entrada habilitada
		 incNumCarLavado();		// Ha entrado coche
		 enable_prove_new = 1; // Activo comprobación entrada
		 cnt_prove_new = s;
		 EnableEntrance = 0;
		 openbarrera();  
		 
		 
		 reg_SO1 = 0; // Actualizo registro SO1 con valor actual
	 }
	 //SO3 [SOL] *PB1*(*PCINT1*) -- MODIFICACIÓN PROPUESTA POR NACHO :) --
	 else if (isClrSet(REG_SOB_PIN,PIN_SO3_PIN) && reg_SO3 == 1 && enable_prove_new == 1 && (s - cnt_prove_new < Tiempo_prove_new)){ // Flanco bajada y entrada habilitada
		 enable_prove_new = 0;
		 
		 reg_SO3 = 0; // Actualizo registro SO3 con valor actual
	 }
	 
	 else if (isBitSet(REG_SOB_PIN,PIN_SO3_PIN) && reg_SO3 == 0 ){ // Flanco subida
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
		 if (reg_SO10 == 1){	
			 ParadaEmergencia();	// Caso en que el coche entre por la salida
		 }
	 }
	 
	 //SO10 [SOB] PB5 (PCINT5)
	 else if (isClrSet(REG_SOB_PIN,PIN_SO10_PIN) && reg_SO10 == 1){ // Flanco bajada
		 reg_SO10 = 0; // Actualizo registro SO12 con valor actual
	 }
	 
	 else if (isBitSet(REG_SOB_PIN,PIN_SO10_PIN) && reg_SO10 == 0){ // Flanco bajada
		 reg_SO10 = 1; // Actualizo registro SO12 con valor actual
	 }
	 	 
 }
 
 ////////////////////////////////////////////////////////////////////////////////////////////////

 ///////////////////////////////////
 ////// PARADA DE EMERGENCIA ///////
 ///////////////////////////////////

 void setup_ParadaEmergencia(){ // SW4 al soltar boton
	 
	 clearBit(REG_SW_DDR,PIN_SW4_DDR); //	Entrada
	 
	 setBit(EICRA,ISC00); // Interrupción flanco subida
	 setBit(EICRA,ISC01);
	 
	 setBit(EIMSK,INT0); // Activo mascara interrupción
	 
	 setBit(EIFR,INTF0); // Borro bandera interrupción
 }

 void ParadaEmergencia() {
	cli();
	//setStop();
	
	// Parte 1
	stop_AlturaH();
	off_LavHorizontal();
	stop_secado();
	
	// Parte 2
	stopbarrera();
	
	// Parte 3
	stopCinta();
	off_Semaforo();
	
	while(1) {
		lavadovertical(0);
	}
 }
 
 
