#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
typedef struct {
    List* inventario;
    float tiempo;
    int puntaje;
    int peso_total;
    Escenario* actual;
} Jugador;

Escenario escenarios[17];
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

  printf("Seleccione el item a recoger, ingrese el nombre como se indica: ");
  char nombre[50];
  fgets(nombre, sizeof(nombre), stdin);
  nombre[strcspn(nombre, "\n")] = 0; 
  

  for (Item *item = list_first(items); item != NULL; item = list_next(items)) {
    if (strcmp(item->nombre, nombre) == 0) {
      if (jugador->peso_total + item->peso <= 10) {
        jugador->peso_total += item->peso;
        jugador->puntaje += item->valor;
        list_pushBack(jugador->inventario, item);
        list_popCurrent(items);

        printf("Item recogido con exito.\n");
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
  if (list_size(jugador->inventario) == 0) {
    printf("No hay items en el inventario.\n");
    return;
  }

  printf("Items en el inventario\n");
  for (Item *item = list_first(jugador->inventario); item != NULL; 
        item = list_next(jugador->inventario)) {
    printf("Item: %s, Valor: %d, Peso: %d\n", item->nombre, item->valor, item->peso);
  }

  printf("Seleccione el item a descartar, ingrese el nombre como se indica: ");
  char nombre[50]; 
  fgets(nombre, sizeof(nombre), stdin);
  nombre[strcspn(nombre, "\n")] = 0; 

  for (Item *item = list_first(jugador->inventario); item != NULL; item = list_next(jugador->inventario)) {
    if (strcmp(item->nombre, nombre) == 0) {
      jugador->peso_total -= item->peso;
      jugador->puntaje -= item->valor;
      list_popCurrent(jugador->inventario);
      free(item);

      printf("Item descartado con exito.\n");
      return;
    }
  }
  printf("Item no encontrado en el inventario.\n");
}

void reiniciarJuego(Jugador* jugador) {
    jugador->actual = escenariosALL[1];  
    jugador->tiempo = 10;
    jugador->puntaje = 0;
    jugador->peso_total = 0;
    
    if (jugador->inventario == NULL)
        jugador->inventario = list_create();
    else
        list_clean(jugador->inventario);
    
    leer_escenarios();  
}

void liberarTodo(Jugador* jugador) {
  // Liberar inventario del jugador
  if (jugador->inventario != NULL) {
    for (Item* item = list_first(jugador->inventario); item != NULL; item = list_next(jugador->inventario)) {
      free(item);
    }
    list_clean(jugador->inventario);
    free(jugador->inventario);
    jugador->inventario = NULL;
  }

    // Liberar cada escenario
  for (int i = 1; i <= 16; i++) {
    Escenario* esc = escenariosALL[i];
    if (esc == NULL) continue;

    // Liberar items del escenario
    if (esc->items) {
      for (Item* item = list_first(esc->items); item != NULL; item = list_next(esc->items)) {
        free(item);
      }
      list_clean(esc->items);
      free(esc->items);
    }

    free(esc->nombre);
    free(esc->descripcion);
    free(esc);
    escenariosALL[i] = NULL;
    }
}

int main() {
  Jugador jugador;
  jugador.actual = NULL;  
  jugador.inventario = NULL;

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

                if (jugador.inventario == NULL) {
                  jugador.inventario = list_create();
                }
                else {
                  list_clean(jugador.inventario);
                }

                jugador.actual = escenariosALL[1];
                jugador.tiempo = 10;
                jugador.puntaje = 0;
                jugador.peso_total = 0;

                if (!jugador.actual) {
                    printf("No se encontro la entrada del laberinto.\n");
                    break;
                }
                // Lógica del juego
                int jugar = 1;
                while (jugar && jugador.tiempo > 0 && jugador.actual != NULL) {
                    printf("\n--- ESCENARIO: %s ---\n", jugador.actual->nombre);
                    printf("%s\n", jugador.actual->descripcion);
                    printf("Tiempo restante: %.1f\n", jugador.tiempo);
                    printf("Puntaje acumulado: %d\n", jugador.puntaje);
                    printf("Peso: %d kg\n", jugador.peso_total);
                    printf("Items disponibles:\n");

                    if (list_size(jugador.actual->items) == 0) {
                        printf("No hay items en este escenario.\n");
                    }
                    else {
                        for (Item* item = list_first(jugador.actual->items); item != NULL; item = list_next(jugador.actual->items))
                        printf(" - %s (%d pts, %d kg)\n", item->nombre, item->valor, item->peso);
                    }

                    printf("Direcciones disponibles:\n");
                    if (jugador.actual->conexiones.arriba != -1) printf(" - Arriba\n");
                    if (jugador.actual->conexiones.abajo != -1) printf(" - Abajo\n");
                    if (jugador.actual->conexiones.izquierda != -1) printf(" - Izquierda\n");
                    if (jugador.actual->conexiones.derecha != -1) printf(" - Derecha\n");

                    printf("\n1. Recoger Item\n2. Descartar Item\n3. Avanzar\n4. Reiniciar\n5. Salir\nOpcion: ");
                    int eleccion;
                    scanf("%d", &eleccion);
                    getchar();

                    switch (eleccion) {
                        case 1:
                            system("cls||clear");
                            jugador.tiempo--;
                            if (jugador.tiempo <= 0) {
                                jugar = 0;
                                break;
                            }
                            recoger_item(&jugador);
                            break;
                        case 2:
                            system("cls||clear");
                            jugador.tiempo--;
                            if (jugador.tiempo <= 0) {
                                jugar = 0;
                                break;
                            }
                            descartar_item(&jugador);
                            break;
                        case 3: {
                            system("cls||clear");
                            char direccion[10];
                            printf("Direccion a moverse, en minusculas: ");
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
                                printf("Direccion invalida.\n");
                            if (siguiente_id != -1) {
                                jugador.actual = escenariosALL[siguiente_id];
                                printf("Te has movido a: %s\n", jugador.actual->nombre); 
                                float tiempo_gasto = ceil((jugador.peso_total + 1) / 10.0);
                                jugador.tiempo -= tiempo_gasto;
                            } 
                            else {
                                printf("No puedes ir por esa direccion.\n");
                            }
                            break;
                        }
                        case 4:
                            system("cls||clear");
                            reiniciarJuego(&jugador);
                            printf("Juego reiniciado correctamente.\n");
                            break;
                        case 5:
                            system("cls||clear");
                            printf("Saliendo al menu principal...\n");
                            jugar = 0;
                            break;
                    }
                    if (jugador.tiempo <= 0) {
                        system("cls||clear");
                        printf("\nSe acabo el tiempo! Buen intento!\n\n");
                        jugar = 0;
                    } else if (jugador.actual && jugador.actual->es_final) {
                        system("cls||clear");
                        printf("\nHas llegado al final! Puntaje total: %d\n\n", jugador.puntaje);
                        jugar = 0;
                    }
                }
                break;
            case 3:
                system("cls||clear");
                liberarTodo(&jugador);
                printf("Gracias por jugar.\n");

                break;
            default:
                system("cls||clear");
                printf("Opcion invalida. Intente nuevamente.\n");
                break;
        }
    } while (opcion != 3);

    return 0;
}

