#include "Stack.h"

//! Don't use with destruct! Check stklog.txt, if it need.

#define ass(stk) ;if (stk.status_error != 0) {							\
						printf ("\n" "Error in line: %d.",__LINE__);	\
						return stk.status_error;}

int main()
{
	Stack <double> stk (98);
	stk.Log();

	stk.Push (1); // ass (stk);
	stk.Push (2); // ass (stk);
	stk.Push (3); // ass (stk);
	stk.Log();

	stk.Push (stk.Pop() + stk.Pop());// ass (stk);

	printf ("%lf\n", stk.Pop());
	//stk.Pop(); // ass (stk);
	return 0;
}
