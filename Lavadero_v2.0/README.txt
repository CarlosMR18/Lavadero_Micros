# Lavadero_2

Firmware for the Lavadero model for group 2

##Estructura de entregable
|\_ Entregable
|\_dist Fichero con los archivos que componen el firmware desarrollado
|\_General._
Ficheros asociados a elementos comunes y de apoyo funciones
|\_Parte_1._
Fichero asociado a las funciones del grupo de trabajo 1
|\_Parte_2._
Fichero asociado a las funciones del grupo de trabajo 2
|\_Parte_3._
Fichero asociado a las funciones del grupo de trabajo 3
|\_Integration._
Fichero de integración

##Puertos usados por Partes (J1)
// PARTE 1
SO3	=>	PB1 // (PCINT1)
SO4	=>	PK5 // (PCINT21)
SO5	=>	PK1 // (PCINT17)
SW2	=>	PD1 // (INT1)
M3_di	=>	PL4 // (OC5B)
M3_en	=>	PD4 // (-)
M4_di	=>	PL6 // (-)  		//**no se usa***
M4_en	=>	PD6 // (-)	

SO7 	=>	PB3 // (PCINT3)
SO8	=>	PK7 // (PCINT23)
SO9	=>	PK3 // (PCINT19)
SW3	=>	PD2 // (INT2)
M5_di	=>	PD7 // (-)
M5_en	=>	PB6 // (PCINT6/OC1B)
  
// PARTE 2
SW1	=>	PD3 // (INT3)		//**no se usa***
SO2 	=>	PK2 // (PCINT18)
M1_di	=>	PK0 // (PCINT16)	//**no se usa***
M1_en	=>	PL2 // (-) 
M2_di	=>	PK4 // (PCINT20)	//**no se usa***
M2_en	=>	PK6 // (PCINT22)
L1 	=>	PL1 // (ICP5)

// PARTE 3
M6_di	=>	PL0 // (ICP4)		//**no se usa***
M6_en	=>	PB7 // (PCINT7)
L4 	=>	PL3 // (OC5A)	
L5 	=>	PL5 // (OC5C)	
SW4	=>	PD0 // (INT0)
  
// INTEGRACIÓN
SO1 	=>	PB0 // (PCINT0)
SO6 	=>	PB4 // (PCINT4/OC2A)
SO10 	=>	PB5 // (PCINT5)		
SO11 	=>	PD5 // (-)	//**no se usa***
SO12 	=>	PB2 // (PCINT2)

#
Parte 1:

IGNACIO BASTERRA ESTEBAN-HANZA, 19031

IGNACIO QUESADA PASCUAL, 20270

Parte 2:

INES FERNANDEZ GIL, 19130

MARIA JOSE FERNANDEZ-CORDEIRO PICCIO-MARCHETTI, 20111

Parte 3:

LEYRE CERRILLO FERNANDEZ DE GOBEO, 18071

CARLOS MUÑOZ RUIZ, 18389
