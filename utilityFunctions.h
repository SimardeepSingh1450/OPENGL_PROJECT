#pragma once
// This is to get malloc
#include<stdlib.h>
// This is to get memset
#include<string.h>
#include<stdio.h>

// This is for file I/0 processing
#include <sys/stat.h>

// This below header file code for Memory Allocation and File I/O was taken from CakezYT - (Video-4 & 5)

// #############################################################################
//                           Defines
// #############################################################################
#define BIT(x) 1 << (x)
#define KB(x) ((unsigned long long)1024 * x)
#define MB(x) ((unsigned long long)1024 * KB(x))
#define GB(x) ((unsigned long long)1024 * MB(x))

// #############################################################################
//                           Bump Allocator
// #############################################################################
struct BumpAllocator
{
    size_t capacity;
    size_t used;
    char* memory;
};

BumpAllocator make_bump_allocator(size_t size)
{
    BumpAllocator ba = {};

    ba.memory = (char*)malloc(size);
    if (ba.memory)
    {
        ba.capacity = size;
        memset(ba.memory, 0, size); // Sets the memory to 0
    }
    else
    {
        printf("Failed to allocate Memory! \n");
        exit(0);
    }

    return ba;
}

char* bump_alloc(BumpAllocator* bumpAllocator, size_t size)
{
    char* result = nullptr;

    size_t allignedSize = (size + 7) & ~7; // This makes sure the first 4 bits are 0 
    if (bumpAllocator->used + allignedSize <= bumpAllocator->capacity)
    {
        result = bumpAllocator->memory + bumpAllocator->used;
        bumpAllocator->used += allignedSize;
    }
    else
    {
        printf("BumpAllocator is full \n");
        exit(0);
    }

    return result;
}

// #############################################################################
//                           File I/O
// #############################################################################
long get_file_size(const char* filePath)
{
    //printf("No filePath supplied!\n");

    long fileSize = 0;
    auto file = fopen(filePath, "rb");
    if (!file)
    {
        printf("Failed opening File: %s", filePath);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    fclose(file);

    return fileSize;
}

/*
* Reads a file into a supplied buffer. We manage our own
* memory and therefore want more control over where it
* is allocated
*/
char* read_file(const char* filePath, int* fileSize, char* buffer)
{
    //printf("No filePath supplied! \n");
    //printf("No fileSize supplied! \n");
    //printf("No buffer supplied! \n");

    *fileSize = 0;
    auto file = fopen(filePath, "rb");
    if (!file)
    {
        printf("Failed opening File: %s", filePath);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    *fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    memset(buffer, 0, *fileSize + 1);
    fread(buffer, sizeof(char), *fileSize, file);

    fclose(file);

    return buffer;
}

char* read_file2(const char* filePath, int* fileSize, BumpAllocator* bumpAllocator)
{
    char* file = nullptr;
    long fileSize2 = get_file_size(filePath);

    if (fileSize2)
    {
        char* buffer = bump_alloc(bumpAllocator, fileSize2 + 1);

        file = read_file(filePath, fileSize, buffer);
    }

    return file;
}