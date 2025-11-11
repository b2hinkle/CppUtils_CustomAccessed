// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <CppUtils_CustomAccessed/CustomAccessed.h>

int main(int argc, char** argv)
{
    CppUtils::CustomAccessed
        <
            int,
            CppUtils::CommonAccessorPolicies::GenericAccessorPolicy
                <
                    int,
                    [](const int& value) -> const int&  { return value; },
                    [](int& value, const int& newValue) { value = 85; }
                >{}
        >
    myCustomAccessedInt(5);

    const int currentValue = myCustomAccessedInt.GetValue();
    myCustomAccessedInt.SetValue(currentValue + 1);

#if 0 // TODO: Add support for this kind of syntax.
    myCustomAccessedInt = myCustomAccessedInt + 1;
#endif

    const bool isSuccess = myCustomAccessedInt.GetValue() == 85;
    return isSuccess
        ? 0
        : 1;
}