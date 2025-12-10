// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_CustomAccess/AccessorPolicies.h>

namespace CppUtils::CustomAccess::AccessorPolicyUtils
{
    template
    <
        class T,
        class Policy
    >
    using GetPolicyCategoryType_t = CppUtils::AccessorPolicies::PolicyTraits<T, Policy>::PolicyCategory_t;
    
    template
    <
        class T,
        class Policy,
        class PolicyCategory
    >
    consteval bool IsPolicyOfPolicyCategory() { return std::is_same_v<GetPolicyCategoryType_t<T, Policy>, PolicyCategory>; }
}

namespace CppUtils::CustomAccess::AccessorPolicyUtils::Detail
{
    template <
        class T,
        template <class, class>
        class PolicyCategory,
        class... Policies>
    struct FindAccessorPolicy;

    template <
        class T,
        template <class, class>
        class PolicyCategory,
        class First,
        class... Rest>
    struct FindAccessorPolicy<T, PolicyCategory, First, Rest...>
    {
        using type = std::conditional_t
        <
            IsPolicyOfPolicyCategory<T, First, PolicyCategory<T, First>>(),
            First,
            typename FindAccessorPolicy<T, PolicyCategory, Rest...>::type
        >;
    };

    template <
        class T,
        template <class, class>
        class PolicyCategory>
    struct FindAccessorPolicy<T, PolicyCategory>
    {
        using type = CppUtils::AccessorPolicies::PolicyCategoryTraits<T, PolicyCategory>::FallbackPolicy; // No policy found
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
        class... AccessorPolicies>
    using GetAccessorPolicyByCategory_T = Detail::FindAccessorPolicy<T, PolicyCategory, AccessorPolicies...>::type;
}
