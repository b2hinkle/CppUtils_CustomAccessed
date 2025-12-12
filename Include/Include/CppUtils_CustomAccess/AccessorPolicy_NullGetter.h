// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

namespace CppUtils
{
    /*
    * Dummy accessor policy to make compiler happy when parsing a getter function which won't be generated.
    * Used as default for when user doesn't provide a getter accessor policy, and it effectively disables the getter (no generation).
    */
    template <class T>
    struct AccessorPolicy_NullGetter
    {
        using ReturnType = T;
        using FirstArg   = T;
        
        static inline ReturnType Get(FirstArg value)
        {
            static_assert(sizeof(T), "Should never execute.");
            return ReturnType{};
        }
    };
}
