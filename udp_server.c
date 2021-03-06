#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */



#define MAX_MSG 1024

// a simple hex-print routine. could be modified to print 16 bytes-per-line
//http://stackoverflow.com/questions/18152913/aes-aes-cbc-128-aes-cbc-192-aes-cbc-256-encryption-decryption-with-openssl-c?rq=1
static void hex_print(const void* pv, size_t len){
    const unsigned char * p = (const unsigned char*)pv;
    if (NULL == pv)
        printf("NULL");
    else
    {
        size_t i = 0;
        for (; i<len;++i)
            printf("%02X ", *p++);
    }
    printf("\n");
}

int main(int argc, char **argv){

  int   sd;
  struct sockaddr_in server, client;
  char buf[MAX_MSG];
  char sendBuf[MAX_MSG];

  puts("try creating socket");

  sd = socket (AF_INET,SOCK_DGRAM,0);
  if( sd<0 ){
    puts("cann't open socket!");
    return 1;
  }


  puts("try binding local server port");
  server.sin_family = AF_INET;  
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  if( argc!=2 ){
    puts("please prive port number to run [ ./server 9090]");
    return 1;
  }
  server.sin_port = htons( atoi(argv[1]) );



  puts("creating a socket to which i will listen, receive and send");
  int rc;
  rc = bind( sd,  (struct sockaddr *) &server, sizeof(server));

  if(rc<0){
    puts("cann't bind port number ");
    return 1;
  }

  printf("waiting for connection at port %d\n", atoi(argv[1]));
  int ln,clen,hp,sendSize;

  for(;;){
    /* init buffer */
    memset(buf,0x0,MAX_MSG);

    clen = sizeof(client);
    ln=recvfrom (sd, buf, MAX_MSG, 0, (struct sockaddr *) &client, &clen );
    if(ln>0){
        printf("From %s:UDP %u : \n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

        hex_print(buf, strlen(buf) );
        printf("Sending data back to client => \n");
        //sprintf(sendBuf,"Server: %s",buf);
        sendSize = sendto(sd, buf, strlen(buf) , 0, ( struct sockaddr *) &client, sizeof(client) );
    }else{
        puts("could not receive data");
    }
  }

   return 0;
}