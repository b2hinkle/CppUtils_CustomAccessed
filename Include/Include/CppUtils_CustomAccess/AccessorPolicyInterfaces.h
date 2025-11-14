// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <utility>


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

    template <class Policy>
    struct PolicyTraits
    {
        static_assert(false && sizeof(Policy), "No policy category defined. You must define a (partial) specialization of this struct for your policy class. This way you can assign a policy category to your policy, allowing us to search for you.");
    };
}

namespace CppUtils::AccessorPolicies
{
    template
    <
        class T,
        class Policy
    >
    struct PolicyCategory_Getter
    {
        static_assert(std::is_invocable_v<decltype(Policy::Get), T>, "The policy class is missing this function.");
    };

    template
    <
        class T,
        class Policy
    >
    struct PolicyCategory_Setter
    {
        static_assert(std::is_invocable_v<decltype(Policy::Set), T, T>, "The policy class is missing this function.");
    };
}
