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
#include <stdbool.h>
#include "registros.h"
#include "termlib.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_color.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

#define HEIGHT 500
#define WIDTH  800
#define ltrT(i) ((i)=='t' || (i)=='T')  //cambia todos los bits del puerto por el opuesto
#define ltrQ(i) ((i)=='q' || (i)=='Q')  //letra q me permite salir del programa
#define ltrC(i) ((i)=='c' || (i)=='C')  //letra c me permite apagar todos los bits
#define ltrS(i) ((i)=='s' || (i)=='S')  //letra s me permite prender todos los bits
#define ltrB(i) ((i)=='b' || (i)=='B') //parpadeo de bits encendidos 
#define numvalido(i)    ((i)>='0' && (i)<='7') // macro para identificar numeros de bit
#define MaskT      0xFF//mascara que sirve para manipular los bits del puerto A
#define MaskC      0x00

extern registros_t *puertos; //defino registros
void create_portax(void);       //puerto A 
/*
 * 
 */
int main() {
    ALLEGRO_DISPLAY  *display=NULL;     //punteros que apuntan a un estructuras de allegro, se los apuntan a NULL para controlar errores
    ALLEGRO_BITMAP *imagen=NULL;
    ALLEGRO_EVENT_QUEUE * event_queue =NULL;
    ALLEGRO_SAMPLE *sample=NULL;
    int portAux;        //puerto que me guarda la configuracion actual del puerto para hacerlo parpadear
    bool close_display = false;
    
    if (!al_init()){        //inicializacion general del allegro
        fprintf (stderr, "error al inicializar el allegro\n");
        return -1;
    }
    
   
    if (!al_install_audio()) {                         //se controla si fallo el audio
        fprintf(stderr, "failed to create audio!\n");
        return -1;
    }  
 
    if (!al_init_acodec_addon()) {                         //se controla si fallo el audio codec
        fprintf(stderr, "failed to create audio codecs!\n");
        return -1;
    }

    if (!al_reserve_samples(1)) {                         //se controla si existio falla
        fprintf(stderr, "failed to reserve samples!\n");
        return -1;
    }
   
    sample = al_load_sample("beep-3.wav");  //cargo el sample

    if (!sample) {                         //se controla si fallo cargar el sample
        fprintf(stderr, "audio clip sample not loaded!\n");
        return -1;
    } 
    
    event_queue = al_create_event_queue();       //se inicializa los eventos
    
    if (!event_queue) {                         //se controla si fallo la init de los eventos
        fprintf(stderr, "failed to create event_queue!\n");
        return -1;
    }
   
   
    if (!al_init_primitives_addon()){       //se controla si fallo la inicializacion de las primitivas
        fprintf (stderr, "error al inicializar las primitivas\n");
        return -1;
    }

   
    display=al_create_display(800,500);       //se crea el display
    al_register_event_source(event_queue, al_get_display_event_source(display)); //se registra la fuente de los eventos de cierre de display
    
    if(!display){
        al_shutdown_primitives_addon();      //se destruye la imagen porque ocupa espacio en heap y el programa fallo por otro motivo
        al_destroy_event_queue(event_queue);
        fprintf(stderr,"failed to create display");
        
        return -1;
    }
    
    
    al_clear_to_color(al_color_name("white"));          //se pinta las dos caras del display con blanco
    al_flip_display();
    al_clear_to_color(al_color_name("white"));
   
    
    create_portax();  //se crea el puerto A 
    
    
    
    while (!close_display) {                            
        
        int entrada,loop=1;    //entrada es una variable que me permite almacenar el dato aportado por el usuario, loop, me permite permanecer en el ciclo
    
        ALLEGRO_EVENT ev;   //se crea la estructura ev
        if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola
        {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){    
                close_display =true;
                loop=0;
            }
        }
    
    
       
        char portA='A';     //solo se desea modificar el puerto A
        printf ("Ingrese numero de bit o letra correspondiente\n");
        
        while(loop && !close_display){
            
            entrada=getchar (); // recibo un comando o bit
            
            
            
            if(getchar()!='\n'){    //si hubo error en la entrada de datos se limpia el buffer
                fflush(stdin);
                getchar();
                printf("caracter no valido\n");
            }
            else{
                
                al_play_sample(sample,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);    // suena un ruido cuando se llena un bit
                
                if (numvalido(entrada)) {   // si se ingreso un numero de bit lo prendo

                     bitSet(portA, entrada);
                     loop=0;
                      printf (" El valor del puerto A es: 0x%hhx\n", (*puertos).px.a);
                }
                else if (ltrT(entrada)){   // si ingreso la T cambio por el opuesto todos los bits

                     MaskToggle(MaskT,portA);
                     loop=0;
                     printf (" El valor del puerto A es: 0x%hhx\n", (*puertos).px.a);
                 }
                else if (ltrC(entrada)){   //si recibo la letra C hago un clear a los bits

                     MaskOff(MaskC,portA);
                     loop=0;
                     printf (" El valor del puerto A es: 0x%hhx\n", (*puertos).px.a);
                }
                else if (ltrS(entrada)){   // si recibo la letra S prendo todoso los leds

                     MaskOn(MaskT,portA);
                     loop=0;
                     printf (" el valor del puerto A es: 0x%hhx\n", (*puertos).px.a);
                }
                else if (ltrQ(entrada)){    //si recibo la Q el programa termina

                    loop=0;
                    close_display= true;    
               }
                else if (ltrB(entrada)){
                    portAux=((*puertos).px.a);      //se guarda los bits del puerto A
                    int fin=1;
                    do{                             //ciclo  que me hace parpadaer los bits
                        while(!kbhit()){            //cuando se detecta una letra apretada en el teclado sale del ciclo.
                        MaskParpOff (portAux,MaskC);
                        fillbits();
                        al_flip_display();
                        MaskParpOn (MaskC,portAux);
                        fillbits();
                        al_flip_display();                               
                        }
                        if(ltrB(getchar())){        // chequeo si es una B para cerrar el ciclo
                            fin=0;
                            fflush(stdin);          //vacio el buffer de entrada
                            getchar();
                        }
                        loop=0;
                    }while(fin);
                }
                else{   // sso fue un caracter no valido me voy del programa
                    loop=0;
                    close_display=true;
                }

            fillbits(); //Funcion que actualiza bits del display
            al_flip_display();
            }
        
        } 
    }
      
    al_destroy_bitmap(imagen);       //se libera la memoria dinamica , destruyendo los elemntos usados
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_shutdown_primitives_addon();
    al_destroy_sample(sample);
    al_uninstall_audio();
    printf ("bye\n");
    
    
    return (EXIT_SUCCESS);
}

