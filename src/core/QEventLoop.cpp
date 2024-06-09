#include "QAbstractEventDispatcher.hpp"
#include "QEventLoop.hpp"

QEventLoop::QEventLoop(QObject* parent)
    : QObject(parent), m_running(false), m_returnCode(0) {

}

QEventLoop::~QEventLoop() {}

void QEventLoop::exec() {
    m_running = true;
    while (m_running) {
        processEvents();
    }
}

void QEventLoop::exit(int returnCode) {
    m_running = false;
    m_returnCode = returnCode;
}

bool QEventLoop::isRunning() const {
    return m_running;
}

void QEventLoop::processEvents() {
    QAbstractEventDispatcher::instance()->processEvents();
}

void QEventLoop::quit() {
    exit();
}
