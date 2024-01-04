#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "packet.h"


struct message {
        char text[TEXT_LEN_MAX];
        char sNum[10];
    }
int main(){
    
    //init socket
    int oursocket = socket(AF_INET,SOCK_STREAM,0);
    //checkup if socket is not uwu
    if(oursocket == -1){
        printf("Socket-Erstellung fehlgeschlagen");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in6 server_addr;
    server_addr.sin6_family = AF_INET6;
    //network order to little endian => htons(128) = htons("1000 0000") => 0000 0001
    server_addr.sin6_port = htons(50000);
    inet_pton(AF_INET6, argv[1], &server_addr.sin6_addr);

    int connector = connect(oursocket,&server_addr,sizeof(server_addr));
    if(connector)== -1){
        printf("connect has failed us.");
        //close socket so nobody can connect to us.
        close(oursocket);
        exit(EXIT_FAILURE);
    }

    while(1){
        //prep to look for message or send
        fd_set read_fds;
        FD_ZERO(&read_fds);
        //stdin
        FD_SET(0,&read_fds);
        //sachen vom Server
        FD_SET(oursocket,&read_fds);


        int selector = select(oursocket+1,&read_fds,NULL,NULL,NULL);
        if(selector ==-1){
            print("select has gone rogue T.T ");
            close(oursocket);
            exit(EXIT_FAILURE);
        }

        if(FD_ISSET(oursocket,&read_fds)){
            struct message from_server;
            ssize_t num_bytes_recv = recv(oursocket,&from_server,sizeof(from_server),0);
            if(num_bytes_recv <= 0){
                printf("Server closed connection or Error while recv");
                close(oursocket);
                exit(EXIT_FAILURE);  
            } else{
                printf("Message from Server: %s from %s\n",from_server.text,from_server.sNum);
            }
        }

        if(FD_ISSET(0,&read_fds)){
            struct message to_server;
            printf("Enter Text to sent...: ");
            char * erg = fgets(&to_server.text, sizeof(to_server.text),stdin);
            if( *erg == NULL){
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
            to_server.text[len_text] = '\O';
            int len_sNum = strcspn(to_server.text,"\n");
            if(len_sNum>10){
                printf("sNum not valid.");
                close(oursocket);
                exit(EXIT_FAILURE);
            }
            to_server.sNum[len_sNum] = '\O';
            if(send(oursocket,&to_server,sizeof(to_server),0)== -1){
                printf("sending has gone boom");
                close(oursocket);
                exit(EXIT_FAILURE);
            }
        }
    }
    close(oursocket);
    return 0;
}
