#include "restaurant.h"

#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int CANTIDAD_MESAS = 10;
const int CANTIDAD_HERRAMIENTAS = 14;
const int CANTIDAD_OBSTACULOS = 5;
const int INDICE_MOPA = 0;

#define MIN_COMENSALES 1
#define MESA  'T'
#define LINGUINI 'L'
#define COCINA 'C'
#define MONEDA 'M'
#define PATINES 'P'
#define MOPA 'O'
#define CHARCO 'H'
#define ARRIBA 'W'
#define ABAJO 'S'
#define IZQUIERDA 'A'
#define DERECHA 'D'
#define VACIO '.'
#define ASIENTO_OCUPADO 'X'

const int GANO = 1;
const int PERDIO = -1;
const int CONTINUAR_JUGANDO = 0;
const int OBJETIVO_DINERO = 150;
const int MAX_MOVIMIENTOS = 200;
const int CANTIDAD_MONEDAS = 8;
const int CANTIDAD_MOPAS = 1;
const int CANTIDAD_PATINES = 5;
const int CANTIDAD_CHARCOS = 5;

/*Structs utilizados*/

typedef struct mesa_aleat{
    coordenada_t coordenada;
    int cantidad_lugares;
}mesa_aleat_t;


/*Mis funciones*/

void esperar_input(){
    getchar();
    getchar();
}

/*
* Pre: - 
* Posts: Inicializa todos los valores como false.
*/

void inicializar_tablero(bool tablero[MAX_FILAS][MAX_COLUMNAS]) {
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            tablero[i][j] = false;
        }
    }
}

/*
* Pre condiciones: Mapa debe estar previamente creado.

* Post condiciones: Inicializa todos los valores del mapa como VACIO.

*/

void inicializar_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            mapa[i][j] = VACIO; 
        }
    }
}


/*
* Pre condiciones: Juego.cantidad_mesas/cantidad_herramientas/cantidad_obstaculos deben estar previamente inicializados. 

* Post condiciones: Asigna las posiciones de los elementos de juego en la matriz mapa recibida por parametro.

*/

void asignar_posiciones(juego_t *juego,char mapa[MAX_FILAS][MAX_COLUMNAS]){

    for(int i = 0; i < juego->cantidad_mesas; i++){
        for(int j = 0; j < juego->mesas[i].cantidad_lugares; j++){
            mapa[juego->mesas[i].posicion[j].fil][juego->mesas[i].posicion[j].col] = MESA;
            if(juego->mesas[i].cantidad_comensales != 0){
                for(int k = 0; k < juego->mesas[i].cantidad_comensales; k++){
                    mapa[juego->mesas[i].posicion[k].fil][juego->mesas[i].posicion[k].col] = ASIENTO_OCUPADO;
                }
            }
        }
    }

    for(int i = 0; i < juego->cantidad_herramientas; i++){
        mapa[juego->herramientas[i].posicion.fil][juego->herramientas[i].posicion.col] = juego->herramientas[i].tipo;
    }

    for(int i = 0; i < juego->cantidad_obstaculos ; i++){
        mapa[juego->obstaculos[i].posicion.fil][juego->obstaculos[i].posicion.col] = juego->obstaculos[i].tipo;
    }

    if(juego->mozo.tiene_mopa == true){
        mapa[juego->herramientas[0].posicion.fil][juego->herramientas[0].posicion.col] = VACIO;
    }
    

    /*Asigno la posicion a linguini y a la cocina*/

    mapa[juego->mozo.posicion.fil][juego->mozo.posicion.col] = LINGUINI;

    mapa[juego->cocina.posicion.fil][juego->cocina.posicion.col] = COCINA;
}

/*
* Pre condiciones: El mapa y todos los campos en juego a mostrar deben estar incializados.

* Post condiciones: Muestra el mapa y los datos importantes del juego por pantalla.

*/

