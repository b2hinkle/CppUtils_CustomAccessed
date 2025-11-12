// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <utility>

/*
* Accessor policies are used to define custom accessor behavior (e.g. getters/setters).
* TODO: We need to support accessors that user other value categories and cv.
*/
namespace CppUtils::AccessorPolicies
{
    template <class T>
    static inline const T& BasicGetter(const T& value) { return value; }

    template <class T>
    static inline void BasicSetter(T& value, const T& newValue) { value = newValue; }

    template <class T>
    using TGetterFuncPtr = const T& (*)(const T& value);

    template <class T>
    using TSetterFuncPtr = void (*)(T& value, const T& newValue);

    template <
        class T,
        class TDerived
    >
    struct GetterAccessorPolicy
    {
        static inline const T& Get(const T& value) { return TDerived::Get(value); }
    };

    /*
    * Option for `Get` policy function definition externalization.
    */
    template <
        class T,
        TGetterFuncPtr<T> GetterFuncPtr = &CppUtils::AccessorPolicies::BasicGetter<T>
        >
    struct GenericGetterAccessorPolicy : GetterAccessorPolicy<T, GenericGetterAccessorPolicy<T, GetterFuncPtr>>
    {
        static inline const T& Get(const T& value) { return GetterFuncPtr(value); }
    };

    template <
        class T,
        class TDerived
    >
    struct SetterAccessorPolicy
    {
        static inline void Set(T& value, const T& newValue) { return TDerived::Set(value); }
    };

    /*
    * Option for `Set` policy function definition externalization.
    */
    template <
        class T,
        TSetterFuncPtr<T> SetterFuncPtr = &CppUtils::AccessorPolicies::BasicSetter<T>
        >
    struct GenericSetterAccessorPolicy : SetterAccessorPolicy<T, GenericSetterAccessorPolicy<T, SetterFuncPtr>>
    {
        static inline void Set(T& value, const T& newValue) { SetterFuncPtr(value, newValue); }
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
