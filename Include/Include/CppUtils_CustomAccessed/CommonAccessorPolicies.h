// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

// Accessor policies are used to define custom accessor behavior.
// TODO: We need to support accessors that user other value categories and cv.
namespace CppUtils::CommonAccessorPolicies
{
    template <class T>
    static inline const T& BasicGetter(const T& value) { return value; }

    template <class T>
    static inline void BasicSetter(T& value, const T& newValue) { value = newValue; }

    template <class T>
    using TGetterFuncPtr = const T& (*)(const T& value);

    template <class T>
    using TSetterFuncPtr = void (*)(T& value, const T& newValue);

    /*
    * Pourpose of this policy is to allow for externalization of behavior when creating an accessor policy.
    * The policy is simply generated with the func ptrs to forward its call to. Cleaner api for user to work with.
    * 
    * TODO: Solve required order for function ptrs.
    */
    template <
        class T,
        TGetterFuncPtr<T> GetterFuncPtr = &CppUtils::CommonAccessorPolicies::BasicGetter<T>,
        TSetterFuncPtr<T> SetterFuncPtr = &CppUtils::CommonAccessorPolicies::BasicSetter<T>
    >
    struct GenericAccessorPolicy
    {
        // Guarentee compile-time instantiation by only providing consteval construct.
        consteval GenericAccessorPolicy() = default;
        GenericAccessorPolicy(const GenericAccessorPolicy&) = delete;
        GenericAccessorPolicy(GenericAccessorPolicy&&) noexcept = delete;
        GenericAccessorPolicy& operator=(const GenericAccessorPolicy&) = delete;
        GenericAccessorPolicy& operator=(GenericAccessorPolicy&&) noexcept = delete;

        static inline const T& CallGetter(const T& value)              { return GetterFuncPtr(value); }
        static inline void     CallSetter(T& value, const T& newValue) { SetterFuncPtr(value, newValue); }
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
