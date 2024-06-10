#pragma once

#include <cstdint>
#include <cstddef>
#include <unistd.h>
#include "QObject.hpp"
#include "QIODeviceBase.hpp"

class QIODevice : public QObject, public QIODeviceBase {
public:
    QIODevice() = default;
    ~QIODevice();

    virtual bool open(OpenMode mode);
    virtual void close();

    bool isOpen() const;
    bool isReadable() const;
    bool isWritable() const;
    OpenMode openMode() const;

    ssize_t read(char* data, size_t maxSize);
    ssize_t write(const char* data, size_t maxSize);

protected:
    virtual ssize_t readData(char* data, size_t maxSize) = 0;
    virtual ssize_t writeData(const char* data, size_t maxSize) = 0;

private:
    OpenMode m_openMode{NotOpen};
    bool m_openFlag{false};
};
