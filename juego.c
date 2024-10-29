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

/*
Pre: -
Post: 
*/


bool hay_asientos_libres(int cantidad_mesas, mesa_t *mesa, int *indice_mesa_con_lugar, int comensales_a_ingresar){
    bool encontre_lugar = false;
    int i = 0;

    if(comensales_a_ingresar == 1){
        while(!encontre_lugar && i < cantidad_mesas){
            if(mesa[i].cantidad_lugares == 1 && mesa[i].cantidad_comensales == 0){
                encontre_lugar = true;
                *indice_mesa_con_lugar = i;
            }
            i++;
        }
        i = 0;
        while(!encontre_lugar && i < cantidad_mesas){
            if(mesa[i].cantidad_lugares == 4 && mesa[i].cantidad_comensales == 0){
                encontre_lugar = true;
                *indice_mesa_con_lugar = i;
            }
            i++;
        }
    }else {
        i = 0;
        while(!encontre_lugar && i < cantidad_mesas){
            if(mesa[i].cantidad_comensales == 0 && mesa[i].cantidad_lugares >= comensales_a_ingresar){
                encontre_lugar = true;
                *indice_mesa_con_lugar = i;
            }
            i++;
        }
    }

    return encontre_lugar;
}


/*
Pre:
Post:
*/

void asignar_comensales(mesa_t *mesa, int cantidad_mesas, int comensales_a_ingresar){
    int indice_mesa_con_lugar;
    bool encontre_lugar = hay_asientos_libres(cantidad_mesas, mesa, &indice_mesa_con_lugar, comensales_a_ingresar);

    if(encontre_lugar){
        mesa[indice_mesa_con_lugar].cantidad_comensales = comensales_a_ingresar;
    }
}

/*
Pre: maximo_valor debe ser si o si mayor o igual a minimo_valor
Post: devuelve un numero aleatorio en el rango especificado.
*/

int generar_numero_aleatorio(int maximo_valor, int minimo_valor){
    return ((rand() % maximo_valor) + minimo_valor); 
}


/*
Pre:
Post:
*/

void llegada_comensales(juego_t *juego){

    int cantidad_comensales = generar_numero_aleatorio(MAX_COMENSALES,MIN_COMENSALES);

    printf("numero aleatorio: %i \n", cantidad_comensales);

    asignar_comensales(juego->mesas,juego->cantidad_mesas, cantidad_comensales);
}


/*
Pre:
Post:
*/

void actualizacion_del_juego(juego_t *juego){
    if(juego->movimientos % 15 == 0 && juego->movimientos > 0){
        llegada_comensales(juego);
    }
    /*if(juego->movimientos % 25 == 0){
        aparacer_cucaracha(&juego);
    }*/
}

int main(){

    juego_t juego;
    srand (( unsigned)time(NULL));
    inicializar_juego(&juego);

    bool sigue_el_juego = true;
    while(sigue_el_juego){
        int juego_estado = estado_juego(juego);
        mostrar_juego(juego);
        actualizacion_del_juego(&juego);
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