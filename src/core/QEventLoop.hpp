#pragma once

#include "QObject.hpp"

class QEventLoop : public QObject {
public:
    explicit QEventLoop(QObject* parent = nullptr);
    ~QEventLoop();

    void exec();
    void exit(int returnCode = 0);
    bool isRunning() const;

    void processEvents();
    void quit();

private:
    bool m_running;
    int m_returnCode;
};
