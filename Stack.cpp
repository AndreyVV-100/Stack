#include "Stack.h"

template <typename ElemT>
Stack <ElemT> :: Stack (size_t max_size):

	capacity_     (max_size),
    min_capacity_ (max_size),
    status_error_ (STK_GOOD)

{
    /*
	ElemT* buf = (ElemT*) malloc ((max_size + 2) * sizeof (ElemT));
	
	if (!buf)
	{
		status_error_ = DO_MEM_ERR;
		Log();
		return;
	}
	*/
	*((storm*)  buffer_)				 = STORMY_PETREL;
	*((storm*) (buffer_ + max_size + 1)) = STORMY_PETREL;

    buffer_++;
	FillPoison (max_size);

	RecountHash();

	return;
}

template <typename ElemT>
Stack <ElemT> :: ~Stack ()
{
	ASSERT_OK_B
	{
    //	stk->buffer_ -= 1;

        FillPoison (capacity_);// + 2);

    //	free (stk->buffer_);

    //	stk->buffer_ = ERR_FREE;
        size_ = 0;
        capacity_ = 0;
        status_error_ = NOT_CREATED;
        buf_hash_ = 0;
        stk_hash_ = 0;
    }
	return;
}

template <typename ElemT>
void Stack <ElemT> :: FillPoison (size_t num)
{	
	for (size_t element = 0; element < num; element++)
		buffer_[element] = Poison (buffer_);

	return;
}

template <typename ElemT>
void Stack <ElemT> :: Push (ElemT elem)
{
	ASSERT_OK_B

	if (size_ == capacity_)
    {
        status_error_ = DO_MEM_ERR;
        Log();
        return;
    }

	if (status_error_ == DO_MEM_ERR) return;

	*(buffer_ + (size_)++) = elem;

	RecountHash();

	ASSERT_OK_E

	return;
}

template <typename ElemT>
ElemT Stack <ElemT> :: Pop ()
{
	ASSERT_OK_B
	{
        if (size_ == 0)
        {
            status_error_ = BAD_SIZE_ZERO;
            Log();
            return Poison (buffer_);
        }

        else
        {
            /*
            if (stk->capacity > stk->min_capacity &&
                stk->capacity >= STK_RESIZE * STK_RESIZE * stk->size)
                StackResizeDown (stk);

            if (stk->status_error_ == DO_MEM_ERR) return Poison (buffer_);
            */

            ElemT popped = buffer_[--(size_)];
            *(buffer_ + size_) = Poison (buffer_);
            
            RecountHash();

            if (Error()) Log();
            return popped;
        }
    }

	return Poison (buffer_);
}

/*
void Stack::ResizeUp ()
{
	ASSERT_OK_B

	ElemT* buf = stk->buffer_ - 1;

	buf = (ElemT*) realloc (buf, sizeof (ElemT) * (stk->capacity * STK_RESIZE + 2));

	if (!buf)
	{
		stk->status_error_ = DO_MEM_ERR;
		StackLog (stk);
		return;
	}

	(stk->capacity) *= STK_RESIZE;

	*((storm*)  buf)					  = STORMY_PETREL;
	*((storm*) (buf + stk->capacity + 1)) = STORMY_PETREL;

	StackFillPoison (buf + stk->size + 1, stk->capacity - stk->size);

	stk->buffer_ = buf + 1;

	RecountHash (stk);

	ASSERT_OK_E

	return;
}

void StackResizeDown (Stack* stk)
{
	ASSERT_OK_B

	ElemT* buf = stk->buffer_ - 1;

	if (!buf)
	{
		stk->status_error_ = DO_MEM_ERR;
		StackLog (stk);
		return;
	}

	*(buf + stk->capacity + 1) = Poison (buffer_);

	if (stk->capacity >= STK_RESIZE * stk->size && 
		stk->capacity >= STK_RESIZE * stk->min_capacity)
			buf = (ElemT*) realloc (buf, 
									  sizeof (ElemT) * (stk->capacity / STK_RESIZE + 2));

	stk->capacity /= STK_RESIZE;
	*((storm*) (buf + stk->capacity + 1)) = STORMY_PETREL;
	stk->buffer_ = buf + 1;

	RecountHash (stk);

	ASSERT_OK_E

	return;
}
*/

template <typename ElemT>
stack_errors Stack <ElemT> :: Error ()
{
	if (!this)
		return STK_NULL;
	
    if (stormy_petrel_begin_ != STORMY_PETREL)
		return status_error_ = STK_PETREL_B;

	if (stormy_petrel_end_ != STORMY_PETREL)
		return status_error_ = STK_PETREL_E;
	
	if (buffer_ == ERR_FREE)
		return status_error_ = STK_DEL;
	
	if (!buffer_)
		return status_error_ = BUF_NULL;
	
	if (size_ > capacity_)
		return status_error_ = BAD_SIZE_CAP;

	if (capacity_ < min_capacity_)
		return status_error_ = BAD_CAP;
	
	if (*((storm*) (buffer_ - 1)) != STORMY_PETREL)
		return status_error_ = BUF_PETREL_B;

	if (*((storm*) (buffer_ + capacity_)) != STORMY_PETREL)
		return status_error_ = BUF_PETREL_E;

	if (status_error_ != 0)
		return status_error_;

	storm hash1 = stk_hash_;
	storm hash2 = buf_hash_;
	RecountHash();

	if (stk_hash_ != hash1)
		return status_error_ = ERR_HASH_STK;

	if (buf_hash_ != hash2)
		return status_error_ = ERR_HASH_BUF;
	
	return STK_GOOD;
}

