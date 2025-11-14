// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <CppUtils_CustomAccess/CustomAccessedBase.h>
#include <CppUtils_CustomAccess/AccessorPolicies.h>
#include <CppUtils_CustomAccess/FindAccessorPolicy.h>


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

        template
        <
            template <class, class>
            class TPolicyCategory
        >
        using GetAccessorPolicyByCategory = CppUtils::CustomAccess::AccessorPolicyUtils::FindAccessorPolicyWithFallback_T
            <
            T,
            TPolicyCategory,                                                                                // To find.
            typename CppUtils::AccessorPolicies::PolicyCategoryTraits<T, TPolicyCategory>::FallbackPolicy, // Fallback.
            AccessorPolicies...                                                                              // Our policies.
            >;

    public:

        CustomAccessed() = default;

        CustomAccessed(T defaultValue)
        {
            SetValue(defaultValue);
        }

#if 0
        static consteval bool IsRefReturn
        {
            return std::is_reference_v<GetReturnType_T<GetAccessorPolicyByCategory<CppUtils::AccessorPolicies::GetterAccessorPolicy_Interface>::Get>>;
        }
#endif
        
        // TODO: We definately need to support other return types in terms of ref vs copy (maybe even cv but not as pressing).
        //       There are many potential use cases where user wants to return an r value, essentially by using the backing value as
        //       part of their calculation.
        const T& GetValue() const
        {
            return GetAccessorPolicyByCategory<CppUtils::AccessorPolicies::PolicyCategory_Getter>::Get(m_BackingValue);
        }

        void SetValue(const T& newValue)
        {
            GetAccessorPolicyByCategory<CppUtils::AccessorPolicies::PolicyCategory_Setter>::Set(m_BackingValue, newValue);
        }

    protected:
    
        // TODO: Would be good to support no backing value instantiations for particular use cases.
        T m_BackingValue{};

    };


}