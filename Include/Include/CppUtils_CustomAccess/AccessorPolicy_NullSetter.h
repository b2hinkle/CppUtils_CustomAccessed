// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

namespace CppUtils
{
    /*
    * Dummy accessor policy to make compiler happy when parsing a setter function which won't be generated.
    * Used as default for when user doesn't provide a setter accessor policy, and it effectively disables the setter (no generation).
    */
    template <class T>
    struct AccessorPolicy_NullSetter
    {
        using FirstArg  = T&;
        using SecondArg = T;

        static inline void Set(FirstArg value, SecondArg newValue)
        {
            static_assert(sizeof(T), "Should never execute.");
        }
    };
}
