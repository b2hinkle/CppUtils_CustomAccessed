// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_CustomAccess/AccessorPolicies.h>

namespace CppUtils::AccessorPolicyUtils
{
    template
    <
        class T,
        class AccessorPolicy
    >
    using GetAccessorPolicyStaticInterface_t = AccessorPolicyTraits<T, AccessorPolicy>::AccessorPolicyStaticInterface_t;
    
    template
    <
        class T,
        class AccessorPolicy,
        class AccessorPolicyStaticInterface
    >
    consteval bool DoesAccessorPolicyBelongToAccessorPolicyStaticInterface() { return std::is_same_v<GetAccessorPolicyStaticInterface_t<T, AccessorPolicy>, AccessorPolicyStaticInterface>; }
}

namespace CppUtils::AccessorPolicyUtils::Detail
{
    template
    <
        class T,
        template <class, class>
        class AccessorPolicyStaticInterface,
        class... AccessorPolicies
    >
    struct FindAccessorPolicy;

    template
    <
        class T,
        template <class, class>
        class AccessorPolicyStaticInterface,
        class First,
        class... Rest
    >
    struct FindAccessorPolicy<T, AccessorPolicyStaticInterface, First, Rest...>
    {
        using AccessorPolicy = std::conditional_t
        <
            DoesAccessorPolicyBelongToAccessorPolicyStaticInterface<T, First, AccessorPolicyStaticInterface<T, First>>(),
            First,
            typename FindAccessorPolicy<T, AccessorPolicyStaticInterface, Rest...>::AccessorPolicy
        >;
    };

    /*
    * No accessor policy found specialization.
    */
    template
    <
        class T,
        template <class, class>
        class AccessorPolicyStaticInterface
    >
    struct FindAccessorPolicy<T, AccessorPolicyStaticInterface>
    {
        using AccessorPolicy = AccessorPolicyStaticInterfaceTraits<T, AccessorPolicyStaticInterface>::FallbackAccessorPolicy;
    };
}

namespace CppUtils::AccessorPolicyUtils
{
    /*
    * Finds the first accessor policy belonging to `AccessorPolicyStaticInterface` in the provided `AccessorPolicies...`. Returns `AccessorPolicyStaticInterface`'s `FallbackAccessorPolicy` if no such accessor policy is found.
    */
    template
    <
        class T,
        template <class, class>
        class AccessorPolicyStaticInterface,
        class... AccessorPolicies
    >
    using GetAccessorPolicyByStaticInterface = Detail::FindAccessorPolicy<T, AccessorPolicyStaticInterface, AccessorPolicies...>::AccessorPolicy;

    /*
    * Builds the static interface for dispatching calls to the correct accessor policy in `AccessorPolicies...`.
    */
    template
    <
        class T,
        template <class, class>
        class AccessorPolicyStaticInterface,
        class... AccessorPolicies
    >
    using BuildAccessorPolicyStaticInterface = AccessorPolicyStaticInterface
    <
        T,
        GetAccessorPolicyByStaticInterface<T, AccessorPolicyStaticInterface, AccessorPolicies...>
    >;
}
