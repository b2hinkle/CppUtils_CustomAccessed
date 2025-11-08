// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

namespace CppUtils
{
    /**
     * Property wrapper implementation.
     */
    template <class T>
    struct CustomAccessed
    {

    public:

        CustomAccessed() = default;

        // TODO: We should support other overloads as well.
        CustomAccessed(T defaultValue)
        {
            GetValue() = defaultValue;
        }

        inline T& GetValue()
        {
            return Value;
        }

        // Implements implicit conversion from this struct to Value's type. Allows you to treat this struct as its Value's type in code.
        operator T() const
        {
            return GetValue();
        }

        // Broadcasts ValueChangeDelegate TODO: We should support other overloads as well.
        // TODO: It would be nice to expose ability to broadcast even when value is same.
        T& operator=(const T& newValue)
        {
            const T oldValue = GetValue();
            GetValue() = newValue;

            if (newValue != oldValue)
            {
    #if 0
                ValueChangeDelegate.Broadcast(oldValue, newValue);
    #endif
            }

            return GetValue();
        }
        
        T Value = T{};

    #if 0
        // TODO: Must have generic solution for this that doesn't care about any delegate.
        // Change event.
        TMulticastDelegate<void(const T&, const T&)> ValueChangeDelegate;
    #endif

    };
}
