#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "packet.h"
#include <netinet/in.h>
#include <errno.h>



// ./server 
/*int verify_member(cdhar* member){
    int len = strlen(member);
    if(len == 6 && member[0] =='s' &&
        isdigit(member[1]) != 0 &&
        isdigit(member[2]) != 0 &&
        isdigit(member[3]) != 0 &&
        isdigit(member[4]) != 0 &&
        isdigit(member[5]) != 0;)
}*/

int main(int argc, char** argv){
    //abprüfen ob sachen ok sind.
    if(argc != 3){
        printf("Parameter are not complete.\n");
        exit(EXIT_FAILURE);
    }
    
    char* sNUM[6];
    strcpy(*sNUM,argv[1]);
    printf("Open for: %s\n", *sNUM);
    int Port_IPv6 = strtol(argv[2],NULL,10);
    if (!Port_IPv6) {
		printf("Port zu lang. Pech gehabt.\n");
		exit(EXIT_FAILURE);
	}
    printf("Port: %d\n\n", Port_IPv6);
    //init socket
    
    int oursocket = socket(AF_INET6,SOCK_STREAM,0);
    //checkup if socket is not uwu
    if(oursocket == -1){
        printf("Socket-Erstellung fehlgeschlagen");
        exit(EXIT_FAILURE);
    }
    printf("Socket ist da jungs. Wir celebrieren. %d\n",oursocket);
    int flagge = 1;
    //setsockopt(oursocket, SOL_SOCKET, SO_REUSEADDR, &flagge, sizeof(flagge));
    

    struct sockaddr_in6 server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    //network order to little endian => htons(128) = htons("1000 0000") => 0000 0001
    server_addr.sin6_addr = in6addr_any;
    server_addr.sin6_port = htons(Port_IPv6);
    //printf("port: %i\n", server_addr.sin6_port);
    //printf("Fam: %i\n", server_addr.sin6_family);

    int errnum;
    int binder = bind(oursocket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    if(binder == -1){
        errnum = errno;
        printf("Socket bind ist fehlgeschlagen\n");
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error Binding: %s\n", strerror( errnum ));
		exit(EXIT_FAILURE);
    }
    printf("Socket wurde gebindet.\n");
    
    int listener = listen(oursocket,7);
    if(listener == -1){
        printf("listen ist fehlgeschlagen\n");
		exit(EXIT_FAILURE);
    }
    printf("Der Server lauscht.\n");
    
    while(1){
        //prep to look for message or send
        fd_set read_fds;
        FD_ZERO(&read_fds);
        //stdin
        /*FD_SET(0,&read_fds);
        //sachen vom Server
        FD_SET(oursocket,&read_fds);

        
        int selector = select(oursocket+1,&read_fds,NULL,NULL,NULL);
        if(selector ==-1){
            printf("select has gone rogue T.T ");
            close(oursocket);
            exit(EXIT_FAILURE);
        }
        
        if(FD_ISSET(oursocket,&read_fds)){
            struct packet from_server;
            ssize_t num_bytes_recv = recv(oursocket,from_server.whole_mess,sizeof(from_server),0);
            for(int i = 0; i< 1032;i++){
            if(i<=7){
                from_server.sNum[i]= from_server.whole_mess[i];
            }else{
                from_server.text[i-8]= from_server.whole_mess[i];
                }
            if(num_bytes_recv <= 0){
                printf("Server closed connection or Error while recv");
                close(oursocket);
                exit(EXIT_FAILURE);  
            } else{
                printf("Message from Server: %s from %s\n",from_server.text,from_server.sNum);
            }
        }
        }
        
        if(FD_ISSET(0,&read_fds)){
            struct packet to_server;
            printf("Enter Text to sent...: ");
            char * erg = fgets(to_server.text, sizeof(to_server.text),stdin);
            if( erg == NULL){
                printf("cant send message...");
                close(oursocket);
                exit(EXIT_FAILURE);
            }
            int len_text = strcspn(to_server.text,"\n");
            if(len_text>1023){
                printf("Message too long.");
                close(oursocket);
                exit(EXIT_FAILURE);
            }
            //end the string properly
            to_server.text[len_text] = '\0';
            int len_sNum = strcspn(to_server.text,"\n");
            if(len_sNum>10){
                printf("sNum not valid.");
                close(oursocket);
                exit(EXIT_FAILURE);
            }
            to_server.sNum[len_sNum] = '\0';
            if(send(oursocket,&to_server,sizeof(to_server),0)== -1){
                printf("sending has gone boom");
                close(oursocket);
                exit(EXIT_FAILURE);
            }
        }
    }*/
    close(oursocket);
    return 0;
    }
}
