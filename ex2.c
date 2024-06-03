#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define MEM_SIZE 1048576 // 1MB
#define PAGE_SIZE 4096   // 4KB
#define NUM_FRAMES 256
#define TLB_SIZE 16
#define MAX_PROCESSES 30

// Structures
typedef struct
{
    int pageNumber;
    int frameNumber;
    int age;
} TLBEntry;

typedef struct
{
    int processID;
    TLBEntry tlb[TLB_SIZE];
    int *pageTable;
    int numPages;
} Process;

Process processes[MAX_PROCESSES];

typedef struct
{
    int free; // 1 for free, 0 otherwise
} Bitmap;

Process *getProcess(int pid);
void requestMemory(Process *process, int size, Bitmap *bitmap);
void terminateProcess(Process *process, Bitmap *bitmap);
void readAddress(Process *process, int logicalAddress, Bitmap *bitmap);
void updateTLB(Process *process, int pageNumber, int frameNumber);
void printBitmapState(Bitmap *bitmap);
void processFile(const char *filename);

Bitmap bitmap[NUM_FRAMES];

int main()
{
    char filename[10];

    if (scanf("%10s", filename) == 1)
    {
        processFile(filename);
    }

    return 0;
}

void processFile(const char *filename)
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        bitmap[i].free = 1;
    }
    memset(processes, 0, sizeof(processes));

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        return;
    }

    char line[256];
    int pid, arg;
    char operation[4];

    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "%d %s %d", &pid, operation, &arg) == 3 ||
            (sscanf(line, "%d %s", &pid, operation) == 2 && strcmp(operation, "TRM") == 0))
        {
            Process *process = getProcess(pid);
            if (process == NULL)
            {
                fprintf(stderr, "Error: Maximum number of processes reached or no available slot.\n");
                continue;
            }
            if (strcmp(operation, "RQ") == 0)
            {
                requestMemory(process, arg, bitmap);
            }
            else if (strcmp(operation, "TRM") == 0)
            {
                terminateProcess(process, bitmap);
                printf("P(id = %d) terminated...\n\n", pid);
                printBitmapState(bitmap);
            }
            else if (strcmp(operation, "RD") == 0)
            {
                readAddress(process, arg, bitmap);
            }
        }
    }
    fclose(file);
    return;
}

Process *getProcess(int pid)
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        if (processes[i].processID == pid)
        {
            return &processes[i];
        }
    }

    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        if (processes[i].processID == 0)
        { // Assuming 0 means unused
            processes[i].processID = pid;
            return &processes[i];
        }
    }

    return NULL;
}

void requestMemory(Process *process, int size, Bitmap *bitmap)
{
    int numFramesNeeded = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    process->numPages = numFramesNeeded;
    process->pageTable = (int *)malloc(numFramesNeeded * sizeof(int));

    for (int i = 0; i < numFramesNeeded; i++)
    {
        process->pageTable[i] = -1;
    }
    for (int i = 0; i < TLB_SIZE; i++)
    {
        process->tlb[i].pageNumber = -1;
        process->tlb[i].age = 0;
    }

    // Allocate frames
    for (int i = 0, frame = 0; i < numFramesNeeded && frame < NUM_FRAMES; frame++)
    {
        if (bitmap[frame].free)
        {
            process->pageTable[i++] = frame;
            bitmap[frame].free = 0;
        }
    }

    printf("P(id = %d) allocated %d pages\n", process->processID, numFramesNeeded);
}

void terminateProcess(Process *process, Bitmap *bitmap)
{
    for (int i = 0; i < process->numPages; i++)
    {
        int frameNumber = process->pageTable[i];
        if (frameNumber >= 0 && frameNumber < NUM_FRAMES)
        {
            bitmap[frameNumber].free = 1;
        }
    }

    free(process->pageTable);
    process->pageTable = NULL;
    process->numPages = 0;
}

void readAddress(Process *process, int logicalAddress, Bitmap *bitmap)
{
    int pageNumber = logicalAddress / PAGE_SIZE;
    int offset = logicalAddress % PAGE_SIZE;
    int frameNumber = -1;
    int tlbHit = 0;

    int oldestAge = -1;
    for (int i = 0; i < TLB_SIZE; i++)
    {
        process->tlb[i].age++;
        if (process->tlb[i].pageNumber == pageNumber)
        {
            frameNumber = process->tlb[i].frameNumber;
            process->tlb[i].age = 0;
            tlbHit = 1;
        }
        if (process->tlb[i].age > oldestAge)
        {
            oldestAge = process->tlb[i].age;
        }
    }

    if (!tlbHit)
    {
        frameNumber = process->pageTable[pageNumber];
        updateTLB(process, pageNumber, frameNumber);
    }

    int physicalAddress = frameNumber * PAGE_SIZE + offset;
    printf("P(id = %d) %d (%s)\n", process->processID, physicalAddress, tlbHit ? "hit" : "miss");
}

void updateTLB(Process *process, int pageNumber, int frameNumber)
{
    int oldestAgeIndex = -1;
    int oldestAge = -1;

    for (int i = 0; i < TLB_SIZE; i++)
    {
        if (process->tlb[i].pageNumber == -1)
        {
            oldestAgeIndex = i;
            break;
        }
        if (process->tlb[i].age > oldestAge)
        {
            oldestAge = process->tlb[i].age;
            oldestAgeIndex = i;
        }
    }

    process->tlb[oldestAgeIndex].pageNumber = pageNumber;
    process->tlb[oldestAgeIndex].frameNumber = frameNumber;
    process->tlb[oldestAgeIndex].age = 0;

    for (int i = 0; i < TLB_SIZE; i++)
    {
        if (i != oldestAgeIndex)
        {
            process->tlb[i].age++;
        }
    }
}

void printBitmapState(Bitmap *bitmap)
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        printf("%d", bitmap[i].free);
        if ((i + 1) % 32 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}
