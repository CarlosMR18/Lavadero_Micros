#include "General.h"

#ifndef PARTE_1_H_
#define PARTE_1_H_

volatile uint32_t ms = 0;

// Variables globales - Lavadero Horizontal
volatile uint8_t so3 = 1;
volatile uint8_t so4 = 1;
volatile uint8_t so5 = 1;
volatile uint8_t so3p = 1;
volatile uint8_t so4p = 1;
volatile uint8_t so5p = 1;
volatile uint8_t aux_lavH = 0;
volatile uint8_t limit_switch_lavH = 0; //'1' SW2 detecta rodillo en un extremo

// Variables globales - Secado
volatile uint8_t so7 = 1;
volatile uint8_t so8 = 1;
volatile uint8_t so9 = 1;
volatile uint8_t so7p = 1;
volatile uint8_t so8p = 1;
volatile uint8_t so9p = 1;
volatile uint8_t aux_sec = 0;
volatile uint8_t limit_switch_sec = 0;        //'1' SW3 detecta rodillo en un extremo

// Lavadero Horizontal
void setup_LavHorizontal(void);

void up_LavHorizontal(void); // Altura rodillo
void down_LavHorizontal(void);
void stop_AlturaH(void);

void on_LavHorizontal(void); // Giro rodillo
void off_LavHorizontal(void);

void lavaderoHorizontal(void); //Actualización Lavadero Horizontal

// Secadero
void setup_secado(void);

void up_secado(void);
void down_secado(void);
void stop_secado(void);

void secado(void); //Actualización Secado

// Común Parte_1
void setup_Parte1(void);
void Parte_1(void);

#endif /* PARTE_1_H_ */
