# eol_task
A header only implementation of a class representing a function without return value and arguments.
The task class doesn't allocate any memory and you can specify the size of a buffer used.
## Using the library
The library allows for storing multiple callable objects in containers even when their signatures don't match.
It allows for calling functions at a later time without having to know their type.
```c++
std::vector<eol::task> tasks;
tasks.emplace_back([](){ std::cout << "w"; });
tasks.emplace_back([=]{ std::cout << local; });
tasks.emplace_back(func);
for(auto& t : tasks)
{
	t();
}
```
