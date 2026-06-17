
#include <msp430.h>

// Variable spara servo 1000(posicion inicial) y direccion izq o drchvolatile int posicion = 1000;
volatile int posicion = 1000;
volatile int direccion = 1;

//resultado de restar los tiempos
volatile unsigned int distancia = 0;

//Variable para ver en que tic lanzo la señal el ultrasonido
volatile unsigned int tiempo_inicio = 0;

//Variable para avisar al main que el el ultrasonido ha hecho una devolucion
volatile int medicion_lista = 0;

// funciones de escritura para la uart
void enviar_letra(char letra) {
    while(!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = letra;
}
//con punteros al principio de un array de char(string para los amigos) conseguimos reducir la funcion mucho mas facil
void enviar_texto(char* texto) {
    while(*texto) enviar_letra(*texto++);
}

//hacemos buffer de el numero correspondiente y vamos metiendo caracter a caracter en el array de char lo obtenemos invertido asi que
//para luego enviarlo  lo invertimos de nuevo
void enviar_numero(int numero) {
    char buffer[6];
    int i = 0;
    if (numero == 0) { enviar_letra('0'); return; }
    while (numero > 0) {
        buffer[i++] = (numero % 10) + '0';
        numero /= 10;
    }
    while (i > 0) enviar_letra(buffer[--i]);
}

void setup() {
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    // Pines Ultrasonido e Interrupciones de Puerto
    P1DIR |= BIT4;              // P1.4 TRIG (Salida)
    P1OUT &= ~BIT4;

    P1DIR &= ~BIT5;             // P1.5 ECHO (Entrada)
    P1IES &= ~BIT5;             // Flanco de Subida (L -> H) ya que devuelve en alto si encuentra
    P1IE |= BIT5;               // Habilitar interrupción en P1.5
    P1IFG &= ~BIT5;             // Limpiar bandera de interrupción por si acaso

    //Pin Servo
    P1DIR |= BIT6;
    P1OUT |= BIT6;

    // UART (UCA1)  UCA1MCTLW = 0x0200 es para hacer divisiones y mantenerlo lo mas estable posible
    P3SEL0 |= BIT4 | BIT5;
    UCA1CTLW0 = UCSWRST;
    UCA1CTLW0 |= UCSSEL__SMCLK;
    UCA1BRW = 104;
    UCA1MCTLW = 0x0200;
    UCA1CTLW0 &= ~UCSWRST;

    // 4. Timer_A0 (Servo)
    TA0CCR0 = posicion;
    TA0CCTL0 = CCIE; //activamos excepcion para cuando iguale el valor que le damos(iniciado a posicion inicial 1000), en la excepcion explico mejor el funcionamiento del servo
    TA0CTL = TASSEL_2 | MC_1 | TACLR;

    // 5. Timer_A1 (Cronómetro para el Echo)
    TA1CTL = TASSEL_2 | MC_2;   // SMCLK, Modo Continuo (siempre corriendo) asi podemos calcular la diferencia de tiempo

    __enable_interrupt();
}

void main(void) {
    setup();

    while(1) {
        // Disparar el sensor cada 100ms
        P1OUT |= BIT4;
        __delay_cycles(10);
        P1OUT &= ~BIT4;

        if (medicion_lista) {
            // Si la distancia es absurdamente alta, es un error de lectura o está tapado
            if (distancia > 400 || distancia == 0) {
                enviar_texto("Fuera de rango o bloqueado\r\n");
            } else {
                enviar_texto("Angulo: ");
                enviar_numero(((posicion - 1000) * 18) / 100);
                enviar_texto(" | ");
                enviar_texto("Distancia: ");
                enviar_numero(distancia);
                enviar_texto(" cm\r\n");
            }
            medicion_lista = 0;
        }
        __delay_cycles(100000);
    }
}

//Los servos son motores muy especiales ya que no les pasas corriente y se activan sino que cada 20ms se actualizan
//en esos 20 ms dependiendo del tiempo que su pin (en este caso 1.6) esta en on y en off significara una cordenada u otra
//por eso empezamos en la cordenada 1000(1ms encendido) y dependienndo de la direccion le sumamos o restamos 10 a esa posicion
//podriamos sumar mas cantidad a la vez o pasar directamente al otro lado pero el servo se moveria demasiado rapido,
//con el resto que nos queda (tiempo maximo(20ms) menos el actual recorrido) inicializammos el reloj para cumplir los 20ms
//ya que si no estaria dando vueltas sin parar completamente loco
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0_ISR(void) {
    if (P1OUT & BIT6) { //si esta activo el servo
        P1OUT &= ~BIT6; //lo apago
        TA0CCR0 = 20000 - posicion; //actualizo el reloj para que recorra el tiempo restante y acabar los 20ms
        if (direccion == 1) {
            posicion += 10;
            if (posicion >= 2000) direccion = 0; //limite en 2000
        } else {
            posicion -= 10;
            if (posicion <= 1000) direccion = 1; //limite en 1000
        }
    } else {
        P1OUT |= BIT6; //"activamos" el servo
        TA0CCR0 = posicion; //ponemos el timer hasta la posicion deseada
    }
}

//tuve que mirar como se hacien interrupciones con pines de entradas para poder saltar interrupcion cuando echo salta asi nos evitamos el polling constante
#pragma vector = PORT1_VECTOR
__interrupt void Port_1_ISR(void) {
    if (P1IFG & BIT5) {         // se ha activado el pin echo(bit5)?
        if (!(P1IES & BIT5)) {  // Si  FLANCO DE SUBIDA
            tiempo_inicio = TA1R; // Capturar tiempo actual
            P1IES |= BIT5;      // Cambiar a detectar FLANCO DE BAJADA
        }
        else {                  // Si  FLANCO DE BAJADA
            unsigned int tiempo_final = TA1R; //cogemos el tiempo final del timer
            distancia = (tiempo_final - tiempo_inicio) / 58; //calculamos la diferencia con la formula dada en la documentacion del ultrasonido

            medicion_lista = 1; // Avisar de que hay dato nuevo
            P1IES &= ~BIT5;     // Volver a FLANCO DE SUBIDA
        }
        P1IFG &= ~BIT5;         //Limpiar la bandera
    }
}
