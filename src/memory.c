
/*
 * Copyright (C) 2019 Derric Lyns [derriclyns@gmail.com]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * description : this file provides memory functions for safe allocation and freeing
 *               without a worry over stack and heap memory initialization
 *
 */


/** @file memory.c
 *
 *  @brief      C library for managed generic memory functions
 *  @details    Macros and functions to allocate, deallocate and free memory
 *
 *  @authors
 *          - Derric Lyns
 */

#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CUTILS_MEMORY_DEBUG

/* heap information */
typedef struct HeapInfo{
    struct HeapInfo *next;
    struct HeapInfo *prev;
    void* ptr;
    size_t size;
}HeapInfo;

/* memory usage information */
typedef struct MemInfo{
    unsigned long allocations_count;
    unsigned long deallocations_count;
    size_t stack_memory_added;
    size_t heap_memory_added;
    size_t heap_memory_freed;
}MemInfo;


/* memory data */
static MemInfo memory_data;
static HeapInfo* imp_heap = NULL;

/* stack init */
#ifdef CUTILS_MEMORY_DEBUG
int _mem_stack_init(void* ptr, size_t size, unsigned line, const char* file)
#else
int _mem_stack_init(void* ptr, size_t size)
#endif // CUTILS_MEMORY_DEBUG
{
    int init_success = 0;

    if( ptr != NULL ){
        ptr = memset(ptr, 0, size);
        if(ptr != NULL){
            memory_data.stack_memory_added += size;
            init_success = 1;
#ifdef CUTILS_MEMORY_DEBUG
            fprintf(stderr,"\n[cutils][mem_declare][ called at file %s line %lu ]", file, (long unsigned)line);
#endif // CUTILS_MEMORY_DEBUG
        }
    }

    return init_success;
}

/* malloc substitute */
#ifdef CUTILS_MEMORY_DEBUG
void* _mem_alloc(size_t size, unsigned line , const char* file)
#else
void* _mem_alloc(size_t size)
#endif // CUTILS_MEMORY_DEBUG
{
    void* _memptr = NULL;

    if(size > 0){
        _memptr = malloc(size);

        if(_memptr != NULL){
            memory_data.allocations_count++;
            memory_data.heap_memory_added += size;

            if(imp_heap == NULL){
                imp_heap = (HeapInfo*)malloc(sizeof(HeapInfo));
                imp_heap->ptr = _memptr;
                imp_heap->size = size;
                imp_heap->next = NULL;
                imp_heap->prev = NULL;
            }
            else{
                HeapInfo* heap_node = (HeapInfo*)malloc(sizeof(HeapInfo));
                heap_node->prev = imp_heap;
                heap_node->next = NULL;
                heap_node->ptr = _memptr;
                heap_node->size = size;
                imp_heap->next = heap_node;
                imp_heap = heap_node;
            }
#ifdef CUTILS_MEMORY_DEBUG
            fprintf(stderr,"\n[cutils][mem_alloc][ called at file %s line %lu ]", file, (long unsigned)line);
#endif // CUTILS_MEMORY_DEBUG
        } else {
            fprintf(stderr,"\n[cutils][mem_alloc][error: Out of memory]");
        }
    }

    return _memptr;
}

/* calloc substitute */
#ifdef CUTILS_MEMORY_DEBUG
void* _mem_calloc(int num, size_t size, unsigned line, const char* file)
#else
void* _mem_calloc(int num, size_t size)
#endif // CUTILS_MEMORY_DEBUG
{
    void* _memptr = NULL;

    if(size > 0){
        _memptr = calloc(num, size);
        if(_memptr != NULL){
            memory_data.allocations_count++;
            memory_data.heap_memory_added += size;

            if(imp_heap == NULL){
                imp_heap = (HeapInfo*)malloc(sizeof(HeapInfo));
                imp_heap->ptr = _memptr;
                imp_heap->size = size;
                imp_heap->next = NULL;
                imp_heap->prev = NULL;
            }
            else{
                HeapInfo* heap_node = (HeapInfo*)malloc(sizeof(HeapInfo));
                heap_node->prev = imp_heap;
                heap_node->next = NULL;
                heap_node->ptr = _memptr;
                heap_node->size = size;
                imp_heap->next = heap_node;
                imp_heap = heap_node;
            }
#ifdef CUTILS_MEMORY_DEBUG
            fprintf(stderr,"\n[cutils][mem_calloc][ called at file %s line %lu ]", file, (long unsigned)line);
#endif // CUTILS_MEMORY_DEBUG
        } else {
            fprintf(stderr,"\n[cutils][mem_calloc][error: Out of memory]");
        }
    }

    return _memptr;
}

