#pragma once

#include "QSignal.hpp"
#include "QEvent.hpp"
#include <vector>

class QObject {
public:
    explicit QObject(QObject* parent = nullptr);
    virtual ~QObject();

    QObject* parent() const;
    void setParent(QObject* parent);
    const std::vector<QObject*>& children() const;

    virtual bool event(QEvent* event);

    void installEventFilter(QObject* filter);
    void removeEventFilter(QObject* filter);

    /**
     * @brief Filters events for the object.
     *
     * This method is called for all events sent to the object
     * before they reach the event handler.
     *
     * @param object The object being watched.
     * @param event The event to filter.
     * @return true if the event should not be handled further; false otherwise.
     */
    virtual bool eventFilter(QObject *watched, QEvent* event);

    const auto& eventFilters() const {
        return m_eventFilters;
    }

    /**
     * @brief Schedule the object for deletion.
     *
     * Schedule the object for deletion after all pending events for the object have been processed.  This is useful for
     * safely deleting objects in asynchronous environments without immediately invalidating pointers or other
     * references.
     */
    void deleteLater();

    template <typename... Args>
    static void connect(const QSignal<void(Args...)>& signal, void(*slot)(Args...)) {
        return signal.connect(slot);
    }

    template <typename... Args, typename Callable>
    static void connect(const QSignal<void(Args...)>& signal, Callable&& callable) {
        return signal.connect(std::forward<Callable>(callable));
    }

    template <typename... Args, typename T>
    static void connect(const QSignal<void(Args...)>& signal, const T* instance, void(T::*method)(Args...)) {
        return signal.connect(instance, method);
    }

    template <typename... Args, typename T>
    static void connect(const QSignal<void(Args...)>& signal, const T* instance, void(T::*method)(Args...) const) {
        return signal.connect(instance, method);
    }

protected:
    void addChild(QObject* child);
    void removeChild(QObject* child);

private:
    QObject* m_parent = nullptr;
    std::vector<QObject*> m_children;
    std::vector<QObject*> m_eventFilters;
};

// Global container of top-level QObject pointers
extern std::vector<QObject*> g_topLevelObjects;
