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
#include <sys/wait.h>

//Función donde tratamos la señal
void manejador(int senial){
	while(waitpid(WAIT_ANY,NULL,WNOHANG) > 0);
}

int main(int argc, char *argv[]){
	if(argc > 1){
		/*VARIABLES*/
		
		/*
		 * servSocket: socket del servidor
		 * conSocket:  socket de conexión
		 * direccion:  estructura para la dirección
		 * tamano:     variable donde se almacenará el tamaño de la estructura
		 * *mensaje:   variable la cual será enviada al cliente
		 * *ip:	       variable que permitirá conocer la ip conectada al servidor
		 * pid:        identificador del proceso hijo
		 */
		int servSocket,conSocket;
		struct sockaddr_in direccion;
		socklen_t tamano;
		char mensaje[200];
		char ip[INET_ADDRSTRLEN];
		int n,pid;

		//Se determina el tamaño de la estructura
		tamano = sizeof(struct sockaddr_in);

		//Configuración de la señal que será tratada (señal envíada por el hijo cuando termina)
		if(signal(SIGCHLD, manejador) > 0){
			fprintf(stderr,"Error en signal");
			exit(EXIT_FAILURE);
		}
		
		//Creación del socket
		servSocket = socket(AF_INET,SOCK_STREAM,0);

		//Comprobación de correcta creación
		if(servSocket < 0){
			perror("No se pudo crear el socket");
			exit(EXIT_FAILURE);
		}

		/*Inicialización de la estructura*/
		//Asignación de dirección al socket con comprobación
		direccion.sin_family = AF_INET;
		direccion.sin_port = htons(atoi(argv[1]));	//Asignación del puerto del servidor
		direccion.sin_addr.s_addr = htonl(INADDR_ANY);

		if(bind(servSocket,(struct sockaddr *) &direccion, tamano) < 0){
			perror("No se pudo asignar dirección");
			exit(EXIT_FAILURE);
		}


		//Se pone el socket a la escucha
		if(listen(servSocket,5) == -1){
			perror("Error en listen");
			exit(EXIT_FAILURE);
		}
		while(1){
			//Aceptamos la conexión
			if((conSocket = accept(servSocket,(struct sockaddr *) &direccion,&tamano)) < 0){
				perror("No se pudo aceptar la conexion");
				exit(EXIT_FAILURE);
			}

			//Impresión de la ip y puertos conectados
			if(inet_ntop(AF_INET, &direccion.sin_addr,ip,sizeof(ip)) == NULL){
				perror("Error en inet_ntop");
				exit(EXIT_FAILURE);
			}

			printf("Direccion IP conectada: %s\n",ip);
			printf("Puerto conectado: %d\n",htons(direccion.sin_port));

			//Creación del proceso hijo
			if((pid = fork()) == -1){					//Error en caso de que no se cree
				perror("Error en la creación del proceso hijo");
				exit(EXIT_FAILURE);
			}else if(pid == 0){						//Proceso hijo
				close(servSocket);					//El hijo cierra el socket de servidor
				printf("Número de proceso: %d\n",getpid());
				//Recibimos los mensajes del cliente
				while(recv(conSocket,mensaje,sizeof(mensaje),0) > 0){
					for(n = 0; n < strlen(mensaje); n++){
						mensaje[n] = toupper(mensaje[n]);	//Se transforma el mensaje del cliente
					}

					//Envío del mensaje al cliente
					if(send(conSocket,mensaje,sizeof(mensaje),0) == -1){
						perror("Error al enviar al cliente");
						exit(EXIT_FAILURE);
					}
				}

				close(conSocket);					//Cierre del socket de conexión por parte del hijo
				exit(EXIT_SUCCESS);
			}else{		//Proceso padre
				close(conSocket);
				//Este se mantendrá a la espera de nuevas conexiones.
			}
		}
		close(servSocket);							//Cierre del socket del servidor
	}else{
		printf("No se han introducido bien los datos\n");
		printf(" -puerto del servidor\n");
		exit(EXIT_FAILURE);
	}
}
