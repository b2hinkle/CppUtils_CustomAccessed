// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <tuple>

/*
* Function pointer traits, used to extract information about a given function.
* Specializations make use of inheritance to avoid dup code.
* TODO: I want to move this to a more general library later.
*/
namespace CppUtils
{
    // Primary template.
    template <class TFunc>
    struct FunctionPointerTraits
    {
        static_assert(sizeof(TFunc) == 0, "FunctionPointerTraits can only be specialized for function pointer types.");
    };

    // Pointer to free function specialization.
    template <class TReturnType, class... TArgs>
    struct FunctionPointerTraits<TReturnType (*)(TArgs...)>
    {
        using ReturnType = TReturnType;

        using ArgsTuple = std::tuple<TArgs...>;

        using ClassType = void; // No class type for free functions.
    };


    // Pointer to member function specializations.

    // Non-const.
    template <class TReturnType, class TClass, class... TArgs>
    struct FunctionPointerTraits<TReturnType (TClass::*)(TArgs...)>
        : FunctionPointerTraits<TReturnType (*)(TArgs...)>
    {
        using ClassType = TClass;
    };

    // Const.
    template <class TReturnType, class TClass, class... TArgs>
    struct FunctionPointerTraits<TReturnType (TClass::*)(TArgs...) const>
        : FunctionPointerTraits<TReturnType (TClass::*)(TArgs...)>
    {};


    // Non-pointer callable types (e.g. lambda) specialization.
    template <class T>
        requires (requires { &T::operator(); })
    struct FunctionPointerTraits<T>
        : FunctionPointerTraits<decltype(&T::operator())>
    {};
}
