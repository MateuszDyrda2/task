#include "eol_task/task.h"

#include <iostream>

using namespace eol;
int main()
{
	task t([]()
		   { std::cout << "AAA\n"; });
	t();
	return 0;
}