// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

namespace CppUtils::AccessorPolicies
{
    /*
    * Dummy policy to make compiler happy when parsing a setter function which won't be generated.
    * Used as default for when user doesn't provide a setter policy, and it effectively disables the setter (no generation).
    */
    template <class T>
    struct NullSetterAccessorPolicy
    {
        using FirstArg  = T&;
        using SecondArg = T;

        static inline void Set(FirstArg value, SecondArg newValue)
        {
            static_assert(sizeof(T), "Should never execute.");
        }
    };
}
