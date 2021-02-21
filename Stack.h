#pragma once

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ERR_FREE (ElemT*) 228
#define ASSERT_OK_B if (this->Error()) this->Log (); else 
#define ASSERT_OK_E if (this->Error()) this->Log ();
#define STK_POISON NAN

enum stack_errors { 
	STK_GOOD	  = 0, 
	STK_NULL	  = 1,
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

typedef unsigned long long storm;
typedef unsigned long long hash_t;

const size_t   STK_RESIZE = 2;
const int STK_HASH = 2 * sizeof (storm) + 3 * sizeof (size_t) +
					 sizeof (void*) + sizeof (int) + 2 * sizeof (unsigned long long);

//! Bad ded idet v botalku ("v" = "b", "ku" didn`t fit). 
const storm STORMY_PETREL = 0xBADDED1DE7BB07A1;

template <typename ElemT>
class Stack
{
	storm stormy_petrel_begin_ = STORMY_PETREL;

	ElemT  buffer_not_dynamic_[100] = {};
    ElemT* buffer_ = buffer_not_dynamic_;

	size_t	size_     = 0,
			capacity_ = 0,
		min_capacity_ = 0;
	stack_errors status_error_ = NOT_CREATED;

	hash_t stk_hash_ = 0;
	hash_t buf_hash_ = 0;

	storm stormy_petrel_end_ = STORMY_PETREL;

public:
    Stack (size_t max_size);
   ~Stack ();

    void Push (ElemT elem);
    ElemT Pop ();
    void Log ();

private:

    void ResizeUp ();
    void ResizeDown ();

    stack_errors Error ();

    void  FillPoison (size_t num);
    hash_t CountHash (size_t num);
    void RecountHash ();

    double Poison (double*);
    int    Poison (int*);
    char   Poison (char*);
    void*  Poison (void**);

    void Print (double elem, FILE* stream = stdout);
    void Print (int    elem, FILE* stream = stdout);
    void Print (char   elem, FILE* stream = stdout);
    void Print (void*  elem, FILE* stream = stdout);
};
