#pragma once

#include <vector>
#include <functional>
#include <algorithm>

template<typename... Args>
class QSignal {
public:
    using SlotType = std::function<void(Args...)>;

    void connect(SlotType slot) {
        slots.push_back(std::move(slot));
    }

    void disconnect(SlotType slot) {
        auto it = std::remove_if(slots.begin(), slots.end(),
                                 [&](const SlotType& other) {
                                     return other.template target<void(Args...)>() == slot.template target<void(Args...)>();
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

// Convenience function
template<typename... Args>
inline void connect(QSignal<Args...>& signal, typename QSignal<Args...>::SlotType slot) {
    signal.connect(std::move(slot));
}
