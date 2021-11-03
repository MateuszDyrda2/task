#include <eol_task/task.h>

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

void f(int w, int* e, int* r)
{
	int sum = w + (*e);
	int ex	= sum * (*r);
	int res;
	int* _res				 = &res;
	((int volatile*)_res)[0] = sum - ex;
}

using namespace eol;
int main()
{
	using namespace std::chrono;

	std::vector<task<20, 8>> tasks3;
	std::vector<std::function<void()>> functions;
	constexpr size_t sampleSize = 1000000;

	tasks3.reserve(sampleSize);
	functions.reserve(sampleSize);

	int e = 2, r = 3;
	int *_e = &e, *_r = &r;
	for (int i = 0; i < sampleSize; ++i)
	{
		tasks3.emplace_back([=]
							{ f(i, _e, _r); });
	}
	for (int i = 0; i < sampleSize; ++i)
	{
		functions.emplace_back([=]
							   { f(i, _e, _r); });
	}

	auto now16 = high_resolution_clock::now();
	for (auto& t : tasks3)
		t();
	auto diff16 = high_resolution_clock::now() - now16;

	auto now2 = high_resolution_clock::now();
	for (auto& fu : functions)
		fu();
	auto diff2 = high_resolution_clock::now() - now2;

	auto l = [=](int i)
	{ f(i, _e, _r); };

	auto now3 = high_resolution_clock::now();
	for (int i = 0; i < sampleSize; ++i)
		l(i);
	auto diff3 = high_resolution_clock::now() - now3;

	std::cout << "task:\t" << std::right << '\t' << diff16.count() << '\n';
	std::cout << "function:" << std::right << '\t' << diff2.count() << '\n';
	std::cout << "normal:\t\t" << std::right << diff3.count() << '\n';

	return 0;
}
