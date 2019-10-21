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
void bitSet (const ports_t* pPorts);
/*
 * 
 */
int main(void) {
    
    typedef union{
        uint16_t    d;
        struct{
            uint8_t a;
            uint8_t b;
        };
    }ports_t;
    
    ports_t *pPorts;
    return (EXIT_SUCCESS);
}

void bitSet (const ports_t* pPorts){
    
   
}