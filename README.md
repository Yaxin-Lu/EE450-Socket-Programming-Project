# EE450-Socket-Programming-Project
Simulation of a covid-19 tesing application process

This is the repository collects the code of 2020 Summer EE450 Socket Programming Project. The project is C/C++ based and aims to simulate a covid-19 tesing application process with TCP and UDP.

Project requirements:

In this project i will simulate a covid-19 tesing application process. This process involves the USC students who apply for covid-19 virus testing, the USC Health Center that manages the testing, and the different hospitals that will provide students the chances to get the test. The communication among the USC students, health center and hospitals are performed over TCP and UDP sockets in a network with a client-server architecture.

The project has 2 major phases: 1) Hospitals provide the USC health center, the list of available departments that could do the testing process, 2) Students apply to the USC center to have the chance of getting tested. The USC health center makes the decision and sends the results to both students and hospitals.

In phase 1 all communications are through TCP sockets.
In phase 2 the communication is through both TCP and UDP sockets. 



What I have done

Phase 1:
In phase1, i use fork() in Hospital.cpp to create 3 socket for 3 hospitals. These child process may execute simultaneously so the  On-screen message of Hospital.cpp may be little messy. It will send departments of Hospital A and departments of Hospital B in nearly  same time(and print out message just same time).  You need to terminate Hospital program at the end by pressing ctrl-C.

I also use fork() in Healthcenter.cpp in order to create a child process when a new TCP connection is built. Then child process will deal with the message from new TCP connection, the father process simply close the new socket built in new TCP connection and go on to accept next new connection. In child process, i use list to store new message from hospital, and at end of each child process i put these message of departments into a file named result.txt. 

Note: because i use number of lines inside result.txt to judge if while loop should end, so if you are trying to test my program several times, please delete the old data in result.txt each time before you test. if you do not delete old data, you need to terminate Healthcenter program at the end by pressing ctrl-C.

Plus, i assume the data in each Hospital#.txt has 3 lines. So if you change lines in Hospital#.txt, the program will fail.

Phase 2:

Hospital.cpp:
In phase 2, i still use child process in hospital that formed by fork, 3 child process will open 3 UDP servers, and wait for Healthcenter's message. 

NOTE:  Because I use while(true) in hospital, so you need to terminate Hospital program at the end by pressing ctrl-C. Plus, if you want to test my program several times, you need to check if there are zombine process left by the last time you ran Hospital. For example, command "lsof -i:21284" can check if there is zombine process using port 21284, then you can kill this process by "kill #PID". if you don't kill zombine process, when you run Hospital twice time, it will end up "bind fail: address already use" in UDP of phase 2.

Student.cpp:
I also use fork() in student in phase 2 to form 5 child process. Each child process will build up a TCP connection with Healthcenter after sleep 10 seconds. So you will see after you run ./student localhost, there will be no output(print message) on screen during first 10 seconds waiting time. I design this waiting time to let Healthcenter to receive hospital's TCP packets firstly. After 10 seconds, the student program will start TCP connection and print message. 

Plus, although i use fork() in student, i don't want 5 child process to connect with Healthcenter at nearly same time, so i put 2 seconds waiting time between every 2 child process by using sleep(). So you will see the Student program connect with health center at 2 second intervals. 

After sending out TCP packets, each child process will wait for TCP reply from health center, if it receive packet with content "1", then it will close TCP socket and start to build UDP server, waiting for UDP packet from Health center; if it receive "0", then it will just close TCP socket. After student(that has valid application) receive Healthcenter's UDP packet, it will print packet out then program end.


Healthcenter.cpp:
Accepting TCP connection in Healthcenter.cpp is all in one while loop. After Healthcenter accpet 3 hospital's 
data in phase 1, it will continue to receive students' data in phase 2 in the same while loop. 

NOTE: After start running Healthcenter, you have to run Hospital first, and then run Student secondly, the order is very important, or data processing will end up fault. 

NOTE: Health center will store the Hospital's data in phase 1 in the file named "result.txt", it will store Student's data sent in TCP connection (phase 2) in the file named "Stu.txt". Because i use the number of lines in those files to judge if I should go to post-processing, so it is important to make sure that old data in result.txt and Stu.txt are deleted before you run program, if you do not delete the old data, then some fault will happen.


Health center will read out the data in result.txt and Stu.txt, form two list respectively and compare corresponding information to get application result, put result into list "resu". Then it will send UDP packets to students to tell them about result of their application. Finally, health center will send UDP packets to hospitals to tell them about assignment of students. Then Healthcenter program will end.  
  


