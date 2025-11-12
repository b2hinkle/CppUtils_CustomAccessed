// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <CppUtils_CustomAccess/CustomAccessed.h>

int main(int argc, char** argv)
{
#if 0 // TODO: Consider if we want this kind of workflow and how we might support it if so.
    constexpr CppUtils::AccessorPolicies::GenericAccessorPolicy<int> myGenericAccessPol{};
    myGenericAccessPol.Get = [](const int& value) -> const int&  { return value; };
    myGenericAccessPol.Set = [](int& value, const int& newValue) { value = 85; };
#endif

    CppUtils::CustomAccessed
        <
            int,
            CppUtils::AccessorPolicies::GenericSetterAccessorPolicy<int, [](int& value, const int& newValue) { value = 85; }>,
            CppUtils::AccessorPolicies::GenericGetterAccessorPolicy<int, [](const int& value) -> const int&  { return value + 1; }>
        >
    myCustomAccessedInt(5);

    const int currentValue = myCustomAccessedInt.GetValue();
    myCustomAccessedInt.SetValue(currentValue + 1);

#if 0 // TODO: Add support for this kind of syntax.
    myCustomAccessedInt = myCustomAccessedInt + 1;
#endif

    const bool isSuccess = myCustomAccessedInt.GetValue() == 86;
    return isSuccess
        ? 0
        : 1;
}