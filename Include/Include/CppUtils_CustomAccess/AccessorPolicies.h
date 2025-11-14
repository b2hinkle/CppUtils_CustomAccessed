// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <utility>
#include <CppUtils_CustomAccess/AccessorPolicyInterfaces.h>

/*
* Accessor policies are used to define custom accessor behavior (e.g. getters/setters).
* TODO: We need to support accessors that user other value categories and cv.
*/
namespace CppUtils::AccessorPolicies
{
    template <class T>
    using TGetterFuncPtr = const T& (*)(const T& value);

    template <class T>
    using TSetterFuncPtr = void (*)(T& value, const T& newValue);

    /*
    * Basic getter behavior, used as default behavior.
    */
    template <class T>
    struct BasicGetterAccessorPolicy
    {
        static inline const T& Get(const T& value) { return value; }
    };

    template <class T>
    struct PolicyCategoryTraits<T, PolicyCategory_Getter>
    {
        using FallbackPolicy = BasicGetterAccessorPolicy<T>;
    };

    template <class T>
    struct PolicyTraits<BasicGetterAccessorPolicy<T>>
    {
        using PolicyCategory_t = PolicyCategory_Getter<T, BasicGetterAccessorPolicy<T>>;
    };

    /*
    * Option for `Get` policy function definition externalization.
    */
    template <
        class T,
        TGetterFuncPtr<T> GetterFuncPtr
        >
    struct GenericGetterAccessorPolicy
    {
        static inline const T& Get(const T& value) { return GetterFuncPtr(value); }
    };

    template
    <
        class T,
        TGetterFuncPtr<T> GetterFuncPtr
    >
    struct PolicyTraits<GenericGetterAccessorPolicy<T, GetterFuncPtr>>
    {
        using PolicyCategory_t = PolicyCategory_Getter<T, GenericGetterAccessorPolicy<T, GetterFuncPtr>>;
    };

    /*
    * Basic setter behavior, used as default behavior.
    */
    template <class T>
    struct BasicSetterAccessorPolicy
    {
        static inline void Set(T& value, const T& newValue) { value = newValue; }
    };

    template <class T>
    struct PolicyCategoryTraits<T, PolicyCategory_Setter>
    {
        using FallbackPolicy = BasicSetterAccessorPolicy<T>;
    };

    template <class T>
    struct PolicyTraits<BasicSetterAccessorPolicy<T>>
    {
        using PolicyCategory_t = PolicyCategory_Setter<T, BasicSetterAccessorPolicy<T>>;
    };

    /*
    * Option for `Set` policy function definition externalization.
    */
    template <
        class T,
        TSetterFuncPtr<T> SetterFuncPtr
        >
    struct GenericSetterAccessorPolicy
    {
        static inline void Set(T& value, const T& newValue) { SetterFuncPtr(value, newValue); }
    };

    template
    <
        class T,
        TSetterFuncPtr<T> SetterFuncPtr
    >
    struct PolicyTraits<GenericSetterAccessorPolicy<T, SetterFuncPtr>>
    {
        using PolicyCategory_t = PolicyCategory_Setter<T, GenericSetterAccessorPolicy<T, SetterFuncPtr>>;
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
