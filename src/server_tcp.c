#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <error.h>      
#include <string.h> 
#include <unistd.h>   
#include <pthread.h> 
#include <semaphore.h>
#include <sys/time.h>    


#define PORTNUMBER 50005
#define BUFSIZE 128*1024
char ADDRESS[];

float bytesRecebidos;
struct timeval now,now1,now2;
double start,end,dif;
void *connection_handler(void *);
FILE *arq;
char url[];
sem_t semaforo;
float byteEnviados=0,bytesTotais=0, envioAtual=0;
int count=0;

//~ void *connection_handler(void *socket_desc)
//~ {
	//~ //Get the socket descriptor
	//~ int sock = *(int*)socket_desc;
	//~ int read_size;
	//~ char client_message[BUFSIZE];
	
	//~ message = "Greetings! I am your connection handler\n";
	//~ write(sock , message , strlen(message));
	
	//~ //Receive a message from client
	//~ while( (read_size = recv(sock , client_message , BUFSIZE , 0)) > 0 )
	//~ {
		//~ memset(&client_message, '\0', sizeof(client_message));
	//~ }
	
	//~ if(read_size == 0)
	//~ {
		//~ puts("Cliente desconectado");
		//~ fflush(stdout);
	//~ }
	//~ else if(read_size == -1)
	//~ {
		//~ perror("recv failed");
	//~ }
		
	//~ //Free the socket pointer
	//~ free(socket_desc);
	
	//~ return 0;
//~ }
//--------------------- ------------------------------------- - ----------------------
//--------------------- ------------------------------------- - ----------------------
//--------------------- ------------------------------------- - ----------------------
//--------------------- ------------------------------------- - ----------------------

void *printar(void *socket_desc)
{
  while(1){
    sleep(1);
    sem_wait(&semaforo); //bloqueia semaforo escrever dados corretos
    
      arq=fopen(url,"a");
      if(arq==NULL){
        printf("Erro ao abrir arquivo \n");
        break;
      }
      else
      {
        //printf("Média %d: %.2f (bit/seg)\n",count, byteEnviados*8);
        //~ while((fgets(info,sizeof(info),arq))!=NULL)
          //~ printf("%s",info);
        //~ printf("Fim do arquivo.\n");
        time_t now;
        now=time(NULL);
        bytesTotais=bytesTotais+byteEnviados;
        count=count+1;
	         printf("Média %d: %.0f (bit/seg)\n",count, byteEnviados*8);
		fprintf(arq, "%d,%.0f,%d\n",count, byteEnviados*8,now);
		byteEnviados	= 0;
		
      }
      fclose(arq);
	
    sem_post(&semaforo); // libera semaforo
  }
  return 0;
}

