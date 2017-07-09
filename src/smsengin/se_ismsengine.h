#ifndef SE_SMS_ENGINE_H
#define SE_SMS_ENGINE_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_ismsgateway.h"
#include "se_smsmessage.h"
#include "se_error.h"
#include "se_attask.h"
//
//	Qt Includes
//
#include <QObject>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	Interface ISmsEngin
//
class SMSENGINE_EXPORT ISmsEngine : public QObject
{
	Q_OBJECT

public:
	inline ISmsEngine( QObject* pParent = nullptr );
	virtual ~ISmsEngine() = default;

public:
	//
	//	Main Interface
	//
	// Send SMS message in none blocking mode
	virtual void SendMessage( CSmsMessage const& sMessage,
							  QString const& sSMSCNumber ) = 0;
	// Returns SMS Device
	virtual ISmsGetway* GetSmsGetway() = 0;

signals:
	//
	//	Signals
	//	
	// This signal is emit when new SMS message is received
	void sigNewMessage( se::t_SmsMessageSharedPtr pMessage );
	void sigMessageWasSent( se::t_SmsMessageSharedPtr pMessage );
	void sigLastTaskFinished( se::STaskResult );
	void sigError( se::CError const& oError );
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	Interface ISmsSerialEngin
//
//! Base interface which is performing SMS transmissions via AT commands 
class SMSENGINE_EXPORT IATSmsEngine : public ISmsEngine
{
	Q_OBJECT

public:
	inline IATSmsEngine( QObject* pParent = nullptr );
	virtual ~IATSmsEngine() = default;

public:
	//
	//	Main Interface
	//
	// Set/Get SMS message transmission mode (TEXT | PDU)
	virtual void     SetMode( ESmsMode eMode ) = 0;
	virtual ESmsMode GetMode() const = 0;

	// Get/Set AT Device
	virtual IATDevice* GetDevice() const = 0;
	virtual void SetDevice( IATDevice* pATDevice ) = 0;

signals:
	//
	//	AT Signals
	//	
	void sigRssiChanged( int nNewValue );
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
//
//	class ISmsEngine
//
inline ISmsEngine::ISmsEngine( QObject* pParent )
	: QObject( pParent )
{}

//
//	class IATSmsEngine
//
inline IATSmsEngine::IATSmsEngine( QObject* pParent )
	: ISmsEngine( pParent )
{}

////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // SE_SMS_ENGINE_H
