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
        auto AccessorPolicy = CppUtils::CommonAccessorPolicies::GenericAccessorPolicy<T>{}
    >
#if 0 // TODO: We need to enforce it to be an accessor policy type, but first, we need to also create a concept defining what the type actually can be.
    requires decltype(AccessorPolicy) == IsAccessorPolicyConcept
#endif
    struct CustomAccessed
    {
        using AccessorPolicyType = decltype(AccessorPolicy);

    public:

        CustomAccessed() = default;

        CustomAccessed(T defaultValue)
        {
            SetValue(defaultValue);
        }
        
        // TODO: We definately need to support other return types in terms of ref vs copy (maybe even cv but not as pressing).
        //       There are many potential use cases where user wants to return an r value, essentially by using the backing value as
        //       part of their calculation.
        const T& GetValue() const
        {
            return AccessorPolicy.GetterFuncPtr(BackingValue);
        }

        void SetValue(const T& newValue)
        {
            AccessorPolicy.SetterFuncPtr(BackingValue, newValue);
        }

    protected:
    
        // TODO: Would be good to support no backing value instantiations for particular use cases.
        T BackingValue{};

    };
}