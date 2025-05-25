# Tarea 3: GraphQuest

## Descripción

**GraphQuest** es un juego de exploración basado en un laberinto cargado desde un archivo CSV, en donde el jugador puede desplazarse entre distintos escenarios, recolectar y descartar ítems, y buscar llegar al final del laberinto antes de que se acabe el tiempo.

Cada movimiento consume tiempo en función del peso transportado. El objetivo es llegar al escenario final con la mayor puntuación posible, recolectando ítems y gestionando el inventario.


## Como ejecutar el programa
   * Requisitos previos:
      - Tener WinRar instalado, si no lo tienes, este es el link de la página para instalarlo: https://winrar.es/descargas
      - Sistema operativo: Windows. Debido a falta de pruebas, no hemos podido determinar si funciona en el sistema macOS

   * Para abrir el programa:
     - Tendrás que descargar el archivo .rar desde este link: https://www.mediafire.com/file/wmgw4fymlh3gnlu/programaTarea3.rar/file. Luego tendrás que extraer los archivos del .rar haciendo click derecho en el y seleccionar la opción: Extraer en "programaTarea3\". A continuación te dejará una carpeta con ambos archivos en su interior, unicamente hará falta ejecutar el archivo "tarea2.exe"
       
## Opciones que funcionan correctamente y problemas conocidos
  - **leer_escenarios**: Carga los posibles escenarios desde un archivo .csv
  - **recoger_item**: Revisa el escenario actual en busca de items, en caso de recogerlo lo elimina del escenario y lo añade al inventario del jugador.
  - **descartar_item**: Revisa el inventario del jugador y en caso de descartar un item, lo elimina completamente, quitando los atributos que le daba el item al jugador.
  - **reiniciarJuego**: Reinicia las estadisticas del jugador y lo devuelve al punto inicial, cargando de nuevo los escenarios.
  - **liberarTodo**: Libera el inventario del jugador, luego libera cada uno de los escenarios, liberando tambien memoria dinámica.

   No se encontraron problemas a la hora de jugar

   A mejorar
      * Podría en algunos casos ser útil que al descartar el item, en lugar de simplemente borrarlo, lo devuelva al escenario actual
      * Implementar una función para mover al jugador entre los distintos escenarios, para tener un orden óptimo en el main
      * A la hora de recoger o descartar un item, usar la funcion toupper() o tolower() para mejor manejo, esto en caso de que el usuario ingrese "cOPA DORADA" en lugar de "Copa dorada" o casos similares.

## Ejemplo de uso

1. Cargar escenarios: Opción seleccionada (1)
```
=====        GraphQuest        =====
1. Cargar Laberinto desde CSV
2. Iniciar Partida
3. Salir
Seleccione una opcion:
```
> Cargando laberinto...

2. Iniciar partida: Opción seleccionada (2)
```
--- ESCENARIO: Entrada principal ---
Una puerta rechinante abre paso a esta mansion olvidada por los dioses y los conserjes. El aire huele a humedad y a misterios sin resolver.
Tiempo restante: 10.0
Puntaje acumulado: 0
Peso: 0 kg
Items disponibles:
No hay items en este escenario.
Direcciones disponibles:
 - Abajo

1. Recoger Item
2. Descartar Item
3. Avanzar
4. Reiniciar
5. Salir
Opcion:
```
3. Movernos por los escenarios: Opción seleccionada (3, abajo), (3, derecha)
```
Direccion a moverse, en minusculas: abajo
Te has movido a: Cocina

--- ESCENARIO: Cocina ---
Restos de una batalla culinaria. Cuchillos por aqui, migajas por alla. Alguien cocinoÔÇª o fue cocinado.
Tiempo restante: 9.0
Puntaje acumulado: 0
Peso: 0 kg
Items disponibles:
 - Cuchillo (3 pts, 1 kg)
 - Pan (2 pts, 1 kg)
Direcciones disponibles:
 - Arriba
 - Abajo
 - Derecha
```
```
Direccion a moverse, en minusculas: derecha
Te has movido a: Comedor

--- ESCENARIO: Comedor ---
Una mesa elegante cubierta de polvo. Parece que nadie ha cenado aqui desde la ultima plaga. Al menos dejaron una copa.
Tiempo restante: 8.0
Puntaje acumulado: 0
Peso: 0 kg
Items disponibles:
 - Copa dorada (8 pts, 3 kg)
Direcciones disponibles:
 - Arriba
 - Abajo
 - Izquierda
 - Derecha
```
> Luego nos moveremos en este orden: derecha, abajo, izquierda, abajo, derecha

4. Recogemos un item: Opción seleccionada (1, "Mapa")
```
--- ESCENARIO: Camara secreta ---
Una sala oculta con simbolos raros y un mapa brillante. Nada que ver con una oficina de gobierno, esto si que es secreto.
Tiempo restante: 3.0
Puntaje acumulado: 0
Peso: 0 kg
Items disponibles:
 - Mapa (10 pts, 1 kg)
Direcciones disponibles:
 - Izquierda
 - Derecha
```
```
Item en el escenario actual: Camara secreta
Item: Mapa, Valor: 10, Peso: 1
Seleccione el item a recoger, ingrese el nombre como se indica:
```
```
Item recogido con exito.

--- ESCENARIO: Camara secreta ---
Una sala oculta con simbolos raros y un mapa brillante. Nada que ver con una oficina de gobierno, esto si que es secreto.
Tiempo restante: 2.0
Puntaje acumulado: 10
Peso: 1 kg
Items disponibles:
No hay items en este escenario.
Direcciones disponibles:
 - Izquierda
 - Derecha
```
5. Finalmente nos movemos una última vez a la derecha: Opción seleccionada (3, derecha)
```
Has llegado al final! Puntaje total: 10

=====        GraphQuest        =====
1. Cargar Laberinto desde CSV
2. Iniciar Partida
3. Salir
Seleccione una opcion:
```
