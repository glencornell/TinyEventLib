#include <gtest/gtest.h>
#include "QPosixSignal.hpp"
#include <signal.h>
#include <QCoreApplication.hpp>
#include <QTimer.hpp>
#include <thread>
#include <chrono>

class QPosixSignalTest : public ::testing::Test {
protected:
    QCoreApplication app{0, nullptr};
};

class SignalReceiver : public QObject {
public:
    bool signalReceived = false;
    QPosixSignal::signal_t lastSignal;

public:
    void onSignalReceived(QPosixSignal::signal_t signal) {
        signalReceived = true;
        lastSignal = signal;
    }
};

TEST_F(QPosixSignalTest, AddSignal) {
    QPosixSignal signalHandler(nullptr);
    signalHandler.add(QPosixSignal::signal_t::POSIX_SIGUSR1);

    // Verify the signal was added successfully
    sigset_t testSet;
    sigemptyset(&testSet);
    sigprocmask(SIG_BLOCK, nullptr, &testSet);
    EXPECT_TRUE(sigismember(&testSet, SIGUSR1));
}

TEST_F(QPosixSignalTest, AddRemoveSignals) {
    QPosixSignal posixSignal(nullptr);

    // Initially, no signals should be managed
    EXPECT_FALSE(posixSignal.isMember(QPosixSignal::signal_t::POSIX_SIGINT));

    // Add a signal
    posixSignal.add(QPosixSignal::signal_t::POSIX_SIGINT);
    EXPECT_TRUE(posixSignal.isMember(QPosixSignal::signal_t::POSIX_SIGINT));

    // Remove the signal
    posixSignal.remove(QPosixSignal::signal_t::POSIX_SIGINT);
    EXPECT_FALSE(posixSignal.isMember(QPosixSignal::signal_t::POSIX_SIGINT));
}

TEST_F(QPosixSignalTest, ReceiveSignal) {
    QPosixSignal signalHandler({QPosixSignal::signal_t::POSIX_SIGUSR1});
    SignalReceiver receiver;

    signalHandler.received.connect(&receiver, &SignalReceiver::onSignalReceived);

    // Send SIGUSR1 to the current process
    std::thread signalThread([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        kill(getpid(), SIGUSR1);
    });

    QTimer::singleShot(500, &app, &QCoreApplication::quit);
    app.exec();
    signalThread.join();

    EXPECT_TRUE(receiver.signalReceived);
    EXPECT_EQ(receiver.lastSignal, QPosixSignal::signal_t::POSIX_SIGUSR1);
}

TEST_F(QPosixSignalTest, AddMultipleSignals) {
    QPosixSignal signalHandler({});
    signalHandler.add({QPosixSignal::signal_t::POSIX_SIGUSR1, QPosixSignal::signal_t::POSIX_SIGUSR2});

    // Verify the signals were added successfully
    sigset_t testSet;
    sigemptyset(&testSet);
    sigprocmask(SIG_BLOCK, nullptr, &testSet);
    EXPECT_TRUE(sigismember(&testSet, SIGUSR1));
    EXPECT_TRUE(sigismember(&testSet, SIGUSR2));
}

TEST_F(QPosixSignalTest, IgnoreNonAddedSignal) {
    QPosixSignal signalHandler({});
    SignalReceiver receiver;

    signalHandler.received.connect(&receiver, &SignalReceiver::onSignalReceived);

    // Send SIGUSR1 to the current process
    std::thread signalThread([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        kill(getpid(), SIGUSR1);
    });

    QTimer::singleShot(500, &app, &QCoreApplication::quit);
    app.exec();
    signalThread.join();

    EXPECT_FALSE(receiver.signalReceived);
}
