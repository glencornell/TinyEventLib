#pragma once

#include <functional>
#include <utility>
#include <type_traits>
#include <cassert>

namespace detail {

// Type-erased base class for the method invoker
template <typename Signature>
class ErasedMethodInvokerBase;

template <typename Ret, typename... Args>
class ErasedMethodInvokerBase<Ret(Args...)> {
public:
    virtual ~ErasedMethodInvokerBase() = default;

    virtual Ret invoke(Args... args) const = 0;
    virtual ErasedMethodInvokerBase* clone(void* storage) const = 0;
    virtual bool equals(const ErasedMethodInvokerBase<Ret(Args...)>* other) const = 0;
};

// Implementation for non-const member functions
template <typename Class, typename Ret, typename... Args>
class MethodInvokerImpl : public ErasedMethodInvokerBase<Ret(Args...)> {
public:
    MethodInvokerImpl(Class* instance, Ret (Class::*method)(Args...))
        : instance_(instance), method_(method) {}

    Ret invoke(Args... args) const override {
        return std::invoke(method_, instance_, std::forward<Args>(args)...);
    }

    ErasedMethodInvokerBase<Ret(Args...)>* clone(void* storage) const override {
        return new (storage) MethodInvokerImpl(*this);
    }

    bool equals(const ErasedMethodInvokerBase<Ret(Args...)>* other) const override {
        auto otherImpl = dynamic_cast<const MethodInvokerImpl*>(other);
        return otherImpl && otherImpl->instance_ == instance_ && otherImpl->method_ == method_;
    }

private:
    Class* instance_;
    Ret (Class::*method_)(Args...);
};

// Implementation for const member functions
template <typename Class, typename Ret, typename... Args>
class MethodInvokerImpl<const Class, Ret, Args...> : public ErasedMethodInvokerBase<Ret(Args...)> {
public:
    MethodInvokerImpl(const Class* instance, Ret (Class::*method)(Args...) const)
        : instance_(instance), method_(method) {}

    Ret invoke(Args... args) const override {
        return std::invoke(method_, instance_, std::forward<Args>(args)...);
    }

    ErasedMethodInvokerBase<Ret(Args...)>* clone(void* storage) const override {
        return new (storage) MethodInvokerImpl(*this);
    }

    bool equals(const ErasedMethodInvokerBase<Ret(Args...)>* other) const override {
        auto otherImpl = dynamic_cast<const MethodInvokerImpl*>(other);
        return otherImpl && otherImpl->instance_ == instance_ && otherImpl->method_ == method_;
    }

private:
    const Class* instance_;
    Ret (Class::*method_)(Args...) const;
};

} // namespace detail

// Type-erased holder for invokers
template <typename Signature>
class method_ref;

template <typename Ret, typename... Args>
class method_ref<Ret(Args...)> {
private:
    using Base = detail::ErasedMethodInvokerBase<Ret(Args...)>;

    // Determine the maximum size required for aligned storage using a dummy class to calculate storage size
    struct DummyClass {};
    static constexpr std::size_t StorageSize =
        std::max(sizeof(detail::MethodInvokerImpl<DummyClass, Ret, Args...>),
                 sizeof(detail::MethodInvokerImpl<const DummyClass, Ret, Args...>));
    using AlignedStorage = std::aligned_storage_t<StorageSize, alignof(Base)>;

public:
    method_ref() : invoker_(nullptr) {}

    // Non-const constructor
    template <typename Class>
    method_ref(Class* instance, Ret (Class::*method)(Args...)) {
        invoker_ = new (&storage_) detail::MethodInvokerImpl<Class, Ret, Args...>(instance, method);
    }

    // Const constructor
    template <typename Class>
    method_ref(const Class* instance, Ret (Class::*method)(Args...) const) {
        invoker_ = new (&storage_) detail::MethodInvokerImpl<const Class, Ret, Args...>(instance, method);
    }

    // Copy constructor
    method_ref(const method_ref& other) {
        if (other.invoker_) {
            invoker_ = other.invoker_->clone(&storage_);
        } else {
            invoker_ = nullptr;
        }
    }

    // Move constructor
    method_ref(method_ref&& other) noexcept : invoker_(nullptr) {
        if (other.invoker_) {
            invoker_ = other.invoker_->clone(&storage_);
            other.invoker_ = nullptr; // Nullify the source's invoker
        }
    }

    // Copy assignment operator
    method_ref& operator=(const method_ref& other) {
        if (this != &other) {
            if (invoker_) {
                invoker_->~ErasedMethodInvokerBase();
            }
            if (other.invoker_) {
                invoker_ = other.invoker_->clone(&storage_);
            } else {
                invoker_ = nullptr;
            }
        }
        return *this;
    }

    // Move assignment operator
    method_ref& operator=(method_ref&& other) noexcept {
        if (this != &other) {
            if (invoker_) {
                invoker_->~ErasedMethodInvokerBase();
            }
            if (other.invoker_) {
                invoker_ = other.invoker_->clone(&storage_);
                other.invoker_ = nullptr; // Nullify the source's invoker
            } else {
                invoker_ = nullptr;
            }
        }
        return *this;
    }

    // Destructor
    ~method_ref() {
        if (invoker_) {
            invoker_->~ErasedMethodInvokerBase();
        }
    }

    Ret operator()(Args... args) const {
        assert(invoker_ && "method_ref: Attempt to call an uninitialized invoker.");
        return invoker_->invoke(std::forward<Args>(args)...);
    }

    bool operator==(const method_ref& other) const {
        if (!invoker_ || !other.invoker_) {
            return invoker_ == other.invoker_;
        }
        return invoker_->equals(other.invoker_);
    }

    bool operator!=(const method_ref& other) const {
        return !(*this == other);
    }

private:
    AlignedStorage storage_;
    Base* invoker_;
};
