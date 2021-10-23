#ifndef EOL_TASK_TASK_H
#define EOL_TASK_TASK_H

#include <cstddef> // size_t
#include <utility> // aligned storage

#ifndef EOL_TASK_STORAGE_SIZE
#	define EOL_TASK_STORAGE_SIZE 32
#endif // !EOL_TASK_STORAGE_SIZE

#ifndef EOL_TASK_STORAGE_ALIGN
#	define EOL_TASK_STORAGE_ALIGN 16
#endif // !EOL_TASK_STORAGE_ALIGN

#ifndef EOL_DISABLE_EXCEPTIONS
#	define EOL_DISABLE_EXCEPTIONS
#endif // !EOL_DISABLE_EXCEPTIONS

namespace eol {
class task
{
  public:
	using self_type	   = task;
	using size_type	   = std::size_t;
	using storage_type = std::aligned_storage_t<EOL_TASK_STORAGE_SIZE + 16,
												EOL_TASK_STORAGE_ALIGN>;

  public:
	task() = default;
	template <class F, class... Args>
	task(F&& f, Args&&... args);
	task(const self_type& other);
	task(self_type&& other) noexcept;
	self_type& operator=(const self_type& other);
	self_type& operator=(self_type&& other) noexcept;
	~task();

	void call();

  private:
};
} // namespace eol
#endif // !EOL_TASK_TASK_H