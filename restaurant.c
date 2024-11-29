#include "restaurant.h"

#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MILANESA_NAPOLITANA 'M'
#define HAMBURGUESA 'H'
#define PARRILLA 'P'
#define RATATOUILLE 'R'
#define MILANESA_NAPOLITANA_ID 1
#define HAMBURGUESA_ID 2
#define PARRILLA_ID 3
#define RATATOUILLE_ID 4
#define TIEMPO_MILANESA_NAPOLITANA 30
#define TIEMPO_HAMBURGUESA 15
#define TIEMPO_PARRILLA 20
#define TIEMPO_RATATOUILLE 25
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
#define TOMAR_PEDIDO 'T'
#define VACIO '.'
#define ASIENTO_OCUPADO 'X'
#define CUCARACHA 'U'
#define COLOR_RESET "\x1b[0m"
#define COLOR_TOMADO "\x1b[31m"
#define COLOR_PREPARACION "\x1b[33m" 
#define COLOR_LISTO "\x1b[32m"

const int CANTIDAD_MESAS = 10;
const int PACIENCIA_CUCARACHA = 2;
const int LLEGADA_COMENSALES = 15;
const int APARECEN_CUCARACHAS = 25;
const int CANTIDAD_HERRAMIENTAS = 14;
const int CANTIDAD_OBSTACULOS = 5;
const int INDICE_MOPA = 0;
const int MAX_LUGARES = 4;
const int MIN_LUGARES = 1;
const int VALOR_MONEDA = 1000;
const int RANGO_PACIENCIA_MINIMA = 100;
const int RANGO_PACIENCIA_MAXIMA = 101;
const int GANO = 1;
const int PERDIO = -1;
const int CONTINUAR_JUGANDO = 0;
const int OBJETIVO_DINERO = 150000;
const int MAX_MOVIMIENTOS = 200;
const int CANTIDAD_MONEDAS = 8;
const int CANTIDAD_MOPAS = 1;
const int CANTIDAD_PATINES = 5;
const int CANTIDAD_CHARCOS = 5;
const int DISTANCIA_CUCARACHA = 2;
const int PRECIO_MESA_CUATRO = 20000;
const int PRECIO_MESA_UNO = 5000;
const int ERROR = -1;

/*Structs utilizados*/

typedef struct mesa_aleat{
    coordenada_t coordenada;
    int cantidad_lugares;
}mesa_aleat_t;


/*Mis funciones*/

/*
* Pre condiciones: Primera posicion y segunda posicion deben estar inicializadas.
* Post condicioness: Devuelve al distancia manhattan de ambas posiciones.
*/
int distancia_manhattan(coordenada_t primera_posicion, coordenada_t segunda_posicion){
    return abs(primera_posicion.fil - segunda_posicion.fil) + abs(primera_posicion.col - segunda_posicion.col);
}

/*
* Pre condiciones: Herramientas debe estar correctamente incializado. Cantidad de herramientas debe ser mayor a cero.
* Post condicioness: Devuelve el indice de la mopa. Si no hay un objeto del tipo mopa devuelve menos 1.
*/
int buscar_indice_mopa(objeto_t herramientas[], int cantidad_herramientas){
    int indice_mopa = -1;
    for(int i = 0; i < cantidad_herramientas; i++){
        if(herramientas[i].tipo == MOPA){
            indice_mopa = i;
        }
    }
    return indice_mopa;
}

/*
Pre condiciones: posicion 1 y 2 deben tener las coordenadas inicializadas.
Post condiciones: devuelve true si las coordenadas son iguales y false en caso contrario.
*/
bool estoy_en_misma_pos(coordenada_t posicion_1, coordenada_t posicion_2){
    return posicion_1.fil == posicion_2.fil && posicion_1.col == posicion_2.col;
}

/*
Pre condiciones: maximo_valor debe ser si o si mayor o igual a minimo_valor
Post condiciones: devuelve un numero aleatorio en el rango especificado.
*/
int generar_numero_aleatorio(int rango_maximo, int rango_minimo){
    return ((rand() % rango_maximo) + rango_minimo); 
}


