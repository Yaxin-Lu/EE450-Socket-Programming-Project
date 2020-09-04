//Students.cpp
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <netinet/in.h>


#define PORT 6184

#define PORT1 21584
#define PORT2 21684
#define PORT3 21784
#define PORT4 21884
#define PORT5 21984



using namespace std;





void do_studentUDP(int);


void do_student(int n){
    sleep(10);

     //start to build socket connection with server(USC)
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
    printf("\n Socket creation error \n");

    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
    printf("\nInvalid address/ Address not supported \n");

    }
    if(n==1){ 
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
        printf("\nConnection Failed \n");

        }
    }
    else if(n==2) {
        sleep(2);
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
    printf("\nConnection Failed \n");

    }
    }
    else if(n==3) {
        sleep(4);
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
    printf("\nConnection Failed \n");

    }
    }
    else if(n==4) {
        sleep(6);
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
    printf("\nConnection Failed \n");

    }
    }
    else if(n==5){
        sleep(8);
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
    printf("\nConnection Failed \n");

    }
    }
    
    
    
            
    string s;
    if(n==1) s = "student1.txt";
    else if(n==2) s = "student2.txt";
        
    
    else if(n==3) s = "student3.txt";
    
    
    else if(n==4) s ="student4.txt";
        
    else if(n==5) s ="student5.txt";
        
    


    ifstream myfile (s);

    string line;
  
    
    if (myfile.is_open())
      {
          getline(myfile,line);
          const char *str = line.c_str();
        send(sock , str , strlen(str) , 0 );
        
        //code do it when connection is built and send is finish! dynamical port
        struct sockaddr_in sin;
        socklen_t len = sizeof(sin);
        if (getsockname(sock, (struct sockaddr *)&sin, &len) == -1)
            perror("getsockname");
        else
            printf("\n<Student%d> has TCP port %d and IP address %s for Phase 2\n", n, ntohs(sin.sin_port), inet_ntoa(sin.sin_addr));
         
          while(getline(myfile,line)){
               sleep(3);
               const char *str1 = line.c_str();
               
               send(sock , str1 , strlen(str1) , 0 );
               
          }
          sleep(3);
          //send the last packet to illustrate which student send here
          const char *sss = s.c_str();
          send(sock , sss , strlen(sss) , 0 );
          printf("\nCompleted sending application for <Student%d>.\n", n);
          int valread= read( sock , buffer, 1024);
          printf("\n<Student%d> has received the reply from the health center.\n", n);
          
          if(buffer[0]=='1') {
              //close TCP socket
              close(sock);
              do_studentUDP(n);
          }
          else close(sock);

          
          
      }

}


void do_studentUDP(int n){
    
    uint16_t portOfStu;
    if(n==1) portOfStu = PORT1;
    else if(n==2) portOfStu = PORT2;
    else if(n==3) portOfStu = PORT3;
    else if(n==4) portOfStu = PORT4;
    else portOfStu = PORT5;


    int sockfd;
    char buffer[1024];
    
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    servaddr.sin_port = htons(portOfStu);

    printf("\n<Student%d> has UDP port %d and IP address %s for Phase 2\n", n, ntohs(servaddr.sin_port), inet_ntoa(servaddr.sin_addr));

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
    sizeof(servaddr)) < 0 )
    {
    perror("bind failed");
    exit(EXIT_FAILURE);
    }
    
    int  n1;
    socklen_t len = sizeof cliaddr;
    n1 = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buffer[n1] = '\0';
    printf("\n<Student%d> has received the application result, the result is: %s\n", n,buffer);
    close(sockfd);
}




int main(int argc, char const *argv[])
{
    //use fork to create 3 parent process

    int count = 0;

      pid_t pid;
       while(true){
            
            pid = fork();
            if (pid == -1)
                exit(EXIT_FAILURE);
            if (pid == 0)
            {   
                // child process
                if(count==0) do_student(1);
                else if(count==1) do_student(2);
                else if(count==2) do_student(3);
                else if(count==3) do_student(4);
                else if(count==4) do_student(5);
                exit(EXIT_SUCCESS);
            
            }
            else{
                 //father process
                 count++;
            }
            if(count==5) break;
            
            
       }
       return 0;
}
