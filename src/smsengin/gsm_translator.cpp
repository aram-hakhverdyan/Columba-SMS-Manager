////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gsm_translator.h"
#include "se_core.h"
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin    
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CGsmDefaultAlphabet
//

//
//	Static member initialization
//
// Init Default Alphabet
const QChar CGsmDefaultAlphabet::s_cszDefaultAlphabet[] = 
{
	QChar( '@' ),  QChar( '£' ),  QChar( '$' ),  QChar( '¥' ),  
	QChar( 'è' ),  QChar( 'é' ),  QChar( 'ù' ),  QChar( 'ì' ),  
	QChar( 'ò' ),  QChar( 'Ç' ),  QChar( 10 ),   QChar( 'Ø' ), 
	QChar( 'ø' ),  QChar( 13 ),   QChar( 'Å' ),  QChar( 'å' ),  
	QChar( L'Δ' ), QChar( '_' ),  QChar( L'Φ' ), QChar( L'Γ' ), 
	QChar( L'Λ' ), QChar( L'Ω' ), QChar( L'Π' ), QChar( L'Ψ' ), 
	QChar( L'Σ' ), QChar( L'Θ' ), QChar( L'Ξ' ), QChar( 27 ),
	QChar( 'Æ' ),  QChar( 'æ' ),  QChar( 'ß' ),  QChar( 'É' ),  
	QChar( 32 ),   QChar( '!' ),  QChar( '"' ),  QChar( '#' ),
	QChar( '¤' ),  QChar( '%' ),  QChar( '&' ),  QChar( '\'' ), 
	QChar( '(' ),  QChar( ')' ),  QChar( '*' ),  QChar( '+' ),
	QChar( ',' ),  QChar( '-' ),  QChar( '.' ),  QChar( '/' ),
	QChar( '0' ),  QChar( '1' ),  QChar( '2' ),  QChar( '3' ),
	QChar( '4' ),  QChar( '5' ),  QChar( '6' ),  QChar( '7' ),
	QChar( '8' ),  QChar( '9' ),  QChar( ':' ),  QChar( ';' ),
	QChar( '<' ),  QChar( '=' ),  QChar( '>' ),  QChar( '?' ),
	QChar( '¡' ),  QChar( 'A' ),  QChar( 'B' ),  QChar( 'C' ),
	QChar( 'D' ),  QChar( 'E' ),  QChar( 'F' ),  QChar( 'G' ),
	QChar( 'H' ),  QChar( 'I' ),  QChar( 'J' ),  QChar( 'K' ),
	QChar( 'L' ),  QChar( 'M' ),  QChar( 'N' ),  QChar( 'O' ),
	QChar( 'P' ),  QChar( 'Q' ),  QChar( 'R' ),  QChar( 'S' ),
	QChar( 'T' ),  QChar( 'U' ),  QChar( 'V' ),  QChar( 'W' ),
	QChar( 'X' ),  QChar( 'Y' ),  QChar( 'Z' ),  QChar( 'Ä' ),
	QChar( 'Ö' ),  QChar( 'Ñ' ),  QChar( 'Ü' ),  QChar( '§' ),  
	QChar( '¿' ),  QChar( 'a' ),  QChar( 'b' ),  QChar( 'c' ),
	QChar( 'd' ),  QChar( 'e' ),  QChar( 'f' ),  QChar( 'g' ),
	QChar( 'h' ),  QChar( 'i' ),  QChar( 'j' ),  QChar( 'k' ),
	QChar( 'l' ),  QChar( 'm' ),  QChar( 'n' ),  QChar( 'o' ),
	QChar( 'p' ),  QChar( 'q' ),  QChar( 'r' ),	 QChar( 's' ),
	QChar( 't' ),  QChar( 'u' ),  QChar( 'v' ),  QChar( 'w' ),
	QChar( 'x' ),  QChar( 'y' ),  QChar( 'z' ),  QChar( 'ä' ),
	QChar( 'ö' ),  QChar( 'ñ' ),  QChar( 'ü' ),  QChar( 'à' )
};

// Init Extension Tabel
const CGsmDefaultAlphabet::t_ExtantionTable 
CGsmDefaultAlphabet::s_cmapDefaultAlphabetExt = CGsmDefaultAlphabet::InitDefaultAlphabetExt();

// InitDefaultAlphabetExt
CGsmDefaultAlphabet::t_ExtantionTable CGsmDefaultAlphabet::InitDefaultAlphabetExt()
{
	QMap<uchar, QChar> oMap;
	oMap[10] = QChar( uchar( 10 ) );
	oMap[20] = QChar( '^' );
	oMap[40] = QChar( '{' );
	oMap[41] = QChar( '}' );
	oMap[47] = QChar( '\\' );
	oMap[60] = QChar( '[' );
	oMap[61] = QChar( '~' );
	oMap[62] = QChar( ']' );
	oMap[64] = QChar( '|' );
	oMap[101] = QChar( 0x20AC ); // €
	return oMap;
}

