/** 
 * @file task.h
 * @brief Fixed size storaga class for abstracting the call to a function
 * @author Mateusz Dyrda
*/
#ifndef EOL_TASK_TASK_H
#define EOL_TASK_TASK_H

#include <cstddef> // size_t
#include <tuple>   // tuple
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
/** Task is a type allowing for representing a function
 * that doesn't return anything that uses type erasure to
 * allow for storing functions with different signatures in containers
*/
class task
{
	struct callable_base;
	template <class F, class... Args>
	struct callable;

  public:
	using self_type	   = task;
	using size_type	   = std::size_t;
	using storage_type = std::aligned_storage_t<EOL_TASK_STORAGE_SIZE + 16,
												EOL_TASK_STORAGE_ALIGN>;

  public:
	/** @brief Constructs a task from a callable object.
	 * The callable must be able to be called with the arguments specified
	 * @param f callable to represent
	 * @param args arguments to pass 
	*/
	template <class F, class... Args>
	task(F&& f, Args&&... args)
	{
		::new (&_storage) callable(
			std::forward<std::decay_t<F>>(f),
			std::forward<std::decay_t<Args>>(args)...);
	}
	/** @brief Copy constructs the task object
	 * @param other task to be copied
	*/
	task(const self_type& other)
	{
		other.get_callable()->copy(&_storage);
	}
	/** @brief Move constructs the task object
	 * @param other task to be moved
	*/
	task(self_type&& other) noexcept
	{
		other.get_callable()->move(&_storage);
	}
	/** @brief Copy assigns task
	 * @param other task to be copied from
	 * @return *this
	*/
	self_type& operator=(const self_type& other)
	{
		if (this != &other)
		{
			get_callable()->~callable_base();
			other.get_callable()->copy(&_storage);
		}
		return *this;
	}
	/** @brief Move assigns task
	 * @param other task to be moved from
	 * @return *this
	*/
	self_type& operator=(self_type&& other) noexcept
	{
		if (this != &other)
		{
			get_callable()->~callable_base();
			other.get_callable()->move(&_storage);
		}
		return *this;
	}
	~task()
	{
		get_callable()->~callable_base();
	}
	/** @brief Calls the associated callable object with 
	 * arguments it was passed at creation
	*/
	inline void call()
	{
		get_callable()->call();
	}
	/** @brief Calls the associated callable object with 
	 * arguments it was passed at creation
	*/
	inline void operator()()
	{
		return call();
	}

  private:
	struct callable_base
	{
		virtual void call()							 = 0;
		virtual ~callable_base()					 = default;
		virtual callable_base* copy(void* dst) const = 0;
		virtual callable_base* move(void* dst)		 = 0;
	};
	template <class F, class... Args>
	struct callable : public callable_base
	{
		callable(F&& f, Args&&... args)
			: function(std::forward<F>(f)),
			  arguments(std::forward_as_tuple(args...))
		{
		}
		callable(const callable&) = default;
		callable(callable&&)	  = default;
		~callable()				  = default;
		void call() final override
		{
			std::apply(std::forward<F>(function),
					   std::forward<std::tuple<Args...>>(arguments));
		}
		callable_base* copy(void* dst) const final override
		{
			return ::new (dst) callable(*this);
		}
		callable_base* move(void* dst) final override
		{
			return ::new (dst) callable(std::move(*this));
		}

		F function;
		std::tuple<Args...> arguments;
	};

  private:
	storage_type _storage;

	callable_base* get_callable()
	{
		return (callable_base*)&_storage;
	}
	callable_base const* get_callable() const
	{
		return (const callable_base*)&_storage;
	}
};
} // namespace eol
#endif // !EOL_TASK_TASK_H