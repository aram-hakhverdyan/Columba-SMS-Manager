#ifndef MODULE_GLOBAL_H
#define MODULE_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MODULE_LIB
# define MODULE_EXPORT Q_DECL_EXPORT
#else
# define MODULE_EXPORT Q_DECL_IMPORT
#endif

#endif // MODULE_GLOBAL_H