/*
* Pre condiciones: - 
* Post condicioness: Inicializa todos los valores como false.
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

    mapa[juego->mozo.posicion.fil][juego->mozo.posicion.col] = LINGUINI;

    mapa[juego->cocina.posicion.fil][juego->cocina.posicion.col] = COCINA;
}


/*
* Pre condiciones: El mapa y todos los campos en juego a mostrar deben estar incializados.

* Post condiciones: Muestra el mapa y los datos importantes del juego por pantalla.

*/
void imprimir_juego(juego_t juego) {
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    inicializar_mapa(mapa);

    asignar_posiciones(&juego, mapa);
    system("clear");

    printf("INFORMACION\nDinero = %i, Movimientos (A los 200 se termina el dia) = %i, Patines disponibles = %i, Patines activados = %s, Mopa agarrada: %s\n", 
        juego.dinero, 
        juego.movimientos,
        juego.mozo.cantidad_patines, 
        juego.mozo.patines_puestos ? "Sí" : "No", 
        juego.mozo.tiene_mopa ? "Sí" : "No"
    );

    printf("\nDETALLES DE MESAS\n");
    for (int i = 0; i < juego.cantidad_mesas; i++) {
        printf("- Mesa %d: ", i);

        bool en_preparacion = false;
        bool listo = false;
        bool en_bandeja = false;
        int j = 0;
        int k = 0;
        int l = 0;

        while(j < juego.cocina.cantidad_preparacion && !en_preparacion){
            if (juego.cocina.platos_preparacion[j].id_mesa == i) {
                en_preparacion = true;
            }
            j++;
        }

        while(k < juego.cocina.cantidad_listos && !listo){
            if (juego.cocina.platos_listos[k].id_mesa == i) {
                listo = true;
            }
            k++;
        }

        while(l < juego.mozo.cantidad_bandeja && !en_bandeja){
            if (juego.mozo.bandeja[l].id_mesa == i) {
                en_bandeja = true;
            }
            l++;
        }

        if (en_preparacion || listo || en_bandeja) {
            if (en_preparacion) {
                printf(COLOR_PREPARACION "Pedido en preparación " COLOR_RESET);
            }
            if (listo) {
                printf(COLOR_LISTO "Pedido listo " COLOR_RESET);
            }
            if (en_bandeja) {
                printf(COLOR_LISTO "Pedido en bandeja " COLOR_RESET);
            }
        } else if (juego.mesas[i].pedido_tomado) {
            printf(COLOR_TOMADO "Pedido tomado " COLOR_RESET);
        } else {
            printf("Sin pedido ");
        }
    }

    printf("\n");
    
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            printf(" %c |", mapa[i][j]);
        }
        printf("\n");
    }
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
* Pre condiciones: La posicion y el tablero deben estar inicializados.
* Post condiciones: Devuelve true si la posicion es valida, false en caso contrario.
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
        valores_mesa_aleatoria.cantidad_lugares = MAX_LUGARES;
    } else {
        valores_mesa_aleatoria.cantidad_lugares = MIN_LUGARES; 
    }

    return valores_mesa_aleatoria;
}

/*
* Pre condiciones: valores_mesa debe estar inicializado.
* Post condiciones: Inicializa el struct mesa.
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

    cocina->cantidad_preparacion = 0;
    cocina->cantidad_listos = 0;
    cocina->platos_preparacion = NULL;
    cocina->platos_listos = NULL;
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

    for(int i = 0; i < juego->cantidad_herramientas; i++){
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
* Pre condiciones: Herramientas debe estar correctamente inciliazado e indice debe ser un valor no negativo y menor que cantidad_herramientas
Post condiciones:La herramienta en la posición de indice será eliminada de herramientas. Todos los elementos de herramientas se moveran una posicion a la derecha
*/
void eliminar_mopa(juego_t *juego, int indice) {
    for (int i = indice; i < juego->cantidad_herramientas - 1; i++) {
        juego->herramientas[i] = juego->herramientas[i + 1];
    }
    juego->cantidad_herramientas--; 
}

/*
Pre condiciones: Herramientas debe estar corrrectamente incializado. Cantidad de herramientas debe ser menor que el maximo de elementos que contiene herramientas. Mopa debe estar correctamente inicializado.
* Post condiciones: Se agrega la mopa a herramientas en la posición del tope que seria cantidad herramientas. 
Se incrementa en uno el tope.
*/
void agregar_mopa(juego_t *juego, objeto_t mopa) {
    juego->herramientas[juego->cantidad_herramientas] = mopa;
    juego->cantidad_herramientas++;
}


/*
* Pre condiciones: La posicion del mozo debe estar inicializada.

* Post condiciones: Realiza la accion relacionada con la mopa.

*/
void interaccion_mopa(coordenada_t posicion_mozo, juego_t *juego){
    int indice_mopa = buscar_indice_mopa(juego->herramientas,juego->cantidad_herramientas);
    if(!juego->mozo.tiene_mopa){
        if(juego->herramientas[indice_mopa].posicion.fil == posicion_mozo.fil && juego->herramientas[indice_mopa].posicion.col == posicion_mozo.col){
            juego->mozo.tiene_mopa = true;
            eliminar_mopa(juego, indice_mopa);
        }
    }else if(juego->mozo.tiene_mopa){
        bool mopa_soltada = puedo_soltar_mopa(posicion_mozo, juego);
        if(mopa_soltada){
            objeto_t mopa;
            mopa.posicion = posicion_mozo;
            mopa.tipo = MOPA;
            agregar_mopa(juego, mopa);
            juego->mozo.tiene_mopa = false;
        }
    }
}

/*
* Pre condiciones: Las posiciones de las mesas, herramientas , obstaculos, mozo y cocina deben estar correctamente inicializados con sus respectivas cantidades..
* Post condiciones: Devuelve true si la posición está libre, false en caso contrario.
*/
bool es_posicion_libre(juego_t juego, coordenada_t posicion){
    bool hay_lugar = true;

    for(int i = 0; i < juego.cantidad_mesas; i++){
        for(int j = 0; j < juego.mesas[i].cantidad_lugares; j++){
            if(posicion.fil == juego.mesas[i].posicion[j].fil && posicion.col == juego.mesas[i].posicion[j].col){
                hay_lugar = false;
            }
        }
    }

    for(int i = 0; i < juego.cantidad_herramientas ; i++){
        if(posicion.fil == juego.herramientas[i].posicion.fil && posicion.col == juego.herramientas[i].posicion.col){
            hay_lugar = false;
        }
    }

    for(int i = 0; i < juego.cantidad_obstaculos ; i++){
        if(posicion.fil == juego.obstaculos[i].posicion.fil && posicion.col == juego.obstaculos[i].posicion.col){
            hay_lugar = false;
        }
    }

    if(posicion.fil == juego.mozo.posicion.fil && posicion.col == juego.mozo.posicion.col){
        hay_lugar = false;
    }

    if(posicion.fil == juego.cocina.posicion.fil && posicion.col == juego.cocina.posicion.col){
        hay_lugar = false;
    }

    if(juego.mozo.tiene_mopa == false && juego.herramientas[0].posicion.fil == posicion.fil && juego.herramientas[0].posicion.col == posicion.col){
        hay_lugar = false;
    }

    return hay_lugar;
}