void imprimir_juego(juego_t juego){

    char mapa[MAX_FILAS][MAX_COLUMNAS];
    inicializar_mapa(mapa);

    asignar_posiciones(&juego, mapa);
    system("clear");
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            printf("|%c", mapa[i][j]);
        }
        printf("\n");
    }
    printf("\nDinero = %i, Movimientos (A los 200 se termina el dia) = %i, Pedidos = %i, Pedidos en bandeja = %i, Patines = %i \n", juego.dinero, juego.movimientos, juego.mozo.cantidad_pedidos, juego.mozo.cantidad_bandeja,juego.mozo.cantidad_patines);
}

/*
* Pre condiciones: Los valores de la mesa y el mapa deben estar previamente inicializados. 

* Post condiciones: Devuelve true si la posicion de la mesa es valida, false en caso contrario.

*/

bool es_mesa_valida(mesa_aleat_t valores_mesa, bool tablero[MAX_FILAS][MAX_COLUMNAS]){

    bool es_valida = true;

    int fila_aleatoria = valores_mesa.coordenada.fil;
    int columna_aleatoria = valores_mesa.coordenada.col;
    int cantidad_lugares = valores_mesa.cantidad_lugares;

    if (fila_aleatoria < 0 || columna_aleatoria < 0 || fila_aleatoria + cantidad_lugares > MAX_FILAS || columna_aleatoria + cantidad_lugares > MAX_COLUMNAS) {
        es_valida = false;
    }


    for (int i = fila_aleatoria - 1; i <= fila_aleatoria + cantidad_lugares; i++) {
        for (int j = columna_aleatoria - 1; j <= columna_aleatoria + cantidad_lugares; j++) {
            if (i >= 0 && i < MAX_FILAS && j >= 0 && j < MAX_COLUMNAS) {
                if (tablero[i][j]) {
                    es_valida = false;
                }
            }
        }
    }


    if(es_valida){
        for(int i = fila_aleatoria; i < fila_aleatoria + cantidad_lugares; i++){
            for(int j = columna_aleatoria; j < columna_aleatoria + cantidad_lugares; j++){
                tablero[i][j] = true;
            }
        }
    }

    return es_valida;
}

/*
* Pre: La posicion y el tablero deben estar inicializados.
* Post: Devuelve true si la posicion es valida, false en caso contrario.
*/

bool es_posicion_valida(coordenada_t posicion_aleatoria, bool tablero[MAX_FILAS][MAX_COLUMNAS]){
    int i = posicion_aleatoria.fil;
    int j = posicion_aleatoria.col;

    if(!tablero[i][j] && i < MAX_FILAS && j < MAX_COLUMNAS){
        tablero[i][j] = true;
        return true;
    }
    return false;
}

/*
* Pre condiciones: i debe estar previamente inicializado.

* Post condiciones:Crea la posicion inicial para una mesa.  Si i es menor a 4 asigna una mesa de 4 comensales, si i es mayor o igual a 4 asigna una mesa de 1 comensal. 

*/

mesa_aleat_t generar_mesa(int i){
    mesa_aleat_t valores_mesa_aleatoria ;
    valores_mesa_aleatoria.coordenada.fil = rand() % MAX_FILAS;
    valores_mesa_aleatoria.coordenada.col = rand() % MAX_COLUMNAS;
    if (i < 4) {
        valores_mesa_aleatoria.cantidad_lugares = 4;
    } else {
        valores_mesa_aleatoria.cantidad_lugares = 1; 
    }

    return valores_mesa_aleatoria;
}

/*
* Pre: valores_mesa debe estar inicializado.
* Post: Inicializa el struct mesa.
*/

