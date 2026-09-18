#ifndef QCEFIUM_GLOBAL_H
#define QCEFIUM_GLOBAL_H
#pragma once

#pragma region qt_headers
#include <QtCore/qglobal.h>
#include <QMetaType>
#pragma endregion

#if defined(QCEFIUM_STATIC)
#define QCEFIUM_EXPORT
#elif defined(QCEFIUM_LIBRARY)
#define QCEFIUM_EXPORT Q_DECL_EXPORT
#else
#define QCEFIUM_EXPORT Q_DECL_IMPORT
#endif

#ifdef QCEFIUM_STATIC
    // Static library, no need export
    #define QCEFIUM_EXPORT
#else
    // Dynamic library
    #ifdef QCEFIUM_LIB
        // Build QCEFIUM project, export API
        #define QCEFIUM_EXPORT Q_DECL_EXPORT
    #else
        // Referenced by other project, import API 
        #define QCEFIUM_EXPORT Q_DECL_IMPORT
        #if _WIN32
            #pragma comment(lib, "qcefium.lib")
        #endif
    #endif
#endif

// CEF version numbers
#include <qcefversion.h>

#define DEPRECATED_SINCE_CEF_VERSION(major, minor, patch) /* @deprecated Since CEF major . minor . patch */

#define DEPRECATED_CEF_API_WARNING(major, minor, patch)                                                                 \
  qWarning() << __FUNCTION__ << " was deprecated since CEF version:"                                                    \
            << major << "." << minor << "." << patch << ". "                                                            \
            << "Current CEF version: "                                                                                  \
            << CEF_VERSION_MAJOR << "." << CEF_VERSION_MINOR << "." << CEF_VERSION_PATCH << ".";


#define INTRODUCED_SINCE_CEF_VERSION(major, minor, patch)

#define INTRODUCED_CEF_API_WARNING(major, minor, patch)                                                                 \
 qWarning() << __FUNCTION__ << " was introduced since CEF version:"                                                     \
            << major << "." << minor << "." << patch << ". "                                                            \
            << "Current CEF version: "                                                                                  \
            << CEF_VERSION_MAJOR << "." << CEF_VERSION_MINOR << "." << CEF_VERSION_PATCH << ".";


#endif // CEFVIEW_GLOBAL_H