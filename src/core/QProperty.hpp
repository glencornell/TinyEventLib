#pragma once

#include "QSignal.hpp"

template<typename T>
class QProperty {
public:
    QProperty() = default;
    QProperty(const T& value) : m_value(value) {}
    QProperty(T&& value) : m_value(std::move(value)) {}

    const T& value() const {
        return m_value;
    }

    void setValue(const T& value) {
        if (m_value != value) {
            m_value = value;
            onValueChanged(m_value);
        }
    }

    void setValue(T&& value) {
        if (m_value != value) {
            m_value = std::move(value);
            onValueChanged(m_value);
        }
    }

    QProperty& operator=(const T& value) {
        setValue(value);
        return *this;
    }

    QProperty& operator=(T&& value) {
        setValue(std::move(value));
        return *this;
    }

    QSignal<const T&> onValueChanged;

private:
    T m_value;
};
