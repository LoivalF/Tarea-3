#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structs a usar
typedef struct {
    char nombre[50]; // Nombre del item
    int peso; // Peso del item
    int valor; // Valor del item
} Item;
typedef struct {
  int arriba;
  int abajo;
  int izquierda;
  int derecha;
} Direcciones;
typedef struct {
    int id; // ID del escenario
    char nombre[50]; // Nombre del escenario
    char descripcion[200]; // Descripción del escenario
    List *items; // Lista de items (Item)
    Direcciones conexiones; // Claves: "arriba", "abajo", "izquierda", "derecha".
    int es_final; // 1 si es final, 0 si no
} Escenario;

/* Carga canciones desde un archivo CSV */
void *leer_escenarios(Map *mapa_escenarios) {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("graphquest.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Escenario *escenario = malloc(sizeof(Escenario));
    if (escenario == NULL) exit(EXIT_FAILURE);

    escenario->id = atoi(campos[0]);
    escenario->es_final = atoi(campos[8]);

    strncpy(escenario->nombre, campos[1], sizeof(escenario->nombre));
    strncpy(escenario->descripcion, campos[2], sizeof(escenario->descripcion));

    escenario->items = list_create();
    if (escenario->items == NULL) exit(EXIT_FAILURE);

    // Cargar los items
    List *items = split_string(campos[3], ";");

    for(char *item = list_first(items); item != NULL; 
          item = list_next(items)){

        List *values = split_string(item, ",");
        char *nombre = list_first(values);
        int valor = atoi(list_next(values));
        int peso = atoi(list_next(values));

        Item *item = malloc(sizeof(Item));
        if (item == NULL) exit(EXIT_FAILURE);

        strncpy(item->nombre, nombre, sizeof(item->nombre));
        item->valor = valor;
        item->peso = peso;
        list_pushBack(escenario->items, item);

        list_clean(values);
        free(values);
    }

    escenario->conexiones.arriba = atoi(campos[4]);
    escenario->conexiones.abajo = atoi(campos[5]);
    escenario->conexiones.izquierda = atoi(campos[6]);
    escenario->conexiones.derecha = atoi(campos[7]);

    int* clave_id = malloc(sizeof(int));
    *clave_id = escenario->id;
    map_insert(mapa_escenarios, clave_id, escenario);
    
    list_clean(items);
    free(items);
    
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas
}

/* Funcion para poder crear un mapa vacio */
int is_equal_str(void *key1, void *key2) { 
    return strcmp((char *)key1, (char *)key2) == 0; 
}
/* Funcion para poder crear un mapa vacio */
int is_equal_int(void *key1, void *key2) {
    return *(int *)key1 == *(int *)key2; 
}



int main() {
  Map *mapa_escenarios = map_create(is_equal_int);
  if (mapa_escenarios == NULL) exit(EXIT_FAILURE);
  leer_escenarios(mapa_escenarios);

  return 0;
}
