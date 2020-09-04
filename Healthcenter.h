#ifndef HEALTH_CENTER_H
#define HEALTH_CENTER_H

#include <vector>


//*************************************************************************
// Node type used for lists
struct Node {
   std::string s;
   int data1;
   int data2;
   Node *next;

   Node(std::string s1, int item1, int item2);
   Node(std::string s1, int item1, int item2, Node *n);

};


typedef Node * ListType;

#endif
