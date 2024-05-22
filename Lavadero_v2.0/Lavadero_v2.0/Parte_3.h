#include "General.h"

#ifndef PARTE_3_H_
#define PARTE_3_H_

// CINTA DE ARRASTRE
void setupCinta(void);
void moveCinta(void);
void stopCinta(void);
void cinta(void);

// Semaforo [LED4 y LED5] 
void setup_semaforo(void);
void semaforo(void);

// PARADA DE EMERGENCIA
void setup_ParadaEmergencia(void);
void ParadaEmergencia(void); //Pendiente (Por interrupción)

// COMÚN
void setup_Parte3(void);
void Parte_3(void);	// Loop

#endif /* PARTE_3_H_ */