#ifndef EOL_TASK_TASK_H
#define EOL_TASK_TASK_H

#include <utility>
#include <type_traits>

namespace eol {
/** Task is the standard function implementation representing 
 * a function wiout a return type and taking no arguments created 
 * without dynamically allocating memory. It allows for specifying
 * the size and alignment of the task. 
*/
template <std::size_t Size = 32, std::size_t Align = 16>
class task
{
  private:
	struct storage_t
	{
		inline void* get() noexcept { return &_storage; }
		inline void const* get() const noexcept { return &_storage; }

		template <class T>
		inline T& get() noexcept { return *static_cast<T*>(get()); }
		template <class T>
		inline T const& get() const noexcept { return *static_cast<T const*>(get()); }

		std::aligned_storage_t<Size, Align> _storage;
	};

	enum operation { clone_fn,
					 destroy_fn };

	template <class F>
	struct manager_t
	{
		inline static F const& get_callable(storage_t const& src) noexcept
		{
			return src.get<F>();
		}
		template <class Fn>
		static void create_fn(storage_t& dst, Fn&& fn)
		{
			::new (dst.get()) F(std::forward<Fn>(fn));
		}
		static void destroy_fn(storage_t& dst)
		{
			dst.get<F>().~F();
		}
		static void _invoke_(storage_t const& dst)
		{
			dst.get<F>()();
		}
		static void _manage_(storage_t& dst, storage_t const& src, operation op)
		{
			switch (op)
			{
			case operation::clone_fn:
				create_fn(dst, get_callable(src));
				break;
			case operation::destroy_fn:
				destroy_fn(dst);
				break;
			default:
				break;
			}
		}
	};
	using manager_f = void (*)(storage_t&, storage_t const&, operation);
	using invoker_f = void (*)(storage_t const&);

  public:
	/** @brief Constructs an empty task object, not representing 
	 * any callable object.
	*/
	task() = default;
	/** @brief Constructs a task representing a callable object.
	 * The return type must be void and it must be invokable without arguments
	 * @param callable callable object to be represented by the task
	*/
	template <class F>
	task(F&& callable)
	requires std::is_invocable_v<F>
	{
		using handler = manager_t<std::decay_t<F>>;

		handler::create_fn(_storage, std::forward<std::decay_t<F>>(callable));
		_manager = &handler::_manage_;
		_invoker = &handler::_invoke_;
	}
	/** @brief Copies the task stored in other
	 * @param other task to be copied 
	*/
	task(task const& other)
	{
		if (other)
		{
			other._manager(_storage, other._storage, clone_fn);
			_manager = other._manager;
			_invoker = other._invoker;
		}
	}
	/** @brief Moves the task stored in other
	 * @param other task to be moved 
	*/
	task(task&& other) noexcept
	{
		if (other)
		{
			_storage	   = other._storage;
			_manager	   = other._manager;
			_invoker	   = other._invoker;
			other._manager = nullptr;
			other._invoker = nullptr;
		}
	}
	~task()
	{
		_manager(_storage, _storage, operation::destroy_fn);
	}
	/** @brief Copy assigns the task 
	 * @param other task to be copied
	 * @return *this
	*/
	task& operator=(task const& other)
	{
		if (this != &other)
		{
			task(other).swap(*this);
		}
		return *this;
	}
	/** @brief Move assigns the task
	 * @param other task to be moved
	 * @return *this
	*/
	task& operator=(task&& other) noexcept
	{
		if (this != &other)
		{
			task(std::move(other)).swap(*this);
		}
		return *this;
	}
	/** @brief Swaps states of two tasks
	 * @param other task to be swaped with
	*/
	void swap(task& other)
	{
		std::swap(_storage, other._storage);
		std::swap(_manager, other._manager);
		std::swap(_invoker, other._invoker);
	}
	/** @brief Calls the stored callable object
	*/
	inline void operator()() const
	{
		_invoker(_storage);
	}
	/** @return true if the task represents a callable object 
	*/
	inline operator bool() const noexcept
	{
		return !!_manager;
	}

  private:
	storage_t _storage;
	manager_f _manager;
	invoker_f _invoker;
};
/** Deduction guide for the task */
template <class F>
task(F&& callable) -> task<sizeof(F), alignof(F)>;

} // namespace eol
#endif // !EOL_TASK_TASK_H
