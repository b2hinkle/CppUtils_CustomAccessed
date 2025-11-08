// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#ifndef CPPUTILS_CUSTOMACCESSED_EXPORT_H
#define CPPUTILS_CUSTOMACCESSED_EXPORT_H

// Include the appropriate "export header" file depending on which type of this library is being compiled.

#if __has_include(<cpputils_customaccessed_shared_export.h>)
#   include <cpputils_customaccessed_shared_export.h>
#elif __has_include(<cpputils_customaccessed_module_export.h>)
#   include <cpputils_customaccessed_module_export.h>
#elif __has_include(<cpputils_customaccessed_object_export.h>)
#   include <cpputils_customaccessed_object_export.h>
#elif __has_include(<cpputils_customaccessed_static_export.h>)
#   include <cpputils_customaccessed_static_export.h>
#else
    // Since no generated header files exist, it must mean we are an interface library (being used as a header-only library).
#   include <cpputils_customaccessed_include_export.h>
#endif

// Undef the generated macros that we've decided not to use.

#ifdef CPPUTILS_CUSTOMACCESSED_DEPRECATED
#   undef CPPUTILS_CUSTOMACCESSED_DEPRECATED
#endif

#ifdef CPPUTILS_CUSTOMACCESSED_DEPRECATED_EXPORT
#   undef CPPUTILS_CUSTOMACCESSED_DEPRECATED_EXPORT
#endif

#ifdef CPPUTILS_CUSTOMACCESSED_DEPRECATED_NO_EXPORT
#   undef CPPUTILS_CUSTOMACCESSED_DEPRECATED_NO_EXPORT
#endif

#ifdef CPPUTILS_CUSTOMACCESSED_NO_DEPRECATED
#   undef CPPUTILS_CUSTOMACCESSED_NO_DEPRECATED
#endif

#endif // #ifndef CPPUTILS_CUSTOMACCESSED_EXPORT_H
