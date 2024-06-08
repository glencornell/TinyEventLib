#include "QIODevice.hpp"

QIODevice::~QIODevice() {
    close();
}

bool QIODevice::open(OpenMode mode) {
    if (mode == NotOpen) {
        return false;
    }
    m_openMode = mode;
    m_openFlag = true;
    return true;
}

void QIODevice::close() {
    m_openMode = NotOpen;
    m_openFlag = false;
}

bool QIODevice::isOpen() const {
    return m_openFlag;
}

bool QIODevice::isReadable() const {
    return (m_openMode & ReadOnly) != 0;
}

bool QIODevice::isWritable() const {
    return (m_openMode & WriteOnly) != 0;
}

QIODevice::OpenMode QIODevice::openMode() const {
    return m_openMode;
}

ssize_t QIODevice::read(char* data, size_t maxSize) {
    if (!isReadable() || !isOpen()) {
        return -1;
    }
    return readData(data, maxSize);
}

ssize_t QIODevice::write(const char* data, size_t maxSize) {
    if (!isWritable() || !isOpen()) {
        return -1;
    }
    return writeData(data, maxSize);
}