/*
Pre condiciones: La posicion y las cantidades de las mesas deben estar correctamente incializadas. Los obstaculos y sus cantidades deben estar correctamente inicializados.

Post condiciones:
Devuelve la distancia mínima en términos de la distancia de Manhattan entre cualquier lugar de la mesa y cualquier cucaracha en el juego.
Si no hay cucarachas en el juego, devuelve 100.
*/
int distancia_de_cucaracha(juego_t *juego, mesa_t mesa) {
    int distancia_minima = 100; 
    for(int j = 0; j < mesa.cantidad_lugares; j++) {
        for(int k = 0; k < juego->cantidad_obstaculos; k++) {
            if(juego->obstaculos[k].tipo == CUCARACHA) {
                int distancia_actual = distancia_manhattan(mesa.posicion[j], juego->obstaculos[k].posicion);
                if(distancia_actual < distancia_minima) {
                    distancia_minima = distancia_actual;
                }
            }
        }
    }
    return distancia_minima;
}

/*
Pre condiciones: El array de pedidos debe estar correctamente inicializado, con todos los campos del tipo pedido_t. La cantidad de pedidos debe ser mayor a cero. La cantidad de pedidos debe ser menor a el máximo de pedidos.
Post condiciones: Elimina el pedido indicado por el id_mesa del array de pedidos. Decrementa la cantidad de pedidos en uno.
*/
void eliminar_pedido(pedido_t pedidos_o_bandeja[], int *cantidad_pedidos, int indice_mesa) {
    for (int i = 0; i < *cantidad_pedidos; i++) {
        if (pedidos_o_bandeja[i].id_mesa == indice_mesa) {
            pedidos_o_bandeja[i] = pedidos_o_bandeja[*cantidad_pedidos - 1];
            (*cantidad_pedidos)--; 
            i--;
        }
    }
}


/*
Pre condiciones: El array de mesas debe estar correctamente inicializado y tener al menos cantidad_mesas elementos.
- El array mesas debe estar correctamente inicializado y tener al menos cantidad_mesas elementos.
- El puntero juego debe apuntar a una estructura de tipo juego_t correctamente inicializada.

Post condiciones: La paciencia de cada mesa en el array mesas se ha disminuye en 1. Si la distancia entre una mesa y una cucaracha es menor o igual a DISTANCIA_CUCARACHA, la paciencia de esa mesa se ha decrementado en PACIENCIA_CUCARACHA adicionalmente. Si la paciencia de una mesa es menor o igual a 0, los comensales de esa mesa se retiran sin pagar y el pedido correspondiente se ha eliminado de la bandeja o de los pedidos del mozo.
*/
void actualizar_paciencia(int cantidad_mesas, mesa_t mesas[], juego_t *juego){
    for(int i = 0; i < cantidad_mesas; i++){

        if(mesas[i].cantidad_comensales != 0){

            mesas[i].paciencia--;

            int distancia_mesa_cucaracha = distancia_de_cucaracha(juego, mesas[i]);

            if(distancia_mesa_cucaracha <= DISTANCIA_CUCARACHA){
                mesas[i].paciencia -= PACIENCIA_CUCARACHA;
            }
    
        }
        
        if(mesas[i].paciencia <= 0){
            mesas[i].cantidad_comensales = 0;

            if(mesas[i].pedido_tomado){
                eliminar_pedido(juego->mozo.bandeja,&juego->mozo.cantidad_bandeja, i);
            }else{
                eliminar_pedido(juego->mozo.pedidos, &juego->mozo.cantidad_pedidos, i);
            }
        }
    }

}

/*
Pre condiciones: La posicion del mozo esta correctamente inicializada, el array de obstaculos esta correctamente incializado con todos los campos. Con su respectivo tope menor al maximo de obstaculos.

Post condiciones: Si el mozo se encuentra en la misma posición que un obstáculo de tipo CHARCO, dicho obstáculo será eliminado del arreglo de obstaculos. La cantidad de obstáculos  se actualizará correctamente.
En caso de ser necesario todos los obstaculos se mueven uno hacia la izquierda.
*/
void interaccion_charcos(mozo_t mozo, objeto_t obstaculos[], int *cantidad_obstaculos){
    for(int i = 0; i < *cantidad_obstaculos ; i++){
        if(obstaculos[i].tipo == CHARCO && obstaculos[i].posicion.fil == mozo.posicion.fil && obstaculos[i].posicion.col == mozo.posicion.col){
            obstaculos[i] = obstaculos[*cantidad_obstaculos - 1];
            (*cantidad_obstaculos)--;
            i--;
        }
    }
}


/*
Pre condiciones: La posicion del mozo esta correctamente inicializada, el array de obstaculos esta correctamente incializado con todos los campos. Con su respectivo tope menor al maximo de obstaculos.

Post condiciones: Si el mozo se encuentra en la misma posición que un obstáculo de tipo CUCARACHA, dicho obstáculo será eliminado del arreglo de obstaculos. La cantidad de obstáculos  se actualizará correctamente.
En caso de ser necesario todos los obstaculos se mueven uno hacia la izquierda.
*/
void interaccion_cucaracha(mozo_t mozo, objeto_t obstaculos[], int *cantidad_obstaculos){
    for(int i = 0; i < *cantidad_obstaculos; i++ ){
        bool hay_cucaracha = estoy_en_misma_pos(mozo.posicion, obstaculos[i].posicion);
        if(obstaculos[i].tipo == CUCARACHA && hay_cucaracha){
            obstaculos[i] = obstaculos[*cantidad_obstaculos - 1];
            (*cantidad_obstaculos)--;
            i--;
        }
    }
}

