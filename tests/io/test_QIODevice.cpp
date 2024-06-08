#include <gtest/gtest.h>
#include <cstring>
#include "QIODevice.hpp"

class TestIODevice : public QIODevice {
public:
    TestIODevice() : m_data(nullptr), m_size(0) {}

    ~TestIODevice() override {
        delete[] m_data;
    }

    void setData(const char* data, size_t size) {
        delete[] m_data;
        m_data = new char[size];
        std::memcpy(m_data, data, size);
        m_size = size;
    }

protected:
    ssize_t readData(char* data, size_t maxSize) override {
        if (m_data == nullptr || m_size == 0) return -1;
        size_t bytesToRead = std::min(maxSize, m_size);
        std::memcpy(data, m_data, bytesToRead);
        return bytesToRead;
    }

    ssize_t writeData(const char* data, size_t maxSize) override {
        delete[] m_data;
        m_data = new char[maxSize];
        std::memcpy(m_data, data, maxSize);
        m_size = maxSize;
        return maxSize;
    }

private:
    char* m_data;
    size_t m_size;
};

TEST(QIODeviceTest, OpenCloseTest) {
    TestIODevice device;
    EXPECT_FALSE(device.isOpen());

    device.open(QIODevice::ReadOnly);
    EXPECT_TRUE(device.isOpen());
    EXPECT_TRUE(device.isReadable());
    EXPECT_FALSE(device.isWritable());

    device.close();
    EXPECT_FALSE(device.isOpen());
}

TEST(QIODeviceTest, ReadWriteTest) {
    TestIODevice device;
    const char* writeData = "Hello, World!";
    char readData[20];

    device.open(QIODevice::ReadWrite);
    EXPECT_TRUE(device.isOpen());
    EXPECT_TRUE(device.isReadable());
    EXPECT_TRUE(device.isWritable());

    ssize_t bytesWritten = device.write(writeData, std::strlen(writeData));
    EXPECT_EQ(bytesWritten, std::strlen(writeData));

    ssize_t bytesRead = device.read(readData, sizeof(readData));
    EXPECT_EQ(bytesRead, std::strlen(writeData));
    readData[bytesRead] = '\0';
    EXPECT_STREQ(readData, writeData);

    device.close();
    EXPECT_FALSE(device.isOpen());
}

TEST(QIODeviceTest, OpenModeTest) {
    TestIODevice device;

    device.open(QIODevice::ReadOnly);
    EXPECT_EQ(device.openMode(), QIODevice::ReadOnly);
    device.close();

    device.open(QIODevice::WriteOnly);
    EXPECT_EQ(device.openMode(), QIODevice::WriteOnly);
    device.close();

    device.open(QIODevice::ReadWrite);
    EXPECT_EQ(device.openMode(), QIODevice::ReadWrite);
    device.close();
}
