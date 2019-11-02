/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: jucardozo
 *
 * Created on 26 de octubre de 2019, 15:03
 */

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "registros.h"

#define ltrT(i) ((i)=='t' || (i)=='T')  //cambia todos los bits del puerto por el opuesto
#define ltrQ(i) ((i)=='q' || (i)=='Q')  //letra q me permite salir del programa
#define ltrC(i) ((i)=='c' || (i)=='C')  //letra c me permite apagar todos los bits
#define ltrS(i) ((i)=='s' || (i)=='S')  //letra s me permite prender todos los bits
#define numvalido(i)    ((i)>='0' && (i)<='7')
#define MaskT      0xFF//mascara que sirve para manipular los bits del puerto A
#define MaskC      0x00
/*
 * 
 */
int main() {
    ALLEGRO_DISPLAY  *display=NULL;
    ALLEGRO_BITMAP *imagen=NULL;
    if (!al_init()){        //inicializacion general del allegro
        fprintf (stderr, "error al inicializar el allegro\n");
        return 0;
    }
    if (!al_init_image_addon()){
        fprintf (stderr, "error al inicializar la imagen\n");
        return 0;
    }
    imagen=al_load_bitmap("leds.png");        //se carga la imagen, si se inicializa correctamente ocupa memoria dinamica
    if (!imagen){
        fprintf(stderr, "error al cargar la imagen");
        return 0; 
    }
    display=al_create_display(800,500);
    if(!display){
        al_destroy_bitmap(imagen);      //se destruye la imagen porque ocupa espacio en heap y el programa fallo por otro motivo
        fprintf(stderr,"error al crear el bitmap");
        return 0;
    }
    al_draw_bitmap(imagen,0,0,0);
    al_flip_display();
    
    int entrada,loop=1;    //entrada es una variable que me permite almacenar el dato aportado por el usuario, loop, me permite permanecer en el ciclo
    extern registros_t *puertos;
    char portA='A';     //solo se desea modificar el puerto A
    printf ("Ingrese numero de bit o letra correspondiente\n");
    do{
        entrada=getchar ();
        if (numvalido(entrada)) {   //usar macro de libreria
            bitSet(portA, entrada);
            printf (" El valor del puerto A es: 0x%hhx\n", (*puertos).px.a);
        }
        else if (ltrT(entrada)){
            MaskToggle(MaskT,portA);
            printf (" El valor del puerto A es: 0x%hhx\n", (*puertos).px.a);
        }
        else if (ltrC(entrada)){
            MaskOff(MaskC,portA);
            printf (" El valor del puerto A es: 0x%hhx\n", (*puertos).px.a);
        }
        else if (ltrS(entrada)){
            MaskOn(MaskT,portA);
            printf (" el valor del puerto A es: 0x%hhx\n", (*puertos).px.a);
        }
        else if (ltrQ(entrada)){
            loop=0;
        }
        
    }while(loop);
    al_destroy_bitmap(imagen);
    al_destroy_display(display);
    printf ("bye\n");
    return (EXIT_SUCCESS);
}

