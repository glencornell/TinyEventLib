#pragma once

#include <utility>
#include <type_traits>

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

template <typename Signature>
class function_ref;

template <typename R, typename... Args>
class function_ref<R(Args...)> {
    using StubType = R(*)(void*, Args&&...);

    void* object_;
    StubType stub_;

    template <typename T>
    static R stub(void* object, Args&&... args) {
        return (*static_cast<T*>(object))(std::forward<Args>(args)...);
    }

public:
    /**
     * @brief Constructs a `function_ref` from a callable object.
     *
     * @tparam T The type of the callable object.
     * @param obj The callable object.
     */
    template <typename T>
    function_ref(T&& obj) : object_((void*)std::addressof(obj)), stub_(&stub<std::decay_t<T>>) {}

    /**
     * @brief Invokes the callable with the given arguments.
     *
     * @param args The arguments to pass to the callable.
     * @return The result of the callable invocation.
     */
    R operator()(Args... args) const {
        return stub_(object_, std::forward<Args>(args)...);
    }
};
