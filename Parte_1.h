#include "General.h"

#ifndef PARTE_1_H_
#define PARTE_1_H_

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
