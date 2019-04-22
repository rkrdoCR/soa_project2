Proyecto Programado 1:  Lotería de Threads
==========================================
MC6004 Sistemas Operativos Avanzados 

Instituto Tecnológico de Costa Rica

Fecha: Lunes 22 de Abril del 2019

Profesor Francisco J. Torres-Rojas

Integrantes del grupo:  

- Marco Acuña
- Ricardo Alfaro  
- Christopher Jiménez
- Randall Jiménez
- Renan Morera

I. Descripción del Proyecto:
============================
El propósito de este proyecto es experimentar con el uso de memoria compartida entre procesos heavyweight. Toda la programación debe realizarse en C sobre Linux.

II. Preliminares:
==============
En caso de no tener experiencia en este tema, es necesario
investigar el uso de mmap() en Linux. Se debe investigar
la sincronización de procesos heavyweight con semáforos.
También, se debe saber como procesar argumentos de la lı́nea
de comandos de un programa ejecutado desde consola.

III. Relación Productor - Consumidor:
================
El concepto de productor-consumidor es fundamental en
Sistemas Operativos. Numerosos problemas pueden ser mode-
lados y resueltos con variantes de esta relación.
Para este proyecto, los elementos producidos y consumidos
serán mensajes a ser depositados en un buffer o buzón circular
de tamaño finito. Dicho buffer es creado e inicializado por
un programa independiente a los productores y consumidores,
y será finalmente liberado por otro programa que cancela, de
manera elegante, a todo el sistema de procesos creados en este
proyecto.
Ası́, tenemos 4 tipos de procesos: creador, productores,
consumidores y finalizador.

IV. Creador:
============
Este programa será responsable de crear el buffer compartido por todos los procesos y de inicializar todas las variables
de control asociadas al mismo (semáforos, banderas, contador
de productores, contador de consumidores, etc.). El nombre
del buffer, el tamaño en entradas para mensajes, y cualquier
otro parámetro que cada grupo considere conveniente serán
recibidos de la lı́nea de comando al ser ejecutado desde
consola.

V. Productores:
==============
Esta es una familia de procesos, todos usando exactamente
el mismo código, que se vinculan al buffer reservado por el
creador. Con tiempos de espera aleatorios generarán mensajes
que colocarán en el buffer (administrado circularmente).
Cada productor recibe como argumentos de la lı́nea de
comandos el nombre del buffer compartido y un parámetro que indique la media en segundos de los tiempos aleatorios,
siguiendo una distribución exponencial, que deben esperar
antes de agregar un nuevo mensaje en el buffer. Por supuesto,
este acceso debe darse de manera sincronizada ya que el
buffer es compartido por múltiples procesos. El usuario puede
ejecutar cuantas veces lo desee este programa, creando un
nuevo productor que compite por el buffer, aunque cada vez
podrı́a indicarse una media de espera diferente. Al crearse un
productor, este incrementa el contador de productores vivos.

Estos procesos repiten un ciclo de espera aleatoria y
fabricación de mensajes hasta que algún tipo de bandera
global en memoria compartida indique que el sistema se debe
suspender. En este caso, los productores terminan, decrementan
el contador de productores vivos, y despliegan su identificación
y algunas estadı́sticas básicas (número de mensajes producidos,
acumulado de tiempos esperados, acumulado de tiempo que
estuvo bloqueado por semáforos, etc.).

El formato especı́fico del mensaje puede ser definido por
cada grupo de trabajo, pero debe incluir al menos:
- Identificación del productor.
- Fecha y hora de creación.
- Llave aleatoria entre 0 y 4.

Cada vez que un mensaje logra ser puesto en el buffer, se
debe desplegar un mensaje a la consola describiendo la acción
realizada incluyendo el ı́ndice de la entrada donde se dejó el
mensaje y la cantidad de productores y consumidores vivos al
instante de este evento.

VI. Consumidores
=====================
Esta es una familia de procesos, todos usando exactamente
el mismo código, que se vinculan al buffer reservado por el
creador y que con tiempos de espera aleatorios consumen
mensajes tomados del buffer.

Cada consumidor recibe como argumentos de la lı́nea de
comandos el nombre del buffer compartido y un parámetro
que indique la media en segundos de los tiempos aleatorios,
siguiendo una distribución exponencial, que deben esperar
antes de consumir el siguiente mensaje del buffer administrado
circularmente. Por supuesto, este acceso debe darse de manera
sincronizada ya que el buffer es compartido por múltiples
procesos.

El usuario puede ejecutar cuantas veces lo desee este
programa, creando un nuevo consumidor que compite por el
buffer, aunque cada vez podrı́a indicarse una media de espera
diferente. Cuando se crea un consumidor, lo primero que éste
hace es incrementar el contador de consumidores activos.

Estos procesos repiten un ciclo de espera aleatoria y consu-
mo de mensajes hasta que lean un mensaje especial que indique
que el sistema se deba suspender, o cuando al leer un mensaje
este incluya una llave (número entero entre 0 y 4) que sea igual
al process id o PID del consumidor módulo 5. En cualquiera
de estos dos casos, el consumidor termina, decrementa el
contador de consumidores activos, despliega su identificación y
algunas estadı́sticas básicas (número de mensajes consumidos,
acumulado de tiempos esperados, acumulado de tiempo que
estuvo bloqueado por semáforos, etc.).

