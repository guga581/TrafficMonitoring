#include <stdio.h> 
#include <netinet/tcp.h>
#include <sys/socket.h>    
#include <arpa/inet.h>         
#include <string.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <error.h>      
#include <pthread.h> 
#include <semaphore.h>
#include <sys/time.h>   
   

#define MAXLINE 200
#define BUFSIZE 128*1024
static const char ADDRESS[] = "10.1.1.80";

//~ FILE *arq;
//~ char url[]="LOG_AverageTraffic.txt";
//~ sem_t semaforo;
//~ float byteEnviados=0,bytesTotais=0, envioAtual=0;
float envioAtual=0;
//~ int count=0;

//~ void *printar(void *socket_desc)
//~ {
  //~ while(1){
    //~ sleep(1);
    //~ sem_wait(&semaforo); //bloqueia semaforo escrever dados corretos
    //~ if(byteEnviados>0){
      //~ arq=fopen(url,"a");
      //~ if(arq==NULL){
        //~ printf("Erro ao abrir arquivo \n");
        //~ break;
      //~ }
      //~ else
      //~ {
        //~ //printf("Média %d: %.2f (bit/seg)\n",count, byteEnviados*8);
        //~ while((fgets(info,sizeof(info),arq))!=NULL)
          //~ printf("%s",info);
        //~ printf("Fim do arquivo.\n");
        
        //~ bytesTotais=bytesTotais+byteEnviados;
        //~ count=count+1;
	         //~ printf("Média %d: %.2f (bit/seg)\n",count, byteEnviados*8);
		//~ fprintf(arq, "Média %d: %.2f (bit/seg)\n",count, byteEnviados*8);
		//~ byteEnviados	= 0;
		
      //~ }
      //~ fclose(arq);
	//~ }
    //~ sem_post(&semaforo); // libera semaforo
  //~ }
  //~ return 0;
//~ }

int main(){
   
   int clientSock;
   int conn,num;
   unsigned int tams;
   char buffer[MAXLINE];
   struct sockaddr_in s;
   int portaServidor = 50005;
   char buff[BUFSIZE];
   //~ sem_init(&semaforo, 0, 1); // inicializa semaforo com 1
	int y;
	for (y=0;y<BUFSIZE;y++)
		buff[y] = 'a';

   //Criando socket cliente
   clientSock = socket(AF_INET, SOCK_STREAM, 0);
   if(clientSock < 0){ 
      perror("Erro na criacao do socket client: ");
   }
   
   memset(&s, 0, sizeof(s));
   
   //Especificacao do endereco do servidor para efetuar conexao
   s.sin_family = AF_INET;
   s.sin_addr.s_addr = inet_addr(ADDRESS);
   int portaUser,segundosUser;
   printf("Em qual porta deseja se comunicar?\n");
   scanf("%d",&portaUser);
   
   s.sin_port = htons(portaUser);        
   fflush(stdin);
   
  
   
   //Efetua conexao com o sevidor
   conn = connect(clientSock, (struct sockaddr *) &s, sizeof(s));
   if(conn < 0){
      perror("Erro ao tentar se conectar com o servidor: ");
      return -1;
   }
	char buffserver[55];
	bzero(buffserver, 55);
	read(clientSock, buffserver, sizeof(buffserver));
	printf("Mensagem do servidor: %s\n",buffserver);	
	
    printf("Por quantos segundos deseja enviar dados?\n");
	scanf("%d",&segundosUser);
	int *teste;
	
	//~ arq=fopen(url,"w");
	//~ struct timeval now1,now2;
	//~ int hours, minutes, seconds, day, month, year;

	//~ // time_t is arithmetic time type
	//~ time_t now;
	
	//~ // Obtain current time
	//~ // time() returns the current time of the system as a time_t value
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
	
	//~ pthread_t printar_thread;
	//~ pthread_create( &printar_thread , NULL ,  printar , (void*) teste);
	struct timeval now1,now2;
	double start,end,dif;
	dif=0;
	gettimeofday(&now1,NULL);
    start = ( (double) now1.tv_usec ) / 1000000 ;
    start += ( (double) now1.tv_sec );
    int value = 1;
	//~ setsockopt(clientSock, IPPROTO_TCP, TCP_NODELAY, (void*)&value, sizeof(value));
    printf("Iniciando transmissão por %d segundos\n",segundosUser);
	while(dif<segundosUser+0.2)
	{
		//usleep(0.01);
		envioAtual = send(clientSock , buff , strlen(buff) , 0);
		if( envioAtual < 0)
		{
			puts("Falha no envio, provavelmente server está desconectado\n");
			return 1;
		}
		//~ if(envioAtual-1!= BUFSIZE)
			//~ printf("Enviou %.2f , e deveria ter mandado 32768\n",envioAtual);
		
		//~ sem_wait(&semaforo);
		//~ byteEnviados	= byteEnviados + envioAtual;
		//~ sem_post(&semaforo); // libera semaforo
		
		fflush(stdin);
		
		gettimeofday(&now2,NULL);
        end = ( (double) now2.tv_usec ) / 1000000 ;
        end += ( (double) now2.tv_sec );
        dif=end-start;
	}

	close(clientSock);
	
	//~ sem_wait(&semaforo); //bloqueia semaforo para um printar por vez
	//~ arq=fopen(url,"a");
      //~ if(arq==NULL){
        //~ printf("Erro ao abrir arquivo \n");
        //~ return -1;
      //~ }
      //~ else
      //~ {
		//~ double resultado, divisor;
		//~ divisor =(double)count;
		//~ printf("Total de bits enviados %.2f\n",bytesTotais*8);
		//~ resultado =(double)((bytesTotais*8)/divisor);
		//~ fprintf(arq, "Média Total do teste: %.2f (bit/seg)\n",resultado);
	  //~ }
	  //~ fclose(arq);
	//~ sem_post(&semaforo); // libera semaforo
  
   exit(0);
}
