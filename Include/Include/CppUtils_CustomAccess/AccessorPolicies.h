// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <type_traits>
#include <CppUtils_CustomAccess/FunctionTraits.h>
#include <CppUtils_CustomAccess/PolicyAndPolicyCategoryTraits_PrimaryTemplates.h>
#include <CppUtils_CustomAccess/AccessorPolicyCategories.h>
#include <CppUtils_CustomAccess/AccessorPolicyUtils.h>

/*
* Accessor policies are used to define custom accessor behavior (e.g. getters/setters). Must specialize a `PolicyTraits` with its policy category `PolicyCategory_t` for it to be findable by `CustomAccessed`.
*/
namespace CppUtils::AccessorPolicies
{
    /*
    * Getter accessor policy for function definition externalization.
    */
    template
    <
        auto UserCallablePtr
    >
    struct GenericGetterAccessorPolicy
    {
        static_assert(CppUtils::CustomAccess::TCallable<UserCallablePtr>, "We expect a user defined callable pointer as the non-type template argument.");

        using UserCallablePtrTraits = CppUtils::CustomAccess::FunctionPointerTraits<decltype(UserCallablePtr)>;
        
        static_assert(std::tuple_size_v<typename UserCallablePtrTraits::ArgsTuple> == 1, "Callable must have 1 argument.");

        using ReturnType = typename UserCallablePtrTraits::ReturnType;
        using FirstArg = std::tuple_element_t<0, typename UserCallablePtrTraits::ArgsTuple>;

        //using ClassType  = UserCallablePtrTraits::ClassType; TODO: Support member function pointers so that we can support the outer object's functions.

        /*
        * Inline to avoid unnecesary copy in the case of non-ref parameter.
        * No need to account for perfect forwarding. No possible rvalue references to account for, given the enforced type of the user's callable.
        */
        static inline ReturnType Get(FirstArg value)
        {
            return UserCallablePtr(value);
        }
    };

    template
    <
        class T,
        auto UserCallablePtr
    >
    struct PolicyTraits<T, GenericGetterAccessorPolicy<UserCallablePtr>>
    {
        using PolicyCategory_t = PolicyCategory_Getter<T, GenericGetterAccessorPolicy<UserCallablePtr>>;
    };

    /*
    * Option for `Set` policy function definition externalization.
    */
    template
    <
        auto UserCallablePtr // TODO: We should make this variatic so user can define both copy and move. Only 2 possible args, but variatic for order agnostic.
    >
    struct GenericSetterAccessorPolicy
    {
        static_assert(CppUtils::CustomAccess::TCallable<UserCallablePtr>, "We expect a user defined callable pointer as the non-type template argument.");

        using UserCallablePtrTraits = CppUtils::CustomAccess::FunctionPointerTraits<decltype(UserCallablePtr)>;
        
        static_assert(std::tuple_size_v<typename UserCallablePtrTraits::ArgsTuple> == 2, "Callable must have 2 arguments.");

        using FirstArg  = std::tuple_element_t<0, typename UserCallablePtrTraits::ArgsTuple>;
        using SecondArg = std::tuple_element_t<1, typename UserCallablePtrTraits::ArgsTuple>;

        //using ClassType  = UserCallablePtrTraits::ClassType; TODO: Support member function pointers so that we can support the outer object's functions.



        /*
        * 
        */
        static inline void Set(FirstArg value, SecondArg newValue)
        {
            if constexpr (std::is_rvalue_reference_v<SecondArg>)
            {
                // Non-const rvalue reference. Non-const because that's already been asserted.
                UserCallablePtr(value, std::move(newValue));
            }
            else
            {
                // Either const lvalue reference or const/non-const copy.
                UserCallablePtr(value, newValue);
            }
        }
    };

    template
    <
        class T,
        auto UserCallablePtr
    >
    struct PolicyTraits<T, GenericSetterAccessorPolicy<UserCallablePtr>>
    {
        using PolicyCategory_t = PolicyCategory_Setter<T, GenericSetterAccessorPolicy<UserCallablePtr>>;
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