Cada vez que un mensaje logra ser leı́do del buffer, se
debe desplegar un mensaje a la consola describiendo la acción
realizada incluyendo el ı́ndice de la entrada de adonde se tomó
el mensaje y la cantidad de productores y consumidores vivos
al instante de este evento).

VII. Finalizador:
=================
Este programa se encarga de cancelar todo el sistema de
procesos, enviando mensajes de finalización a cada consumidor
vivo usando el buzón diseñado para este proyecto, e indicándo-
le a los productores que cesen actividades con alguna bandera
global en memoria compartida. Una vez que la cantidad de
productores y consumidores llega a cero, el buffer compartido
es liberado. El finalizador deberá dar mensajes y todas las
estadı́sticas posibles de su gestión.

VIII. Generalidades:
=================
Los programas son corridos desde consola. El uso correcto
de la lı́nea de argumentos será muy tomado en cuenta para
la evaluación de este proyecto. Todos los componentes de
este proyecto deben ser muy robustos y considerar cualquier
combinación de eventos y cualquier valor en los parámetros.
Si la ejecución no es posible, cada programa dará un mensaje
apropiado y terminará de manera controlada

IX. Referencias:
=================
- https://www.sao.ru/hq/sts/linux/doc/ipc_guide/semaphores.html
- https://pubs.opengroup.org/onlinepubs/007904875/functions/semop.html
- http://man7.org/linux/man-pages/man1/ipcs.1.html


Documentación:
==============
El repositorio contiene cuatro ejecutables separados: creador, productores, consumidores y finalizador. Los cuales comparten espacio en memoria para la intercomunicación de mensajes. De forma individual cada productor y consumidor van a poder acceder al buffer compartido para la escritura y lectura de mensajes. Se usan semaforos para el control del _overflow_ y _underflow_.

La siguiente es la metodología seguida por el grupo de trabajo para la solución de éste proyecto, el programa está estructurado de la siguiente forma:
 1. Se tiene un directorio principal llamado "soa_project2" el cual a su vez contiene varios directorios y algunos archivos de codigo importante tales como Makefile, Readme.md, y los archivos _main_ de los cuatro procesos.
```
|--- Makefile
|--- set_environment_variables
|---consumer
|    |--consumer_main.c
|---creator
|    |--creator_main.c
|---data_estructures
|    |--circular_buffer.c
|    |--circular_buffer.h 
|    |--message.c
|    |--message.h
|    |--semaphore_union.c
|    |--semaphore_union.h
|    |--shared_memory.c
|    |--shared_memory.h
|---finalizer
|    |--finalizer_main.c
|---producer
|    |--producer_main.c
|---tmux
|    |--.tmux.conf
|    |--tmux_multi.sh
|    |--tmux_script_poc
|---utilities
|    |--check_mod.c
|    |--check_mod.h
|    |--exp_dist.c
|    |--exp_dist.h
|    |--get_key.c
|    |--get_key.h
|    |--test_exp_dist.c
|    |--time_util.c
|    |--time_util.h
```
2. La ejecución de lo programas deben seguir el siguiente orden:
	1. El primer programa a ejecutar es el _creator_ con la siguiente sintasis:

			./creator_main bufferSize(int)

		Donde _bufferSize_ es un número entero mayor a 0 para asignar el tamaño de la memoria compartida. La salida de este programa va a ser el el ID de la memoria compartida que los demás programas van a usar.
	2. A partir de este momento se puede usar la memoria compartida, lo ideal sería empezar con los productores para que vayan escribiendo mensajes en ese espacio.

			./producer_main sharedMemoryId(int) productionTime(int)

		Este programa necesita dos parámetros para su ejecución el primero es el id de la memoria compartida (se obtuvo con el creator_main) y el segundo es la media en segundos de los tiempos aleatorios, este valor va a seguir una distribución exponencial que debe esperar antes de agregar un nuevo mensaje en el buffer.
	3. Una vez los productores estén generando mensajes en el buffer compartido los consumidores van a poder leerlos. Es importante recalcar que si la ejecución se hace de forma inversa (primero los consumidores) las aplicaciones van a funcionar de forma correcta.

			./consumer_main sharedMemoryId(int) consumptionTime(int)

		Este programa también necesita dos parámetros el primero es el id de la memoria compartida (se obtuvo con el creator_main) y el segundo es la media en segundos de los tiempos aleatorios, este valor va a seguir una distribución exponencial que debe esperar antes de consumir un nuevo mensaje en el buffer.
	
	4. En el momento que se quiera detener la generación y lectura de mensajes por parte de los productores y consumidores respectivamente se necesita ejecutar al finalizador

				./finalizer_main sharedMemoryId(int)
		
		Con únicamente el valor del ID de la memoria compartida el finalizador va a ser capaz de detener todos los procesos, liberar los recursos y generar un reporte.

 