/* realloc substitute */
#ifdef CUTILS_MEMORY_DEBUG
void* _mem_realloc(void *ptr, size_t size, unsigned line, const char* file)
#else
void* _mem_realloc(void *ptr, size_t size)
#endif // CUTILS_MEMORY_DEBUG
{
    void* _memptr = ptr;

    if(size > 0){
        _memptr = realloc(_memptr, size);
        if(_memptr != NULL){
            memory_data.allocations_count++;
            memory_data.heap_memory_added += size;

            if(imp_heap == NULL){
                imp_heap = (HeapInfo*)malloc(sizeof(HeapInfo));
                imp_heap->ptr = _memptr;
                imp_heap->size = size;
                imp_heap->next = NULL;
                imp_heap->prev = NULL;
            }
            else{
                HeapInfo* heap_node = (HeapInfo*)malloc(sizeof(HeapInfo));
                heap_node->prev = imp_heap;
                heap_node->next = NULL;
                heap_node->ptr = _memptr;
                heap_node->size = size;
                imp_heap->next = heap_node;
                imp_heap = heap_node;
            }
#ifdef CUTILS_MEMORY_DEBUG
            fprintf(stderr,"\n[cutils][mem_realloc][ called at file %s line %lu ]", file, (long unsigned)line);
#endif // CUTILS_MEMORY_DEBUG
        }else{
            fprintf(stderr,"\n[cutils][mem_realloc][error: Out of memory]");
        }
    }

    return _memptr;
}

/* stdlib free substitute */
#ifdef CUTILS_MEMORY_DEBUG
int _mem_free(void* ptr, unsigned line, const char* file)
#else
int _mem_free(void* ptr)
#endif // CUTILS_MEMORY_DEBUG
{
    int free_ret = 0;
    HeapInfo* root = imp_heap;
    HeapInfo* freenode = NULL;

    while(root != NULL){
        if(root->ptr == ptr){
            free_ret = 1;
            freenode = root;
            break;
        }
        root = root->prev;
    }

    if(free_ret){
        memory_data.deallocations_count++;
        memory_data.heap_memory_freed += root->size;

        HeapInfo *prev_node = freenode->prev;
        HeapInfo *next_node = freenode->next;

        if(prev_node != NULL){
            prev_node->next = next_node;
        }

        if(next_node != NULL){
            next_node->prev = prev_node;
        }

        if(imp_heap == freenode){
            imp_heap = prev_node;
        }

        free(freenode);
    }

#ifdef CUTILS_MEMORY_DEBUG
        fprintf(stderr,"\n[cutils][mem_free][ called at file %s line %lu ]", file, (long unsigned)line);
#endif // CUTILS_MEMORY_DEBUG

    free(ptr);
    return free_ret;
}


void mem_dump()
{
#ifdef CUTILS_MEMORY_DEBUG
    fprintf(stdout,"\n[cutils][mem_dump][info: stack size used %lu bytes ]", (long unsigned)memory_data.stack_memory_added);
    fprintf(stdout,"\n[cutils][mem_dump][info: number of dynamic allocations %lu ]", (long unsigned)memory_data.allocations_count);
	fprintf(stdout,"\n[cutils][mem_dump][info: number of dynamic deallocations %lu ]", (long unsigned)memory_data.deallocations_count);
    fprintf(stdout,"\n[cutils][mem_dump][info: heap_size_added %lu bytes ]", (long unsigned)memory_data.heap_memory_added);
    fprintf(stdout,"\n[cutils][mem_dump][info: heap_size_freed %lu bytes ]", (long unsigned)memory_data.heap_memory_freed);
#endif
}
