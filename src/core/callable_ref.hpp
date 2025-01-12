#pragma once

#include <functional>
#include <utility>
#include <type_traits>
#include <variant>
#include "method_ref.hpp"
#include "function_ref.hpp"

template <typename Signature>
class callable_ref;

template <typename Ret, typename... Args>
class callable_ref<Ret(Args...)> {
private:
    using function_ref_t = function_ref<Ret(Args...)>;
    using method_ref_t = method_ref<Ret(Args...)>;

    std::variant<function_ref_t, method_ref_t> callable_;

public:
    template <typename Callable, typename = std::enable_if_t<!std::is_same_v<std::decay_t<Callable>, method_ref_t>>>
    callable_ref(Callable&& callable) : callable_(function_ref_t(std::forward<Callable>(callable))) {}

    template <typename T>
    callable_ref(T* instance, Ret (T::*method)(Args...)) : callable_(method_ref_t(instance, method)) {}

    template <typename T>
    callable_ref(const T* instance, Ret (T::*method)(Args...) const) : callable_(method_ref_t(instance, method)) {}

    Ret operator()(Args... args) const {
        return std::visit(
            [&args...](auto&& callable) -> Ret {
                return callable(std::forward<Args>(args)...);
            },
            callable_);
    }
};
