#ifndef GSM_TRANSLATOR_H
#define GSM_TRANSLATOR_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_core.h"
// Qt
#include <QString>
#include <QMap>
#include <QHash>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Enum GsmAlphabets
//
enum class EGsmAlphabets
{
	Default,
	UCS2
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	struct SUserData
//
struct SUserData
{
	EGsmAlphabets eAlphabet;
	QString sData;
};
typedef QList<SUserData> t_lstUserData;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CATCommand
//
class SMSENGINE_EXPORT CGsmDefaultAlphabet
{
	typedef QMap<uchar, QChar> t_ExtantionTable;

public:
	//
	//	Static Methods
	//
	//! Translate from GSM user data
	static QString Translate( QString const& sSource );
	static QString Translate( std::string const& sSource );
	static t_lstUserData MakeUserData( QString const& sSource );

private:
	//
	//	Static Tools
	//
	// Initializators
	static t_ExtantionTable InitDefaultAlphabetExt();
	static QHash<QChar, char> InitDefaultAlphabetReversed();
	static QHash<QChar, char> InitDefaultAlphabetReversedExt( );

	// Helpers
	//! Returns a single QChar symbol or 2 length string in case of escaped character
	//! if the specified symbol belongs to Deafault Alphabet, otherwise returns NULL string
	static QString ToDefaultAlphabet( QChar );

private:
	//
	//	Content
	//
	static const QChar              s_cszDefaultAlphabet[];
	static const t_ExtantionTable	s_cmapDefaultAlphabetExt;
	static const QHash<QChar, char> s_chDefaultAlphabet;
	static const QHash<QChar, char> s_chDefaultAlphabetExt;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // GSM_TRANSLATOR_H
