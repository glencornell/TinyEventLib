#pragma once

#include <functional>
#include <vector>
#include <algorithm>

template<typename... Args>
class QSignal {
public:
    using slot_t = std::function<void(Args...)>;
    using id_t = std::size_t;

    id_t connect(slot_t slot) {
        Slot s{m_nextId++, slot};
        m_slots.emplace_back(std::move(s));
        return m_nextId - 1;
    }

    id_t connect(void (*function)(Args...)) {
        return connect(slot_t(function));
    }

    template <typename Functor>
    auto connect(Functor&& functor) {
        return connect(slot_t(std::forward<Functor>(functor))); // Assumes functor outlives the signal
    }

    void disconnect(id_t id) {
        auto it = std::find_if(m_slots.begin(), m_slots.end(),
                               [id](auto const& s){ return s.id == id; });
        if (it != m_slots.end()) {
            m_slots.erase(it);
        }
    }

    void operator()(Args... args) const {
        for (const auto& slot : m_slots) {
            slot.fn(std::forward<Args>(args)...);
        }
    }

private:
    struct Slot {
        id_t   id;
        slot_t fn;
    };

    std::vector<Slot> m_slots;
    id_t m_nextId{0};
};

// Convenience functions
template<typename... Args>
inline auto connect(QSignal<Args...>& signal, typename QSignal<Args...>::slot_t slot) {
    return signal.connect(std::move(slot));
}

template<typename... Args>
inline auto connect(QSignal<Args...>& signal, void (*function)(Args...)) {
    return signal.connect(function);
}

template <typename Functor, typename... Args>
inline auto connect(QSignal<Args...>& signal, Functor&& functor) {
    return signal.connect(functor);
}
