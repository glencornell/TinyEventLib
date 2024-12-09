#include "QCoreApplication.hpp"

QCoreApplication* QCoreApplication::m_instance{nullptr};
QAbstractEventDispatcher* QCoreApplication::m_eventDispatcher{nullptr};

QCoreApplication::QCoreApplication(int argc, char* argv[]) :
    m_running(false),
    m_returnCode(0) {
    m_instance = this;
}

QCoreApplication* QCoreApplication::instance() {
    return m_instance;
}

QCoreApplication::~QCoreApplication() {
    m_instance = nullptr;
}

void QCoreApplication::exit(int returnCode) {
    m_returnCode = returnCode;
    m_running = false;
}

void QCoreApplication::quit() {
    exit();
}

int QCoreApplication::exec() {
    if (m_running) {
        return -1;
    }
    m_running = true;
    while (m_running) {
        processEvents();
    }
    return m_returnCode;
}

void QCoreApplication::processEvents() {
    // Process event queue
    while (!m_eventQueue.empty()) {
        auto [receiver, event] = m_eventQueue.front();
        m_eventQueue.pop();
        notify(receiver, event);
        delete event;
    }

    // process timers and socket notifiers
    if (m_eventDispatcher) {
        m_eventDispatcher->processEvents();
    }
}

void QCoreApplication::postEvent(QObject* receiver, QEvent* event) {
    m_eventQueue.push({receiver, event});
}

bool QCoreApplication::sendEvent(QObject* receiver, QEvent* event) {
    if (!receiver || !event) {
        return false;
    }
    return receiver->event(event);
}

bool QCoreApplication::notify(QObject* receiver, QEvent *event) {
    if (!receiver || !event) {
        return false;
    }

    // Apply event filters (if any are installed on the receiver)
    QObject *current = receiver;
    while (current) {
        const auto &filters = current->eventFilters();
        for (QObject *filter : filters) {
            if (filter && filter->eventFilter(receiver, event)) {
                // the event is filtered out
                return true;
            }
        }
        current = current->parent(); // Check parent filters (if any)
    }

    // If no filter handled the event, deliver it to the receiver
    return receiver->event(event);
}

void QCoreApplication::setEventDispatcher(QAbstractEventDispatcher* eventDispatcher) {
    m_eventDispatcher = eventDispatcher;
}

QAbstractEventDispatcher* QCoreApplication::eventDispatcher() {
    return m_eventDispatcher;
}
