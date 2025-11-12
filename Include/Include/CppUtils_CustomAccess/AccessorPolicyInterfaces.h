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
    struct PolicyInterfaceTraits
    {
        // Write the condition to be dependent on a template parameter, to avoid the static assertion being evaluated during the parsing phase of this template struct.
        static_assert(false && sizeof(T), "No traits defined. You must define a (partial) specialization of this struct for your policy interface class.");
    };
}

namespace CppUtils::AccessorPolicies
{
    template <
        class T,
        class TDerived
    >
    struct GetterAccessorPolicy_Interface
    {
        static inline const T& Get(const T& value) { return TDerived::Get(value); }
    };

    template <
        class T,
        class TDerived
    >
    struct SetterAccessorPolicy_Interface
    {
        static inline void Set(T& value, const T& newValue) { return TDerived::Set(value); }
    };
}