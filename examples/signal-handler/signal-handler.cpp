#include <QCoreApplication.hpp>
#include <QPosixSignal.hpp>
#include <QTimer.hpp>
#include <iostream>
#include <thread>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    QPosixSignal handler({QPosixSignal::signal_t::POSIX_SIGUSR1});
    handler.received.connect([&app](QPosixSignal::signal_t signum) {
        std::cout << "Received signal: " << static_cast<int>(signum) << std::endl;
        app.quit();
    });
    QTimer timer;
    timer.timeout.connect([] {
        std::cout << "timeout called" << std::endl;
    });
    timer.start(100);

    std::thread signalThread([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        kill(getpid(), SIGUSR1);
    });

    app.exec();
    signalThread.join();

    return 0;
}
