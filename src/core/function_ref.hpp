#pragma once

/**
 * @brief `function_ref` provides a lightweight, non-owning reference to a callable object.
 *
 * It is similar to `std::function` but does not perform dynamic memory allocation
 * and does not take ownership of the callable object.
 *
 * ## Limitations
 * - `function_ref` does not own the callable. The user must ensure that the callable
 *   outlives the `function_ref` instance.
 * - It does not provide type erasure for state; it simply stores a reference to the callable.
 * - Callables referenced by `function_ref` are typically immutable.
 * - No copy or assignment semantics are provided, as `function_ref` holds a non-owning reference.
 *
 * ## Usage Rules
 * - Ensure the lifetime of the callable exceeds that of the `function_ref` instance.
 * - Avoid modifying the state of the callable through a `function_ref` to prevent undefined behavior.
 */

#include <type_traits>
#include <cassert>
#include <iostream>

template <typename Signature>
class function_ref;

template <typename R, typename... Args>
class function_ref<R(Args...)> {
    using callback_type = R(*)(void*, Args...);

    void* object = nullptr;
    callback_type callback = nullptr;

public:
    function_ref() = default;

    /**
     * @brief Constructs a `function_ref` from a callable object.
     *
     * @tparam T The type of the callable object.
     * @param obj The callable object.
     */
    template<typename F>
    function_ref(F&& f) noexcept {
        static_assert(std::is_invocable_r_v<R, F, Args...>,
            "Function object does not match function signature");

        if constexpr (std::is_pointer_v<std::decay_t<F>> && std::is_function_v<std::remove_pointer_t<std::decay_t<F>>>) {
            object = reinterpret_cast<void*>(f);
            callback = [](void* obj, Args... args) -> R {
                return reinterpret_cast<R(*)(Args...)>(obj)(std::forward<Args>(args)...);
            };
        } else {
            object = reinterpret_cast<void*>(&f);
            callback = [](void* obj, Args... args) -> R {
                return (*reinterpret_cast<std::decay_t<F>*>(obj))(std::forward<Args>(args)...);
            };
        }
    }

    /**
     * @brief Invokes the callable with the given arguments.
     *
     * @param args The arguments to pass to the callable.
     * @return The result of the callable invocation.
     */
    R operator()(Args... args) const {
        assert(callback != nullptr);
        return callback(object, std::forward<Args>(args)...);
    }

    const void* target() const noexcept {
        return object;
    }
};
