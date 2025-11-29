// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <CppUtils_CustomAccess/CustomAccessed.h>

const int& Getv(const int& value)
{
    return value;
}

struct MyStruct_StaticTest
{
    static const int& Getv(const int& value)
    {
        return value;
    }
};

struct MyStruct_InstanceTest
{
    const int& Getv(const int& value)
    {
        return value;
    }
};

int main(int argc, char** argv)
{
    //MyStruct_InstanceTest instance;

    CppUtils::CustomAccessed
        <
            int,
            CppUtils::AccessorPolicies::GenericGetterAccessorPolicy<[](int value) -> int { return value; }>,
            CppUtils::AccessorPolicies::GenericSetterAccessorPolicy<[](int& value, const int newValue) {value = newValue + 8;}>
        >
    myCustomAccessedInt;

    myCustomAccessedInt.SetValue(2);
    const int currentValue = myCustomAccessedInt.GetValue();
//
//#if 0 // TODO: Add support for this kind of syntax.
//    myCustomAccessedInt = myCustomAccessedInt + 1;
//#endif

    const bool isSuccess = myCustomAccessedInt.GetValue() == 10 /*true*/;
    return isSuccess
        ? 0
        : 1;
}