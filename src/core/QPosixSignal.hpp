#pragma once

#include "QObject.hpp"
#include "QSocketNotifier.hpp"
#include "QSignal.hpp"
#include <signal.h>
#include <initializer_list>

class QPosixSignal : public QObject {
public:
    enum class signal_t : int {
        POSIX_SIGHUP = SIGHUP,
        POSIX_SIGINT = SIGINT,
        POSIX_SIGQUIT = SIGQUIT,
        POSIX_SIGILL = SIGILL,
        POSIX_SIGABRT = SIGABRT,
        POSIX_SIGFPE = SIGFPE,
        POSIX_SIGKILL = SIGKILL,
        POSIX_SIGSEGV = SIGSEGV,
        POSIX_SIGPIPE = SIGPIPE,
        POSIX_SIGALRM = SIGALRM,
        POSIX_SIGTERM = SIGTERM,
        POSIX_SIGUSR1 = SIGUSR1,
        POSIX_SIGUSR2 = SIGUSR2,
        POSIX_SIGCHLD = SIGCHLD,
        POSIX_SIGCONT = SIGCONT,
        POSIX_SIGSTOP = SIGSTOP,
        POSIX_SIGTSTP = SIGTSTP,
        POSIX_SIGTTIN = SIGTTIN,
        POSIX_SIGTTOU = SIGTTOU
    };

    QPosixSignal() = delete;
    explicit QPosixSignal(QObject *parent = nullptr);
    explicit QPosixSignal(std::initializer_list<signal_t> signals, QObject *parent = nullptr);

    virtual ~QPosixSignal();

    void add(signal_t signal);
    void add(std::initializer_list<signal_t> signals);
    void remove(signal_t signal);
    void remove(std::initializer_list<signal_t> signals);
    bool isMember(signal_t signal) const;

    QSignal<QPosixSignal::signal_t> received;

private:
    sigset_t m_signal_set;
    sigset_t m_oldset;
    int m_fd;
    QSocketNotifier *m_notifier;

    void setupSignalHandler();
    void updateSignalHandler();
    void onSignalReceived();
};
