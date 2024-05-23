#include "General.h"

#ifndef PARTE_2_H_
#define PARTE_2_H_
 

// BARRERA
void setup_barrera(void);	// Inicialización
void openbarrera(void);
void closebarrera(void);
void stopbarrera(void);
void barrera(void);	// Loop


// LUZ (L1)
void setup_luz(void);
void control_LED1(void);

// LAVADO VERTICAL

void setup_lv(void);
/*
void lavadoV_on(void);
void lavadoV_off(void);
*/
void lavadovertical(uint8_t modo_lavado);

// COMÚN
void setup_Parte2(void);
void Parte_2(void);

#endif /* PARTE_2_H_ */