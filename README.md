# 👨🏻‍💻PROYECTO 1 SISTEMA DE ARCHIVOS EXT2 Y EXT3 👨🏻‍💻
## Universidad San Carlos de Guatemala
### Facultad de Ingenieria

* Manejo e Implementación de Archivos.
* Escuela de vacaciones diciembre 2021. 
* Marvin Eduardo Catalán Véliz.
* 201905554
- Consiste en una aplicación de comandos en consola que simulan un sistema de archivos a excepción de los reportes en Graphviz. No distingue entre mayúsculas y minúsculas.
## Instalacion
- Descarga repositorio
- Ejecutar aplicacion desde consola con el comando ./main posicionandose en el mismo lugar donde se encuentra el archivo.(imagen de ejemplo).

![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/Ejecutar.png)
# ADMINISTRACION DE DISCOS
## Comandos
- MKDISK: creará un archivo binario que simulará un disco duro, estos archivos
binarios tendrán la extensión disk y su contenido al inicio será 0. Deberá ocupar
físicamente el tamaño indicado por los parámetros, (no importa que el sistema
operativo no muestre el tamaño exacto).
- RMDISK: elimina un archivo que representa a un disco duro mostrando un
mensaje de confirmación para eliminar.
- FDISK: Este comando administra las particiones en el archivo que representa al disco duro.
Deberá mostrar un error si no se pudo realizar la operación solicitada sobre la
partición, especificando por qué razón no pudo crearse (Por espacio, por
restricciones de particiones, etc.).
- MOUNT: Este comando montará una partición del disco en el sistema. Cada partición se
identificará por un id que tendrá la siguiente estructura: VD + LETRA + NUMERO.
Por ejemplo: vda1, vda2, vdb1, vdc1… La letra será la misma para particiones en el
mismo disco y el número diferente para particiones en el mismo disco.
- UNMOUNT: Desmonta una partición del sistema. Se utilizará el id que se le asignó a la partición
al momento de cargarla.
- MKFS: Este comando realiza un formateo completo de la partición, se formateará como
ext2 por defecto si en caso el comando fs no está definido. También creará un
archivo en la raíz llamado users.txt que tendrá los usuarios y contraseñas del sistema
de archivos.

![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/MKDISK.png)
![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/mkdisk2.png)

# ADMINISTRACION DE USUARIO Y GRUPOS
## Comandos
- LOGIN: Este comando se utiliza para iniciar sesión en el sistema. No se puede iniciar otra
sesión sin haber hecho un logout antes, si no, debe mostrar un mensaje de error
indicando que debe cerrar sesión antes.
- LOGOUT: Este comando se utiliza para cerrar sesión. Debe haber una sesión activa
anteriormente para poder utilizarlo, si no, debe mostrar un mensaje de error.
- MKGRP: Este comando creará un grupo para los usuarios de la partición y se guardará en el
archivo users.txt de la partición, este comando solo lo puede utilizar el usuario root.
Si otro usuario lo intenta ejecutar, deberá mostrar un mensaje de error, si el grupo
a ingresar ya existe deberá mostrar un mensaje de error.
- RMGRP: Este comando eliminará un grupo para los usuarios de la partición. Solo lo puede
utilizar el usuario root, si lo utiliza alguien más debe mostrar un error.
- MKUSR: Este comando crea un usuario en la partición. Solo lo puede ejecutar el usuario root,
si lo utiliza otro usuario deberá mostrar un error.
- RMUSR: Este comando elimina un usuario en la partición. Solo lo puede ejecutar el usuario
root, si lo utiliza otro usuario deberá mostrar un error.

![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/adminCarpetasArchivosPermisos.png)
## Usuario Root
- ***Root***: Este usuario es especial y no importando que permisos tenga el archivo o carpeta,
el siempre tendrá los permisos 777 sobre cualquier archivo o carpeta (Esto se
explica en detalle posteriormente). Podrá mover, copiar, eliminar, crear, etc. Todos
los archivos o carpetas que desee. No se le negará ninguna operación por
permisos, ya que él los tiene todos. Los permisos únicamente se pueden cambiar
con chmod que se explicará posteriormente. Se debe tomar en cuenta en que
categoría está el usuario, si es el propietario, si pertenece al mismo grupo en que
está el propietario o si es otro usuario que no pertenece al grupo del propietario.
En base a esta comprobación, el usuario puede estar en tres distintas categorías:
Propietario (U), Grupo (G) u Otro (O). Dependiendo de estas categorías se
determinan los permisos hacia el archivo o carpeta.

# ADMINISTRACION DE CAPRTAS ARCHIVOS Y PERMISOS
## Comandos
- CHMOD: Cambiará los permisos de uno o varios archivos o carpetas. Lo podrá utilizar el
usuario root en todos los archivos o carpetas y también lo podrán utilizar otros
usuarios, pero solo sobre sus propios archivos.
- MKFILE: Este comando permitirá crear un archivo, el propietario será el usuario que
actualmente ha iniciado sesión. Tendrá los permisos 664. El usuario deberá tener el
permiso de escritura en la carpeta padre, si no debe mostrar un error.
- MKDIR: Este comando es similar a mkfile, pero no crea archivos, sino carpetas. El propietario
será el usuario que actualmente ha iniciado sesión. Tendrá los permisos 664. . El
usuario deberá tener el permiso de escritura en la carpeta padre, si no debe mostrar
un error.

# EJECUCION DE SCRIPT 
- Son archivos con los comandos definidos en este documento. También puede
haber comentarios y líneas en blanco. Tendrán la extensión sh se utilizarán
para que los ejecute el comando exec.
- EXEC: El programa podrá ejecutar scripts con el comando exec. Debe mostrar el
contenido de la línea que está leyendo y su resultado. También debe mostrar los
comentarios del script.
![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/Exec.png) 
# COMANDOS EXTRAS DE FUNCIONES ESPECIALES
- PAUSE: Este comando será solo la palabra “pause” no tiene atributos al ingresar este
comando se pondrá en pausa solicitando que apache cualquier tecla para continuar.
Este comando NO detiene la ejecución de un archivo solo queda a la espera de
presionar una tecla para continuar su ejecución.
- EXIT: Este comando finaliza la ejecución del programa. 
![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/pause.png) 
![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/EXIT.png) 

# REPORTES
- Recibirá el nombre del reporte que se desea y lo generará con graphviz en una
carpeta existente.
   - MBR
   - DISK
   - INODE
   - JOURNALING
   - BLOCK
   - BM_INODE
   - BM_BLOCK
   - TREE
   - SB
   - FILE
   - LS
![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/reportes.png) 
![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/Reportes2.png) 
![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/Reportes3.png) 
![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/reporte4.png) 
![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/reportes5.png) 
![No se encuentra la imagen](https://github.com/MarvEdCV/Proyecto_1/blob/main/Imagenes/Reportes6.png) 
