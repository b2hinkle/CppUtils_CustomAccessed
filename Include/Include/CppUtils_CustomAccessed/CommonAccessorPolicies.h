// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <utility>

/*
* Accessor policies are used to define custom accessor behavior (e.g. getters/setters).
* TODO: We need to support accessors that user other value categories and cv.
*/
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
    * Purpose of this accessor policy is to remove the need to manualy write accessor policy structs. It does this by allowing the externalization of policy function definitions.
    * The policy functions simply forwards execution to optional user functions, essentially overriding default behaviors.
    * User functions are specified via builder functions through function chaining. This way, "argument" order is up to the user and there are no forced argument situations.
    * 
    * Most (if not all) custom accessor policies can be generated using this, but custom accessor policies can always be manually written if that control is needed.
    */
    template <class T>
    struct GenericAccessorPolicy
    {
        // We explicitly declare the special member functions to mark them consteval to guarentee compile-time only lifetime.
        consteval GenericAccessorPolicy() = default;
        consteval GenericAccessorPolicy(const GenericAccessorPolicy&) = default;
        consteval GenericAccessorPolicy(GenericAccessorPolicy&&) noexcept = default;
        consteval GenericAccessorPolicy& operator=(const GenericAccessorPolicy&) = default;
        consteval GenericAccessorPolicy& operator=(GenericAccessorPolicy&&) noexcept = default;

        // Builder functions.
        consteval GenericAccessorPolicy<T>&& SetGetterFuncPtr(const TGetterFuncPtr<T> value) &&
        {
            GetterFuncPtr = value;
            return std::move(*this);
        }
        consteval GenericAccessorPolicy<T>&& SetSetterFuncPtr(const TSetterFuncPtr<T> value) &&
        {
            SetterFuncPtr = value;
            return std::move(*this);
        }

        // Func ptr members.
        TGetterFuncPtr<T> GetterFuncPtr = &BasicGetter<T>;
        TSetterFuncPtr<T> SetterFuncPtr = &BasicSetter<T>;
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