/*
Pre condiciones: La posicion del mozo debe estar correctamente inicializada. EL objeto mesa_t recibido debe eatar correctamente incializado y el comensal debe estar dentro del rango de cantidad de lugares de la mesa.

Post condiciones: Devuelve `true` si la distancia de Manhattan entre la posición del mozo y la posición del comensal en la mesa es menor o igual a 1.
- Devuelve `false` en caso contrario.
*/
bool esta_cerca_del_mozo(mozo_t *mozo, mesa_t mesa, int comensal) {
    int distancia = distancia_manhattan(mozo->posicion, mesa.posicion[comensal]);
    return distancia <= 1;
}

/*
Pre condiciones:
Mozo debe tener sus peedido_t en bandeja inicializados correctamente.
Mesa debe ser una meza con todos sus campos correctamente incializados. 
EL id_mesa debe ser un numero válido que representa el identificador de una mesa.

Post condiciones: Si el pedido correspondiente a id_mesa se encuentra en la bandeja del mozo, será eliminado.
El dinero del juego se incrementará según la cantidad de lugares en la mesa.
- Los atributos pedido tomado, paciencia y cantidad comensales de la mesa serán actualizados.
*/
void procesar_entrega_pedido(mozo_t *mozo, mesa_t *mesa, juego_t *juego, int id_mesa) {
    int i = 0;
    bool pedido_entregado = false;

    while (i < mozo->cantidad_bandeja && !pedido_entregado && mozo->cantidad_bandeja > 0) {
        if (mozo->bandeja[i].id_mesa == id_mesa) {
            eliminar_pedido(mozo->bandeja, &mozo->cantidad_bandeja, id_mesa);
            if(mesa->cantidad_lugares > 1){
                juego->dinero += PRECIO_MESA_CUATRO;
            }else{
                juego->dinero += PRECIO_MESA_UNO;
            }

            mesa->pedido_tomado = false;
            mesa->paciencia = 0;
            mesa->cantidad_comensales = 0;
            pedido_entregado = true;
        }
        i++;
    }
}

/* Pre condiciones: Mozo y mesa debe estar correctamente inicializado. Procesar_entrega_pedido y esta_cerca_del_mozo deben estar correctamente implementadas. Id mesa debe ser un identificador válido de una mesa existente.

Post condiciones: Si el mozo está cerca de la mesa y el pedido ha sido tomado, se procesa la entrega del pedido.
*/
void entregar_pedidos_mesa(mozo_t *mozo, mesa_t *mesa, juego_t *juego, int id_mesa) {
    int j = 0;
    bool puedo_entregar_pedido = false;
    while (j < mesa->cantidad_comensales && !puedo_entregar_pedido){
        bool mozo_cerca_mesa = esta_cerca_del_mozo(mozo, *mesa, j);
        if (mozo_cerca_mesa && mesa->pedido_tomado) {
            procesar_entrega_pedido(mozo, mesa, juego, id_mesa);
            puedo_entregar_pedido = true;
        }
        j++;
    }
}

/* Pre condiciones: Mozo y mesas deben estar correctamente inicializados. Con cantidad mesas menor al maximo del arreglo de mesas. entegar_pedidos_mesa debe estar correctamente implementada.

Post condiciones: La función entregar_pedidos_mesa habrá sido llamada una vez por cada mesa en el arreglo mesas.
*/
void entregar_pedidos(mozo_t *mozo, mesa_t mesas[], int cantidad_mesas, juego_t *juego) {
    for (int i = 0; i < cantidad_mesas; i++) {
        entregar_pedidos_mesa(mozo, &mesas[i], juego, i);
    }
}

/*Pre condiciones: La posicion del mozo debe estar correctamente incializada. La cantidad de herramientas debe ser menor al maximo del arreglo de herramientas. EL dinero debe estar incializado.

Post condiciones: Si el mozo está en la misma posición que una moneda, la moneda se elimina del arreglo herramientas.
La cantidad de herramientas se decrementa por cada moneda recogida. Se le suma el dinero.
*/
void interaccion_monedas(mozo_t *mozo, objeto_t herramientas[], int *cantidad_herramientas, int *dinero){
    for(int i = 0; i < *cantidad_herramientas; i++ ){
        bool hay_moneda = estoy_en_misma_pos(mozo->posicion, herramientas[i].posicion);
        if(herramientas[i].tipo == MONEDA && hay_moneda){
            herramientas[i] = herramientas[*cantidad_herramientas - 1];
            (*cantidad_herramientas)--;
            (*dinero) += VALOR_MONEDA;
            i--;
        }
    }
}

/*
Pre condiciones: Mozo debe estar correctamente inicializado. El arreglo de herramientas debe estar inicializado y contener al menos un elemento. La cantidad de herramientas debe ser mayor o igual a 0.

Post condiciones: Si el mozo está en la misma posición que unos patines, estos se eliminan del arreglo de herramientas. La cantidad de herramientas se decrementa si se recogen patines. La cantidad de patines del mozo se incrementa si se recogen patines.
*/
void interaccion_patines(mozo_t *mozo, objeto_t herramientas[], int *cantidad_herramientas){
    bool no_hay_patines = true;
    int i = 0;

    while(no_hay_patines && i < *cantidad_herramientas){
        bool hay_patines = estoy_en_misma_pos(mozo->posicion,herramientas[i].posicion);
        if(herramientas[i].tipo == PATINES && hay_patines){
            herramientas[i] = herramientas[*cantidad_herramientas - 1];
            i--;
            no_hay_patines = false;
            (*cantidad_herramientas)--;
            mozo->cantidad_patines++;
            
        }
        i++;
    }
}

