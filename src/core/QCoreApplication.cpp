#include "QCoreApplication.hpp"

QCoreApplication* QCoreApplication::m_instance{nullptr};
QAbstractEventDispatcher* QCoreApplication::m_eventDispatcher{nullptr};

QCoreApplication::QCoreApplication(int argc, char* argv[]) :
    m_running(false) {
    if (m_instance == nullptr)
        m_instance = this;
}

QCoreApplication* QCoreApplication::instance() {
    return m_instance;
}

QCoreApplication::~QCoreApplication() {
    m_instance = nullptr;
}

void QCoreApplication::exit(int returnCode) {
    std::cout << "Exiting with return code: " << returnCode << std::endl;
    m_returnCode = returnCode;
    m_running = false;
}

void QCoreApplication::quit() {
    exit();
}

int QCoreApplication::exec() {
    if (m_running) {
        std::cerr << "Already running" << std::endl;
        return -1;
    }
    m_running = true;
    std::cout << "Starting event loop" << std::endl;
    while (m_running) {
        if (m_eventDispatcher) {
            m_eventDispatcher->processEvents();
        }
    }
    std::cout << "Event loop finished" << std::endl;
    return m_returnCode;
}

void QCoreApplication::processEvents() {
    if (m_eventDispatcher) {
        m_eventDispatcher->processEvents();
    }
}

void QCoreApplication::postEvent(QObject* receiver, QEvent* event) {
    if (!receiver || !event) {
        return;
    }
    if (m_eventDispatcher) {
        m_eventDispatcher->postEvent(receiver, event);
    }
}

bool QCoreApplication::sendEvent(QObject* receiver, QEvent* event) {
    if (!receiver || !event) {
        return false;
    }
    return receiver->event(event);
}

void QCoreApplication::setEventDispatcher(QAbstractEventDispatcher* eventDispatcher) {
    m_eventDispatcher = eventDispatcher;
}

QAbstractEventDispatcher* QCoreApplication::eventDispatcher() {
    return m_eventDispatcher;
}

bool QCoreApplication::event(QEvent* e) {
    return false;
}
