#include "Stack.h"

Stack::Stack (size_t max_size):

    stormy_petrel_begin_ (STORMY_PETREL),
    buffer_not_dynamic_ (),
	buffer_ (buffer_not_dynamic_),     

    size_         (0),
	capacity_     (max_size),
    min_capacity_ (max_size),
    status_error_ (STK_GOOD),

    stk_hash_     (0),
	buf_hash_     (0),
    stormy_petrel_end_   (STORMY_PETREL)

{
	// ask : repeat constructor
    // ask : Stack* stk = nullptr

    /*
	stk_type* buf = (stk_type*) malloc ((max_size + 2) * sizeof (stk_type));
	
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
	Poison (max_size);

	RecountHash();

	return;
}

Stack::~Stack ()
{
	ASSERT_OK_B{
	
//	stk->buffer_ -= 1;

	Poison (capacity_);// + 2);

//	free (stk->buffer_);

//	stk->buffer_ = ERR_FREE;
	size_ = 0;
	capacity_ = 0;
	status_error_ = NOT_CREATED;
	buf_hash_ = 0;
	stk_hash_ = 0;
	GG
	return;
}

void Stack::Poison (size_t num)
{	
	for (size_t element = 0; element < num; element++)
		buffer_[element] = STK_POISON;

	return;
}

void Stack::Push (stk_type elem)
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

stk_type Stack::Pop ()
{
	ASSERT_OK_B
	
	if (size_ == 0)
	{
		status_error_ = BAD_SIZE_ZERO;
		Log();
		return STK_POISON;
	}
	else
	{
        /*
		if (stk->capacity > stk->min_capacity &&
			stk->capacity >= STK_RESIZE * STK_RESIZE * stk->size)
			StackResizeDown (stk);

		if (stk->status_error_ == DO_MEM_ERR) return STK_POISON;
        */

		stk_type popped = buffer_[--(size_)];
		*(buffer_ + size_) = STK_POISON;
		
		RecountHash();

		if (Error()) Log();
		return popped;
	GG

	return STK_POISON;
}
/*
void Stack::ResizeUp ()
{
	ASSERT_OK_B

	stk_type* buf = stk->buffer_ - 1;

	buf = (stk_type*) realloc (buf, sizeof (stk_type) * (stk->capacity * STK_RESIZE + 2));

	if (!buf)
	{
		stk->status_error_ = DO_MEM_ERR;
		StackLog (stk);
		return;
	}

	(stk->capacity) *= STK_RESIZE;

	*((storm*)  buf)					  = STORMY_PETREL;
	*((storm*) (buf + stk->capacity + 1)) = STORMY_PETREL;

	StackPoison (buf + stk->size + 1, stk->capacity - stk->size);

	stk->buffer_ = buf + 1;

	RecountHash (stk);

	ASSERT_OK_E

	return;
}

void StackResizeDown (Stack* stk)
{
	ASSERT_OK_B

	stk_type* buf = stk->buffer_ - 1;

	if (!buf)
	{
		stk->status_error_ = DO_MEM_ERR;
		StackLog (stk);
		return;
	}

	*(buf + stk->capacity + 1) = STK_POISON;

	if (stk->capacity >= STK_RESIZE * stk->size && 
		stk->capacity >= STK_RESIZE * stk->min_capacity)
			buf = (stk_type*) realloc (buf, 
									  sizeof (stk_type) * (stk->capacity / STK_RESIZE + 2));

	stk->capacity /= STK_RESIZE;
	*((storm*) (buf + stk->capacity + 1)) = STORMY_PETREL;
	stk->buffer_ = buf + 1;

	RecountHash (stk);

	ASSERT_OK_E

	return;
}
*/

stack_errors Stack::Error ()
{
    /*
	if (!stk)
		return STK_NULL;
    */
	
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

void Stack::Log()
{
	FILE* file = fopen ("stklog.txt", "a");

	struct tm* mytime = nullptr;
	time_t alltime = 0;
	time (&alltime);
	mytime = localtime (&alltime);
	fprintf (file, "\n%s", asctime (mytime));

	switch (status_error_)
	{
        #define fast_ret fclose (file); return

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
		fprintf (file, "[%d] = %lf\n", element, buffer_[element]);

	fprintf (file, "\n");
	fclose  (file);

	return;
}

//ToDo: hash_t
unsigned long long Stack::CountHash (size_t num)
{
	const int shift = 5;
	unsigned long long ans = 0;
	char buf = 0;
    char* buffer_r = (char*) buffer_;

	for (size_t count = 0; count < num; count++)
	{
		ans += buffer_r[count];
		buf = ans >> (sizeof (unsigned long long) - shift);
		ans = (ans << shift) + buf;
	}

	return ans;
}

void Stack::RecountHash ()
{
	stk_hash_ = 0;
	buf_hash_ = 0;
	
	stk_hash_ = CountHash (stk_hash_);
	buf_hash_ = CountHash (capacity_ * sizeof (stk_type));

	return;
}
