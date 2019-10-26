/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: mcorcos
 *
 * Created on 21 de octubre de 2019, 14:40
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "registros.h"
#define Is_portA(i)  (((i)=='a')||((i)='A'))    //macros que sirven para validar los datos
#define Is_portB(i)  (((i)=='b')||((i)='B'))    
#define numvalido(i)    ((i)>='0' && (i)<='7')  
static registros_t portd;                   //se crea una variable de tipo registro_t que a su vez es una estructura


void bitSet (char port, int numbit){            //funcion que modifica un bits por 1,recibe un puerto y el numero de bit deseado
    if (Is_portA(port) && numvalido(numbit)){      //se diferencia entre el puerto A y el puerto B,para tener facilel acceso
            switch (numbit){
                case 0:portd.bi.a0=1;break;
                case 1:portd.bi.a1=1;break;
                case 2:portd.bi.a2=1;break;
                case 3:portd.bi.a3=1;break;
                case 4:portd.bi.a4=1;break;
                case 5:portd.bi.a5=1;break;
                case 6:portd.bi.a6=1;break;
                case 7:portd.bi.a7=1;break;
            }
        }
            
    else if (Is_portB(port)&& numvalido(numbit)){
         switch (numbit){
                case 0:portd.bi.b0=1;break;
                case 1:portd.bi.b1=1;break;
                case 2:portd.bi.b2=1;break;
                case 3:portd.bi.b3=1;break;
                case 4:portd.bi.b4=1;break;
                case 5:portd.bi.b5=1;break;
                case 6:portd.bi.b6=1;break;
                case 7:portd.bi.b7=1;break;
        }
     }   
}

void bitClr (char port,int numbit){             //funcion que modifica un bit por un cero, recibe el puerto y el numero deseado de bit
    if (Is_portA(port) && numvalido(numbit)){      //se diferencia entre el puerto A y el puerto B,para tener facilel acceso
            switch (numbit){
                case 0:portd.bi.a0=0;break;
                case 1:portd.bi.a1=0;break;
                case 2:portd.bi.a2=0;break;
                case 3:portd.bi.a3=0;break;
                case 4:portd.bi.a4=0;break;
                case 5:portd.bi.a5=0;break;
                case 6:portd.bi.a6=0;break;
                case 7:portd.bi.a7=0;break;
            }
        }
            
    else if (Is_portB(port)&& numvalido(numbit)){
         switch (numbit){
                case 0:portd.bi.b0=0;break;
                case 1:portd.bi.b1=0;break;
                case 2:portd.bi.b2=0;break;
                case 3:portd.bi.b3=0;break;
                case 4:portd.bi.b4=0;break;
                case 5:portd.bi.b5=0;break;
                case 6:portd.bi.b6=0;break;
                case 7:portd.bi.b7=0;break;
        }
     }   
}
int bitGet (char port,int numbit){                  //funcion que devuelve el estado del bit
    int state;
    if (Is_portA(port) && numvalido(numbit)){      //se diferencia entre el puerto A y el puerto B,para tener facilel acceso
            switch (numbit){                        //se devuelve el esatdo del bit deseado
                case 0:portd.bi.a0=state;
                       return state;break;
                case 1:portd.bi.a1=state;
                       return state;break;
                case 2:portd.bi.a2=state;
                       return state;break;
                case 3:portd.bi.a3=state;                        
                       return state;break;
                case 4:portd.bi.a4=state;                 
                       return state;break;
                case 5:portd.bi.a5=state;
                       return state;break;
                case 6:portd.bi.a6=state;
                       return state;break;
                case 7:portd.bi.a7=state;
                       return state;break;

            }
        }
            
    else if (Is_portB(port)&& numvalido(numbit)){
         switch (numbit){
                case 0:portd.bi.b0=state;
                       return state;break;
                case 1:portd.bi.b1=state;
                       return state;break;
                case 2:portd.bi.b2=state;
                       return state;break;
                case 3:portd.bi.b3=state;
                       return state;break;
                case 4:portd.bi.b4=state;
                       return state;break;
                case 5:portd.bi.b5=state;
                       return state;break;
                case 6:portd.bi.b6=state;
                       return state;break;
                case 7:portd.bi.b7=state;
                       return state;break;
        }
     }  
}
void bitToggle (char port, int numbit){ //funcion que utiliza otra funciones , porque cambia el el estado del bit por un 1 o un 0
    int state;
    state=bitGet(port,numbit);  //obtengo el estado orginal del bit
    if (state==1){              //si es 1 lo cambio por cero
        bitClr(port,numbit);
    }
    else{                       //si es 0 lo cambio por 1
        bitSet(port,numbit);
    }
        
}

void MaskOn (int Mask,char port){   //funcion que me prende los bit que estan prendidos en la mascara, respetandos el estado de los bit del puerto 
    if (Is_portA(port)){            //se tiene que tener en cuenta que Mask tiene que ser un numero en hexa, para mejor funcionamiento
        portd.px.a=(portd.px.a | Mask); 
    }
    else if (Is_portB(port)){
        portd.px.b=(portd.px.b | Mask);
    }
}

void MaskOff (int Mask, char port ){ //debe apagar los bit que estan prendidos en la mascara, respetando a los bits restantes
    if (Is_portA(port)){
        portd.px.a=(portd.px.a & Mask); //mascara con el operador xor
    }
    else if (Is_portB(port)){
    portd.px.b= (portd.px.b & Mask);    
    }
}
void MaskToggle (int Mask, char port ){
     if (Is_portA(port)){
        portd.px.a=(portd.px.a ^ Mask); //mascara con el operador xor
    }
    else if (Is_portB(port)){
    portd.px.b= (portd.px.b ^ Mask);    
    }
}
