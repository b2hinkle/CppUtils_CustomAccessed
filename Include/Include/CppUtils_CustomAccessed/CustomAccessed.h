// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_CustomAccessed/CustomAccessedBase.h>
#include <CppUtils_CustomAccessed/CommonAccessorPolicies.h>
#include <CppUtils_CustomAccessed/FindAccessorPolicy.h>


namespace CppUtils
{
    /**
     * Property wrapper implementation.
     * User generates access behavior by providing policy classes as template arguments. Generic accessor policies are most common, as they simply forward execution to users' external function definitions. Default behavior exists where user doesn't specify behavior.
-    * User functions are specified via packed template type parameters. This way, "argument" order of policies is up to the user and there are no forced argument situations.
     * TODO: We should support all special member functions.
     */
    template <
        class T,
        class... AccessorPolicies
    >
    struct CustomAccessed
    {
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
            using foundAccessorPolicy = AccessorPolicyUtils::FindAccessPolicy_T<T, CppUtils::CommonAccessorPolicies::GetterAccessorPolicy, AccessorPolicies...>;
            if constexpr (std::is_same_v<foundAccessorPolicy, void>)
            {
                return CppUtils::CommonAccessorPolicies::BasicGetter<T>(m_BackingValue);
            }
            else
            {
                return foundAccessorPolicy::Get(m_BackingValue);
            }
        }

        void SetValue(const T& newValue)
        {
            using foundAccessorPolicy = AccessorPolicyUtils::FindAccessPolicy_T<T, CppUtils::CommonAccessorPolicies::SetterAccessorPolicy, AccessorPolicies...>;
            if constexpr (std::is_same_v<foundAccessorPolicy, void>)
            {
                return CppUtils::CommonAccessorPolicies::BasicSetter<T>(m_BackingValue, newValue);
            }
            else
            {
                return foundAccessorPolicy::Set(m_BackingValue, newValue);
            }
        }

    protected:
    
        // TODO: Would be good to support no backing value instantiations for particular use cases.
        T m_BackingValue{};

    };


}