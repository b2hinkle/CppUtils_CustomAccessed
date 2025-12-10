// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <type_traits>
#include <CppUtils_CustomAccess/AccessorPolicy_NullGetter.h>
#include <CppUtils_CustomAccess/AccessorPolicy_NullSetter.h>
#include <CppUtils_CustomAccess/AccessorPolicyUtils.h>


/*
* 
*/
namespace CppUtils::AccessorPolicies
{
    /*
    * Static interface for getter accessor policies. Enforces requirements, and dispatches to getter policies.
    */
    template
    <
        class T,
        class AccessorPolicy
    >
    struct AccessorPolicyStaticInterface_Getter
    {
        static_assert ( requires { typename AccessorPolicy::ReturnType; }, "The accessor policy must provide a using declaration for the return type.");
        static_assert ( requires { typename AccessorPolicy::FirstArg; },   "The accessor policy must provide a using declaration for the first parameter type.");

        using ReturnType = typename AccessorPolicy::ReturnType;
        using FirstArg   = typename AccessorPolicy::FirstArg;

        static_assert(std::is_invocable_v<decltype(AccessorPolicy::Get), FirstArg>, "The policy class is missing this function.");

        static_assert(std::is_same_v
        <
            std::remove_cvref_t<ReturnType>,
            T
        >,
        "Return value type must match the value type of the encapsulated data. Note that cv and ref qualifiers don't impact value type.");
        
        static_assert(std::is_same_v
        <
            std::remove_cvref_t<FirstArg>,
            T
        >,
        "First parameter value type must match the value type of the encapsulated data, since the encapsulated data is provided as the first argument. Note that cv and ref qualifiers don't impact value type.");

        static_assert(
            (!std::is_reference_v<FirstArg>)
            ||
            (std::is_reference_v<FirstArg> && CppUtils::IsConstAfterRemovingRef<FirstArg>()),
            "Reference as the first parameter is only allowed if its value type is made const, since the encapsulated data is provided as this first argument.");

        static_assert(
            !std::is_rvalue_reference_v<FirstArg>,
            "First parameter should not be an rvalue reference. It does not make sense to steal the data of the backing value through a getter, nor does it make sense to prefer a const rvalue ref over a const lvalue ref."
            );

        static inline ReturnType Get(FirstArg value)
            requires (!std::is_same_v<AccessorPolicy, AccessorPolicy_NullGetter<T>>)
        {
            return AccessorPolicy::Get(value);
        }
    };

    /*
    * Static interface for setter accessor policies. Enforces requirements, and dispatches to setter policies.
    */
    template
    <
        class T,
        class AccessorPolicy
    >
    struct AccessorPolicyStaticInterface_Setter
    {
        static_assert ( requires { typename AccessorPolicy::FirstArg; },  "The accessor policy must provide a using declaration for the first parameter type.");
        static_assert ( requires { typename AccessorPolicy::SecondArg; }, "The accessor policy must provide a using declaration for the second parameter type.");

        using FirstArg  = typename AccessorPolicy::FirstArg;
        using SecondArg = typename AccessorPolicy::SecondArg;

        static_assert(std::is_lvalue_reference_v<FirstArg> && !CppUtils::IsConstAfterRemovingRef<FirstArg>(), "First parameter must be a non-const lvalue reference. Otherwise, the callback doesn't make sense.");

        static_assert // TODO: Test this
        (
            (std::is_rvalue_reference_v<SecondArg> && !CppUtils::IsConstAfterRemovingRef<SecondArg>())
            ||
            !std::is_rvalue_reference_v<SecondArg>,
            "If using an rvalue reference as the second parameter, its value type should be non-const. Otherwise, the callback doesn't make sense."
        );

        static_assert
        (
            (std::is_lvalue_reference_v<SecondArg> && CppUtils::IsConstAfterRemovingRef<SecondArg>())
            ||
            !std::is_lvalue_reference_v<SecondArg>,
            "If second parameter is an lvalue reference, its value type must be const. Otherwise, we can't guarantee that the setter argument won't be modified."
        );

        static_assert(std::is_same_v
        <
            std::remove_cvref_t<FirstArg>,
            T
        >,
        "First parameter value type must match the value type of the encapsulated data, since the encapsulated data is provided as the first argument. Note that cv and ref qualifiers don't impact value type.");
        
        // We're require explicitly for now since it's only more predictable, but we could change in the future if we find a reason. 
        static_assert(std::is_same_v
        <
            std::remove_cvref_t<SecondArg>,
            T
        >,
        "Second parameter value type must match the value type of the encapsulated data, since it's used for setting the encapsulated data. Note that cv and ref qualifiers don't impact value type.");

        


        

        static_assert(std::is_invocable_v<decltype(AccessorPolicy::Set), FirstArg, SecondArg>, "The policy class is missing its set function.");
        
        /*
        * 
        */
        static inline void Set(FirstArg value, SecondArg newValue)
            requires (!std::is_same_v<AccessorPolicy, AccessorPolicy_NullSetter<T>>)
        {
            if constexpr (std::is_rvalue_reference_v<SecondArg>)
            {
                // Non-const rvalue reference. Non-const because that's already been asserted.
                AccessorPolicy::Set(value, std::move(newValue));
            }
            else
            {
                // Either const lvalue reference or const/non-const copy.
                AccessorPolicy::Set(value, newValue);
            }
        }
    };
}
