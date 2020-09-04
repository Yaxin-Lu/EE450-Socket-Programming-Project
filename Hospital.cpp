// Client side C/C++ program to demonstrate Socket programming
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
#define PORT 6184


#define PORTA 21284
#define PORTB 21384
#define PORTC 21484


using namespace std;


void do_HospitalUDP(int);


void HospitalToUSC(int n){
    
    string s1;
    string s2;
    if(n==1){
    s1 = "A";
    s2 ="HospitalA.txt";
    }
    else if(n==2){
    s1 = "B";
    s2 ="HospitalB.txt";
    }
    else {
    s1 = "C";
    s2 ="HospitalC.txt";
    }

    ifstream myfile (s2);
    
    string line1;
    string line2;
    string line3;
    if (myfile.is_open())
    {
        getline(myfile,line1);
        const char *str1 = line1.c_str();

        getline(myfile,line2);
        const char *str2 = line2.c_str();


        getline(myfile,line3);    
        const char *str3 = line3.c_str();


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


        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
        printf("\nConnection Failed \n");

        }
        
        send(sock , str1 , strlen(str1) , 0 );




        //code do it when connection is built and send is finish! dynamical port
        struct sockaddr_in sin;
        socklen_t len = sizeof(sin);
        if (getsockname(sock, (struct sockaddr *)&sin, &len) == -1)
            perror("getsockname");
        else
            printf("\n<Hospital%s> has TCP port %d and IP address %s for Phase 1\n", s1.c_str(), ntohs(sin.sin_port), inet_ntoa(sin.sin_addr));

        //print
        printf("\n<Hospital %s> is now connected to the admission office\n",s1.c_str());
        printf("\n<Hospital %s> has sent <department%s1> to the admission office\n",s1.c_str(),s1.c_str());

        sleep(3);
        send(sock , str2 , strlen(str2) , 0 );
        printf("\n<Hospital %s> has sent <department%s2> to the admission office\n",s1.c_str(),s1.c_str());
        sleep(3);
        send(sock , str3 , strlen(str3) , 0 );    
        printf("\n<Hospital %s> has sent <department%s3> to the admission office\n",s1.c_str(),s1.c_str());
        printf("\nUpdating the admission office is done for <Hospital %s>\n",s1.c_str());
        printf("\nEnd of Phase 1 for <Hospital %s>\n",s1.c_str());
        //TCP done, close TCP
       close(sock);
       sleep(5);
       do_HospitalUDP(n);

    }
    


}


void do_HospitalUDP(int n){
    uint16_t portOfHos;
    string s;
    if(n==1) {
        portOfHos = PORTA;
        s = "A";
    }
    else if(n==2) {
        portOfHos = PORTB;
        s = "B";
    }
    else if(n==3){ 
        portOfHos = PORTC;
        s = "C";
    }

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
    servaddr.sin_port = htons(portOfHos);

    printf("\n<Hospital %s> has UDP port %d and IP address %s for Phase 2\n", s.c_str(), ntohs(servaddr.sin_port), inet_ntoa(servaddr.sin_addr));

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
    sizeof(servaddr)) < 0 )
    {
    perror("bind failed");
    exit(EXIT_FAILURE);
    }
    
    int n1;
    socklen_t len = sizeof cliaddr;
    while(true){
    n1 = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buffer[n1] = '\0';
    printf("\n<Student%c> has been admitted to <Hospital %s> \n", buffer[7], s.c_str());
    }
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
                if(count==0) HospitalToUSC(1);
                else if(count==1) HospitalToUSC(2);
                else if(count==2) HospitalToUSC(3);
                
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











