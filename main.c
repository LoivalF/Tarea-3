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
    char *nombre; // Nombre del escenario
    char *descripcion; // Descripción del escenario
    List *items; // Lista de items (Item)
    Direcciones conexiones; // Claves: "arriba", "abajo", "izquierda", "derecha".
    int es_final; // 1 si es final, 0 si no
} Escenario;

Escenario escenarios[17];
typedef struct {
    List* inventario;
    int tiempo;
    int puntaje;
    int peso_total;
    Escenario* actual;
} Jugador;

/* Funcion para poder crear un mapa vacio */
int is_equal_str(void *a, void *b) { 
    return strcmp((char*)a, (char*)b) == 0; 
}
/* Funcion para poder crear un mapa vacio */
int is_equal_int(void *a, void *b) {
    return *((int*)a) == *((int*)b);
}


Escenario* escenariosALL[17];

/* Carga canciones desde un archivo CSV */
void leer_escenarios() {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("graphquest.csv", "r");
  if (archivo == NULL) {
    perror("Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  fgets((char[256]){0}, 256, archivo); 

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Escenario *nuevoEscenario = malloc(sizeof(Escenario));

    int id = atoi(campos[0]);
    nuevoEscenario->id = id;
    nuevoEscenario->nombre = strdup(campos[1]);
    nuevoEscenario->descripcion = strdup(campos[2]);
    nuevoEscenario->items = list_create();


    if (strcmp(campos[8] ,"Si") == 0) 
      nuevoEscenario->es_final = 1;
    else 
      nuevoEscenario->es_final = 0;
  
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
        list_pushBack(nuevoEscenario->items, item);

        list_clean(values);
        free(values);
    }
    list_clean(items);
    free(items);

    nuevoEscenario->conexiones.arriba = atoi(campos[4]);
    nuevoEscenario->conexiones.abajo = atoi(campos[5]);
    nuevoEscenario->conexiones.izquierda = atoi(campos[6]);
    nuevoEscenario->conexiones.derecha = atoi(campos[7]);
    
    // Agregar el nuevo escenario al arreglo de escenarios
    escenariosALL[id] = nuevoEscenario; // El primero se guarda con ID 1

  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas
}

void recoger_item(Jugador *jugador) {
  printf("Item en el escenario actual: %s\n", jugador->actual->nombre);
  List *items = jugador->actual->items;
  if (list_size(items) == 0) {
    printf("No hay items en este escenario.\n");
    return;
  }
  for (Item *item = list_first(items); item != NULL; item = list_next(items)) {
    printf("Item: %s, Valor: %d, Peso: %d\n", item->nombre, item->valor, item->peso);
  }

  printf("Seleccione el item a recoger: ");
  char nombre[50];
  scanf("%s", nombre); getchar();

  for (Item *item = list_first(items); item != NULL; item = list_next(items)) {
    if (strcmp(item->nombre, nombre) == 0) {
      if (jugador->peso_total + item->peso <= 10) {
        jugador->peso_total += item->peso;
        jugador->puntaje += item->valor;
        list_pushBack(jugador->inventario, item);
        printf("Item %s recogido.\n", item->nombre);
      } 
      else {
        printf("No puedes cargar mas peso.\n");
      }
      return;
    }
  }
  printf("Item no encontrado.\n");
}

void descartar_item(Jugador *jugador) {
  printf("Items en el inventario\n");
  if (list_size(jugador->inventario == 0)) {
    printf("No hay items en el inventario.\n");
    return;
  }
  for (Item *item = list_first(jugador->inventario); item != NULL; 
        item = list_next(jugador->inventario)) {
    printf("Item: %s, Valor: %d, Peso: %d\n", item->nombre, item->valor, item->peso);
  }
  printf("Seleccione el item a descartar: ");
  char nombre[50]; scanf("%s", nombre); getchar();
  for (Item *item = list_first(jugador->inventario); item != NULL; 
        item = list_next(jugador->inventario)) {
    if (strcmp(item->nombre, nombre) == 0) {
      jugador->peso_total -= item->peso;
      jugador->puntaje -= item->valor;

      list_popCurrent(jugador->inventario, item);
      free(item->nombre);
      free(item);

      printf("Item %s descartado correctamente.\n", item->nombre);
      return;
    }
  }
  printf("Item no encontrado en el inventario.\n");
}

int condicionTiempo(Jugador *jugador) {
  if (jugador->tiempo <= 0) {
    printf("¡Se acabó el tiempo!\n");
    return 0;
  }
  return 1;
}

int main() {
  Jugador jugador;
  jugador.inventario = list_create();
  jugador.tiempo = 10;
  jugador.puntaje = 0;
  jugador.peso_total = 0;

  jugador.actual = NULL;  

  int opcion;

    do {
        printf("=====        GraphQuest        =====\n");
        printf("1. Cargar Laberinto desde CSV\n");
        printf("2. Iniciar Partida\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            // Cargar laberinto
            case 1:
                system("cls||clear");
                leer_escenarios();
                printf("Cargando laberinto...\n");
                break;
            // Iniciar partida
            case 2:
                system("cls||clear");
                if (!escenariosALL[1]) {
                    printf("Primero debe cargar el laberinto.\n");
                    break;
                }

                jugador.actual = escenariosALL[1]; // Comienza en el escenario con ID 1

                if (!jugador.actual) {
                    printf("No se encontro la entrada del laberinto.\n");
                    break;
                }
                // Lógica del juego
                int jugar = 1;
                while (jugar && jugador.tiempo > 0 && jugador.actual != NULL) {
                    printf("\n--- ESCENARIO: %s ---\n", jugador.actual->nombre);
                    printf("%s\n", jugador.actual->descripcion);
                    printf("Tiempo restante: %d\n", jugador.tiempo);
                    printf("Puntaje acumulado: %d\n", jugador.puntaje);
                    printf("Peso: %d kg\n", jugador.peso_total);
                    printf("Items disponibles:\n");

                    for (Item* item = list_first(jugador.actual->items); item != NULL; item = list_next(jugador.actual->items))
                        printf(" - %s (%d pts, %d kg)\n", item->nombre, item->valor, item->peso);

                    printf("Direcciones disponibles:\n");
                    if (jugador.actual->conexiones.arriba != -1) printf(" - Arriba\n");
                    if (jugador.actual->conexiones.abajo != -1) printf(" - Abajo\n");
                    if (jugador.actual->conexiones.izquierda != -1) printf(" - Izquierda\n");
                    if (jugador.actual->conexiones.derecha != -1) printf(" - Derecha\n");

                    printf("\n1. Recoger Item\n2. Descartar Item\n3. Avanzar\n4. Reiniciar\n5. Salir\nOpción: ");
                    int eleccion;
                    scanf("%d", &eleccion);
                    getchar();

                    switch (eleccion) {
                        case 1:
                            system("cls||clear");
                            jugador.tiempo--;
                            if (condicionTiempo(&jugador) == 0) {
                                jugar = 0;
                                break;
                            }
                            recoger_item(&jugador);
                            break;
                        case 2:
                            system("cls||clear");
                            jugador.tiempo--;
                            if (condicionTiempo(&jugador) == 0) {
                                jugar = 0;
                                break;
                            }
                            break;
                        case 3: {
                            system("cls||clear");
                            char direccion[10];
                            printf("Dirección a moverse: ");
                            scanf("%s", direccion);
                            int siguiente_id = -1;

                            if (strcmp(direccion, "arriba") == 0)
                                siguiente_id = jugador.actual->conexiones.arriba;
                            else if (strcmp(direccion, "abajo") == 0)
                                siguiente_id = jugador.actual->conexiones.abajo;
                            else if (strcmp(direccion, "izquierda") == 0)
                                siguiente_id = jugador.actual->conexiones.izquierda;
                            else if (strcmp(direccion, "derecha") == 0)
                                siguiente_id = jugador.actual->conexiones.derecha;
                            else
                                printf("Dirección inválida.\n");

                            if (siguiente_id != -1) {
                                //jugador.actual = 
                                int tiempo_gasto = (jugador.peso_total + 1 + 9) / 10;
                                jugador.tiempo -= tiempo_gasto;
                            } else {
                                printf("No puedes ir por esa dirección.\n");
                            }
                            break;
                        }
                        case 4:
                            system("cls||clear");
                            jugador.actual = escenariosALL[1];
                            jugador.tiempo = 10;
                            jugador.peso_total = 0;
                            jugador.puntaje = 0;
                            list_clean(jugador.inventario);
                            break;
                        case 5:
                            system("cls||clear");
                            jugar = 0;
                            break;
                    }

                    if (jugador.tiempo <= 0) {
                        printf("¡Se acabó el tiempo!\n");
                        jugar = 0;
                    } else if (jugador.actual && jugador.actual->es_final) {
                        printf("¡Has llegado al final! Puntaje final: %d\n", jugador.puntaje);
                        jugar = 0;
                    }
                }
                break;

            case 3:
                printf("Saliendo...\n");
                break;
            default:
                system("cls||clear");
                printf("Opción inválida. Intente nuevamente.\n");
                break;
        }
    } while (opcion != 3);

    return 0;
}

