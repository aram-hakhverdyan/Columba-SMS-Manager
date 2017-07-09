#ifndef SE_PHONE_NUMBER_H
#define SE_PHONE_NUMBER_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "smsengine_global.h"
//	Qt Includes
#include <QRegExp>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CPhoneNumber
//
class SMSENGINE_EXPORT CPhoneNumber
{
public:
	//
	//	Constructor & Destructor
	//
	//! Default Constructor
	inline CPhoneNumber( QString const& sPhoneNumber = QString() );
	~CPhoneNumber() = default;

public:
	//
	//	Const Interface
	//
	inline bool IsValid() const;
	inline QString GetPhoneNumber() const;
	inline void SetPhoneNumber( QString const& sPhoneNumber );
	static QRegExp GetValidationRegExp();
	// 
	static bool Validate( QString const& sPhoneNumber );

private:
	//
	//	Content
	//
	QString m_sPhoneNumber;
	bool	m_bIsValid;
	static QRegExp s_oValidator;
};
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
//! Constructor
inline CPhoneNumber::CPhoneNumber( QString const& sPhoneNumber )
{
	SetPhoneNumber( sPhoneNumber );
}

inline bool CPhoneNumber::IsValid() const { return m_bIsValid; }
inline QString CPhoneNumber::GetPhoneNumber() const { return m_sPhoneNumber; }

inline void CPhoneNumber::SetPhoneNumber( QString const& sPhoneNumber )
{
	m_sPhoneNumber = sPhoneNumber;
	m_sPhoneNumber = m_sPhoneNumber.simplified();
	m_sPhoneNumber.replace( " ", "" );
	m_bIsValid = Validate( sPhoneNumber );
}

////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////
#endif // SE_PHONE_NUMBER_H
