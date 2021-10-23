#include "eol_task/task.h"

#include <iostream>

using namespace eol;
int main()
{
	task t[] = {[]
				{ std::cout << "1\n"; },
				[]
				{ std::cout << "2\n"; },
				[]
				{ std::cout << "3\n"; },
				[]
				{ std::cout << "4\n"; },
				[]
				{ std::cout << "5\n"; }};
	for (int i = 0; i < 5; ++i)
		t[i]();

	return 0;
}