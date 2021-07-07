#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]){
	if(argc == 4){
		/*VARIABLES*/
		int clientSocket; 		//Socket del cliente
		struct sockaddr_in direccion;	//Estructura para la dirección
		socklen_t tamano;		//tamaño de la estructura
		char lineas[200];		//cadena donde se irá guardando las lineas leidas
		char recibido[200];		//cadena donde se recibirá el mensaje
		int i,nBytes;
		
		//Creación del socket
		clientSocket = socket(AF_INET,SOCK_STREAM,0);

		//Comprobación de correcta creación del socket
		if(clientSocket < 0){
			perror("Error en la creación del socket");
			exit(EXIT_FAILURE);
		}

		/*Inicialización de la estructura*/
		direccion.sin_family = AF_INET;
		//Conversión de IPv4 introducida por parámetro a binario
		if(inet_pton(AF_INET,argv[1],&direccion.sin_addr) ==-1){
			perror("Error en inet_pton");
			exit(EXIT_FAILURE);
		}		
		//Se convierte el puerto a orden de red
		direccion.sin_port = htons(atoi(argv[2]));

		//Solicitud de conexión
		tamano = sizeof(struct sockaddr_in);
		if(connect(clientSocket,(struct sockaddr *) &direccion,tamano) == -1){
			perror("No se pudo conectar");
		}

		/*Operaciones sobre archivo*/
		
		//Apertura del archivo
		FILE *archivo;					//Archivo recibido por parámetro
		FILE *salida;					//Archivo de salida
		archivo = fopen(argv[3],"r");
		char nombreNuevo[40];
		for(i = 0; i < strlen(argv[3]); i++){		//Modificación del nombre del archivo a todo mayúsculas
			nombreNuevo[i] = toupper(argv[3][i]);
		}
		nombreNuevo[i++] = '\0';
		printf("Nombre nuevo para el archivo: %s\n",nombreNuevo);
		salida = fopen(nombreNuevo,"a");
		
		if(archivo == NULL){				//Se comprueba existencia del archivo
			perror("No se encuentra el archivo");
			exit(EXIT_FAILURE);
		}
		
		//Leemos el archivo
		while(fgets(lineas,sizeof(lineas),archivo) != NULL){
			sleep(10);						//Entre cada envío se espera 10 segundos
			if(send(clientSocket,lineas,sizeof(lineas),0) == -1){	//Envío al servidor con comprobación de error
				perror("Error en el envío");
			}

			//Recibimos el mensaje con la correspondiente comprobación de error
			if((nBytes = recv(clientSocket,recibido,sizeof(recibido),0)) == -1){
				perror("Error al recibir del servidor");
				exit(EXIT_FAILURE);
			}
			printf("Línea recibida: %s\n",recibido);

			//Escritura del mensaje recivido en el archivo
			fputs(recibido,salida);
		}


		//Cerramos los archivos
		fclose(salida);
		fclose(archivo);

		//Cerramos el socket
		close(clientSocket);
	}else{							//Aviso en caso de parámetros mal introducidos.
		printf("Datos mal introducidos\n");
		printf("-ipServidor\t-puertoServidor\t-archivo\n");
		exit(EXIT_FAILURE);
	}

}
