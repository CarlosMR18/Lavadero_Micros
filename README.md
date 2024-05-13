# Lavadero_Micros
Trabajo Sistemas Microprocesadores 2023/2024 - EQUIPO LAVADERO 2
# Puertos a usar (J1)
	// MOTORES
		M1_di	=>	PK0 // (PCINT16)  //**no se usa***
		M1_en	=>	PL2 // (-)
		M2_di	=>	PK4 // (PCINT20)  //**no se usa***
		M2_en	=>	PK6 // (PCINT22)
  		M3_di	=>	PL4 // (OC5B)
    		M3_en	=>	PD4 // (-)
    		M4_di	=>	PL6 // (-)
      		M4_en	=>	PD6 // (-)
      		M5_di	=>	PB6 // (PCINT6/OC0A/OC1C)
		M5_en	=>	PD7 // (-)  
		M6_di	=>	PB5 // (PCINT5)
		M6_en	=>	PB7 // (PCINT7)
	// SWITCHES
		SW1	=>	PD3 	// (INT3)
		SW2	=>	PD1 	// (INT1)
		SW3	=>	PD2 	// (INT2)
		SW4	=>	PD0 	// (INT0)
	// LED
		L1 	=>	PL1 // (ICP5)
		L4 22 	=>	PL3 // (OC5A)	22
		L5 24	=>	PL5 // (OC5C)	24
		L6 	=>	PL7 // (-)
	// SENSORES	
		SO1 	=>	PB0 // (PCINT0)
		SO2 	=>	PK2 // (PCINT18)
 		SO3	=>	PB1 // (PCINT1)
		SO4	=>	PK5 // (PCINT21)
  		SO5	=>	PK1 // (PCINT17)
		SO6 	=>	PB4 // (PCINT4/OC1B)
  		SO7 	=>	PB3 // (PCINT3)
    		SO8	=>	PK7 // (PCINT23)
      		SO9	=>	PK3 // (PCINT19)
		SO10 	=>	PL0 // (ICP4)
		SO11 	=>	PD5 // (-)
		SO12 	=>	PB2 // (PCINT2/OC2A)
  
# Partición de la maqueta : Lavadero
Parte 1: Control de los rodillos de lavado y secado para seguir los perfiles de los vehículos.

Parte 2: Control de la barrera de entrada, rodillo de lavado vertical y de la luz de indicación de estado (luz periódica con destellos cada 30s o intermitente cada segundo).

Parte 3: Control de la cinta de arrastre, semáforo, parada de emergencia e integración del sistema (apertura de la barrera en las condiciones adecuadas, gestión de luces, parada de seguridad, parada de emergencia…
