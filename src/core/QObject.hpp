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

    template<typename... Args>
    static void connect(QSignal<Args...>& signal, typename QSignal<Args...>::SlotType slot) {
        signal.connect(std::move(slot));
    }

    template <typename Sender, typename Signal, typename Receiver, typename Slot>
    static void connect(Sender* sender, Signal signal, Receiver* receiver, Slot slot) {
        (sender->*signal).connect([receiver, slot](auto&&... args) {
            (receiver->*slot)(std::forward<decltype(args)>(args)...);
        });
    }

    template <typename Sender, typename Signal, typename Func>
    static void connect(Sender* sender, Signal signal, Func func) {
        (sender->*signal).connect(func);
    }

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
