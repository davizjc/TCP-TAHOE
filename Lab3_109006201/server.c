#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SA struct sockaddr
#define LENGTH 16
int ROUND = 50;
int MAXROUND = 50;

//======================================
// Data sequence
// sample sequence packet:
//    seq_num: sequence number
//    Data: data
// you can add another variables if need
//======================================
struct Data_sequence{
    int seq_num;
    char Data[LENGTH];
};

//================================
// ACK packet with sequence number
//================================
struct ack_pkt{
    int seq_num;
};

//======================
// Parameter declaration
//======================
int sockfd = 0;
struct sockaddr_in serverInfo;
int clientSockfd;
struct sockaddr_in clientInfo;
struct Data_sequence data_seq;
struct ack_pkt ACK;
int cwnd = 1;
int ssthresh = 8;
int i;
int lossNum = -1;

void sender(int clientSockfd){
    //===========================================
    // Write your send here
    // Todo: 1. send cwnd sequences of data that
    //          starts with right sequence number
    // Required format: 
    //       send: seq_num = [sequence number]
    //===========================================
    for (i = 0; i<cwnd;i++){
         
        if(lossNum != -1){              //if loss resend 
            printf("send: seq_num = %d\n",ACK.seq_num );
            send(clientSockfd, (void*)&ACK, sizeof(ACK) ,0);
            ACK.seq_num++;
            lossNum = -1;
            continue;
        }
         if (ROUND > 0){                // normal send 
            printf("send: seq_num = %d\n",data_seq.seq_num);
            send(clientSockfd, (void*)&data_seq, sizeof(data_seq.Data) ,0);
            data_seq.seq_num++;
            ROUND--;
        }
        else {
            break;
        }
       
    }
    
}

void receiver(int clientSockfd)
{
    int duplicate = 0;
    int previous = -1;
    int i;
    for(i = 0;i<cwnd;i++){
        
        recv(clientSockfd, (void*)&ACK, sizeof(ACK), 0);

        printf("ACK:  seq_num = %d\n", ACK.seq_num );
        if(previous == ACK.seq_num) duplicate++;
        else duplicate = 1;
        
        if(duplicate == 3)
        {
            ssthresh = cwnd / 2;
            lossNum = ACK.seq_num;
            printf("3 duplicate ACKS; seq_num = %d, ssthresh = %d\n",ACK.seq_num,ssthresh); 
        }
        previous = ACK.seq_num;
    }

    if(duplicate < 3)
    {
        if (cwnd >= ssthresh)
        {
            printf("state: congestion avoidance\n");
            cwnd += 1;
        }
        else {
            printf("state: slow start\n");
            cwnd *= 2;
        }
        
       // printf("current val is: %d\n", MAXROUND - ROUND);
       // printf("buff value is: %d\n", ACK.seq_num);
        if(MAXROUND - ROUND != ACK.seq_num + 1){        
            lossNum = ACK.seq_num ;
           // printf ("lossnum is %d", lossNum);
        }
    } else cwnd = 1;

}

int main(int argc, char *argv[])
{
    //===========================================
    // Todo: Create TCP socket and TCP connection
    int sockfd, clientSockfd, len;
    struct sockaddr_in servaddr, cli;
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons (atoi(argv[1]));
   
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    // Now server is ready to listen and verification
    if ((listen(sockfd, 1)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    
    len = sizeof(cli);
   
    // Accept the data packet from client and verification
    clientSockfd = accept(sockfd, (SA*)&cli, &len);
    
    if (clientSockfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
    //===========================================
    


    //============================================================================
    // Start data transmission
    // To be simple, we receive ACKs after all cwnd sequences of data are sent.
    // Thus we can not react to 3-duplicate immediately, which is OK for this lab.
    //============================================================================
    printf("state: slow start\n");
    while(ROUND>0){
        printf("cwnd = %d, ssthresh = %d\n", cwnd, ssthresh);
        sender(clientSockfd);
        receiver(clientSockfd);
    }
    
    close(clientSockfd);
    close(sockfd);


    return 0;
}