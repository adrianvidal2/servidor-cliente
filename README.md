# Programa Cliente-Servidor Concurrente

Implementación de un **programa cliente-servidor** con procesos usando TCP y **escrito en C**. Se dispondrá de un programa servidor concurrente que aceptará peticiones de múltiples clientes.
El servidor se quedará a la espera de nuevas conexiones por lo que es necesario la detención de este para finalizar.

El cliente enviará línea a línea de un archivo al servidor. Este último transformará cada línea a mayúsculas y se las enviará de nuevo cliente, el cual las escribirá
en un nuevo archivo.

## Ficheros disponibles
- **cliente**: Código asociado a la implementación del cliente.
- **servidor**: Código asociado a la implementación del servidor.

## Compilación y ejecución
La compilación de ambos archivos se realiza:

```
cd <carpeta-implementacion>
gcc -Wall [implementacion.c] -o [ejecutable]
```
Para la ejecución del servidor será necesario indicar el puerto utilizado por el cual se realizarán las conexiones:
```
cd <carpeta-servidor>
./servidor [puerto-servidor]
```

El cliente necesita conocer la ip del servidor, el puerto al cual conectarse y finalmente el archivo a enviar: 
```
cd <carpeta-cliente>
./cliente [ip-servidor] [puerto-servidor] [archivo]
```
## Autor:
[Adrián Vidal Lorenzo](https://github.com/adrianvidal2)
