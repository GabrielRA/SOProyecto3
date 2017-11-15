
PROYECTO 2


GABRIEL RAMÍREZ RAMÍREZ (201020244)

BRAYAN FAJARDO ALVARADO (201157035)





Empacador de archivos TAR



Prof. Ing. Armando Arce Orozco


![](https://fotos.subefotos.com/513ff90b04afc1f7e14f93270de61a19o.png "TEC")


TEC

TECNOLÓGICO DE COSTA RICA

ESCUELA DE INGENIERÍA EN COMPUTACIÓN,

IC6600 PRINCIPIOS DE SISTEMAS OPERATIVOS,

GRUPO 02,

CARTAGO

II SEMESTRE 2017





# Tabla de Contenidos


1. [Introducción](#introducción)

2. [Descripción del problema](#descripción-del-problema)

3. [Definición de estructuras de Datos](#definición-de-estructuras-de-Datos)

4. [Descripción detallada y explicación de los componentes principales del programa](#descripción-detallada-y-explicación-de-los-componentes-principales-del-programa)

    4.1. [Mecanismo de acceso a archivos y directorios](#mecanismo-de-acceso-a-archivos-y-directorios)

    4.2. [Estructura de la tabla de asignación de espacio](#estructura-de-la-tabla-de-asignación-de-espacio)

    4.3. [Estrategia de administración de bloques libres](#estrategia-de-administración-de-bloques-libres)

    4.4. [Procedimiento de desfragmentación del archivo (opcional)](#procedimiento-de-desfragmentación-del-archivo-opcional)

5. [Análisis de resultados de pruebas](#análisis-de-resultados-de-pruebas)

6. [Conclusiones sobre rendimiento](#conclusiones-sobre-rendimiento)


# Introducción

Diariamente, cuando interactuamos con algún sistema operativos,directa o indirectamente estamos trabajando con archivos. La manipulación de archivos está presente en los sistemas operativos desde su creación y aunque todos tienen diversos formatos para administrarlos, internamente son muy similares. 

En este documento, se plantea una solución a un problema que se vuelve recurrente cuando se trabaja con múltiples archivos y es necesario ya sea ahorrar espacio en disco o trasladar dichos archivos de un lado a otro, comúnmente al enviar archivos por medio de correo electrónico, la creación de un archivo tar.

Un archivo tar, es una especie de contenedor que permite el almacenaje de uno o más archivos, distribuidos libremente o en carpetas, si función principal no es el comprimir el tamaño de los archivos, como en el caso de los archivos zip, aunque en ocasiones puede realizar dicho labor.

El software star, que se ha desarrollado para resolver el problema del traslado de archivos, cuenta con funciones limitadas a comparación del programa TAR integrado en los sistemas UNIX, dichas funciones son descritas más adelante en la descripción del problema

Además de esto, también se encuentra un análisis del comportamiento en memoria que tiene el programa, llamadas pruebas de correctitud. Unido a esto se brinda una breve explicación de las estructuras implementadas para la creación del archivo y para finalizar, la sección de conclusiones.



# Descripción del problema

Un problema que se presenta con el servicio FTP (file transfer protocol), utilizado tradicionalmente para la transferencia de archivos entre máquinas, es la tarea
a veces complicada de instalar un programa servidor en la máquina a la que se desean transferir/recuperar archivos (modelo cliente-servidor), ya que se
requieren privilegios de administrador.

Un enfoque más sencillo para implementar un servicio FTP sería simplemente que dos programas clientes se comuniquen entre sí y transfieran archivos entre
ellos sin necesidad de instalar un servidor (modelo peer-to-peer).

En este proyecto se pretende desarrollar un cliente FTP que funcione de ese modo, es decir, que permita transferir y recibir archivos entre programas clientes
que se encuentren instalados en máquinas remotas.

## Autenticación

El programa a desarrollar, que llamaremos bftp (basic ftp), realizará la autenticación de usuarios al inicio de la ejecución del programa:

~~~
bftp://<usuario>:<contraseña>@<dirección-ip>/<url-ruta>
~~~

Donde usuario es el nombre de usuario, dirección-ip es el IP de la máquina remota, contraseña es la contraseña de acceso, y url-ruta es el directorio donde
se iniciará la sesión.

Cada máquina en donde se ejecute el programa contendrá un archivo llamado .passwords que almacenará el nombre de usuario y su contraseña. De este modo
al iniciar una conexión se debe primero verificar las credenciales del usuario antes de permitirle ejecutar comandos.

Optativo: No es conveniente que los password queden expuestos en dicho archivo, por lo que se deben codificar utilizando una librería tipo “BASE64” (10
% adicional)

## Comandos ftp

El programa btfp reconocerá un subconjunto de instrucciones de FTP que permitirán la conexión y transferencia de archivos:

~~~
open <dirección-ip>: establece una conexión remota
close: cierra la conexión actual
quit: termina el programa
cd <directorio>: cambia de directorio remoto
get <archivo>: recupera un archivo remoto
lcd <directorio>: cambia de directorio local
ls: lista los archivos del directorio remoto
put <archivo>: envía un archivo a la máquina remota
pwd: muestra el directorio activo remoto
~~~

La transferencia de archivos realizará una copia del archivo en cuestión desde la máquina remota hasta la máquina local (o viceversa) almacenando éste con el
mismo nombre que se encontraba en la otra máquina.

## Comandos adicionales (optativo - 25 %)

El programa también podrá ejecutar un subconjunto adicional de instrucciones:

~~~
delete <archivo>: borra el archivo en la máquina remota
mdelete <patrón>: borra los archivos que cumplen el patrón
mget <patrón>: recupera los archivos que cumplen el patrón
mput <patrón>: envía los archivos que cumplen el patrón
rename <archivo> <nombre>: cambia el nombre del archivo
rmdir <directorio>: borra el directorio remoto
mkdir <directorio>: crea un directorio en la máquina remota
~~~


El patrón consiste en una hilera de texto junto con el carácter comodín *, por ejemplo:

~~~
tareas*.txt
documentos.*
*.txt
~~~

## Modo de operación

El programa bftp se debe ejecutar en al menos dos máquinas al mismo tiempo, y quedarán esperando conexiones en el puerto 8889. Posteriormente, permitirá
que el usuario ingrese comandos en la terminal y los ejecutará. Note que un usuario solo puede establecer una conexión con una máquina a un tiempo, pero
múltiples máquinas remotas pueden establecer conexión con la máquina de dicho usuario al mismo tiempo.

Internamente cada programa se deberá comportar con un servidor y un cliente al mismo tiempo. Por eso se debe utilizar un mecanismo (tareas) que le permita
al programa estar haciendo varias cosas al mismo tiempo: recibir comandos del usuario, atender todas las conexiones entrantes y ejecutar sus comandos.

El programa debe utilizar tanto sockets clientes como un socket servidor para realizar las conexiones adecuadamente.


# Definición de estructuras de Datos


## Uso de structs

Se define el uso de structs como la unidad básica de estructura del sistema. Se define una estructura para los componentes básicos del sistema, como lo es las colas de archivos que se van a comprimir.

Dicha struct de nombre de tar, presenta datos básicos sobre los archivos siguiendo el formato POSIX-1998, dicho formato está representado como una estructura internada dentro del struct tar principal. Además de contar con un indicador de donde comienza el archivo dentro del tar, un char que representa la información o encabezado del archivo que sirve de insumo para agregar los datos al formato posix. Para finalizar, un puntero a la información del siguiente archivo
	
## Almacenamiento de datos

Los datos de contenidos que se leen de los archivos que se comprimen son almacenados en el sistema usando structs y listas simples. Al inicio del archivo, se almacena información que hace referencia a los archivos o bloques vacíos de memoria. Luego de esto, comienza la información de cada uno de los archivos, cuando estos finalizan, se le agrega un bloque extra de información representando por “0”, esto para cumplir con estándares de escritura de los archivos tar.


# Descripción detallada y explicación de los componentes principales del programa:

## Mecanismo de acceso a archivos y directorios

Los archivos son leídos a través de una estructura que lee el contenido de los archivos.

Se implementa el método `open()` , el cual logra leer los archivos de prueba que serán comprimidos en el sistema.

También está el método `openFile()`, que logra leer la estructura del tar, y que retorna la cantidad de elementos que posee el archivo.


## Estructura de la tabla de asignación de espacio

La tabla de asignación del espacio es manejada en un struct o estructura que almacena los espacios ocupados y los espacios libres del tar. Estos espacios se muestran en una tabla que se imprime en consola.

## Estrategia de administración de bloques libres

Los bloques libres son administrados a través de un método que une o comprime los espacios para que sean asignados posteriormente a un nuevo archivo que se agregue al tar.

Los espacios están indicados de manera que se pueda acceder a ellos cuando se requiera hacer uso de ellos.

## Procedimiento de desfragmentación del archivo (opcional)

Se implementa un procedimiento de compresión, mencionado en el punto anterior, con el que se unen todos los espacios libres, a través del método `joinSpace()`, con el que se contempla el archivo tar y los archivos incluidos dentro del tar.

# Pruebas de correctitud

Para determinar el estado de un archivo, se realizará la implementación del comando `-d (-dump)` que mostrará el contenido del archivo y los espacios libres que posee este.
A partir de esta definición, se establecen las siguientes pruebas que verifican el correcto estado del archivo:


| Prueba        | Resultado     | Comentarios   |
| ------------- | ------------- | ------------- |
| Muestra de contenido de archivos agregados al tar  | Implementación incompleto  | Se implementa el método, pero no se logra hacer la llamada directamente desde el main. |
| Muestra de espacios vacíos en el tar  | Implementación incompleto  | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra la información de los datos editados | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de crear el tar con archivos| Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de crear el tar con archivos| Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main. | 
| Se muestra el dump en pantalla después de extraer del tar uno o varios archivos | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main. |
| Se muestra el dump en pantalla después de listar los archivos del tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de borrar archivos del tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
|Se muestra el dump en pantalla después de actualizar el tar con archivos | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de agregar archivos al tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de crear el tar con directorios | Implementación incompleto |Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de extraer directorios del tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.
| Se muestra el dump en pantalla después de listar los directorios del tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.
| Se muestra el dump en pantalla después de borrar directorios del tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se muestra el dump en pantalla después de actualizar los directorios del tar | Implementación incompleto| Se implementa el método, pero no se logra hacer la llamada directamente desde el main. | 
| Se muestra el dump en pantalla después de agregar directorios del tar | Implementación incompleto | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|


# Análisis de resultados de pruebas

En este punto se describirá y analizará cada una de las pruebas realizadas a cada unas de las funciones que debe realizar el sistema.
Las pruebas a realizar se describirán a como sigue en una tabla, mencionando el nombre de la prueba, los resultados obtenidos y comentarios acerca del resultado de la prueba, entre otros puntos por comentar.


| Prueba| Resultado | Comentarios | 
| ------------- | ------------- | ------------- |
| Se crea el tar con archivos | Implementación completa | Sin comentarios | 
| Se extrae del tar uno o varios archivos del tar | Implementación completa |Sin comentarios | 
| Se enlista del tar los archivos agregados | Implementación completa | Sin comentarios |
| Se borra uno o más archivos comprimidos dentro del tar | Implementación completa | Sin comentarios|
| Se actualizan archivos en el tar | Implementación completa | Sin comentarios | 
| Se agregan archivos al tar | Implementación completa |Sin comentarios |
| Se crea el tar con directorios | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.| 
| Se extraen del tar uno varios directorios | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se enlistan del tar uno o varios directorios | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se borra uno o más directorios comprimidos del tar | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se actualizan los directorios en el tar | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.| 
| Se agregan los directorios al tar | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|
| Se desfragmentan los archivos | Implementación incompleta | Se implementa el método, pero no se logra hacer la llamada directamente desde el main.|


# Conclusiones sobre rendimiento

De la implementación del comando star, se puede extraer las siguientes conclusiones:

1. La creación de un archivo tar implementado en C,permite una gran cantidad de opciones con respecto al manejo de 
memoria que quizás no estén disponibles o sean más complejas en lenguajes de más alto nivel.

2. Durante la investigación, se descubrió que el formato tar en variante y que existen más de una estructura válida que debe de seguir el encabezado de los archivos para poder ser mostrados correctamente por el sistema operativo.

3. A pesar de que el programa no cumple con todas las especificaciones pedidas, se considera que tiene un buen manejo de memoria con respecto a la estructura interna del tar.

# Referencias Bibliográficas