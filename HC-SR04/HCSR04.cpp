/**
 * @author Nestor Pereira-Neto Team: SalvadorEngenharia
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


#include "HCSR04.h"
#include "mbed.h"


HCSR04::HCSR04(PinName trigger, PinName echo) : _t(trigger), _e(echo) {

    }

float HCSR04::getCm(void){
    distcm = readEcho()/58;
    return distcm;
    }

float HCSR04::getIn(void){
    distin = readEcho()/148;
    return distin;
    }

float HCSR04::readEcho(void){     
    _t=1;                       //Inicio do trigger
    wait_us(10);                //10us de pulso
    _t=0;                       //Fim do trigger
    while(!_e);
    _tempo.start();
    while(_e);
    tdist = _tempo.read_us();   //Leitura do tempo transcorrido 
    _tempo.stop();              //Paro o temporizador
    _tempo.reset();             //Reset para o próximo ciclo
    return tdist;
}