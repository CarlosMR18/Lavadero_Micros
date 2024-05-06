#include "General.h"
#include "Parte_1.h"
#include "Parte_2.h"
#include "Parte_3.h"

int main(void) {
    setup_Parte_1();
	setup_Parte_2();
	//setup_Parte_3();
	
	while(1) {
        if(!getStop()){
			Parte_1();
			//Parte_2();
			//Parte_3();
		}
		
    }
}
