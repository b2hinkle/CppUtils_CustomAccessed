// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <type_traits>
#include <CppUtils_CustomAccess/CustomAccessedBase.h>
#include <CppUtils_CustomAccess/AccessorPolicies.h>
#include <CppUtils_CustomAccess/FindAccessorPolicy.h>
#include <CppUtils_CustomAccess/AccessorPolicyUtils.h>


namespace CppUtils
{
    /**
     * Property wrapper implementation.
     * User generates access behavior by providing policy classes as template arguments. Generic accessor policies are most common, as they simply forward execution to users' external function definitions. Default behavior exists where user doesn't specify behavior.
-    * User functions are specified via packed template type parameters. This way, "argument" order of policies is up to the user and there are no forced argument situations.
     * TODO: We should support all special member functions.
     * TODO: Provide a default value constructor, to support pr value sematics.
     * TODO: The case of no user-defined get/set should not fallback on a default get/set implementation. The accessor should simply not exist if not defined.
     */
    template <
        class T, // TODO: Possibly loop through all accessor policies to ensure they agree on T, and give a clean error if not.
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
            TPolicyCategory,                                                                               // To find.
            typename CppUtils::AccessorPolicies::PolicyCategoryTraits<T, TPolicyCategory>::FallbackPolicy, // Fallback.
            AccessorPolicies...                                                                            // Our policies.
        >;

        using GetterAccessPolicy = GetAccessorPolicyByCategory<CppUtils::AccessorPolicies::PolicyCategory_Getter>;
        using SetterAccessPolicy = GetAccessorPolicyByCategory<CppUtils::AccessorPolicies::PolicyCategory_Setter>;

    public:

        CustomAccessed() = default;
        
        inline const T& GetValue() const
            requires
            (
                std::is_lvalue_reference_v           <typename GetterAccessPolicy::ReturnType> &&
                CustomAccess::IsConstAfterRemovingRef<typename GetterAccessPolicy::ReturnType>()
            )
        {
            return GetterAccessPolicy::Get(m_BackingValue);
        }

        inline T GetValue() const
            requires
            (
                !std::is_reference_v<typename GetterAccessPolicy::ReturnType> &&
                !std::is_const_v    <typename GetterAccessPolicy::ReturnType>
            )
        {
            // CPP 17 prvalue semantics gives us guaranteed copy elision.
            return GetterAccessPolicy::Get(m_BackingValue);
        }




        inline void SetValue(const T& newValue)
            requires
            (
                std::is_lvalue_reference_v           <typename SetterAccessPolicy::SecondArg> &&
                CustomAccess::IsConstAfterRemovingRef<typename SetterAccessPolicy::SecondArg>()
            )
        {
            SetterAccessPolicy::Set(m_BackingValue, newValue);
        }

        inline void SetValue(T newValue)
            requires
            (
                !std::is_reference_v<typename SetterAccessPolicy::SecondArg> &&
                !std::is_const_v    <typename SetterAccessPolicy::SecondArg>
            )
        {
            SetterAccessPolicy::Set(m_BackingValue, newValue);
        }

        inline void SetValue(T&& newValue)
            requires
            (
                std::is_rvalue_reference_v            <typename SetterAccessPolicy::SecondArg> &&
                !CustomAccess::IsConstAfterRemovingRef<typename SetterAccessPolicy::SecondArg>()
            )
        {
            SetterAccessPolicy::Set(m_BackingValue, std::move(newValue));
        }




    protected:
    
        // TODO: Would be good to support no backing value instantiations for particular use cases.
        T m_BackingValue{};

    };


}