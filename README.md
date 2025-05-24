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

  - No se encontraron problemas a la hora de jugar

  - A mejorar
      * Podría en algunos casos ser útil que al descartar el item, en lugar de simplemente borrarlo, lo devuelva al escenario actual
      * Implementar una función para mover al jugador entre los distintos escenarios, para tener un orden óptimo en el main
      * A la hora de recoger o descartar un item, usar la funcion toupper() o tolower() para mejor manejo, esto en caso de que el usuario ingrese "cOPA DORADA" en lugar de "Copa dorada" o casos similares.

## Ejemplo de uso

1. Cargar escenarios: Opción seleccionada (1)
                                =====        GraphQuest        =====
                                1. Cargar Laberinto desde CSV
                                2. Iniciar Partida
                                3. Salir
                                Seleccione una opcion:                                  
