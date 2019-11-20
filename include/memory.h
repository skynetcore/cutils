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

#ifndef _CUTILS_MEMORY_H_
#define _CUTILS_MEMORY_H_
#define CUTILS_MEMORY_DEBUG

/** @file cutils.h
 *
 *  @brief      C library for managed generic memory functions
 *  @details    Macros and functions to allocate, deallocate and free memory
 *              these functions are thread safe
 *
 */


#include <string.h>


#ifdef CUTILS_MEMORY_DEBUG
/* declare in stack a struct/variable with pointers uninitialized */
#define mem_declare(type, v) type v; _mem_stack_init(&v, sizeof(type), __LINE__, __FILE__)
/* allocate in heap struct/variables safely */
#define mem_alloc(s) _mem_alloc(s,__LINE__, __FILE__)
/* allocate in heap struct/variables safely and initialize them*/
#define mem_calloc(n,s) _mem_calloc(n,s,__LINE__, __FILE__)
/* re allocate in heap struct/variables safely */
#define mem_realloc(p,s) _mem_realloc(p,s,__LINE__,__FILE__)
/* free heap struct/variables safely */
#define mem_free(p) _mem_free(p, __LINE__, __FILE__)
/* get struct/variable size safely */
#define mem_size(p) _mem_size(p, __LINE__, __FILE__)
#else
/* declare in stack a struct/variable with pointers uninitialized */
#define mem_declare(type, v) type v; _mem_stack_init(&v, sizeof(type))
/* allocate in heap struct/variables safely */
#define mem_alloc(s) _mem_alloc(s)
/* allocate in heap struct/variables safely and initialize them*/
#define mem_calloc(n,s) _mem_calloc(n, s)
/* re allocate in heap struct/variables safely */
#define mem_realloc(p,s) _mem_realloc(p,s)
/* free heap struct/variables safely */
#define mem_free(p) _mem_free(p)
/* get struct/variable size safely */
#define mem_size(p) _mem_size(p)
#endif


/*
 * function:  _mem_stack_init
 * initializes variable in stack before use
 *
 *  ptr: the pointer which holds address of static allocated memory
 *  size: amount of memory being held
 *  line: line number of the file
 *  file: file name of the source
 *
 *  returns: 1 on success and
 *           0 on failure in initializing stack
 */
#ifdef CUTILS_MEMORY_DEBUG
int _mem_stack_init(void* ptr, size_t size, unsigned line, const char* file);
#else
int _mem_stack_init(void* ptr, size_t size);
#endif // CUTILS_MEMORY_DEBUG

/*
 * function:  _mem_alloc
 * allocates memory safely and adds up
 *
 *  size: amount of memory required
 *  line: line number of the file
 *  file: file name of the source
 *
 *  returns: pointer to memory on successfull allocation and
 *           NULL on failure to allocate memory
 */
#ifdef CUTILS_MEMORY_DEBUG
void* _mem_alloc(size_t size, unsigned line , const char* file);
#else
void* _mem_alloc(size_t size);
#endif // CUTILS_MEMORY_DEBUG

/*
 * function:  _mem_realloc
 * reallocates memory safely/reuses the memory block and adds up to total used heap
 *
 *  ptr: pointer to previous reallocated block
 *  size: amount of memory required
 *  line: line number of the file
 *  file: file name of the source
 *
 *  returns: pointer to memory on successfull reallocation and
 *           NULL on failure to reallocate
 */
#ifdef CUTILS_MEMORY_DEBUG
void* _mem_realloc(void *ptr, size_t size, unsigned line, const char* file);
#else
void* _mem_realloc(void *ptr, size_t size);
#endif // CUTILS_MEMORY_DEBUG

/*
 * function:  _mem_calloc
 * allocates memory safely/reuses the memory block and adds up to total used heap
 * initializes to zero
 *
 *  num: number of size blocks that needs to be allcated
 *  size: amount a memory block requires
 *  line: line number of the file
 *  file: file name of the source
 *
 *  returns: pointer to memory on successfull reallocation and
 *           NULL on failure to reallocate
 */
#ifdef CUTILS_MEMORY_DEBUG
void* _mem_calloc(int num, size_t size, unsigned line, const char* file);
#else
void* _mem_calloc(int num, size_t size);
#endif // CUTILS_MEMORY_DEBUG

/*
 * function:  _mem_free
 * frees up used memory
 *
 *  ptr: the memory that was allocated earlier
 *  line: line number of the file
 *  file: file name of the source
 *
 *  returns: the function return 1 on success and
 *           returns 0 if failed
 */
#ifdef CUTILS_MEMORY_DEBUG
int _mem_free(void* ptr, unsigned line, const char* file);
#else
int _mem_free(void* ptr);
#endif // CUTILS_MEMORY_DEBUG


/*
 * function:  mem_dump
 * dumps current memory information onto console
 *
 */
void mem_dump();

#endif // IMPLIB_H
