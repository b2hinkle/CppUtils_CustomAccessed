// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_CustomAccess/AccessorPolicies.h>

namespace CppUtils::CustomAccess::AccessorPolicyUtils::Detail
{
    template <
        class T,
        template <class, class>
        class PolicyToFind,
        class FallbackPolicy,
        class... Policies>
    struct FindAccessorPolicy;

    template <
        class T,
        template <class, class>
        class PolicyToFind,
        class FallbackPolicy,
        class First,
        class... Rest>
    struct FindAccessorPolicy<T, PolicyToFind, FallbackPolicy, First, Rest...>
    {
        using type = std::conditional_t
        <
            std::is_base_of_v
            <
                PolicyToFind<T, First>,
                First
            >,
            First,
            typename FindAccessorPolicy<T, PolicyToFind, FallbackPolicy, Rest...>::type
        >;
    };

    template <
        class T,
        template <class, class>
        class PolicyToFind,
        class FallbackPolicy>
    struct FindAccessorPolicy<T, PolicyToFind, FallbackPolicy>
    {
        using type = FallbackPolicy; // No policy found
    };
}

namespace CppUtils::CustomAccess::AccessorPolicyUtils
{
    /*
    * Finds the first accessor policy of type `PolicyToFind` in the provided `AccessorPolicies...`. Returns `FallbackPolicy` if no such policy is found.
    */
    template <
        class T,
        template <class, class>
        class PolicyToFind,
        class    FallbackPolicy,
        class... AccessorPolicies>
    using FindAccessorPolicyWithFallback_T = Detail::FindAccessorPolicy<T, PolicyToFind, FallbackPolicy, AccessorPolicies...>::type;

    /*
    * Finds the first accessor policy of type `PolicyToFind` in the provided `AccessorPolicies...`. Returns `void` if no such policy is found.
    */
    template <
        class T,
        template <class, class>
        class PolicyToFind,
        class... AccessorPolicies>
    using FindAccessorPolicy_T = FindAccessorPolicyWithFallback_T<T, PolicyToFind, void, AccessorPolicies...>::type;
}
