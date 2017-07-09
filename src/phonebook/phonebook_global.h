#ifndef PHONEBOOK_GLOBAL_H
#define PHONEBOOK_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef PHONEBOOK_LIB
# define PHONEBOOK_EXPORT Q_DECL_EXPORT
#else
# define PHONEBOOK_EXPORT Q_DECL_IMPORT
#endif

#endif // PHONEBOOK_GLOBAL_H
