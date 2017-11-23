• Programa bftp

– Operaciones de transferencia: put, get  (Implementados como send_file and get_file)
– Operaciones sobre directorios: cd, pwd, lcd, ls (ls --> scanFilesFromDirectory())

– Procedimiento de autenticación
– Manejo de múltiples conexiones simultáneas



• Documentación (20%)
– Descripción de estructuras, componentes, etc.
– Análisis de resultados y conclusiones


COMANDOS EXTRAS


delete <archivo>: borra el archivo en la máquina remota
mdelete <patrón>: borra los archivos que cumplen el patrón
mget <patrón>: recupera los archivos que cumplen el patrón
mput <patrón>: envía los archivos que cumplen el patrón (enlistar archivos, con mètodo de )
rename <archivo> <nombre>: cambia el nombre del archivo
rmdir <directorio>: borra el directorio remoto
mkdir <directorio>: crea un directorio en la máquina remota