// Init Reversed Default Alphabet Table
const QHash<QChar, char> CGsmDefaultAlphabet::s_chDefaultAlphabet = 
	CGsmDefaultAlphabet::InitDefaultAlphabetReversed();

// InitDefaultAlphabetReversed
QHash<QChar, char> CGsmDefaultAlphabet::InitDefaultAlphabetReversed( )
{
	QHash<QChar, char> hAlphabet;
	for (int i = 0; i < 128; ++i)
		hAlphabet.insert( s_cszDefaultAlphabet[i], i );
	return hAlphabet;
}


// Init Reversed Default Alphabet Table
const QHash<QChar, char> CGsmDefaultAlphabet::s_chDefaultAlphabetExt =
CGsmDefaultAlphabet::InitDefaultAlphabetReversedExt( );

// InitDefaultAlphabetReversed
QHash<QChar, char> CGsmDefaultAlphabet::InitDefaultAlphabetReversedExt( )
{
	QHash<QChar, char> hExtAlphabet;
	auto it = s_cmapDefaultAlphabetExt.begin();
	for (; it != s_cmapDefaultAlphabetExt.end(); ++it)
		hExtAlphabet.insert( it.value(), it.key() );
	return hExtAlphabet;
}



// Translate
QString CGsmDefaultAlphabet::Translate( std::string const& sSource )
{
	QString sResult;
	bool bExt = false;
	for (int i = 0; i < sSource.size(); ++i)
	{
		// '@' symbol bug fix ||| 
		// Bad Solution((( in this way if the last symbol will correctly be '@' it will be ignored 
		if (sSource[i] == '\0' && i == sSource.size() - 1)
		{
			sResult.append( '\0' );
			continue;
		}

		uchar nIdx = uchar( sSource[i] );
		QChar c;
		if (!bExt)
		{
			SE_ASSERT( nIdx < 128 );
			if (nIdx >= 128)
				continue;
			c = s_cszDefaultAlphabet[nIdx];
		}
		else
		{
			c = s_cmapDefaultAlphabetExt.value(nIdx);
			bExt = false;
		}

		if (c == uchar( 27 ))
		{
			// Extanction character
			bExt = true;
			// Ignore ESC character
			continue;
		}
		sResult.append( c );
	}
	return sResult;
}

// Translate
QString CGsmDefaultAlphabet::Translate( QString const& sSource )
{
	// Bad Solution(
	return Translate( sSource.toStdString() );
}

// ToDefaultAlphabet
QString CGsmDefaultAlphabet::ToDefaultAlphabet( QChar c)
{
	auto it = s_chDefaultAlphabet.find( c );
	if (it != s_chDefaultAlphabet.end())
	{
		QChar qc( it.value() );
		return QString( qc );
	}
	// Check thrue escaped symbols
	it = s_chDefaultAlphabetExt.find( c );
	if (it != s_chDefaultAlphabetExt.end( ))
	{
		QChar qc( it.value() );
		return QString( QChar( 27 ) ).append( qc );
	}
	// Default alphabet not contains specified symbol
	return QString();
}

// MakeUserData
t_lstUserData CGsmDefaultAlphabet::MakeUserData( QString const& sSource )
{
	int nSize = sSource.size();
	EGsmAlphabets eCurrentAlphabet = EGsmAlphabets::Default;
	int nCurrentUserDataLength = 160;
	QString sCurrentUserData;
	t_lstUserData lstResult;

	for (int i = 0; i < nSize; ++i)
	{
		QChar c = sSource[i];
		
		if (eCurrentAlphabet == EGsmAlphabets::Default)
		{
			QString sDASym = ToDefaultAlphabet( c );
			if (!sDASym.isNull())
			{
				sCurrentUserData.append( sDASym );
			}
			else
			{
				// Switch to UCS2
				int nCurrentSize = sCurrentUserData.size();
				if ( nCurrentSize > 70)
				{
					// Store not unicode data in single userdata object
					SUserData strUserData;
					strUserData.eAlphabet = EGsmAlphabets::Default;
					strUserData.sData = sCurrentUserData;
					lstResult.append( strUserData );
					sCurrentUserData.clear();
				}
				sCurrentUserData.append( c );
				eCurrentAlphabet = EGsmAlphabets::UCS2;
				nCurrentUserDataLength = 70;
			}
		}
		else // UCS2
		{
			sCurrentUserData.append( c );
		}
		
		if (sCurrentUserData.size() == nCurrentUserDataLength)
		{ // Length is enough to add user data
			SUserData strUserData;
			strUserData.eAlphabet = eCurrentAlphabet;
			strUserData.sData = sCurrentUserData;
			lstResult.append( strUserData );
			// Prepere for other chunks
			sCurrentUserData.clear();
			eCurrentAlphabet = EGsmAlphabets::Default;
			nCurrentUserDataLength = 160;
		}
	}

	// Add last user data chunk
	if (!sCurrentUserData.isEmpty())
	{
		SUserData strUserData;
		strUserData.eAlphabet = eCurrentAlphabet;
		strUserData.sData = sCurrentUserData;
		lstResult.append( strUserData );
	}
	return lstResult;
}


////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////
