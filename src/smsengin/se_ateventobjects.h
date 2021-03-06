#ifndef SE_AT_EVENT_OBJECTS_H
#define SE_AT_EVENT_OBJECTS_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_at_responce.h"
#include "se_smsmessage.h"
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CRssiData
//
class SMSENGINE_EXPORT CRssiData : public CIntValueResponce
{
	typedef CIntValueResponce Base;
public:
	// Constructor & Destructor
	inline CRssiData( int nRssiValue ) 
		: Base( nRssiValue, EATResponceType::RSSI )
	{}
	virtual ~CRssiData() = default;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CRssiData
//
class SMSENGINE_EXPORT CNewMessageData : public CATResponce
{
	typedef CATResponce Base;
public:
	// Constructor & Destructor
	inline CNewMessageData( t_SmsMessageSharedPtr pSMS )
		: Base( EATResponceType::NewMessage ),
		  m_pSMS( pSMS )
	{}
	virtual ~CNewMessageData() = default;

public:
	// Interface
	inline t_SmsMessageSharedPtr GetMessage() const { return m_pSMS; }

private:
	t_SmsMessageSharedPtr m_pSMS;
};
////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // SE_AT_EVENT_OBJECTS_H
