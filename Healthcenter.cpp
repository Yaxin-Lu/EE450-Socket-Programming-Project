// Server side C/C++ program to demonstrate Socket programming
// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <memory.h>
#include "Healthcenter.h"

#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include<signal.h>
#include <sys/wait.h>

#include <fstream>
#include <iostream>
#include <set>

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


#include <ctime>

#define PORT 6184

#define PORT1 21584
#define PORT2 21684
#define PORT3 21784
#define PORT4 21884
#define PORT5 21984

#define PORTA 21284
#define PORTB 21384
#define PORTC 21484

using namespace std;




Node::Node(string s1, int item1, int item2){
   s = s1;
   data1 = item1;
   data2 = item2;
   next = NULL;
}

Node::Node(string s1, int item1, int item2, Node *n){
   s = s1;
   data1 = item1;
   data2 = item2;
   next = n;
}



void sigCatcher(int n) {
        //printf("a child process dies\n");
        while(waitpid(-1, NULL, WNOHANG) > 0);
    }


void do_Hospital(int);
void do_Student(int);

int main(int argc, char const *argv[])
{


    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    //signal(SIGCHLD, SIG_IGN);
    signal(SIGCHLD, sigCatcher);
   
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
    perror("socket failed");
    exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
    &opt, sizeof(opt)))
    {
    perror("setsockopt");
    exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons( PORT );


    printf("\nThe health center has TCP port %d and IP address %s\n", ntohs(address.sin_port), inet_ntoa(address.sin_addr));
    

    // Forcefully attaching socket to the port 6184
    if (bind(server_fd, (struct sockaddr *)&address,
    sizeof(address))<0)
    {
    perror("bind failed");
    exit(EXIT_FAILURE);
    }
    //start listen
    if (listen(server_fd, 3) < 0)
    {
    perror("listen");
    exit(EXIT_FAILURE);
    }

    
    int flag=0;
    pid_t pid;
    while(true){
        

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
        (socklen_t*)&addrlen))<0)
        {
        perror("accept");
        exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid == -1)
            exit(EXIT_FAILURE);
        if (pid == 0)
        {
            // child process(
            close(server_fd);
            if(flag<=2) do_Hospital(new_socket);
            else if(flag==3){
           
            //start phase 2
            printf("\nThe health center has TCP port %d and IP address %s\n", ntohs(address.sin_port), inet_ntoa(address.sin_addr));
            do_Student(new_socket);
            }
            else do_Student(new_socket);
            exit(EXIT_SUCCESS);
           
        }
        else{
            close(new_socket); //father process
            flag++;
        }
            
        if(flag==8) {
            break;
        }
       
    }
   
   
    //get all department data into one list
    ListType dep =NULL;
 
    ifstream myfile ("result.txt");
    string line;
    if (myfile.is_open()){
        while(getline(myfile,line)){   
            char* pch;
            char* p = const_cast<char*> (line.c_str());
            pch = strtok(p," ");
            string ss = pch;
            pch = strtok (NULL, " ");
            int d1 = atoi(pch);
            pch = strtok (NULL, " ");
            int d2 = atoi(pch);
            ListType dummy = dep;
            dep= new Node(ss,d1,d2,dummy);
        }

    }
    myfile.close();
    //use resu list to store result.
    ListType resu =NULL;
    // start compare student's sypotom with their interests' range
    
    while(true){
        int numberofline = 0;
        ifstream myfile2 ("Stu.txt");
        if (myfile2.is_open()){
        while(getline(myfile2,line)){
              numberofline++;
           }
        myfile2.close();
        if(numberofline==5) break;
        
       }
    }
    sleep(5);
         
    ifstream myfile2 ("Stu.txt");
    if (myfile2.is_open()){
        while(getline(myfile2,line)){
                    char* pch;
                    char* p = const_cast<char*> (line.c_str());
                    pch = strtok(p," ");
                    int syp = atoi(pch);
                    
                    string match;
                    int student=0;
                    while(pch = strtok (NULL, " ")){
                        string ins = pch;
                        if(ins.length()==1) {
                            student= atoi(pch);
                            break;
                        }
                        ListType cur = dep;
                        while(cur!=NULL){
                            if(cur->s==ins){
                                //find interest
                                if(cur->data1 <= syp && cur->data2 >= syp) match = cur->s;
                                break;    
                            }
                            else cur=cur->next;
                        }
                        //break as soon as we find match
                        if(match.empty() == false) break;
                    }
                    if(match.empty()) resu = new Node("rej",student,0,resu);
                    else {
                        //find which student he is
                        while(pch = strtok (NULL, " ")){
                            string ins = pch;
                            if(ins.length()==1) {
                                student= atoi(pch);
                                break;
                            }
                        }
                        resu = new Node(match,student,syp,resu);
                        
                    }
        }

    }
    myfile2.close();
        
    //close TCP parent socket and start UDP
    close(server_fd);
    //UDP GO
    int sockfd;
    
    struct sockaddr_in servaddr;
    
    //  according to resu list, send UDP to Hos and student!
        ListType p2 = resu;
        uint16_t portOfHos;
        uint16_t portOfStu;
        
        int cou = 0;
        // Creating socket file descriptore, first send to student
            if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
            }

        while(p2!=NULL) {
            if(p2->data1 == 1) portOfStu = PORT1;
            else if(p2->data1 == 2) portOfStu = PORT2;
            else if(p2->data1 == 3) portOfStu = PORT3;
            else if(p2->data1 == 4) portOfStu = PORT4;
            else if(p2->data1 == 5) portOfStu = PORT5;
            
        
            memset(&servaddr, 0, sizeof(servaddr));

            // Filling server information, first send to student
            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons(portOfStu);
            servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            
            
            
            int n, len;

            if(p2->s=="rej"){ 
                const char *rej = "Reject";
                sendto(sockfd, (const char *)rej, strlen(rej),MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
                if(cou==0){ 
                    //code do it when connection is built and send is finish! dynamical port
                    struct sockaddr_in sin;
                    socklen_t len = sizeof(sin);
                    if (getsockname(sockfd, (struct sockaddr *)&sin, &len) == -1)
                        perror("getsockname");
                    else
                        printf("\nThe health center has UDP port %d and IP address %s for Phase 2\n", ntohs(sin.sin_port), inet_ntoa(sin.sin_addr));
                    
                    cou++;
                }
                
                printf("\nThe health center has send the application result to <Student%d>\n", p2->data1);

                //close(sockfd);
                p2 = p2->next;
                continue;
            }
            else{
                string b = "Accept#"+ p2->s + "#hospital"+ (p2->s).substr(0,1);
                const char *acc = b.c_str();
                sendto(sockfd, (const char *)acc, strlen(acc),MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
                if(cou==0){
                    //code do it when connection is built and send is finish! dynamical port
                    struct sockaddr_in sin;
                    socklen_t len = sizeof(sin);
                    if (getsockname(sockfd, (struct sockaddr *)&sin, &len) == -1)
                        perror("getsockname");
                    else
                        printf("\nThe health center has UDP port %d and IP address %s for Phase 2\n", ntohs(sin.sin_port), inet_ntoa(sin.sin_addr));
                    
                    cou++;
                }
                printf("\nThe health center has send the application result to <Student%d>\n", p2->data1);
               // close(sockfd);

               
            }
            
            //printf("\nStudent%d has been assigned to %s \n", p2->data1, (p2->s).c_str());
            p2=p2->next;
        }
        p2 = resu;
        while(p2!=NULL) {
            if(p2->s=="A1"||p2->s=="A2"||p2->s=="A3") portOfHos = PORTA;
            else if(p2->s=="B1"||p2->s=="B2"||p2->s=="B3") portOfHos = PORTB;
            else portOfHos = PORTC; 

          // Creating socket file descriptore, send to Hospital
           if(p2->s !="rej"){
               
                memset(&servaddr, 0, sizeof(servaddr));
                servaddr.sin_family = AF_INET;
                servaddr.sin_port = htons(portOfHos);
                servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
                int n, len;
                string hos = "Student" + to_string(p2->data1) + "#" + to_string(p2->data2) + "#" + p2->s;
                const char* reply = hos.c_str();
                sendto(sockfd, (const char *)reply, strlen(reply),MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

                printf("\nThe health center has send one admitted student to <Hospital%s>\n", ((p2->s).substr(0,1)).c_str());
               //close(sockfd);
           }
           p2 = p2->next;
        }

    close(sockfd);
    
    return 0;
            
    
    
}



void do_Hospital(int new_socket){
                        char buffer[1024];
                        char sss1;
                        int valread = read( new_socket , buffer, 1024);
                        sss1 = buffer[0];
                        char * pch;
                        //use list to store
                        ListType cur = NULL;
                        //printf ("\nSplitting string %s into tokens:\n",buffer);
                        pch = strtok (buffer," #,");
                        string ss = pch;
                        pch = strtok (NULL, " #,");
                        int d1 = atoi(pch);
                        pch = strtok (NULL, " #,");
                        int d2 = atoi(pch);
                        ListType dummy = cur;
                        cur= new Node(ss,d1,d2,dummy);
                        //read again
                        valread = read( new_socket , buffer, 1024); 
                        //printf ("\nSplitting string %s into tokens:\n",buffer);
                        pch = strtok (buffer," #,");
                        ss = pch;
                        pch = strtok (NULL, " #,");
                        d1 = atoi(pch);
                        pch = strtok (NULL, " #,");
                        d2 = atoi(pch);
                        dummy = cur;
                        cur= new Node(ss,d1,d2,dummy);
                        //read again
                        valread = read( new_socket , buffer, 1024);
                        //printf ("\nSplitting string %s into tokens:\n",buffer);
                        pch = strtok (buffer," #,");
                        ss = pch;
                        pch = strtok (NULL, " #,");
                        d1 = atoi(pch);
                        pch = strtok (NULL, " #,");
                        d2 = atoi(pch);
                        dummy = cur;
                        cur= new Node(ss,d1,d2,dummy);
                        
                        
                        //close child socket
                        close(new_socket);
                        printf ("\nReceived the department list from <Hospital %c>\n",sss1);
                        //write into txt files
                        ofstream myfile2 ("result.txt",ios::app);                     
                        
                        if(myfile2.is_open()){
                            //print this list
                            ListType p = cur;
                            while(p!=NULL){
                            //printf ("%c is : %s, %d, %d\n", sss1, (p->s).c_str(), p->data1, p->data2);
                            myfile2 <<(p->s).c_str()<<" " << p->data1 <<" "<< p->data2 << '\n'; 
                            p=p->next;
                            }
                        }
                        myfile2.close();
                        if(sss1=='C') printf ("\nEnd of Phase 1 for the health center\n");
                        



}

void do_Student(int new_socket){
                    
                    char buffer[1024];
                    char sss1;
                    //read first packet
                    int valread = read( new_socket , buffer, 1024);
                    sss1 = buffer[0];
                    char * pch;
                    int valid2 = 0;
                    const char *valid ="1";
                    const char *invalid = "0";
                    
                     //get department from result.txt, put it in set
                     ifstream myfile ("result.txt");
                     string line;
                     set<string> s;
                     if (myfile.is_open()){
                         while(getline(myfile,line)){   
                         s.insert(line.substr(0,2));
                         }

                     }
                     myfile.close();


                    int number;
                    //printf ("\nSplitting string %s into tokens:\n",buffer);
                        pch = strtok (buffer," :");
                        string ss = pch;
                        pch = strtok (NULL, " :");
                        int d1 = atoi(pch);
                        //open a student file to store it 
                        ofstream myfile2 ("Stu.txt",ios::app);   
                        if(myfile2.is_open()){
                            myfile2 << d1 <<" ";
                            while(true){
                                    
                                    //read again
                                    valread = read( new_socket , buffer, 1024); 
                                  //printf ("\nSplitting string %s into tokens:\n",buffer);
                                    if(buffer[0]!='s'){
                                        myfile2 << buffer[10] << buffer[11] <<" ";
                                        string b(buffer+10, buffer+12);
                                        if(s.count(b)==1&&valid2==0){
                                            //send valid to student
                                            send(new_socket,valid, strlen(valid),0);
                                            valid2 =1;
                                        }
                                    }
                                    else{
                                        //read the last packet
                                        myfile2 << buffer[7] <<'\n';
                                        number = buffer[7] - '0';  
                                        //if valid2=0, send invalid
                                        if(valid2==0) send(new_socket,invalid, strlen(invalid),0);
                                        break;
                                    }
                                    
                            }
                                    
                        }
                        myfile2.close();
                        //done storation, close child socket
                        close(new_socket);
                        printf ("\nhealth center receive the application from <Student%d>\n",number);
                        
                        
                        

}
