/*
 * main.c
 *
 * Created: 22/05/2024 16:51:05
 * Author : Grupo2Lavadero
 */ 

#include "Integration.h"


int main(void) {
	
	// Setup de todas las partes
	setup_General();
	setup_Integration();
	setup_Parte1();
	setup_Parte2();
	setup_Parte3();
	
	// Loop de cada parte
	while(1) {
		Parte_1();
		Parte_2();
		Parte_3();
	}
	
}