/*
Pre condiciones:  La cantidad de platos debe ser mayor o igual a 0. El puntero a los platos debe estar inicializado.

Post condiciones: Si la cantidad de platos es mayor a 0, se realoca la memoria para los platos. Si la cantidad de platos es 0, se libera la memoria de los platos y se establece el puntero a NULL. Si no se puede realocar la memoria, se imprime un mensaje de error.
*/
void realocar_mem_platos(int cantidad_platos, pedido_t **platos) {
    if (cantidad_platos > 0) {
        pedido_t *aux_platos = realloc(*platos, (size_t)cantidad_platos * sizeof(pedido_t));
        if (aux_platos == NULL) {
            printf("Error al reallocar memoria en platos_preparacion.\n");
            return;
        }
        *platos = aux_platos; 
    } else if (cantidad_platos == 0) {
        free(*platos);  
        *platos = NULL;
    } else {
        printf("Error: cantidad_platos negativa en realocar_mem_platos.\n");
    }
}


/*
Pre condiciones: El mozo y la cocina deben estar inicializados. La cantidad de platos listos en la cocina debe ser mayor o igual a 0. La cantidad de platos en la bandeja del mozo debe ser menor o igual a MAX_BANDEJA.

Post condiciones: Los platos listos de la cocina se mueven a la bandeja del mozo hasta que la bandeja esté llena o no queden platos listos. La cantidad de platos listos en la cocina se actualiza. La memoria para los platos listos en la cocina se realoca según la nueva cantidad de platos listos.
*/
void pedidos_en_bandeja(mozo_t *mozo, cocina_t *cocina) {
    int i = 0;
    int espacio_libre = mozo->cantidad_bandeja;

    while (i < cocina->cantidad_listos && espacio_libre < MAX_BANDEJA) {
        mozo->bandeja[espacio_libre] = cocina->platos_listos[i];
        mozo->cantidad_bandeja++;
        espacio_libre++;
        i++;
    }
    for (int j = i; j < cocina->cantidad_listos; j++) {
        cocina->platos_listos[j - i] = cocina->platos_listos[j];
    }

    cocina->cantidad_listos -= i;
    realocar_mem_platos(cocina->cantidad_listos, &cocina->platos_listos);

}

/*
Pre condiciones: La cocina debe estar inicializada. La cantidad de platos en preparación debe ser mayor o igual a 0.

Post condiciones: Decrementa el tiempo de preparación de cada plato en preparación. Si el tiempo de preparación de un plato llega a 0, se mueve a la lista de platos listos. La cantidad de platos en preparación se actualiza. La memoria para los platos en preparación y platos listos se realoca según sea necesario. Si no se puede realocar la memoria, se imprime un mensaje de error.
*/
void actualizar_pedidos(cocina_t *cocina) {
    if (cocina == NULL) {
        printf("Error: cocina es NULL.\n");
        return;
    }

    int i = 0;
    while (i < cocina->cantidad_preparacion) {

        cocina->platos_preparacion[i].tiempo_preparacion--;

        if (cocina->platos_preparacion[i].tiempo_preparacion <= 0) {

            pedido_t *aux_platos_listos = realloc(cocina->platos_listos, (size_t)(cocina->cantidad_listos + 1) * sizeof(pedido_t));
            if (aux_platos_listos == NULL) {
                printf("Error al reservar memoria para platos listos.\n");
                return;
            }
            cocina->platos_listos = aux_platos_listos;

            cocina->platos_listos[cocina->cantidad_listos] = cocina->platos_preparacion[i];
            cocina->cantidad_listos++;

            for (int j = i; j < cocina->cantidad_preparacion - 1; j++) {
                cocina->platos_preparacion[j] = cocina->platos_preparacion[j + 1];
            }
            cocina->cantidad_preparacion--;

            realocar_mem_platos(cocina->cantidad_preparacion, &cocina->platos_preparacion);

        } else {
            i++;
        }
    }
}

/*
Pre condiciones: El mozo y la cocina deben estar inicializados. La cantidad de pedidos del mozo debe ser mayor o igual a 0.

Post condiciones: Los pedidos del mozo se mueven a la cocina para su preparación. La memoria para los platos en preparación en la cocina se realoca según la nueva cantidad de platos en preparación. Si no se puede realocar la memoria, se imprime un mensaje de error.
*/
void encargar_pedidos(mozo_t *mozo, cocina_t *cocina) {
    int pedidos_encargados = mozo->cantidad_pedidos;

    cocina->platos_preparacion = realloc(cocina->platos_preparacion, (size_t)(cocina->cantidad_preparacion + pedidos_encargados) * sizeof(pedido_t));
    if (cocina->platos_preparacion == NULL) {
        printf("Error al reservar la memoria para los platos en preparación.\n");
        return;
    }

    int i = 0;
    while (i < pedidos_encargados && cocina->cantidad_preparacion < MAX_BANDEJA) {
        cocina->platos_preparacion[cocina->cantidad_preparacion] = mozo->pedidos[i];
        eliminar_pedido(mozo->pedidos, &mozo->cantidad_pedidos, mozo->pedidos[i].id_mesa);
        cocina->cantidad_preparacion++;
        i++;
    }
}


