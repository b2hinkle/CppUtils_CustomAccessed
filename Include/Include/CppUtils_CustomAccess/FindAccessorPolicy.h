// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_CustomAccess/AccessorPolicies.h>

namespace CppUtils::CustomAccess::AccessorPolicyUtils::Detail
{
    template <class Policy>
    using GetPolicyCategoryType_t = CppUtils::AccessorPolicies::PolicyTraits<Policy>::PolicyCategory_t;

    template
    <
        class Policy,
        class PolicyCategory
    >
    consteval bool IsPolicyOfPolicyCategory() { return std::is_same_v<GetPolicyCategoryType_t<Policy>, PolicyCategory>; }

    template <
        class T,
        template <class, class>
        class PolicyCategory,
        class FallbackPolicy,
        class... Policies>
    struct FindAccessorPolicy;

    template <
        class T,
        template <class, class>
        class PolicyCategory,
        class FallbackPolicy,
        class First,
        class... Rest>
    struct FindAccessorPolicy<T, PolicyCategory, FallbackPolicy, First, Rest...>
    {
        using type = std::conditional_t
        <
            IsPolicyOfPolicyCategory<First, PolicyCategory<T, First>>(),
            First,
            typename FindAccessorPolicy<T, PolicyCategory, FallbackPolicy, Rest...>::type
        >;
    };

    template <
        class T,
        template <class, class>
        class PolicyCategory,
        class FallbackPolicy>
    struct FindAccessorPolicy<T, PolicyCategory, FallbackPolicy>
    {
        using type = FallbackPolicy; // No policy found
    };
}

namespace CppUtils::CustomAccess::AccessorPolicyUtils
{
    /*
    * Finds the first accessor policy of type `PolicyCategory` in the provided `AccessorPolicies...`. Returns `FallbackPolicy` if no such policy is found.
    */
    template <
        class T,
        template <class, class>
        class PolicyCategory,
        class    FallbackPolicy,
        class... AccessorPolicies>
    using FindAccessorPolicyWithFallback_T = Detail::FindAccessorPolicy<T, PolicyCategory, FallbackPolicy, AccessorPolicies...>::type;

    /*
    * Finds the first accessor policy by `PolicyCategory` in the provided `AccessorPolicies...`. Returns `void` if no such policy is found.
    */
    template <
        class T,
        template <class, class>
        class PolicyCategory,
        class... AccessorPolicies>
    using FindAccessorPolicy_T = FindAccessorPolicyWithFallback_T<T, PolicyCategory, void, AccessorPolicies...>::type;
}
