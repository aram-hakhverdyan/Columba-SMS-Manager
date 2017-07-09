#ifndef SE_ERROR_H
#define SE_ERROR_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "smsengine_global.h"
//	Qt Includes
#include <QString>
#include <QCoreApplication>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CError
//
class CError
{
public:
	//
	//	Error Types
	//
	enum EType
	{
		eUndefined			= 0,
		eUnexpectedResult,
		eSerialPortError,
		eInvalidInternalConfiguration,
		eNotSupported,
		eStateCheckFailed,
		eSmsSendFailed,

		// Data Base Errors
		DBError,

		// Phone Book Errors
		eInvalidContactInfo,

		/* ... */
		eUserError = 64
	};
public:
	//
	//	Constructor & Destructor
	//
	//! Default Constructor
	inline CError( EType eErrorType = eUndefined, QString sErrMsg = QString() );
	inline CError( QString sErrMsg );
	~CError() = default;

public:
	//
	//	Const Interface
	//
	inline EType   GetErrorType()    const;
	inline QString GetErrorMessage() const;

protected:
	// Returns default error message for the each error type
	inline virtual QString GetErrorTemplate( EType eType ) const;

private:
	//
	//	Content
	//
	EType   m_eType;
	QString m_sErrorMessage;
};
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
//! Constructor
inline CError::CError( EType eErrorType, QString sErrMsg )
: m_eType( eErrorType )
{
	if (sErrMsg.isNull())
		m_sErrorMessage = GetErrorTemplate( eErrorType );
	else
		m_sErrorMessage = sErrMsg;
}
//! Constructor
inline CError::CError( QString sErrMsg )
: m_eType( EType::eUndefined )
{
	if (sErrMsg.isNull())
		m_sErrorMessage = GetErrorTemplate( m_eType );
	else
		m_sErrorMessage = sErrMsg;
}
// GetErrorType
inline CError::EType CError::GetErrorType() const
{ return m_eType; }
// GetErrorMessage
inline QString CError::GetErrorMessage() const
{ return m_sErrorMessage; }
// GetErrorTemplate
inline QString CError::GetErrorTemplate( EType eType ) const
{
	char* szContext = "Default Error Messages";
	switch (eType)
	{
		case eUndefined:
			return QCoreApplication::translate( szContext, "Undefined Error" );
		default:
			return QCoreApplication::translate( szContext, "Undefined Error" );
	}
}
////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////
#endif // SE_ERROR_H
