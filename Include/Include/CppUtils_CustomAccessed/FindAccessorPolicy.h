// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_CustomAccessed/CommonAccessorPolicies.h>

namespace CppUtils::AccessorPolicyUtils::Detail
{
    template <
        class T,
        template <class, class>
        class PolicyToFind,
        class... Policies>
    struct FindAccessPolicy;

    template <
        class T,
        template <class, class>
        class PolicyToFind,
        class First,
        class... Rest>
    struct FindAccessPolicy<T, PolicyToFind, First, Rest...>
    {
        using type = std::conditional_t
        <
            std::is_base_of_v
            <
                PolicyToFind<T, First>,
                First
            >,
            First,
            typename FindAccessPolicy<T, PolicyToFind, Rest...>::type
        >;
    };

    template <
        class T,
        template <class, class>
        class PolicyToFind>
    struct FindAccessPolicy<T, PolicyToFind>
    {
        using type = void; // No policy found
    };
}

namespace CppUtils::AccessorPolicyUtils
{
    template <
        class T,
        template <class, class>
        class PolicyToFind,
        class... AccessorPolicies>
    using FindAccessPolicy_T = Detail::FindAccessPolicy<T, PolicyToFind, AccessorPolicies...>::type;

}