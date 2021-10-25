#include "eol_task/task.h"

#include <iostream>

using namespace eol;
int main()
{
	task t[] = {{[](int i)
				 { std::cout << i << '\n'; },
				 1},
				{[]()
				 { std::cout << "2\n"; }},
				{[](int i)
				 { std::cout << i << '\n'; },
				 3},
				[]
				{ std::cout << "4\n"; },
				[]
				{ std::cout << "5\n"; }};
	for (int i = 0; i < 5; ++i)
		t[i]();

	return 0;
}