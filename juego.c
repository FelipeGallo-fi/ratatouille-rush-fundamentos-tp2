#include "restaurant.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_COMENSALES 1
#define ARRIBA 'W'
#define ABAJO 'S'
#define IZQUIERDA 'A'
#define DERECHA 'D'
#define MOPA 'O'
#define PATINES 'P'
#define TOMAR_PEDIDO 'T'

const int JUEGO_GANADO = 1;
const int CONTINUAR_JUEGO = 0;
const int DINERO_NECESARIO = 150000;

/*
Pre: Accion y las constantes deben estar previamente incializados.
Post: Devuelve un booleano si la accion es igual a ARRIBA, ABAJO, DERECHA o MOPA.
*/

bool es_movimiento_valido(char accion){
    return (accion ==  ARRIBA || accion == ABAJO || accion == DERECHA || accion == IZQUIERDA || accion == MOPA || accion == PATINES || accion == TOMAR_PEDIDO);
}

/*
Pre: -
Post: Pregunta por una accion valida.
*/

char preguntar_accion(){
    char accion;
    printf("\nIngrese un movimiento (%c: arriba, %c: izquierda, %c: abajo, %c: derecha, %c: para agarrar la mopa, de ser posible, %c: para agarrar los patines, de ser posible, %c: para tomar el pedido de una mesa ocupada , es necesario estar a 1 posicion de distancia de una mesa):  \n", ARRIBA, ABAJO, IZQUIERDA, DERECHA, MOPA, PATINES, TOMAR_PEDIDO);
    scanf(" %c",&accion);

    while(!es_movimiento_valido(accion)){
        printf("Ingrese un movimiento VALID( %c: arriba, %c: izquierda, %c: abajo, %c: derecha, %c: para agarrar la mopa, de ser posible, %c: para agarrar los patines, de ser posible, %c: para tomar el pedido , es necesario estar a 1 posicion de distancia de una mesa):  \n", ARRIBA, ABAJO, IZQUIERDA, DERECHA, MOPA, PATINES, TOMAR_PEDIDO);
        scanf(" %c",&accion);
    }

    return accion;
}


int main(){

    juego_t juego;
    srand (( unsigned)time(NULL));
    inicializar_juego(&juego);

    int dinero_faltante;
    bool sigue_el_juego = true;
    while(sigue_el_juego){
        int juego_estado = estado_juego(juego);
        mostrar_juego(juego);
        if(juego_estado == CONTINUAR_JUEGO){
            char accion = preguntar_accion();
            realizar_jugada(&juego, accion);
        }else if(juego_estado == JUEGO_GANADO){
            printf("Dia terminado! Felicitaciones has ganado!\n");
            sigue_el_juego = false;
            destruir_juego(&juego);
        }else{
            dinero_faltante = DINERO_NECESARIO - juego.dinero  ;
            printf("Dia terminado! Has perdido, te faltaron $%i pesos :(\n", dinero_faltante);
            sigue_el_juego = false;
            destruir_juego(&juego);
        }
    }
    


    return 0;
}