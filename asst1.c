// #include <stdio.h>

// int main() {

//     char* array = "122333";
//     char* listOnes = "";
//     char* listTwos = "";
//     char* listThrees = "";

//     for(int i = 0; i <= sizeof(array); ++i){
//         if(array[i] != 1 || array[i] != 2 || array[i] != 3)
//             break;
//         else {
//             if(array[i] == 1)
//                 listOnes.push(array[i]);
//             else if(array[i] == 2)
//                 listTwos.push(array[i]);
//             else 
//                 listThrees.push(array[i]);
//         }
//     }

//     if(sizeof(listOnes) <= 3 && sizeof(listTwos) <= 3 && sizeof(listThrees) <= 3)
//         return 1;
    
//     return 0;
// }

// void method(char* array){
//     int counter = 0;
//     char* array = "122333";

//     for(int i = 0; i <= counter; ++i){
//         for(int j = 0; j <= )
//     }
// }

#include <stdio.h>
#include <stdlib.h>
#include "ass1.h"

// Function to create a new node with the given data
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    newNode->previous = NULL;
    return newNode;
}

// Function to create a new empty doubly linked list
DoubleLinkedList* createList() {
    DoubleLinkedList* newList = (DoubleLinkedList*)malloc(sizeof(DoubleLinkedList));
    if (newList == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newList->head = NULL;
    newList->tail = NULL;
    newList->size = 0;
    return newList;
}

// Function to insert a node at the beginning of the list
void InsertBeg(DoubleLinkedList* list, const int value) {
    Node* newNode = createNode(value);
    if (list->size == 0) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        newNode->next = list->head;
        list->head->previous = newNode;
        list->head = newNode;
    }
    list->size++;
}

// Function to remove a specified node from the list
void RemoveNode(DoubleLinkedList* list, Node* target_node) {
    if (list->size == 0 || target_node == NULL) {
        return; // Nothing to remove
    }

    if (target_node == list->head) {
        list->head = target_node->next;
    } else {
        target_node->previous->next = target_node->next;
    }

    if (target_node == list->tail) {
        list->tail = target_node->previous;
    } else {
        target_node->next->previous = target_node->previous;
    }

    free(target_node);
    list->size--;
}

// Function to alter the value at a specific index
void AlterAt(DoubleLinkedList* list, const size_t index, const int value) {
    if (index >= list->size) {
        fprintf(stderr, "Index out of range.\n");
        return;
    }

    Node* currentNode = list->head;
    size_t currentIndex = 0;

    while (currentNode != NULL) {
        if (currentIndex == index) {
            currentNode->data = value;
            break;
        }
        currentNode = currentNode->next;
        currentIndex++;
    }
}

// Function to print the list forward
void PrintListForward(const Node* head) {
    printf("Forward: ");
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

// Function to print the list backward
void PrintListBackward(const Node* tail) {
    printf("Backward: ");
    while (tail != NULL) {
        printf("%d ", tail->data);
        tail = tail->previous;
    }
    printf("\n");
}

// Function to insert a node at the end of the list
void InsertEnd(DoubleLinkedList* list, const int value) {
    Node* newNode = createNode(value);
    if (list->size == 0) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        newNode->previous = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}

int main() {
    DoubleLinkedList* list = createList();

    InsertBeg(list, 3);
    InsertBeg(list, 2);
    InsertBeg(list, 1);
    InsertEnd(list, 4);

    PrintListForward(list->head);
    PrintListBackward(list->tail);

    AlterAt(list, 2, 10);

    PrintListForward(list->head);
    PrintListBackward(list->tail);

    RemoveNode(list, list->head);
    RemoveNode(list, list->tail);

    PrintListForward(list->head);
    PrintListBackward(list->tail);

    return 0;
}
