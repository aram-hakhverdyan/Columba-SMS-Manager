////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_ateventhandler.h"
#include "se_ateventobjects.h"
#include "se_pduconverter.h"
//
//	Qt Includes
//
////////////////////////////////////////////////////////////////////////////////
							   
////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin    
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 
//	class CRssiEventHandler
//
// CRssiEventHandler
CRssiEventHandler::CRssiEventHandler()
	: Base("^RSSI:")
{}

// Handle [RSSI]
t_ATResponceSharedPtr CRssiEventHandler::Handle( QString sData, EParsingStatus& eStatus )
{
	// Remove Key
	sData.remove( 0, GetKey().size() );
	bool bOk = true;
	int nValue = sData.toInt( &bOk );
	t_ATResponceSharedPtr pResponce;
	if (bOk)
	{
		eStatus = EParsingStatus::Processed;
		return t_ATResponceSharedPtr( new CRssiData( nValue ) );
	}
	eStatus = EParsingStatus::Unrecognised;
	return t_ATResponceSharedPtr();
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 
//	class CATNewMessageHandler
//
// CRssiEventHandler
CATNewMessageHandler::CATNewMessageHandler()
	: Base( "+CMT:" ),
	m_bIsHeadParsed( false ),
	m_eSmsMode( ESmsMode::eUnknown )
{}

#define RETURN_INVALID_RESPONCE( _msg_ ) \
	{									 \
		eStatus = EParsingStatus::Failed; \
		return t_ATResponceSharedPtr( new CInvalidEventResponce( _msg_ ) ); \
	}  

void CATNewMessageHandler::Clear()
{
	m_bIsHeadParsed = false;
	m_eSmsMode = ESmsMode::eUnknown;
	m_sPhoneNumber.clear();
}

// Handle [CMT]
t_ATResponceSharedPtr CATNewMessageHandler::Handle( QString sData, EParsingStatus& eStatus )
{
	// Remove " *: "
	/*int nIdx = sData.indexOf( ":" );
	sData.remove( 0, nIdx + 1 );*/
	t_ATResponceSharedPtr pResponce;

	if (!m_bIsHeadParsed)
	{
		QString sKey = GetKey();
		int nKeySize = sKey.size();
		if (sData.left( nKeySize ) != sKey)
		{
			eStatus = EParsingStatus::Unrecognised;
			return t_ATResponceSharedPtr();
		}
		sData.remove( 0, nKeySize );
		// Remove whitespaces
		sData = sData.trimmed();
		SE_ASSERT( !sData.isEmpty() );
		if (sData.isEmpty())
			RETURN_INVALID_RESPONCE( qtr( "Invalid message" ) );

		int nIdx = sData.indexOf( ',' );
		if (nIdx == -1)
			RETURN_INVALID_RESPONCE( qtr( "Invalid message" ) );

		QString sFirst = sData.left( nIdx ).trimmed();
		sData.remove( 0, nIdx + 1 );

		if (!sFirst.isEmpty() && sFirst.at( 0 ) == '"')
		{
			// Text mode SMS message
			m_eSmsMode = ESmsMode::eTextMode;
			// Fetch Phone Number
			int nIdx = sFirst.indexOf( '"', 1 );
			if (nIdx == -1)
				RETURN_INVALID_RESPONCE( qtr( "Invalid message" ) );
			m_sPhoneNumber = sFirst.mid( 1, nIdx - 1 );

			// Fetch Date Time
			int nDTStart = sData.indexOf( '"' );
			if (nDTStart == -1)
				RETURN_INVALID_RESPONCE( qtr( "Invalid message" ) );
			sData.remove( 0, nDTStart + 1 );

			int nDTEnd = sData.indexOf( '"' );
			if (nDTEnd == -1)
				RETURN_INVALID_RESPONCE( qtr( "Invalid message" ) );
			QString sDateTime = sData.left( nDTEnd );
			sData.remove( 0, nDTEnd + 1 );
			QString sTimeZone = sDateTime.right( 3 );
			sDateTime.remove( sDateTime.size() - sTimeZone.size(), sTimeZone.size() );
			bool bOk;
			int nTimeZone = sTimeZone.toInt( &bOk );
			if (!bOk)
				RETURN_INVALID_RESPONCE( qtr( "Invalid message" ) );

			m_oDateTime = CPDUConverter::GetNormalizedTimeStamp( sDateTime, nTimeZone );
			if (!m_oDateTime.isValid())
				RETURN_INVALID_RESPONCE( qtr( "Invalid message" ) );
			
		}
		else
		{
			// PDU mode SMS message
			/*int nIdx = sData.indexOf( "\r\n" );
			if (nIdx == -1)
				RETURN_INVALID_RESPONCE( qtr( "Invalid message" ) );
			QString sPduLength = sData.left( nIdx );
			sData.remove( 0, nIdx );*/
			m_eSmsMode = ESmsMode::ePDUMode;
			int nPDULength = sData.toInt();
		}
		m_bIsHeadParsed = true;
		eStatus = EParsingStatus::NeedMoreData;
	}
	else
	{
		if (m_eSmsMode == ESmsMode::eTextMode)
		{
			QString sSmsMessage = sData.trimmed();
			SE_ASSERT( !m_sPhoneNumber.isEmpty() );
			SE_ASSERT( m_oDateTime.isValid() );
			t_SmsMessageSharedPtr pSMS( new CSmsMessage( m_sPhoneNumber, sSmsMessage, m_oDateTime ) );
			// Store result
			pResponce.reset( new CNewMessageData( pSMS ) );
			eStatus = EParsingStatus::Processed;
		}
		else if (m_eSmsMode == ESmsMode::ePDUMode)
		{
			QString sPduData = sData.trimmed();
			if (sPduData.isEmpty())
				RETURN_INVALID_RESPONCE( qtr( "Invalid message" ) );
			//QString sAckPdu;
			try
			{
				t_SmsMessageSharedPtr pSMS = CPDUConverter::Decode( sPduData/*, &sAckPdu*/ );
				pResponce.reset( new CNewMessageData( pSMS ) );
				eStatus = EParsingStatus::Processed;
			}
			catch ( std::exception const& oErr )
			{
				LOG( QString( "Error: %1 :|: PDU: %2" ).arg( oErr.what() ).arg( sPduData ) );
				throw oErr;
			}
		
		}
		else
		{
			SE_ASSERT( false );
			eStatus = EParsingStatus::Failed;
		}
		Clear();
	}
	return pResponce;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// 
//	class CCMSErrorHandler
//
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////s/////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////
