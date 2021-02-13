#include "Stack.h"

//! Don't use with destruct! Check stklog.txt, if it need.

#define ass(stk) ;if (stk.status_error != 0) {							\
						printf ("\n" "Error in line: %d.",__LINE__);	\
						return stk.status_error;}

int main()
{
	Stack stk = {};
	StackConstructor (&stk, 2);
	StackLog (&stk);

	StackPush (&stk, 1) ass (stk);
	StackPush (&stk, 2) ass (stk);
	StackPush (&stk, 3) ass (stk);
	StackLog  (&stk);

	StackPush (&stk, StackPop (&stk) + StackPop (&stk)) ass (stk);

	printf ("%lf", StackPop (&stk));
	StackPop (&stk) ass (stk);
	StackDestructor (&stk);
	StackDestructor (&stk);
	StackPop(&stk) ass (stk);
	return 0;
}
