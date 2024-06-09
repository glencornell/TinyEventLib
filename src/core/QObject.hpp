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

    static void connect(QSignal<>& signal, QSignal<>::SlotType slot);

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
