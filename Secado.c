#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>


// Definicion de macros de trabajo
#define setBit(Registro,Bit)    (Registro |= (1 << Bit))    // pone a 1 el bit B del puerto P
#define clearBit(Registro,Bit)    (Registro &= ~(1 << Bit))    // pone a 0 el bit B del puerto P
#define toggleBit(Registro,Bit) (Registro ^= (1 << Bit))      // cambia el valor del bit B del puerto P
#define isBitSet(Registro, Bit) (Registro & (1 << Bit))        // devuelve '1' si dicho bit es '1'
#define isClrSet(Registro, Bit) (!isBitSet(Registro, Bit))    // devuelve '1' si dicho bit es '0'

// Caracteristicas del microprocesador
#define Freq_uC 8000000
#define Check_height_sensors 10        // MS en comprobar estado (Regular en maqueta)
#define Tiempo_prove_new 1200        // Tiempo comprobación entrada nueva en ms(dos sensores distintos)

// MOTOR M5 direccion
#define REG_M5_di_PORT    PORTD
#define REG_M5_di_DDR    DDRD
#define REG_M5_di_PIN    PIND

#define PIN_M5_di_PORT    PD7
#define PIN_M5_di_DDR    DDD7
#define PIN_M5_di_PIN    PIND7

// MOTOR M5 enable
#define REG_M5_en_PORT    PORTB
#define REG_M5_en_DDR    DDRB
#define REG_M5_en_PIN    PINB

#define PIN_M5_en_PORT    PB6
#define PIN_M5_en_DDR      DDB6
#define PIN_M5_en_PIN      PINB6

// SWITCHES registros generales
#define REG_SW_PORT PORTD
#define REG_SW_DDR  DDRD
#define REG_SW_PIN  PIND

// SW3 (INT2)
#define PIN_SW3_PORT PD2
#define PIN_SW3_DDR  DDD2
#define PIN_SW3_PIN  PIND2

// REGISTROS S01-S05-S06-S12 B --> NO SE USA (?)
#define REG_SOB_PORT PORTB
#define REG_SOB_DDR  DDRB
#define REG_SOB_PIN  PINB

// REGISTROS S02-S03-S04-S010 L --> NO SE USA (?)
#define REG_SOK_PORT PORTK
#define REG_SOK_DDR  DDRK
#define REG_SOK_PIN  PINK

// REGISTROS S07-S08-S09-2011 D
#define REG_SOD_PORT PORTD
#define REG_SOD_DDR  DDRD
#define REG_SOD_PIN  PIND

// SO7 (PCINT3)
#define PIN_SO7_PORT PB3
#define PIN_SO7_DDR  DDB3
#define PIN_SO7_PIN  PINB3

// SO8 (PCINT23)
#define PIN_SO8_PORT PK7
#define PIN_SO8_DDR  DDK7
#define PIN_SO8_PIN  PINK7

// SO9 (PCINT19)
#define PIN_SO9_PORT PK3
#define PIN_SO9_DDR  DDK3
#define PIN_SO9_PIN  PINK3

#define CICLO_TRABAJO    400                    // rango hasta 1600

volatile uint32_t ms = 0;

// Variables globales - Secado
volatile uint8_t so7 = 1;
volatile uint8_t so8 = 1;
volatile uint8_t so9 = 1;
volatile uint8_t so7p = 1;
volatile uint8_t so8p = 1;
volatile uint8_t so9p = 1;
volatile uint8_t aux_sec = 0;
volatile uint8_t limit_switch_sec = 0;        //'1' SW3 detecta rodillo en un extremo

void setupTimers(void){
    cli();

    // TIMER 4 => Timer segundos : Modo CTC (OCRnA) con preescalado 256
    TCCR4A = 0b00000000;                    //WGM40 y WGM41 == 0
    TCCR4B = 0b00001101;                    //WGM42(bit3) == 1  CS42 == 1(preescalado 256)(bit2)
    TIMSK4 = 0b00000010;
    OCR4A =  Freq_uC/256;

    // TIMER 3 => Timer milisegundos :  Modo CTC (OCRnA) sin preescalado
    TCCR3A = 0b00000000;                    //WGM30 y WGM31 == 0
    TCCR3B = 0b00001001;                    //WGM32(bit3) == 1  CS30 == 1(no preescalado)(bit0)
    TIMSK3 = 0b00000010;
    OCR3A = Freq_uC/1000;

    sei();
}