void create_portax(void){           //se crea el grafico del puerto A(los 8flags)
    
    
        al_draw_circle( WIDTH*1/9 , HEIGHT /2 , 40.0, al_color_name("black"),5);
        al_draw_circle( WIDTH*2/9 , HEIGHT /2 , 40.0, al_color_name("black"),5);
        al_draw_circle( WIDTH *3/9 , HEIGHT /2 , 40.0, al_color_name("black"),5);
        al_draw_circle( WIDTH *4/9 , HEIGHT /2 , 40.0, al_color_name("black"),5);
        al_draw_circle( WIDTH *5/9 , HEIGHT /2 , 40.0, al_color_name("black"),5);
        al_draw_circle( WIDTH *6/9 , HEIGHT /2 , 40.0, al_color_name("black"),5);
        al_draw_circle( WIDTH *7/9 , HEIGHT /2 , 40.0, al_color_name("black"),5);
        al_draw_circle( WIDTH *8/9 , HEIGHT /2 , 40.0, al_color_name("black"),5);
     
        al_flip_display();
}

void fillbits(void){        //controla el esatdo de los bits
    
    
    
    int state=0,i;
    
    for(i=0;i<=7;++i){
        
    
        switch(i){      //pregunta bit por bit 
            case 7 :state=(((*puertos).px.a) & 0x01);
               ;break;
            case 6 :state=(((*puertos).px.a) & 0x02);
                ;break;
            case 5 :state=(((*puertos).px.a) & 0x04);
                ;break;
            case 4 :state= (((*puertos).px.a) & 0x8);                     
                break;
            case 3 :state= (((*puertos).px.a) & 0x10) ;              
                break;
            case 2 :state=(((*puertos).px.a) & 0x20);
                break;
            case 1 :state=(((*puertos).px.a) & 0x40);
                break;
            case 0 :state=(((*puertos).px.a) & 0x80);
                break;
        }
    
        if (state){     //si esta encendido lo pinta de rojo
            al_draw_filled_circle(WIDTH *(i+1)/9 , HEIGHT /2 , 40.0, al_color_name("red"));
      
        }
        else{   //de lo contrario lo deja blanco
            al_draw_filled_circle(WIDTH *(i+1)/9 , HEIGHT /2 , 40.0, al_color_name("white"));
        }
    }   
}