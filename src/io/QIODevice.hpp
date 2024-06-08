#pragma once

#include <cstdint>
#include <cstddef>
#include <unistd.h>

class QIODevice {
public:
    enum OpenMode {
        NotOpen = 0x0000,
        ReadOnly = 0x0001,
        WriteOnly = 0x0002,
        ReadWrite = ReadOnly | WriteOnly,
        Unbuffered = 0x0020
    };

    QIODevice() = default;
    virtual ~QIODevice();

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
