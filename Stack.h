#pragma once

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#pragma warning (disable : 4996)
//'fopen': This function or variable may be unsafe.Consider using fopen_s instead.To disable deprecation, use _CRT_SECURE_NO_WARNINGS.

#define ERR_FREE (stk_type*) 228
#define ASSERT_OK_B  if (this->Error()) this->Log (); else {
#define ASSERT_OK_E }if (this->Error()) this->Log ();
#define GG }}

enum stack_errors { 
	STK_GOOD	  = 0, 
	STK_NULL	  = 1, // Not actually on new versions
	STK_DEL		  = 2, 
	BUF_NULL	  = 3, 
	BAD_SIZE_CAP  = 4, 
	BAD_SIZE_ZERO = 5, 
	BAD_CAP		  = 6,
	STK_PETREL_B  = 7,
	STK_PETREL_E  = 8,
	BUF_PETREL_B  = 9,
	BUF_PETREL_E  = 10,
	DO_MEM_ERR	  = 11,
	NOT_CREATED   = 12,
	BAD_CREATE	  = 13,
	ERR_HASH_STK  = 14,
	ERR_HASH_BUF  = 15
};

typedef double stk_type;
typedef unsigned long long storm;

const stk_type STK_POISON = NAN;
const size_t   STK_RESIZE = 2;
const int STK_HASH = 2 * sizeof (storm) + 3 * sizeof (size_t) +
					sizeof (stk_type*) + sizeof (int) + 2 * sizeof (unsigned long long);

//! Bad ded idet v botalku ("v" = "b", "ku" didn`t fit). 
const storm STORMY_PETREL = 0xBADDED1DE7BB07A1;

// ToDo: Rename with _

struct Stack
{
	storm stormy_petrel_begin_ = STORMY_PETREL;

	stk_type  buffer_not_dynamic_[100] = {};
    stk_type* buffer_ = buffer_not_dynamic_;

	size_t	size_     = 0,
			capacity_ = 0,
		min_capacity_ = 0;
	stack_errors status_error_ = NOT_CREATED;

	unsigned long long stk_hash_ = 0;
	unsigned long long buf_hash_ = 0;

	storm stormy_petrel_end_ = STORMY_PETREL;

    Stack (size_t max_size);
   ~Stack ();

    void Push (stk_type elem);
    stk_type Pop ();

    void ResizeUp ();
    void ResizeDown ();

    stack_errors Error ();
    void Log ();

    void Poison (size_t num);

    unsigned long long CountHash (size_t num);
    void RecountHash ();
};

/*
Stack::Stack (size_t max_size);

void ~Stack ();

void Push (stk_type elem);

stk_type Pop ();

void StackResizeUp ();

void StackResizeDown ();

int Error ();

void Log ();

void StackPoison (stk_type* buffer, size_t num);

unsigned long long CountHash(char* buffer, size_t num);

void RecountHash ();
*/
