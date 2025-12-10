// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_CustomAccess/AccessorPolicyCategories.h>
#include <CppUtils_CustomAccess/NullGetterAccessorPolicy.h>
#include <CppUtils_CustomAccess/NullSetterAccessorPolicy.h>

namespace CppUtils::AccessorPolicies
{
    template
    <
        class T,
        template <class, class> // NOTE: Will this still be valid if we require a new policy with more template parameters?
        class TPolicyInterface
    >
    struct PolicyCategoryTraits
    {
        // Write the condition to be dependent on a template parameter, to avoid the static assertion being evaluated during the parsing phase of this template struct.
        static_assert(false && sizeof(T), "No traits defined. You must define a (partial) specialization of this struct for your policy category class.");
    };

    template <class T>
    struct PolicyCategoryTraits<T, PolicyCategory_Getter>
    {
        using FallbackPolicy = NullGetterAccessorPolicy<T>;
    };

    template <class T>
    struct PolicyCategoryTraits<T, PolicyCategory_Setter>
    {
        using FallbackPolicy = NullSetterAccessorPolicy<T>;
    };
}

namespace CppUtils::AccessorPolicies
{
    template 
    <
        class T,
        class Policy
    >
    struct PolicyTraits
    {
        static_assert(false && sizeof(Policy), "No policy category defined. You must define a (partial) specialization of this struct for your policy class. This way you can assign a policy category to your policy, allowing us to search for you.");
    };
    
    template <class T>
    struct PolicyTraits<T, NullGetterAccessorPolicy<T>>
    {
        using PolicyCategory_t = PolicyCategory_Getter<T, NullGetterAccessorPolicy<T>>;
    };

    template <class T>
    struct PolicyTraits<T, NullSetterAccessorPolicy<T>>
    {
        using PolicyCategory_t = PolicyCategory_Setter<T, NullSetterAccessorPolicy<T>>;
    };
}