/*
Pre condiciones: El mozo y la cocina deben estar inicializados. La cantidad de pedidos del mozo debe ser mayor o igual a 0. La cantidad de platos listos en la cocina debe ser mayor o igual a 0. La cantidad de platos en la bandeja del mozo debe ser menor o igual a MAX_BANDEJA.

Post condiciones: Si el mozo tiene pedidos, estos se mueven a la cocina para su preparación. Si hay platos listos en la cocina y hay espacio en la bandeja del mozo, los platos se mueven a la bandeja del mozo.
*/
void gestionar_pedidos_mozo(mozo_t *mozo, cocina_t *cocina) {
    
    if(mozo->cantidad_pedidos > 0){
        encargar_pedidos(mozo, cocina);
    }
    if(cocina->cantidad_listos > 0 && mozo->cantidad_bandeja <= 6){
        pedidos_en_bandeja(mozo, cocina);
    }
}


/*
Pre condiciones: El mozo y la cocina deben estar inicializados. La posición del mozo y la posición de la cocina deben estar inicializadas.

Post condiciones: Si el mozo está en la misma posición que la cocina y no tiene mopa, se gestionan los pedidos del mozo.
*/
void interaccion_cocina(mozo_t *mozo, cocina_t *cocina){
    if(!mozo->tiene_mopa ){
        bool misma_posicion_cocina = estoy_en_misma_pos(mozo->posicion,cocina->posicion);
        if(misma_posicion_cocina){
            gestionar_pedidos_mozo(mozo, cocina);
        }
    }else{
        return;
    }
}


/*
Pre condiciones: El mozo, los obstáculos, la cantidad de obstáculos y las mesas deben estar inicializados.

Post condiciones: Si el mozo está en la misma posición que un charco, los comensales de las mesas correspondientes a los pedidos en la bandeja del mozo se eliminan. Los pedidos en la bandeja del mozo se eliminan.
*/
void interaccion_charcos_bandeja(mozo_t *mozo, objeto_t obstaculos[], int *cantidad_obstaculos, mesa_t mesas[]) {
    for (int i = 0; i < *cantidad_obstaculos; i++) {
        if (mozo->posicion.fil == obstaculos[i].posicion.fil && mozo->posicion.col == obstaculos[i].posicion.col && obstaculos[i].tipo == CHARCO) {
            for (int j = mozo->cantidad_bandeja - 1; j >= 0; j--){
                mesas[mozo->bandeja[j].id_mesa].cantidad_comensales = 0;
                eliminar_pedido(mozo->bandeja, &mozo->cantidad_bandeja, mozo->bandeja[j].id_mesa);
            }
        }
    }
}

/*
Pre condiciones: El mozo debe estar inicializado. El arreglo de herramientas debe estar inicializado y contener al menos un elemento. La cantidad de herramientas debe ser mayor o igual a 0. El puntero al dinero debe estar inicializado.

Post condiciones: Si el mozo está en la misma posición que unos patines, estos se eliminan del arreglo de herramientas y la cantidad de patines del mozo se incrementa. Si el mozo está en la misma posición que una moneda, esta se elimina del arreglo de herramientas y el dinero del mozo se incrementa. La cantidad de herramientas se decrementa por cada herramienta recogida.
*/
void interaccion_herramientas(mozo_t *mozo, objeto_t herramientas[], int *cantidad_herramientas, int *dinero){

    if(mozo->tiene_mopa){
        return;
    }else{
        interaccion_patines(mozo, herramientas, cantidad_herramientas);
        interaccion_monedas(mozo, herramientas, cantidad_herramientas, dinero);
    }
    
}


/*
Pre condiciones: El mozo, los obstáculos, la cantidad de obstáculos y las mesas deben estar inicializados.

Post condiciones: Si el mozo tiene mopa, interactúa con los charcos. Si el mozo no tiene mopa y tiene pedidos en la bandeja, interactúa con los charcos y elimina los pedidos. Interactúa con las cucarachas.
*/
void interaccion_obstaculos(mozo_t *mozo, objeto_t obstaculos[], int *cantidad_obstaculos, mesa_t mesas[]) {

    if(mozo->tiene_mopa){
        interaccion_charcos(*mozo, obstaculos, cantidad_obstaculos);
    }else{
        if(mozo->cantidad_bandeja > 0){
            interaccion_charcos_bandeja(mozo, obstaculos, cantidad_obstaculos, mesas);
        }
        interaccion_cucaracha(*mozo, obstaculos, cantidad_obstaculos);
    }
}

/*
Pre condiciones: El juego y la acción deben estar inicializados. La posición del mozo debe estar inicializada.

Post condiciones: Realiza la acción del mozo con patines y actualiza su posición y estado. Si el movimiento es válido, interactúa con los obstáculos y herramientas. Incrementa el contador de movimientos del juego. Desactiva los patines del mozo y decrementa la cantidad de patines.
*/
void accion_mozo_patines(juego_t *juego, char accion) {
    bool movimiento_valido = true;
    coordenada_t posicion_actual_mozo = juego->mozo.posicion;
    
    while (movimiento_valido) {
        if (accion == ARRIBA) {
            posicion_actual_mozo.fil -= 1;
        } else if (accion == ABAJO) {
            posicion_actual_mozo.fil += 1;
        } else if (accion == DERECHA) {
            posicion_actual_mozo.col += 1;
        } else if (accion == IZQUIERDA) {
            posicion_actual_mozo.col -= 1;
        }
        
        movimiento_valido = es_accion_valida_mozo(posicion_actual_mozo, *juego);

        if(movimiento_valido){
            interaccion_obstaculos(&juego->mozo, juego->obstaculos, &juego->cantidad_obstaculos, juego->mesas);
            interaccion_herramientas(&juego->mozo, juego->herramientas, &juego->cantidad_herramientas, &juego->dinero);
            juego->mozo.posicion = posicion_actual_mozo; 
        }
    }
    juego->movimientos++;
    juego->mozo.patines_puestos = false;
    juego->mozo.cantidad_patines--;
}

