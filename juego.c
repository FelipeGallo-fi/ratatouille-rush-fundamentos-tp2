#include "restaurant.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define ARRIBA 'W'
#define ABAJO 'S'
#define IZQUIERDA 'A'
#define DERECHA 'D'
#define MOPA 'O'

const int JUEGO_GANADO = 1;
const int CONTINUAR_JUEGO = 0;

/*
Pre: Accion y las constantes deben estar previamente incializados.
Post: Devuelve un booleano si la accion es igual a ARRIBA, ABAJO, DERECHA o MOPA.
*/

bool chequear_movimiento_ingresado(char accion){
    return (accion ==  ARRIBA || accion == ABAJO || accion == DERECHA || accion == IZQUIERDA || accion == MOPA);
}

/*
Pre: -
Post: Pregunta por una accion valida.
*/

char preguntar_accion(){
    char accion;
    printf("\nIngrese un movimiento (%c: arriba, %c: izquierda, %c: abajo, %c: derecha, %c: para agarrar la mopa, de ser posible): \n", ARRIBA, ABAJO, IZQUIERDA, DERECHA, MOPA);
    scanf(" %c",&accion);

    while(!chequear_movimiento_ingresado(accion)){
        printf("Ingrese un movimiento VALIDO porfavor (%c: arriba, %c: izquierda, %c: abajo, %c: derecha, %c: para agarrar la mopa, de ser posible): \n", ARRIBA, ABAJO, IZQUIERDA, DERECHA, MOPA);
        scanf(" %c",&accion);
    }

    return accion;
}


int main(){

    juego_t juego;
    srand (( unsigned)time(NULL));
    inicializar_juego(&juego);

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
        }else{
            printf("Dia terminado! Has perdido :(\n");
            sigue_el_juego = false;
            }
    }
    


    return 0;
}