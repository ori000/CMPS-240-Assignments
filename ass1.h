//################################ DO NOT ALTER THIS FILE ####################################################
// You are only allowed to use these library elements
#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "string.h"

//######################################################################################
// Replace "" with your info (without spaces)
#define STUDENT_NAME "" 
#define STUDENT_AUBNET ""
#define STUDENT_ID ""
//######################################################################################

//Ex1 ######################################################################################
char* UpsideDownTraingle(const int size);

//Ex2 ######################################################################################
typedef struct Node
{
    /* data */
    struct Node* next;
    struct Node* previous;
    int   data;
} Node;

typedef struct DoubleLinkedList{
    Node* head;
    Node* tail;
    size_t size;
} DoubleLinkedList;

DoubleLinkedList* createList();
void  AlterAt(DoubleLinkedList* list,const size_t index,const int value);
void  InsertBeg(DoubleLinkedList* list,const int value);
void  RemoveNode(DoubleLinkedList* dlist,Node * target_node);
void  PrintListForward(const Node* head);
void  PrintListBackward(const Node* Tail);


//EX3 ######################################################################################
char* FindRelativePath(const char* From,const char* To);

//EX4 ######################################################################################
#define FILTER_DIM 5
float filter_c_[FILTER_DIM][FILTER_DIM];
void convolution(float filter[][FILTER_DIM], float* input, float* output, unsigned int width, unsigned int height) {}

//EX X BONUS ###############################################################################
size_t JAYJAY(size_t K);