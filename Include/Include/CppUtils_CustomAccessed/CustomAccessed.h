// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_CustomAccessed/CustomAccessedBase.h>
#include <CppUtils_CustomAccessed/CommonAccessorPolicies.h>


namespace CppUtils
{
    /**
     * Property wrapper implementation.
     * TODO: We should support all special member functions.
     */
    template <
        class T,
        class AccessorPolicy = CppUtils::CommonAccessorPolicies::GenericAccessorPolicy<T>
    >
    struct CustomAccessed
    {

    public:

        CustomAccessed() = default;

        CustomAccessed(T defaultValue)
        {
            SetValue(defaultValue);
        }
        
        const T& GetValue() const
        {
            return AccessorPolicy::Get(BackingValue);
        }

        void SetValue(const T& newValue)
        {
            AccessorPolicy::Set(BackingValue, newValue);
        }

    protected:
    
        T BackingValue = T{};

    };
}