template <typename ElemT>
void Stack <ElemT> :: Log()
{
	FILE* file = fopen ("stklog.txt", "a");

	struct tm* mytime = nullptr;
	time_t alltime = 0;
	time (&alltime);
	mytime = localtime (&alltime);
	fprintf (file, "\n%s", asctime (mytime));

    #define fast_ret fclose (file); return

    if (!this)
	{
		fprintf (file, "Stack [STK_NULL] : The address of stack is null.\n\n");
        fast_ret;
	}

	switch (status_error_)
	{
        case BUF_NULL:
            fprintf (file, "Stack [BUF_NULL] : The address of buffer_ is nullptr.\n");
            fast_ret;

        case STK_PETREL_B:
            fprintf (file, "Stack [STK_PETREL_B] : The begin stormy petrel in stack was damaged.\n");
            fast_ret;

        case STK_PETREL_E:
            fprintf (file, "Stack [STK_PETREL_E] : The end stormy petrel in stack was damaged.\n");
            fast_ret;

        case STK_DEL:
            fprintf (file, "Stack [STK_DEL] : Stack was deleted.\n");
            fast_ret;

        case DO_MEM_ERR:
            fprintf (file, "Stack [DO_MEM_ERR] : The program can`t pick out new memory.\n");
            fast_ret;

        case NOT_CREATED:
            fprintf (file, "Stack [NOT_CREATED] : Attempt to manipulation with stack which wasn`t constructed.\n");
            fast_ret;

        #undef fast_ret

		case STK_GOOD:
			fprintf (file, "Stack [STK_GOOD] : Nice!\n");
			break;

		case BAD_SIZE_CAP:
			fprintf (file, "Stack [BAD_SIZE_CAP] : The size_ of stack is more then capacity.\n");
			break;

		case BAD_SIZE_ZERO:
			fprintf (file,  "Stack [BAD_SIZE_ZERO] : StackPop was called when size_ was zero.\n");
			break;

		case BAD_CAP:
			fprintf (file, "Stack [BAD_CAP] : The capacity of stack is less then minimal capacity.\n");
			break;

		case BUF_PETREL_B:
			fprintf (file, "Stack [BUF_PETREL_B] : The begin stormy petrel in buffer_ was damaged.\n");
			break;

		case BUF_PETREL_E:
			fprintf (file, "Stack [BUF_PETREL_E] : The end stormy petrel in buffer_ was damaged.");
			break;

		case BAD_CREATE:
			fprintf (file, "Stack [BAD_CREATE] : Trying to construct stack which already not empty.\n");
			break;

		case ERR_HASH_STK:
			fprintf (file, "Stack [ERR_HASH_STK] : Data in stack was changed illegally.");
			break;

		case ERR_HASH_BUF:
			fprintf (file, "Stack [ERR_HASH_BUF] : Data in buffer_ was changed illegally.");
			break;

		default:
			fprintf (file, "Stack [UNK] : Unknown error.\n");
			break;
	};

	fprintf (file, "size = %lu\n" "capacity = %lu\n" "min_capacity = %lu\n" "buffer_:\n", 
			        size_,         capacity_,         min_capacity_);

	/*
	fprintf (file, "begin buffer_ petrel = %llX\n" "end buffer_ petrel   = %llX\n",
		*((storm*) (stk->buffer_ - 1)),
		*((storm*) (stk->buffer_ + stk->capacity)));
	*/

	for (int element = 0; element < capacity_; element++)
    {
		fprintf (file, "[%d] = ", element);
        Print   (buffer_[element], file);
        fprintf (file, "\n");
    }

	fprintf (file, "\n");
	fclose  (file);

	return;
}

template <typename ElemT>
hash_t Stack <ElemT> :: CountHash (size_t num)
{
	const int shift = 5;
	hash_t ans = 0;
	char buf = 0;
    char* buffer = (char*) buffer_;

	for (size_t count = 0; count < num; count++)
	{
		ans += buffer[count];
		buf = ans >> (sizeof (unsigned long long) - shift);
		ans = (ans << shift) + buf;
	}

	return ans;
}

template <typename ElemT>
void Stack <ElemT> ::RecountHash ()
{
	stk_hash_ = 0;
	buf_hash_ = 0;
	
	stk_hash_ = CountHash (stk_hash_);
	buf_hash_ = CountHash (capacity_ * sizeof (ElemT));

	return;
}

template <typename ElemT>
double Stack <ElemT> :: Poison (double*) {return NAN;}

template <typename ElemT>
int    Stack <ElemT> :: Poison (int*)    {return 0;}

template <typename ElemT>
char   Stack <ElemT> :: Poison (char*)   {return '\0';}

template <typename ElemT>
void*  Stack <ElemT> :: Poison (void**)  {return nullptr;}

template <typename ElemT>
void Stack <ElemT> :: Print (double elem, FILE* stream) {fprintf (stream, "%lf", elem);}

template <typename ElemT>
void Stack <ElemT> :: Print (int    elem, FILE* stream) {fprintf (stream, "%d",  elem);}

template <typename ElemT>
void Stack <ElemT> :: Print (char   elem, FILE* stream) {fprintf (stream, "%c",  elem);}

template <typename ElemT>
void Stack <ElemT> :: Print (void*  elem, FILE* stream) {fprintf (stream, "%p",  elem);}
