// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <CppUtils_CustomAccess/CustomAccessed.h>

int main(int argc, char** argv)
{
    CppUtils::CustomAccessed
        <
            int,
            CppUtils::AccessorPolicies::GenericSetterAccessorPolicy<int, [](int& value, const int& newValue)  { value = 1; }>,
            CppUtils::AccessorPolicies::GenericGetterAccessorPolicy<int, [](const int& value) -> const int&  { return value + 1; }>
        >
    myCustomAccessedInt(5);

    const int currentValue = myCustomAccessedInt.GetValue();
    myCustomAccessedInt.SetValue(currentValue + 1);

#if 0 // TODO: Add support for this kind of syntax.
    myCustomAccessedInt = myCustomAccessedInt + 1;
#endif

    const bool isSuccess = myCustomAccessedInt.GetValue() == 2;
    return isSuccess
        ? 0
        : 1;
}