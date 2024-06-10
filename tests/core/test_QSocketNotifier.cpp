#include <gtest/gtest.h>
#include <QSocketNotifier.hpp>
#include <QCoreApplication.hpp>
#include <unistd.h>

static QCoreApplication app{0,nullptr};

class QSocketNotifierTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        socketNotifier = new QSocketNotifier(pipefd[0], QSocketNotifier::Read);

        QObject::connect(socketNotifier, &QSocketNotifier::activated, [this]() {
            char buffer[1];
            read(pipefd[0], buffer, 1);
            dataReceived = true;
        });

        dataReceived = false;
    }

    void TearDown() override {
        delete socketNotifier;
        close(pipefd[0]);
        close(pipefd[1]);
    }

    int pipefd[2];
    QSocketNotifier *socketNotifier;
    bool dataReceived;
};

TEST_F(QSocketNotifierTest, IsEnabledByDefault) {
    ASSERT_TRUE(socketNotifier->isEnabled());
}

TEST_F(QSocketNotifierTest, CanDisableSocketNotifier) {
    socketNotifier->setEnabled(false);
    ASSERT_FALSE(socketNotifier->isEnabled());
}

TEST_F(QSocketNotifierTest, CanEnableSocketNotifier) {
    socketNotifier->setEnabled(false);
    socketNotifier->setEnabled(true);
    ASSERT_TRUE(socketNotifier->isEnabled());
}

TEST_F(QSocketNotifierTest, SocketType) {
    ASSERT_EQ(socketNotifier->type(), QSocketNotifier::Read);
}

TEST_F(QSocketNotifierTest, SocketDescriptor) {
    ASSERT_EQ(socketNotifier->socket(), pipefd[0]);
}

TEST_F(QSocketNotifierTest, DetectsDataOnPipe) {
    char buffer[1] = {'A'};
    write(pipefd[1], buffer, 1);
    app.processEvents();
    ASSERT_TRUE(dataReceived);
}
