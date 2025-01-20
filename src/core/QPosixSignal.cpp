#include "QPosixSignal.hpp"
#include <sys/signalfd.h>
#include <unistd.h>

QPosixSignal::QPosixSignal(QObject *parent)
    : QObject(parent), m_fd(-1), m_notifier(nullptr) {
    // TODO: enforce that only one instance of this class is created per thread.
    // TODO: do we create a mechnaism to coordinate signals across threads?
    ::sigemptyset(&m_signal_set);
    setupSignalHandler();
}

QPosixSignal::QPosixSignal(std::initializer_list<signal_t> signals, QObject *parent)
    : QObject(parent), m_fd(-1), m_notifier(nullptr) {
    ::sigemptyset(&m_signal_set);
    for (auto signal : signals) {
        ::sigaddset(&m_signal_set, static_cast<int>(signal));
    }
    setupSignalHandler();
}

QPosixSignal::~QPosixSignal() {
    if (m_fd != -1) {
        ::close(m_fd);
        delete m_notifier;
        ::sigprocmask(SIG_UNBLOCK, &m_oldset, nullptr);
    }
}

void QPosixSignal::add(signal_t signal) {
    if (::sigaddset(&m_signal_set, static_cast<int>(signal)) == -1) {
        return; // Fail silently
    }
    updateSignalHandler();
}

void QPosixSignal::add(std::initializer_list<signal_t> signals) {
    for (auto signal : signals) {
        if (::sigaddset(&m_signal_set, static_cast<int>(signal)) == -1) {
            return; // Fail silently
        }
    }
    updateSignalHandler();
}

void QPosixSignal::remove(signal_t signal) {
    if (::sigdelset(&m_signal_set, static_cast<int>(signal)) == -1) {
        return; // Fail silently
    }
    updateSignalHandler();
}

void QPosixSignal::remove(std::initializer_list<signal_t> signals) {
    for (auto signal : signals) {
        if (::sigdelset(&m_signal_set, static_cast<int>(signal)) == -1) {
            return; // Fail silently
        }
    }
    updateSignalHandler();
}

bool QPosixSignal::isMember(signal_t signal) const {
    return ::sigismember(&m_signal_set, static_cast<int>(signal)) == 1;
}

void QPosixSignal::setupSignalHandler() {
    if (::sigprocmask(SIG_BLOCK, &m_signal_set, &m_oldset) == -1) {
        return; // Fail silently
    }
    m_fd = ::signalfd(-1, &m_signal_set, SFD_NONBLOCK | SFD_CLOEXEC);
    if (m_fd == -1) {
        return; // Fail silently
    }
    m_notifier = new QSocketNotifier(m_fd, QSocketNotifier::Read, this);
    m_notifier->activated.connect(this, &QPosixSignal::onSignalReceived);
}

void QPosixSignal::updateSignalHandler() {
    if (m_fd != -1) {
        ::close(m_fd);
        delete m_notifier;
        m_notifier = nullptr;
        ::sigprocmask(SIG_UNBLOCK, &m_oldset, nullptr);
    }
    setupSignalHandler();
}

void QPosixSignal::onSignalReceived() {
    while (true) {
        struct signalfd_siginfo siginfo;
        ssize_t bytesRead = ::read(m_fd, &siginfo, sizeof(siginfo));

        if (bytesRead == -1) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                break; // No more signals to read
            }
            return; // Ignore other read errors
        }

        if (bytesRead == sizeof(siginfo)) {
            received(static_cast<QPosixSignal::signal_t>(siginfo.ssi_signo));
        }
    }
}