int main(int argc, char *argv[]){
   int i = 0;
   int serversocket,portaUser;
   struct sockaddr_in server_addr;
   int socket_desc , client_sock , c , *new_sock, *teste;
   struct sockaddr_in server , client;
   sem_init(&semaforo, 0, 1); // inicializa semaforo com 1
   
   if(argc==3){
	strcat(ADDRESS,argv[1]);
	portaUser = atoi(argv[2]);}
	

   //Criacao do socket no servidor
   printf("Iniciando o servidor. \n");
   if ((serversocket = socket(AF_INET,SOCK_STREAM,0))<0){
      perror("Erro ao criar socket do servidor: \n");
   }   

   //Configuracao do endereco do servidor que sera ligado ao socket
   //zera toda a estrutura
   memset(&server_addr, 0, sizeof(server_addr));

   //Tipo de endereco
   server_addr.sin_family = AF_INET;
   //especificacao do endereco
   //server_addr.sin_addr.s_addr   = INADDR_ANY;

   //Querendo especificar manualmente um endereco pode-se usar
   server_addr.sin_addr.s_addr   = inet_addr(ADDRESS);
//	printf("Que porta deseja escutar?\n");
//	scanf("%d" , &portaUser);
//	fflush(stdin);
   //Numero da porta
   server_addr.sin_port   = htons(portaUser);
   
   sprintf(url,"../log/%d.csv",portaUser);
//   printf("%s",url);
   //
   //memset(&(server_addr.sin_zero), '\0', sizeof(server_addr.sin_zero));

   //Ligando o endereco do servidor ao socket
   if (bind(serversocket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))<0){
      perror("Erro de ligacao de socket no servidor: \n");
   } 

   //Indica que o socket deve "escutar"
   if (listen(serversocket, 3)<0){
      perror("Erro ao inicializar a escuta do socket no servidor: \n");
   }
	//Aceito e aguardando conexão
	puts("Aguardando conexões...");
	c = sizeof(struct sockaddr_in);
	
	//~ arq=fopen(url,"w");
	//~ struct timeval now1,now2;
	//~ int hours, minutes, seconds, day, month, year;

	//~ // time_t is arithmetic time type
	//~ time_t now, nowCSV;
	
	//~ // Obtain current time
	//~ // time() returns the current time of the system as a time_t value
	//~ nowCSV = time(NULL);
	//~ time(&now);
	
	//~ struct tm *local = localtime(&now);

	//~ hours = local->tm_hour;			// get hours since midnight	(0-23)
	//~ minutes = local->tm_min;		// get minutes passed after the hour (0-59)
	//~ seconds = local->tm_sec;		// get seconds passed after the minute (0-59)

	//~ day = local->tm_mday;			// get day of month (1 to 31)
	//~ month = local->tm_mon + 1;		// get month of year (0 to 11)
	//~ year = local->tm_year + 1900;	// get year since 1900
	
	//~ fprintf(arq,"Média de tráfego por segundo (em bit/seg)\nRealizado em: %02d/%02d/%d, %02d:%02d:%02d\n",day, month, year, hours,minutes,seconds);
	
	//~ fclose(arq);
	
	arq=fopen(url,"w");
	if(arq!=NULL){
	fprintf(arq,"NUM, BAND, TIME\n");
	fclose(arq);
	}
	
	
	pthread_t printar_thread;
	pthread_create( &printar_thread , NULL ,  printar , (void*) teste);
	
	
	while( (client_sock = accept(serversocket, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Conexão aceita");
		int read_size;
		char client_message[BUFSIZE];
		
		char message[] = "Olá! Voce efetuou a conexão com o server com sucesso!";
		write(client_sock , message , strlen(message));
		
		while( (read_size = recv(client_sock , client_message , BUFSIZE , 0)) > 0 )
		{
			memset(&client_message, '\0', sizeof(client_message));
			sem_wait(&semaforo);
			byteEnviados	= byteEnviados + read_size;
			sem_post(&semaforo); // libera semaforo
		}
		
		sem_wait(&semaforo); //bloqueia semaforo para um printar por vez
			double resultado, divisor;
			divisor =(double)count;
			printf("Total de bits enviados %.2f\n",bytesTotais*8);
			resultado =(double)((bytesTotais*8)/divisor);
			printf("Média Total do teste: %.2f (bit/seg)\n",resultado);
		sem_post(&semaforo); // libera semaforo
		
		if(read_size == 0)
		{
			puts("Cliente desconectado");
			fflush(stdout);
		}
		else if(read_size == -1)
		{
			perror("recv failed");
		}
		
		//~ pthread_t sniffer_thread;
		//~ new_sock = malloc(1);
		//~ *new_sock = client_sock;
		
		//~ if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		//~ {
			//~ perror("could not create thread");
			//~ return 1;
		//~ }
		
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		//puts("Handler assigned");
	}
	
	
	
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	
	return 0;
}
	
//--------------------- ------------------------------------- - ----------------------
//--------------------- ------------------------------------- - ----------------------
//--------------------- ------------------------------------- - ----------------------

