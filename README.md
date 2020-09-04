# EE450 Socket Programming Project
Simulation of a covid-19 tesing application process

This is the repository collects the code of 2020 Summer EE450 Socket Programming Project. The project is C/C++ based and aims to simulate a covid-19 tesing application process with TCP and UDP.

## Project requirements:

This project will simulate a covid-19 tesing application process，which involves the USC students who apply for covid-19 virus testing, the USC Health Center that manages the testing, and the different hospitals that will provide students the chances to get the test. The communication among the USC students, health center and hospitals are performed over TCP and UDP sockets in a network with a client-server architecture.

The project has 2 major phases: 
1) Hospitals provide the USC health center, the list of available departments that could do the testing process
2) Students apply to the USC center to have the chance of getting tested. The USC health center makes the decision and sends the results to both students and hospitals.

## what i have done
I have already finished requires phases in description:
# 1. phase 1 
all communications are through TCP sockets. 
create 3 runnable client as 3 hospitals, and 1 server as healthcenter. Using fork() to deal with concurrent processing.
# 2. phase 2A 
In phase 2 of this project, each student sends his/her application to the USC health center through TCP connections. SO create 5  runnable client as 5 students.
When the USC health center receives all the packets from a student, it searches his/her interested department in the file or the array/linked list that stored the information of departments. the USC health center replies with number 0 to the student whose application is invalid.

# 3.p phase 2B
the USC health center uses UDP connections to communicate with the hospitals and the students. So created a UDP sockect as healthcenter, and 5 student UDP sockects, and a 3 sockects as 3 hospitals.  
Implement the communication between hospitals and healthcenter, students and healthcenter.
