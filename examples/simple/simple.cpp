#include <QCoreApplication.hpp>
#include <QTimer.hpp>
#include <QSocketNotifier.hpp>
#include <QProperty.hpp>
#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    QProperty<int> property1 { 32 };
    connect(property1.onValueChanged, [] (int i) {
        std::cout << "property 1 changed: " << i << std::endl;
    });

    QTimer timer1;
    connect(timer1.timeout, [&property1]() {
        std::cout << "Timer 1 expired!" << std::endl;
        property1 = property1.value() + 1;
    });
    timer1.start(1000); // 1 second

    QTimer timer2;
    QObject::connect(&timer2, &QTimer::timeout, []() {
        std::cout << "Timer 2 expired!" << std::endl;
    });
    timer2.start(2000); // 2 seconds

    int pipefd[2];
    pipe(pipefd);
    QSocketNotifier socketNotifier(pipefd[0], QSocketNotifier::Read);
    connect(socketNotifier.activated, [pipefd]() {
        char buffer[1024];
        read(pipefd[0], buffer, sizeof(buffer));
        std::cout << "Data available on pipe: " << buffer << std::endl;
    });

    // Simulate writing to the pipe after 3 seconds
    QTimer singleShotTimer;
    connect(singleShotTimer.timeout, [pipefd]() {
        write(pipefd[1], "Hello, world!", 13);
    });
    singleShotTimer.startSingleShot(3000); // 3 seconds

    app.exec();

    return 0;
}