/*
Pre condiciones: El arreglo de pedidos debe estar inicializado. La cantidad de pedidos debe ser mayor o igual a 0. La mesa debe estar inicializada y contener comensales. El índice de la mesa debe ser válido.

Post condiciones: Genera un nuevo pedido para la mesa y lo agrega al arreglo de pedidos. Actualiza la cantidad de pedidos. Asigna el tiempo de preparación y la cantidad de platos al pedido.
*/
void generar_pedido(pedido_t pedidos[], int *cantidad_pedidos, mesa_t mesa, int indice_mesa) {
    int cantidad_platos_actual = 0;
    int tiempo_de_preparacion_actual = 0;
    for(int i = 0; i < mesa.cantidad_comensales; i++) {
        int numero_pedido = generar_numero_aleatorio(4, 1);  
        if (numero_pedido == MILANESA_NAPOLITANA_ID) {
            pedidos[*cantidad_pedidos].platos[cantidad_platos_actual] = MILANESA_NAPOLITANA;
            if(TIEMPO_MILANESA_NAPOLITANA > tiempo_de_preparacion_actual)
                tiempo_de_preparacion_actual = TIEMPO_MILANESA_NAPOLITANA;
        } else if (numero_pedido == PARRILLA_ID) {
            pedidos[*cantidad_pedidos].platos[cantidad_platos_actual] = PARRILLA;
            if(TIEMPO_PARRILLA > tiempo_de_preparacion_actual)
                tiempo_de_preparacion_actual = TIEMPO_PARRILLA;
        } else if (numero_pedido == HAMBURGUESA_ID) {
            pedidos[*cantidad_pedidos].platos[cantidad_platos_actual] = HAMBURGUESA;
            if(TIEMPO_HAMBURGUESA > tiempo_de_preparacion_actual)
                tiempo_de_preparacion_actual = TIEMPO_HAMBURGUESA;
        } else {
            pedidos[*cantidad_pedidos].platos[cantidad_platos_actual] = RATATOUILLE;
            if(TIEMPO_RATATOUILLE > tiempo_de_preparacion_actual)
                tiempo_de_preparacion_actual = TIEMPO_RATATOUILLE;
        }

        cantidad_platos_actual++;
    }
    pedidos[*cantidad_pedidos].tiempo_preparacion = tiempo_de_preparacion_actual;
    pedidos[*cantidad_pedidos].id_mesa = indice_mesa;
    pedidos[*cantidad_pedidos].cantidad_platos = cantidad_platos_actual;
    (*cantidad_pedidos)++;
}

/*
Pre condiciones: Las posiciones del mozo y de la mesa deben estar inicializadas.

Post condiciones: Devuelve la distancia de Manhattan entre el mozo y la mesa.
*/
int distancia_mozo_mesa(coordenada_t posicion_mozo, coordenada_t posicion_ocupada_mesa){
    return distancia_manhattan(posicion_mozo, posicion_ocupada_mesa);
}

/*
Pre condiciones: La posición del mozo y el juego deben estar inicializados.

Post condiciones: Toma el pedido de la mesa si el mozo está cerca y la mesa no ha tomado un pedido. Genera un nuevo pedido y lo agrega al arreglo de pedidos del mozo.
*/
void tomar_pedido(coordenada_t posicion_mozo, juego_t *juego) {
    for (int i = 0; i < juego->cantidad_mesas; i++) {
        if (juego->mesas[i].cantidad_comensales != 0 && !juego->mesas[i].pedido_tomado) {
            for (int j = 0; j < juego->mesas[i].cantidad_comensales; j++) {
                int distancia_mozo = distancia_mozo_mesa(posicion_mozo, juego->mesas[i].posicion[j]);

                if (distancia_mozo <= 1 && juego->mozo.cantidad_pedidos < MAX_PEDIDOS ){
                    juego->mesas[i].pedido_tomado = true;
                    generar_pedido(juego->mozo.pedidos, &juego->mozo.cantidad_pedidos, juego->mesas[i], i);
                }
            }
        }
    }
}



/*
Pre condiciones: La cantidad de patines y el juego deben estar inicializados. La acción debe ser válida.

Post condiciones: Activa los patines del mozo si tiene patines disponibles y no tiene mopa.
*/
void activar_patines(int cantidad_patines, juego_t *juego, char accion){
    if(juego->mozo.tiene_mopa){
        return;
    }
    if(cantidad_patines > 0){
        if(!juego->mozo.patines_puestos){
            juego->mozo.patines_puestos = true;
        }
    }
}

/*
Pre condiciones: La posición del mozo y el juego deben estar inicializados.

Post condiciones: Si el mozo no tiene mopa, toma el pedido de la mesa si está cerca y la mesa no ha tomado un pedido.
*/
void interaccion_pedidos(coordenada_t posicion_mozo, juego_t *juego){
    if(!juego->mozo.tiene_mopa){
        tomar_pedido(posicion_mozo, juego);
    }
}


