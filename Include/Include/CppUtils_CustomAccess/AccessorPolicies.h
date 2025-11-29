// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <type_traits>
#include <CppUtils_CustomAccess/FunctionTraits.h>
#include <CppUtils_CustomAccess/AccessorPolicyCategories.h>
#include <CppUtils_CustomAccess/AccessorPolicyUtils.h>

/*
* Accessor policies are used to define custom accessor behavior (e.g. getters/setters).
* TODO: We need to support accessors that user other value categories and cv.
* TODO: How do we want to handle basic get and set accessor policies. If we have a basic policy, it should not assume T is copyable or moveabe.
*       This should be detectable, but at the end of the day, doing so would make it more of a black box for the user, so it's probable that this wouldn't be worth it.
*/
namespace CppUtils::AccessorPolicies
{
    template <auto TFunc>
    concept TCallable = requires { typename CppUtils::CustomAccess::FunctionPointerTraits<decltype(TFunc)>; };

#if 0
    // TODO: Finish implementation
    template <auto TFunc>
    concept TGetterCallable = TCallable<TFunc> && /* Only 1 arg and its type matches return type */;

    // TODO: Finish implementation
    template <auto TFunc>
    concept TSetterCallable = TCallable<TFunc> && /* Only 1 arg */;
#endif

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
    struct PolicyTraits<T, BasicGetterAccessorPolicy<T>>
    {
        using PolicyCategory_t = PolicyCategory_Getter<T, BasicGetterAccessorPolicy<T>>;
    };

    /*
    * Option for `Get` policy function definition externalization.
    */
    template
    <
        auto GetterFuncPtr
    >
    requires (TCallable<GetterFuncPtr> /* TODO: Use TGetterCallable when implemented */)
    struct GenericGetterAccessorPolicy
    {
        using GetterFuncPtrTraits = CppUtils::CustomAccess::FunctionPointerTraits<decltype(GetterFuncPtr)>;
        using ReturnType = GetterFuncPtrTraits::ReturnType;
        //using ClassType  = GetterFuncPtrTraits::ClassType; TODO: Support member function pointers so that we can support the outer object's functions.
        using FirstArg = std::tuple_element_t<0, typename GetterFuncPtrTraits::ArgsTuple>;

        using ReturnValueType = std::remove_cv_t<std::remove_reference_t<ReturnType>>;

        static inline const ReturnValueType& Get(const ReturnValueType& value)
            requires
            (
                std::is_lvalue_reference_v<ReturnType> &&
                CppUtils::CustomAccess::IsConstAfterRemovingRef  <ReturnType>() &&
                std::is_lvalue_reference_v<FirstArg>   &&
                CppUtils::CustomAccess::IsConstAfterRemovingRef  <FirstArg>()
            )
        {
            return GetterFuncPtr(value);
        }

        static inline ReturnValueType Get(const ReturnValueType& value)
            requires
            (
                !std::is_reference_v       <ReturnType> &&
                !CppUtils::CustomAccess::IsConstAfterRemovingRef  <ReturnType>() &&
                 std::is_lvalue_reference_v<FirstArg>   &&
                 CppUtils::CustomAccess::IsConstAfterRemovingRef  <FirstArg>()
            )
        {
            return GetterFuncPtr(value);
        }

        static inline ReturnValueType Get(ReturnValueType value)
            requires
            (
                !std::is_reference_v     <ReturnType> &&
                !CppUtils::CustomAccess::IsConstAfterRemovingRef<ReturnType>() &&
                !std::is_reference_v     <FirstArg>   &&
                !CppUtils::CustomAccess::IsConstAfterRemovingRef<FirstArg>()
            )
        {
            return GetterFuncPtr(value);
        }
    };

    template
    <
        class T,
        auto GetterFuncPtr
    >
    struct PolicyTraits<T, GenericGetterAccessorPolicy<GetterFuncPtr>>
    {
        using PolicyCategory_t = PolicyCategory_Getter<T, GenericGetterAccessorPolicy<GetterFuncPtr>>;
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
    struct PolicyTraits<T, BasicSetterAccessorPolicy<T>>
    {
        using PolicyCategory_t = PolicyCategory_Setter<T, BasicSetterAccessorPolicy<T>>;
    };

    /*
    * Option for `Set` policy function definition externalization.
    */
    template
    <
        auto SetterFuncPtr // TODO: We should make this variatic so user can define both copy and move. Only 2 possible args, but variatic for order agnostic.
    >
    requires (TCallable<SetterFuncPtr> /* TODO: Use TSetterCallable when implemented */)
    struct GenericSetterAccessorPolicy
    {
        using SetterFuncPtrTraits = CppUtils::CustomAccess::FunctionPointerTraits<decltype(SetterFuncPtr)>;
        using ReturnType = SetterFuncPtrTraits::ReturnType;
        //using ClassType  = SetterFuncPtrTraits::ClassType; TODO: Support member function pointers so that we can support the outer object's functions.
        using ArgsTuple = SetterFuncPtrTraits::ArgsTuple;
        using FirstArg  = std::tuple_element_t<0, ArgsTuple>;
        using SecondArg = std::tuple_element_t<1, ArgsTuple>;

        using FirstArgValueType = std::remove_cv_t<std::remove_reference_t<FirstArg>>;

        static consteval bool IsFirstArgValid()
        {
            return std::is_lvalue_reference_v<FirstArg> &&
                   !CppUtils::CustomAccess::IsConstAfterRemovingRef <FirstArg>();
        }

        // Copy sets.

        static inline void Set(FirstArgValueType& value, const FirstArgValueType& newValue)
            requires
            (
                IsFirstArgValid()                     &&
                std::is_lvalue_reference_v<SecondArg> &&
                CppUtils::CustomAccess::IsConstAfterRemovingRef  <SecondArg>()
            )
        {
            SetterFuncPtr(value, newValue);
        }

        static inline void Set(FirstArgValueType& value, FirstArgValueType newValue)
            requires
            (
                IsFirstArgValid()                    &&
                !std::is_reference_v     <SecondArg> &&
                !std::is_const_v<SecondArg>
            )
        {
            SetterFuncPtr(value, newValue);
        }

        // Move set.
        static inline void Set(FirstArgValueType& value, FirstArgValueType&& newValue)
            requires
            (
                IsFirstArgValid()                     &&
                std::is_rvalue_reference_v<SecondArg> &&
                !CppUtils::CustomAccess::IsConstAfterRemovingRef <SecondArg>()
            )
        {
            SetterFuncPtr(value, std::move(newValue));
        }
    };

    template
    <
        class T,
        auto SetterFuncPtr
    >
    struct PolicyTraits<T, GenericSetterAccessorPolicy<SetterFuncPtr>>
    {
        using PolicyCategory_t = PolicyCategory_Setter<T, GenericSetterAccessorPolicy<SetterFuncPtr>>;
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
