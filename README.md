# eol_task
A header only implementation of a class representing a function without return value stored together with arguments you call it with.
The task class doesn't allocate any memory and you can specify the size of a buffer used.
## Using the library
The library allows for storing multiple function pointers with their arguments in containers even when their signatures don't match.
It allows for calling functions at a later time without having to know their type as the arguments are already stored with them.
```c++
std::vector<eol::task> tasks;
tasks.emplace_back(myFunc, 1, 1.4);
tasks.emplace_back([](){std::cout << "w";});
tasks.emplace_back(&MyClass::method, &obj, 3);
for(auto& t : tasks)
{
	t();
}
```