/*
Pre condiciones: La accion debe ser valida la posicion del mozo debe estar inicializada.

Post condiciones: Genera la nueva posicion del mozo en base a que accion se ingreso.

*/
void generar_nueva_accion_mozo(juego_t *juego, char accion){

    if(juego->mozo.patines_puestos){
        accion_mozo_patines(juego, accion); 
        return;
    }

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
        actualizar_paciencia(juego->cantidad_mesas, juego->mesas, juego);
        juego->mozo.posicion.fil = posicion_actual_mozo_mod.fil;
        juego->mozo.posicion.col = posicion_actual_mozo_mod.col;

        interaccion_obstaculos(&juego->mozo, juego->obstaculos, &juego->cantidad_obstaculos, juego->mesas);
        interaccion_herramientas(&juego->mozo, juego->herramientas, &juego->cantidad_herramientas, &juego->dinero);
        if(juego->cocina.cantidad_preparacion > 0){
            actualizar_pedidos(&juego->cocina);
        }
        interaccion_cocina(&juego->mozo, &juego->cocina);
        if(!juego->mozo.tiene_mopa){
            entregar_pedidos(&juego->mozo, juego->mesas, juego->cantidad_mesas, juego);
        }
        juego->movimientos++;
    }
}

/*
Pre condiciones: La cantidad de mesas debe ser mayor o igual a 0. El arreglo de mesas debe estar inicializado. El puntero a índice de mesa con lugar debe estar inicializado. La cantidad de comensales a ingresar debe ser mayor a 0.

Post condiciones: Devuelve true si hay asientos libres para la cantidad de comensales a ingresar, false en caso contrario. Actualiza el índice de la mesa con lugar si se encuentra una mesa disponible.
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
Pre condiciones: El arreglo de mesas debe estar inicializado. La cantidad de mesas debe ser mayor o igual a 0. La cantidad de comensales a ingresar debe ser mayor a 0.

Post condiciones: Asigna los comensales a una mesa disponible. Actualiza la cantidad de comensales y la paciencia de la mesa asignada.
*/
void asignar_comensales(mesa_t *mesa, int cantidad_mesas, int comensales_a_ingresar){

    int indice_mesa_con_lugar;
    bool encontre_lugar = hay_asientos_libres(cantidad_mesas, mesa, &indice_mesa_con_lugar, comensales_a_ingresar);

    if(encontre_lugar){
        int paciencia_aleatoria = generar_numero_aleatorio(RANGO_PACIENCIA_MAXIMA, RANGO_PACIENCIA_MINIMA);
        mesa[indice_mesa_con_lugar].cantidad_comensales = comensales_a_ingresar;
        mesa[indice_mesa_con_lugar].paciencia = paciencia_aleatoria;
    }
}


/*
Pre condiciones: El arreglo de obstáculos debe estar inicializado y tener suficiente espacio para agregar nuevos obstáculos. La función generar_posicion_aleatoria debe estar implementada y devolver una posición válida. La función es_posicion_libre debe estar implementada y devolver true si la posición está libre.


Post condiciones: Genera una posición aleatoria y, si está libre, coloca una cucaracha en esa posición. Incrementa la cantidad de obstáculos en el juego.
*/
void aparecer_cucarachas(juego_t *juego){
    coordenada_t posicion_aleatoria;
    bool posicion_invalida = true;

    while(posicion_invalida){
        posicion_aleatoria = generar_posicion_aleatoria();
        bool encontre_posicion = es_posicion_libre(*juego, posicion_aleatoria);
        if(encontre_posicion){
            juego->obstaculos[juego->cantidad_obstaculos].posicion.fil = posicion_aleatoria.fil;
            juego->obstaculos[juego->cantidad_obstaculos].posicion.col = posicion_aleatoria.col;
            juego->obstaculos[juego->cantidad_obstaculos].tipo = CUCARACHA;
            juego->cantidad_obstaculos++;
            posicion_invalida = false;
        }
    }
}


/*
Pre condiciones: El arreglo de mesas debe estar inicializado y tener suficiente espacio para agregar nuevos comensales. La función generar_numero_aleatorio debe estar implementada y devolver un número válido de comensales. La función asignar_comensales debe estar implementada y ser capaz de asignar comensales a las mesas disponibles.


Post condiciones: Genera una cantidad aleatoria de comensales y los asigna a las mesas disponibles en el juego.
*/
void llegada_comensales(juego_t *juego){

    int cantidad_comensales = generar_numero_aleatorio(MAX_COMENSALES,MIN_COMENSALES);

    asignar_comensales(juego->mesas,juego->cantidad_mesas, cantidad_comensales);
}


/*
Pre condiciones: Los movimientos del juego debe estar inicializado.
Las constantes LLEGADA_COMENSALES y APARECEN_CUCARACHAS deben estar definidas y ser mayores a 0.

Post condiciones: Si el número de movimientos en el juego es múltiplo de LLEGADA_COMENSALES y mayor a 0, se llamará a la función llegada_comensales. Si el número de movimientos en el juego es múltiplo de APARECEN_CUCARACHAS y mayor a 0, se llamará a la función'aparecer_cucarachas.
*/

void actualizacion_del_juego(juego_t *juego){
    if(juego->movimientos % LLEGADA_COMENSALES == 0 && juego->movimientos > 0){
        llegada_comensales(juego);
    }
    if(juego->movimientos % APARECEN_CUCARACHAS == 0 && juego->movimientos > 0){
        aparecer_cucarachas(juego);
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
    }else if(accion == PATINES){
        activar_patines(juego->mozo.cantidad_patines, juego, accion);
    }else if(accion == TOMAR_PEDIDO){
        interaccion_pedidos(juego->mozo.posicion, juego);
    }
    else{
        generar_nueva_accion_mozo(juego, accion);
    }

    actualizacion_del_juego(juego);
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

void destruir_juego(juego_t *juego){
    free(juego->cocina.platos_preparacion);
    free(juego->cocina.platos_listos);
}