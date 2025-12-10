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
        using GetAccessorPolicy = CppUtils::CustomAccess::AccessorPolicyUtils::GetAccessorPolicyByCategory_T
        <
            T,
            TPolicyCategory,    // To find.
            AccessorPolicies... // Our policies.
        >;

        // TODO: Change from directly accessing the accessor policies to using the policy category, this way we operated based on an interface.
        using GetterAccessorPolicy = GetAccessorPolicy<CppUtils::AccessorPolicies::PolicyCategory_Getter>;
        using SetterAccessorPolicy = GetAccessorPolicy<CppUtils::AccessorPolicies::PolicyCategory_Setter>;



        using GetterAccessorPolicyCategory = CppUtils::AccessorPolicies::PolicyCategory_Getter<T, GetterAccessorPolicy>; // TODO: We shoudl probably make this process not have to know about the exact policy.
        using SetterAccessorPolicyCategory = CppUtils::AccessorPolicies::PolicyCategory_Setter<T, SetterAccessorPolicy>; // TODO: We shoudl probably make this process not have to know about the exact policy.

    public:

        CustomAccessed() = default;

        /*
        * In the case of copy return value, cpp 17 prvalue semantics gives us guaranteed copy elision.
        * Since we're able to avoid decltype(auto) return type, we do. This is because its deduction phase can trigger unnecessary errors when our policy isn't defined correctly.
        */
        inline GetterAccessorPolicyCategory::ReturnType GetValue() const
            requires ( requires(T dummyBackingValue) { GetterAccessorPolicyCategory::Get(dummyBackingValue); } )
        {
            return GetterAccessorPolicyCategory::Get(m_BackingValue);
        }

        inline void SetValue(SetterAccessorPolicyCategory::SecondArg newValue)
            requires (requires(T dummyBackingValue, T dummyNewValue) { SetterAccessorPolicyCategory::Set(dummyBackingValue, dummyNewValue); } )
        {
            if constexpr (std::is_rvalue_reference_v<typename SetterAccessorPolicyCategory::SecondArg>)
            {
                // Non-const rvalue reference. Non-const because that's already been asserted.
                SetterAccessorPolicyCategory::Set(m_BackingValue, std::move(newValue));
            }
            else
            {
                // Either const lvalue reference or const/non-const copy.
                SetterAccessorPolicyCategory::Set(m_BackingValue, newValue);
            }
        }


    protected:
    
        // TODO: Would be good to support no backing value instantiations for particular use cases.
        T m_BackingValue{};

    };


}