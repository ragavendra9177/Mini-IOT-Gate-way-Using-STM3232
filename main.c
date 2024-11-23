#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Memory Block Structure
typedef struct MemoryBlock {
    char data[20];               // Fixed-size data block (20 bytes)
    struct MemoryBlock* next;    // Pointer to the next block
} MemoryBlock;

// File Structure
typedef struct File {
    char name[50];               // File name
    MemoryBlock* head;           // Head pointer to memory blocks
} File;

// Function to create a new memory block
MemoryBlock* createMemoryBlock(const char* data, size_t size) {
    MemoryBlock* block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    if (!block) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    memset(block->data, 0, sizeof(block->data)); // Clear the block
    memcpy(block->data, data, size > 20 ? 20 : size); // Copy data into the block
    block->next = NULL;
    return block;
}

// Function to create a new file
File* createFile(const char* fileName) {
    File* file = (File*)malloc(sizeof(File));
    if (!file) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    memset(file->name, 0, sizeof(file->name)); // Clear the name
    strncpy(file->name, fileName, 50);         // Set the file name
    file->head = NULL;
    return file;
}

// Function to add data to file (handles splitting into blocks)
void addDataToFile(File* file, const char* data, size_t size) {
    size_t offset = 0;
    MemoryBlock* lastBlock = NULL;

    // Find the last block if blocks already exist
    if (file->head) {
        lastBlock = file->head;
        while (lastBlock->next) {
            lastBlock = lastBlock->next;
        }
    }

    // Split data into 20-byte chunks and add as memory blocks
    while (offset < size) {
        size_t chunkSize = (size - offset) > 20 ? 20 : (size - offset);
        MemoryBlock* block = createMemoryBlock(data + offset, chunkSize);
        offset += chunkSize;

        if (!file->head) {
            file->head = block;  // Set as head if no blocks exist
        } else {
            lastBlock->next = block; // Link the new block
        }
        lastBlock = block;
    }
}

// Function to serialize employee data into a single string
void serializeEmployeeData(char* buffer, size_t bufferSize, const char* name, const char* timestamp, int employee_id) {
    snprintf(buffer, bufferSize, "%s|%s|%d", name, timestamp, employee_id);
}

// Function to print file data
void printFileData(File* file) {
    printf("File: %s\n", file->name);
    MemoryBlock* temp = file->head;
    int blockNum = 1;
    while (temp) {
        printf("Block %d: %.*s\n", blockNum++, (int)sizeof(temp->data), temp->data);
        temp = temp->next;
    }
}

int main() {
    // Create a file
    File* file = createFile("EmployeeLogs");

    // Simulate employee data
    char serializedData[100];
    serializeEmployeeData(serializedData, sizeof(serializedData), "John", "2024-11-19 10:00", 101);
    addDataToFile(file, serializedData, strlen(serializedData));

    serializeEmployeeData(serializedData, sizeof(serializedData), "Alice", "2024-11-19 10:30", 102);
    addDataToFile(file, serializedData, strlen(serializedData));

    // Print the file data
    printFileData(file);

    // Clean up memory
    // (Freeing memory not implemented here for brevity)

    return 0;
}
