/*
 * Parte_2.h
 *
 * Created: 22/05/2024 16:51:05
 * Author : Grupo2Lavadero
 */ 

#include "General.h"

#ifndef PARTE_2_H_
#define PARTE_2_H_
 

// BARRERA
void setup_barrera(void);	// Inicializaci�n
void openbarrera(void);
void closebarrera(void);
void stopbarrera(void);
void barrera(void);			// Loop


// LUZ (L1)
void setup_luz(void);
void control_LED1(void);

// LAVADO VERTICAL
void setup_lv(void);
void lavadovertical(uint8_t modo_lavado);

// COM�N
void setup_Parte2(void);
void Parte_2(void);

#endif /* PARTE_2_H_ */