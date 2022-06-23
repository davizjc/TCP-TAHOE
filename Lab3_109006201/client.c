#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SA struct sockaddr

#define LENGTH 16
struct Data_sequence{
    int seq_num;
    char Data[LENGTH];
};
struct ack_pkt{
    int seq_num;
};

//========================
// Receive data / send ack
//========================
struct Data_sequence data_seq;
struct ack_pkt ACK;


int unACK[1000];
int front = 0, back = 0;
int i = 0, j = 0;
int count_loss = 0;


int main(int argc , char *argv[])
{
    //===========================================
    // Todo: Create TCP socket and TCP connection
    int sockfd = 0; 
    struct sockaddr_in servaddr;
   
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons (atoi(argv[1]));
   
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connect sucessfully\n");

    // function for chat
    // func(sockfd);
    
    for(j = 0; j < 1000 ; j++) unACK[j] = -1;
    while(1) {
    //=================================================================
    // Todo: 1. receive data and send ACKs with correct sequence number
    //       2. simulate packet loss (or you can implement in server.c)
    // Required format: 
    //       received: seq_num = [sequence number]
    //       loss: seq_num = [seq_num]
    //=================================================================
        
        if(recv(sockfd, (void*)&data_seq, sizeof(data_seq.Data),0)) {
            
            //lost
            if(i == data_seq.seq_num && (data_seq.seq_num == 8)) {
                printf("loss: seq_num = %d\n", data_seq.seq_num);
                ACK.seq_num = unACK[back] = data_seq.seq_num;
                back++;
                i++;
            } else if(unACK[front] == data_seq.seq_num || i == data_seq.seq_num) {
                printf("received: seq_num = %d\n", data_seq.seq_num);
                printf("ack seq num: %d\n", ACK.seq_num);
                if(unACK[front] == data_seq.seq_num) front++;
                else i++;
                if(front < back) ACK.seq_num = unACK[front];
                else ACK.seq_num = data_seq.seq_num;
            }
                
            send(sockfd, &ACK, sizeof(ACK.seq_num),0);
            printf("send %d\n", ACK.seq_num);
        }
    }
    // close the socket
    close(sockfd);


    return 0;
}


