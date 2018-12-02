#pragma once

#include <utility>

template <class T>
class PtrObserver;

template <class T>
class PtrOwner
{
    template<class S> 
    friend class PtrObserver;

public:
    constexpr PtrOwner() = default;
    constexpr PtrOwner(std::nullptr_t);
    template <class... TArgs> explicit constexpr PtrOwner(TArgs&&... _args);
    template<class S, ENABLE_IF(IS_BASE_OF(S, T))> constexpr PtrOwner<T>& operator=(PtrOwner<S>&& _other) noexcept;
    inline ~PtrOwner();

    constexpr PtrObserver<T> get() const;
    //std::add_lvalue_reference_t<T> operator*() const;
    constexpr T* operator->() const noexcept;

private:
    T* m_ptr = nullptr;
};

template <class T>
class PtrObserver
{
public:
    explicit constexpr PtrObserver(const PtrOwner<T>& _owner);
    
    constexpr operator T*() const;
    //constexpr T* get();

private:
    T* m_ptr;
};

template<class T>
template<class ...TArgs>
inline constexpr PtrOwner<T>::PtrOwner(TArgs&&..._args)
    : m_ptr(new T(std::forward<TArgs>(_args)...))
{
}

template<class T>
template<class S, class>
inline constexpr PtrOwner<T>& PtrOwner<T>::operator=(PtrOwner<S>&& _other) noexcept
{
    m_ptr = std::exchange(_other.m_ptr, nullptr);
    return *this;
}

template<class T>
inline constexpr PtrOwner<T>::PtrOwner(std::nullptr_t)
    : m_ptr(nullptr)
{
}

template<class T>
inline PtrOwner<T>::~PtrOwner()
{
    delete m_ptr;
}

template<class T>
inline constexpr PtrObserver<T> PtrOwner<T>::get() const
{
    return PtrObserver<T>(m_ptr);
}

template<class T>
inline constexpr T* PtrOwner<T>::operator->() const noexcept
{
    return m_ptr;
}

template<class T>
inline constexpr PtrObserver<T>::PtrObserver(const PtrOwner<T>& _owner)
    : m_ptr(_owner.m_ptr)
{
}

template <class T>
inline constexpr PtrObserver<T>::operator T*() const
{
    return m_ptr;
}
