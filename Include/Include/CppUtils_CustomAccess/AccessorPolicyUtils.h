// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <type_traits>
#include <CppUtils_CustomAccess/FunctionTraits.h>

namespace CppUtils
{
    /*
    * A type which is callable.
    */
    template <auto T>
    concept TCallable = requires { typename FunctionPointerTraits<decltype(T)>; };

    template <class TFunction, class... TArgs>
    consteval bool IsRefReturnType()
    {
        using returnType = std::invoke_result_t<TFunction, TArgs...>;
        return std::is_reference_v<returnType>;
    }

    template <class T>
    consteval bool IsConstAfterRemovingRef() { return std::is_const_v<std::remove_reference_t<T>>; }
}
