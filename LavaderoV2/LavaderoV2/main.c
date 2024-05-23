/*
 * LavaderoV2.c
 *
 * Created: 22/05/2024 16:51:05
 * Author : Admin
 */ 

#include "Integration.h"


int main(void) {

	setup_General();
	setup_Integration();
	setup_Parte1();
	setup_Parte2();
	setup_Parte3();
	

	while(1) {
		Parte_1();
		Parte_2();
		Parte_3();
	}
	
}