void asignar_mesa(mesa_t *mesa, mesa_aleat_t valores_mesa) {
    if(valores_mesa.cantidad_lugares == 1){
        mesa->posicion[0].fil = valores_mesa.coordenada.fil;
        mesa->posicion[0].col = valores_mesa.coordenada.col;

        mesa->cantidad_lugares = valores_mesa.cantidad_lugares;
    }else{
        mesa->posicion[0].fil = valores_mesa.coordenada.fil;
        mesa->posicion[0].col = valores_mesa.coordenada.col;
        mesa->posicion[1].fil = valores_mesa.coordenada.fil;
        mesa->posicion[1].col = valores_mesa.coordenada.col + 1;
        mesa->posicion[2].fil = valores_mesa.coordenada.fil + 1;
        mesa->posicion[2].col = valores_mesa.coordenada.col;
        mesa->posicion[3].fil = valores_mesa.coordenada.fil + 1;
        mesa->posicion[3].col = valores_mesa.coordenada.col + 1;
    
        mesa->cantidad_lugares= valores_mesa.cantidad_lugares;
    }
    mesa->cantidad_comensales = 0;
    mesa->pedido_tomado = false;
    mesa->paciencia = 0;
}


/*
* Pre condiciones: MAX_FILAS y MAX_COLUMNAS deben estar previamente incializados.

* Post condiciones: Crea una valor aleatorio en el rango de MAX_FILAS y MAX_COLUMNAS, creando una fila y columna aleatoria. 

*/

coordenada_t generar_posicion_aleatoria(){
    coordenada_t posicion_aleatoria ;
    posicion_aleatoria.fil = rand() % MAX_FILAS;
    posicion_aleatoria.col = rand() % MAX_COLUMNAS;
    return posicion_aleatoria;
}

/*
* Pre condiciones: Tablero y la cantidad de mesas deben estar previamente inicializados.

* Post condiciones: Inicializa las mesas.

*/

void inicializar_mesas(bool tablero[MAX_FILAS][MAX_COLUMNAS], juego_t *juego){

    
    int i = 0;
    while (i <= juego->cantidad_mesas){


        mesa_aleat_t valores_mesa_aleatoria = generar_mesa(i);


        bool mesa_chequeada = es_mesa_valida(valores_mesa_aleatoria, tablero);

        
        if(mesa_chequeada){
            asignar_mesa(&juego->mesas[i], valores_mesa_aleatoria);
            i++;
        }

        
    }
}

/*
* Pre condiciones: Tablero debe estar previamente inicializado.

* Post condiciones: Inicializa la cocina.

*/

void inicializar_cocina(bool tablero[MAX_FILAS][MAX_COLUMNAS], cocina_t *cocina){
    
    coordenada_t posicion_aleatoria_cocina;

    bool es_pos_valida_cocina;

    bool posicion_invalida = true;

    while(posicion_invalida){
        posicion_aleatoria_cocina = generar_posicion_aleatoria();
        es_pos_valida_cocina = es_posicion_valida(posicion_aleatoria_cocina, tablero);
        if(es_pos_valida_cocina){
            cocina->posicion.fil = posicion_aleatoria_cocina.fil;
            cocina->posicion.col = posicion_aleatoria_cocina.col;
            posicion_invalida = false;
        }
    }
}

/*
* Pre condiciones: Tablero debe estar previamente inicializado.

* Post condiciones: Inicializa un objeto.

*/

void inicializar_objetos(bool tablero[MAX_FILAS][MAX_COLUMNAS], char tipo, objeto_t *objeto){
        bool es_pos_valida_objetos;
        bool posicion_invalida = true;
        while(posicion_invalida){
            coordenada_t posicion = generar_posicion_aleatoria();
            es_pos_valida_objetos = es_posicion_valida(posicion, tablero);
            if(es_pos_valida_objetos){
                objeto->posicion.fil = posicion.fil;
                objeto->posicion.col = posicion.col;
                objeto->tipo = tipo;
                posicion_invalida = false;
            }
        }
    }

/*
* Pre condiciones: El tablero, la cantidad de herramientas y cantidad de patines deben estar inicializados.

* Post condiciones: Inicializa los elementos de herramientas.
*/