// Filtrado rebotes
ISR(TIMER3_COMPA_vect){ // Milisegundos
    ms++;
    if(ms % Check_height_sensors == 0){   //cada cuanto chequeo los sensores de altura //#define macro en General.h
        so7p = so7;
        so8p = so8;
        so9p = so9;

        //cargo los valores de los sensores de ese instante
        so7 = isBitSet(REG_SOB_PIN,PIN_SO7_PIN);
        so8 = isBitSet(REG_SOK_PIN,PIN_SO8_PIN);
        so9 = isBitSet(REG_SOK_PIN,PIN_SO9_PIN);
        limit_switch_sec = isClrSet(REG_SW_PIN,PIN_SW3_PIN); // isClrSet porque SW3 '0' al detectar


        if((so7p==so7) && (so8p==so8) && (so9p==so9)){  //Si los valores son los mismos que en instante anterior
            aux_sec = 1;          //muevo el rodillo
            }else{
            aux_sec = 0;          //no hago nada
        }

    }
}

void setup_secado(){
    cli();

    // Motor 5: Altura secador
    setBit(REG_M5_en_DDR,PIN_M5_en_DDR);                    // Definir como salida    :: DDRD |= (1 << DDD7)
    setBit(REG_M5_di_DDR,PIN_M5_di_DDR);                    //                        :: DDRB |= (1 << DDB6)
    setBit(REG_M5_en_PORT,PIN_M5_en_PORT);                    // Subir secador        :: PORTD |= (1 << PD7)
    clearBit(REG_M5_di_PORT,PIN_M5_di_PORT);                //                        :: PORTB &= ~ (1 << PB6)

    TCCR1A = (1 << WGM10) | (1 << COM1B1) | (1 << COM1C1);    // (1 << WGM10) | (1 << COM1C1);
    TCCR1B = (1 << WGM13) | (1 << CS12) | (0 << CS11) | (1 << CS10); //0b00010001;
    TIMSK1 = 0b00000010;
    OCR1A =  Freq_uC/5000;    // Frecuencia de 5KHz, 0.2ms el ciclo
    OCR1B = CICLO_TRABAJO - 1;                                // Valor de OCR1B (define ciclo de trabajo)

    sei();
}

void up_secado(){
    setBit(REG_M5_en_PORT,PIN_M5_en_PORT);
    setBit(REG_M5_di_PORT,PIN_M5_di_PORT); // COMPROBAR EN MAQUETA si es setBit() o clearBit()
}

void down_secado(){
    setBit(REG_M5_en_PORT,PIN_M5_en_PORT);
    clearBit(REG_M5_di_PORT,PIN_M5_di_PORT); // COMPROBAR EN MAQUETA si es ~[setBit() o clearBit()]
}

void stop_secado(){
    clearBit(REG_M5_en_PORT,PIN_M5_en_PORT);
}

void secado(){

        if(aux_sec) {
            if( so8 && so7 && so9)                 // no detecta abajo
                down_secado();                    // baja
            else if( so8 || !(so7 && so9) )
                up_secado();
            else if( !(so8 || (so7 && so9)) )    // detecta abajo y en algun lateral
                up_secado();                    // sube
            else if( !so8 && so7 && so9 )        // detecta abajo pero no en los laterales
                stop_secado();                    // permanece quieto
            else
                stop_secado();
        }

    /*    CON EL MISMO ALGORITMO DE LAVADO
    if(aux_sec){                                //si los valores de los sensores son los mismos que en instante anterior (antirrebotes)
        if (so8==0 && so7!=0){                    //detecta abajo pero no a los lados
            stop_secado();
            }else if(so7==0){                            //detecta alguno de los lados
            up_secado();                        //sube el secador
            }else{
            down_secado();                    //baja secador
        }            //no detecta nada
        //baja secador


    }*/


}

volatile uint8_t Stop = 0;
void setStop(void){
    Stop = 1;
}

uint8_t getStop(void){
    return Stop;
}

int main(void){
    setupTimers();
    setup_secado();

    while(1) {
        if(!getStop()){
            secado();
        }
    }
    return 0;
}

//aplicar PWM al secado para el secado-->mas despacio
//mirar señales osciloscopio--> rebotes
