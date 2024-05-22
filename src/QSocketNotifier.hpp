#pragma once

#include "QAbstractEventDispatcher.hpp"
#include "QObject.hpp"
#include "QSignal.hpp"

class QSocketNotifier : public QObject {
public:
    enum Type {
        Read,
        Write,
        Exception
    };

    QSocketNotifier(int fd, Type type, QObject* parent = nullptr);
    ~QSocketNotifier();

    int fd() const;

    QSignal<> activated;

private:
    int m_fd;
    Type m_type;
};
