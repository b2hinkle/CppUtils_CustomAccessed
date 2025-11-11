// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

// Accessor policies are used to define custom accessor behavior.
// TODO: We should provide default accessor behavior for all value categories and cv.
namespace CppUtils::CommonAccessorPolicies
{
    template <class T>
    static inline const T& BasicGet(const T& value) { return value; }

    template <class T>
    static inline void BasicSet(T& value, const T& newValue) { value = newValue; }

    template <class T>
    using TGetterFuncPtr = const T& (*)(const T& value);

    template <class T>
    using TSetterFuncPtr = void (*)(T& value, const T& newValue);

    /*
    * Pourpose of this policy is to allow for externalization of behavior when creating an accessor policy.
    * The policy is simply generated with the func ptrs to forward its call to. Cleaner api for user to work with.
    */
    template <
        class T,
        TGetterFuncPtr<T> GetFuncPtr = &CppUtils::CommonAccessorPolicies::BasicGet<T>,
        TSetterFuncPtr<T> SetFuncPtr = &CppUtils::CommonAccessorPolicies::BasicSet<T>
    >
    struct GenericAccessorPolicy
    {
        static inline const T& Get(const T& value) { return GetFuncPtr(value); }
        static inline void Set(T& value, const T& newValue) { SetFuncPtr(value, newValue); }
    };

}

#if 0 // TODO: This should be a reusable broadcast on set function for convenience.
    // TODO: It would be nice to have another function that even broadcasts when valueRef is same.
    template <class T>
    void BroadcastOnSet(T& value, const T& newValue)
    {
        const T oldBackingValue = value;
        value = newBackingValue;

        if (newBackingValue != oldBackingValue)
        {
#if 0
            ValueChangeDelegate.Broadcast(oldBackingValue, newBackingValue);
#endif
        }
    }
#endif