void inicializar_herramientas(bool tablero[MAX_FILAS][MAX_COLUMNAS], juego_t *juego){
    for(int i = 0; i < juego->cantidad_herramientas ; i++){
        if( i == 0){
            inicializar_objetos(tablero, MOPA, &juego->herramientas[i]);
        }else if(i <= CANTIDAD_PATINES){
            inicializar_objetos(tablero, PATINES, &juego->herramientas[i]);
        }else{
            inicializar_objetos(tablero, MONEDA, &juego->herramientas[i]);
        }
    }
}

/*
* Pre condiciones: Tablero y cantidad de obstaculos deben estar previamente inicializados.

* Post condiciones: Inicializa los obstaculos.

*/

void inicializar_obstaculos(bool tablero[MAX_FILAS][MAX_COLUMNAS], juego_t *juego){
    for(int i = 0; i < juego->cantidad_obstaculos ; i++){
        inicializar_objetos(tablero, CHARCO, &juego->obstaculos[i]);
    }
}

/*
* Pre condiciones: Tablero debe estar previamente inicializado.

* Post condiciones: Inicializa el mozo.

*/

void inicializar_mozo(bool tablero[MAX_FILAS][MAX_COLUMNAS], mozo_t *mozo){
    
    coordenada_t posicion_aleatoria_mozo;

    bool es_pos_valida_mozo;

    bool posicion_invalida = true;
    while(posicion_invalida){
        posicion_aleatoria_mozo = generar_posicion_aleatoria();
        es_pos_valida_mozo = es_posicion_valida(posicion_aleatoria_mozo, tablero);
        if(es_pos_valida_mozo){
            mozo->posicion.fil = posicion_aleatoria_mozo.fil;
            mozo->posicion.col = posicion_aleatoria_mozo.col;
            posicion_invalida = false;
        }
    }
    mozo->tiene_mopa = false;
    mozo->patines_puestos = false;
    mozo->cantidad_patines = 0;
    mozo->cantidad_pedidos = 0;
    mozo->cantidad_bandeja = 0;
}



/*
* Pre condiciones: Mapa debe estar previamente inicializado, la accion debe ser valida y la posicion debe estar inicializada.

* Post condiciones: Devuelve la validez de la nueva posicion generada del mozo.

*/

bool es_accion_valida_mozo(coordenada_t posicion, juego_t juego){

    bool es_valida = true;

    if(posicion.fil >= MAX_FILAS || posicion.fil < 0 || posicion.col >= MAX_COLUMNAS || posicion.col < 0){
        es_valida = false;
    }

    for(int i = 0; i < juego.cantidad_mesas; i++){
        for(int j = 0; j < juego.mesas[i].cantidad_lugares; j++){
            if(posicion.fil == juego.mesas[i].posicion[j].fil && posicion.col == juego.mesas[i].posicion[j].col){
                es_valida = false;
            }
        }
    }

    return es_valida;
}

/*
* Pre condiciones: Las posiciones de las herramientas, los obstaculos y el mozo deben estar previamente inicializadas.

* Post condiciones: Devuelve si se puede soltar la mopa.

*/

bool puedo_soltar_mopa(coordenada_t posicion_mozo, juego_t *juego){

    bool es_accion_valida = true;

    for(int i = 1; i < juego->cantidad_herramientas; i++){
        if(juego->herramientas[i].posicion.fil == posicion_mozo.fil && juego->herramientas[i].posicion.col == posicion_mozo.col){
            es_accion_valida = false;
        }
    }

    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if(juego->obstaculos[i].posicion.fil == posicion_mozo.fil && juego->obstaculos[i].posicion.col == posicion_mozo.col){
            es_accion_valida = false;
        }
    }

    if(juego->cocina.posicion.fil == posicion_mozo.fil && juego->cocina.posicion.col == posicion_mozo.col){
        es_accion_valida = false;
    }

    return es_accion_valida;
}

