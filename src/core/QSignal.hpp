#pragma once

#include "function_ref.hpp"
#include <vector>
#include <algorithm>

template<typename... Args>
class QSignal {
public:
    using SlotType = function_ref<void(Args...)>;

    void connect(SlotType slot) {
        slots.push_back(std::move(slot));
    }

    void connect(void (*function)(Args...)) {
        connect(SlotType(function));
    }

    template <typename Functor>
    void connect(Functor&& functor) {
        connect(SlotType(std::forward<Functor>(functor))); // Assumes functor outlives the signal
    }

    // Connect a non-const member function
    template <typename C>
    void connect(C& instance, void (C::*method)(Args...)) {
        // NOTE: the lifetime of this lambda ends when this function returns, potentially causing a segfault when the
        // signal is emitted. TODO: find a solution within the function_ref class to store class instances and their corresponding methods.
        connect(SlotType([instancePtr = &instance, method](Args... args) {
            (instancePtr->*method)(std::forward<Args>(args)...);
        }));
    }

    // Connect a const member function
    template <typename C>
    void connect(const C& instance, void (C::*method)(Args...) const) {
        // NOTE: the lifetime of this lambda ends when this function returns, potentially causing a segfault when the
        // signal is emitted. TODO: find a solution within the function_ref class to store class instances and their corresponding methods.
        connect(SlotType([instancePtr = &instance, method](Args... args) {
            (instancePtr->*method)(std::forward<Args>(args)...);
        }));
    }

    void disconnect(SlotType slot) {
        auto it = std::remove_if(slots.begin(), slots.end(),
                                 [&](const SlotType& other) {
                                     return other.target() == slot.target();
                                 });
        slots.erase(it, slots.end());
    }

    void operator()(Args... args) const {
        for (const auto& slot : slots) {
            slot(args...);
        }
    }

private:
    std::vector<SlotType> slots;
};

// Convenience functions
template<typename... Args>
inline void connect(QSignal<Args...>& signal, typename QSignal<Args...>::SlotType slot) {
    signal.connect(std::move(slot));
}

template<typename... Args>
inline void connect(QSignal<Args...>& signal, void (*function)(Args...)) {
    signal.connect(function);
}

template <typename Functor, typename... Args>
inline void connect(QSignal<Args...>& signal, Functor&& functor) {
    signal.connect(functor);
}

// Convenience overload for non-const member functions
template <typename C, typename... Args>
inline void connect(QSignal<Args...>& signal, C& instance, void (C::*method)(Args...)) {
    signal.connect(instance, method);
}

// Convenience overload for const member functions
template <typename C, typename... Args>
inline void connect(QSignal<Args...>& signal, const C& instance, void (C::*method)(Args...) const) {
    signal.connect(instance, method);
}
