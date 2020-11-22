/**
 * @author Nestor Pereira-Neto  Team: SalvadorEngenharia
 *
 * @section LICENSE
 *
 * Copyright (c) 2010 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * Biblioteca para uso do sonar HC-SR04, possui rotinas para permitir medições 
 * de distências retornado valores em centímetros, polegadas ou o tempo 
 * percorrido pelo som (em microsegundos).
 * Esta biblioteca pode ser usada com qualquer pino do microcontrolador.
 */


#ifndef HCSR04_H
#define HCSR04_H

#include "mbed.h"



/** 
 * Sonar HC-SR04 example.
 * @code
 * #include "mbed.h"
 * #include "HCSR04.h"
 *
 * DigitalOut myled(LED1);
 * Serial pc(USBTX,USBRX);     
 *
 * HCSR04 sonar(PTD5, PTA13);
 *
 * int main() {
 *     while(1) {
 *         printf("Distancia detectada pelo sensor Frente %.2f cm \n", sonar.getCm()); 
 *         wait_ms(1000);
 *     }
 * }
 *
 * @endcode
*/

class HCSR04 {    
    public:    
        /** Constructor, create HCSR04 instance 
          *
          * @param trigger TRIG pin
          * @param echo ECHO pin
          */
        HCSR04(PinName trigger, PinName echo);
 
        /** It make a reading of the sonar Faz uma leitura do sonar
          * 
          * @returns Tempo do pulso echo em microsegundos*/
        float readEcho(void);
        
        /** It messures the distance in centimeter "cm" 
          *
          *@returns Distance in centimeter
          */
        float getCm(void);
        
        /** Mede a distência em polegadas "in" 
          *
          *@returns Distência em in*/
        float getIn(void);
        
    private:
        float tdist;        //Leitura do tempo transcorrido
        float distcm;       //Guarda o valor da distanciância em centímetros
        float distin;       //Guarda o valor da distência em polegadas
        
        DigitalOut _t;      //Configuração do pino de Trigger  
        DigitalIn _e;       //Configuração do pino de Echo
        Timer _tempo;       //Cria um objeto timer
        
};

#endif