/*
* Pre condiciones: La posicion del mozo debe estar inicializada.

* Post condiciones: Realiza la accion relacionada con la mopa.

*/

void interaccion_mopa(coordenada_t posicion_mozo, juego_t *juego){

    if(!juego->mozo.tiene_mopa){
        if(juego->herramientas[INDICE_MOPA].posicion.fil == posicion_mozo.fil && juego->herramientas[INDICE_MOPA].posicion.col == posicion_mozo.col){
            printf("\nMopa agarrada.\n");
            sleep(1);
            juego->mozo.tiene_mopa = true;
            juego->movimientos++;
        }else{
            printf("\nNo es posible agarrar la mopa.\n");
            sleep(1);
        }
    }else if(juego->mozo.tiene_mopa){
        bool mopa_soltada = puedo_soltar_mopa(posicion_mozo, juego);

        if(!mopa_soltada){
            printf("\nNo es posible soltar la mopa sobre un espacio ocupado.\n");
            sleep(1);
        }else{
            printf("\nMopa soltada.\n");
            sleep(1);
            juego->herramientas[0].posicion.fil = posicion_mozo.fil;
            juego->herramientas[0].posicion.col = posicion_mozo.col;
            juego->mozo.tiene_mopa = false;
            juego->movimientos++;
        }
    }
}

/*
* Pre condiciones: La accion debe ser valida la posicion del mozo debe estar inicializada.

* Post condiciones: Genera la nueva posicion del mozo en base a que accion se ingreso.

*/

void generar_nueva_accion_mozo(juego_t *juego, char accion){
    
    coordenada_t posicion_actual_mozo_mod;
    posicion_actual_mozo_mod.fil = juego->mozo.posicion.fil;
    posicion_actual_mozo_mod.col = juego->mozo.posicion.col;

    if(accion == ARRIBA){
        posicion_actual_mozo_mod.fil -= 1;
    }
    if ( accion == ABAJO){
        posicion_actual_mozo_mod.fil += 1;
    }
    if(accion == DERECHA){
        posicion_actual_mozo_mod.col += 1;
    }
    if (accion == IZQUIERDA){
        posicion_actual_mozo_mod.col -= 1;
    }

    bool accion_mozo_valida = es_accion_valida_mozo(posicion_actual_mozo_mod, *juego);

    if(accion_mozo_valida){
        juego->mozo.posicion.fil = posicion_actual_mozo_mod.fil;
        juego->mozo.posicion.col = posicion_actual_mozo_mod.col;
        juego->movimientos++;
    }
}


/*Funciones del .h*/


void inicializar_juego(juego_t *juego){

    juego->cantidad_mesas = CANTIDAD_MESAS;
    juego->cantidad_herramientas = CANTIDAD_HERRAMIENTAS;
    juego->cantidad_obstaculos = CANTIDAD_OBSTACULOS;
    juego->dinero = 0;
    juego->movimientos = 0;

    bool tablero[MAX_FILAS][MAX_COLUMNAS];
    inicializar_tablero(tablero);


    inicializar_mesas(tablero, juego);



    inicializar_herramientas(tablero, juego);


    inicializar_obstaculos(tablero, juego);

    
    inicializar_mozo(tablero, &juego->mozo);


    inicializar_cocina(tablero, &juego->cocina);

}



void mostrar_juego(juego_t juego){

    imprimir_juego(juego);
}


void realizar_jugada(juego_t *juego, char accion){

    if(accion == MOPA){
        interaccion_mopa(juego->mozo.posicion, juego);
    }else{
        generar_nueva_accion_mozo(juego, accion);
    }
}

int estado_juego(juego_t juego){
    if(juego.movimientos == MAX_MOVIMIENTOS){
        if(juego.dinero >= OBJETIVO_DINERO){
            return GANO;
        }else{
            return PERDIO;
        }
    }
    return CONTINUAR_JUGANDO;
}