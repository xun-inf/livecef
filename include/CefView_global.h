#ifndef CEFVIEW_GLOBAL_H
#define CEFVIEW_GLOBAL_H
#pragma once

#pragma region qt_headers
#include <QtCore/qglobal.h>
#include <QMetaType>
#pragma endregion

#if defined(LIVECEF_STATIC)
#define LIVECEF_EXPORT
#elif defined(LIVECEF_LIBRARY)
#define LIVECEF_EXPORT Q_DECL_EXPORT
#else
#define LIVECEF_EXPORT Q_DECL_IMPORT
#endif

// CEF version numbers
#include <CefVersion.h>

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