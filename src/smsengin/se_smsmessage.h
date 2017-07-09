#ifndef SE_SMS_MESSAGE_H
#define SE_SMS_MESSAGE_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "smsengine_global.h"
//	Qt Includes
#include <QString>
#include <QDateTime>
// STL
#include <memory>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSmsMessage
//
class SMSENGINE_EXPORT CSmsMessage
{
public:
	// 
	//	SMS Message Origin
	//
	enum class EMessageOrigin
	{
		eUndefined = 0,
		eMessageToSubmit,
		eDeliveredMessage
	};

public:
	//
	//	Constructor & Destructor
	//
	//! Default Constructor
	// Constructs invalid message
	inline CSmsMessage();
	/*inline CSmsMessage( QString    const& sPhone,
				 QString    const& sMessage,
				 QDateTime  const& oTime = QDateTime::currentDateTime(),
				 EMessageOrigin EMessageOrigin = EMessageOrigin::eUndefined
				 );*/

	// Constructs SMS message in submit origin  
	inline CSmsMessage( QString    const& sPhone,
						QString    const& sMessage );
	// Constructs SMS message in delivered origin  
	inline CSmsMessage( QString    const& sPhone,
						QString    const& sMessage,
						QDateTime  const& oTime
						);

	~CSmsMessage() = default;

public:
	//
	//	Const Interface
	//
	inline QString    GetPhoneNumber() const;
	inline QString    GetMessage()     const;
	inline QDateTime  GetDateTime()    const;
	inline EMessageOrigin GetOrigin()      const;
	// Validity
	inline bool IsValid() const;

	//
	//	Editing Interface
	//
	inline void SetPhoneNumber( QString const& sPhone );
	inline void SetMessage( QString const& sMessage );
	inline void SetDateTime( QDateTime const& oTime = QDateTime::currentDateTime( ) );
	inline void SetOrigin( EMessageOrigin EMessageOrigin );

private:
	//
	//	Content
	//
	QString     m_sPhoneNumber;
	QString     m_sMessage;
	QDateTime   m_oTime;
	EMessageOrigin  m_eMessageOrigin;
};

typedef std::shared_ptr<CSmsMessage> t_SmsMessageSharedPtr;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
//! Constructor
inline CSmsMessage::CSmsMessage( QString    const& sPhone,
								 QString    const& sMessage )
								 : m_sPhoneNumber( sPhone ),
								 m_sMessage( sMessage ),
								 m_eMessageOrigin( EMessageOrigin::eMessageToSubmit )
{}

//! Constructor
inline CSmsMessage::CSmsMessage( QString    const& sPhone,
								 QString    const& sMessage,
								 QDateTime  const& oTime )
								 : m_sPhoneNumber( sPhone ),
								 m_sMessage( sMessage ),
								 m_oTime( oTime ),
								 m_eMessageOrigin( EMessageOrigin::eDeliveredMessage )
{}

//! Constructor
inline CSmsMessage::CSmsMessage()
	:m_eMessageOrigin( EMessageOrigin::eUndefined )
{}

// GETs
inline QString    CSmsMessage::GetPhoneNumber() const { return m_sPhoneNumber; }
inline QString    CSmsMessage::GetMessage()     const { return m_sMessage; }
inline QDateTime  CSmsMessage::GetDateTime( )   const { return m_oTime; }
inline CSmsMessage::EMessageOrigin CSmsMessage::GetOrigin()					
												const { return m_eMessageOrigin; }


// IsValid
inline bool CSmsMessage::IsValid() const
{
	if (m_eMessageOrigin == EMessageOrigin::eUndefined)
		return false;
	if (m_eMessageOrigin == EMessageOrigin::eMessageToSubmit)
		return !m_sPhoneNumber.isEmpty();
	//EMessageOrigin::eDeliveredMessage
		return !m_sPhoneNumber.isEmpty() && m_oTime.isValid();
}

// SETs
inline void CSmsMessage::SetPhoneNumber( QString const& sPhone ) { m_sPhoneNumber = sPhone; }
inline void CSmsMessage::SetMessage( QString const& sMessage )   { m_sMessage = sMessage; }
inline void CSmsMessage::SetDateTime( QDateTime const& oTime )   { m_oTime = oTime; };
inline void CSmsMessage::SetOrigin( EMessageOrigin EMessageOrigin )         { m_eMessageOrigin = EMessageOrigin; }

////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // SE_SMS_MESSAGE_H
