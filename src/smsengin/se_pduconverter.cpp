////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_pduconverter.h"
#include "se_core.h"
#include "gsm_translator.h"
//	Qt Includes
#include <QDate>
#include <QTime>
#include <QTimeZone>
// PduLib 
#include "Message.h"
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin    
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CPDUConverter
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Encoding
//
// MakeSubmitPDUFromStream
QString CPDUConverter::MakeSubmitPDUFromStream( std::stringstream& sourceSS )
{

	typedef PduLib::Message<PduLib::SmsSubmit> MsgSubmit;
	/*MsgSubmit submit( sourceSS );

	std::stringstream midSS;
	submit.to_stream( midSS );*/
	sourceSS.unsetf( std::ios_base::skipws );

	std::istream_iterator<unsigned char> start( sourceSS );
	std::istream_iterator<unsigned char> end;

	std::stringstream ss;
	ss << std::setfill( '0' ) << std::hex << std::uppercase;
	
	for (; start != end; ++start)
	{
		/*char c = *start;
		int nC = int( c );*/
		ss << std::setw( 2 ) << /*nC*/ (int)*start;
	}
	std::string str = ss.str();
	return QString::fromStdString( str );
}

// MakeSubmitPDUFromStream
QString CPDUConverter::MakeReportActPDUFromStream( std::stringstream& sourceSS )
{
	typedef PduLib::Message<PduLib::SmsDeliverReportAck> MsgReportAck;
	MsgReportAck oReportAct( sourceSS );

	std::stringstream midSS;
	oReportAct.to_stream( midSS );
	midSS.unsetf( std::ios_base::skipws );

	std::istream_iterator<unsigned char> start( midSS );
	std::istream_iterator<unsigned char> end;

	std::stringstream ss;
	ss << std::setfill( '0' ) << std::hex << std::uppercase;

	for (; start != end; ++start)
		ss << std::setw( 2 ) << (int)*start;
	std::string str = ss.str();
	return QString::fromStdString( str );
}

int CPDUConverter::CalculatePduLengthToSubmit( QString const& sPDU )
{
	uint nTrash = sPDU.left( 2 ).toUInt( 0, 16 );
	int nPduLength = (sPDU.size( ) / 2) - (1 + nTrash);
	return nPduLength;
}


// Encode
QStringList CPDUConverter::Encode( CSmsMessage const& oMsg, 
								   QString const& sSMSCNumber,
								   bool bStatusReportRequest )
{
	SE_ASSERT( !sSMSCNumber.isEmpty() );
	typedef PduLib::Message<PduLib::SmsSubmit> MsgSubmit;
	/*try
	{*/
	MsgSubmit submit;

	submit.serviceCenterAddress( sSMSCNumber.toStdString() );
	submit.rejectDuplicates( false );
	submit.statusReportRequest( bStatusReportRequest );
	submit.replyPath( true );
	submit.messageReference( 0x01 );
	QString sPhoneNumber = oMsg.GetPhoneNumber();
	SE_ASSERT( !sPhoneNumber.isEmpty() );
	submit.destinationAddress( sPhoneNumber.toStdString() );
	submit.protocolIdentifier( 0 ); // Implicit Protocol Identifier
	submit.validityPeriod( MsgSubmit::NoValidityPeriod( ) ); // Not Specified

	// Store User Data
	QString sUserData = oMsg.GetMessage();
	// Make SMS user data chunks
	t_lstUserData lstUserData = CGsmDefaultAlphabet::MakeUserData( sUserData );

	QStringList lstResultPDUs;
	for (int i = 0; i < lstUserData.size(); ++i)
	{
		SUserData strUserData = lstUserData[i];
		if (strUserData.eAlphabet == EGsmAlphabets::Default)
		{
			submit.dataCodingScheme(
				MsgSubmit::GeneralDataCodingScheme(
				false,                                  // not compressed
				MsgSubmit::NoMessageClass,              // has no Message Class
				MsgSubmit::DefaultAlphabet ) );           // use Default Alphabet
			submit.userData( MsgSubmit::DefaultUserData( strUserData.sData.toStdString() ) );
		}
		else if (strUserData.eAlphabet == EGsmAlphabets::UCS2)
		{
			submit.dataCodingScheme(
				MsgSubmit::GeneralDataCodingScheme(
				false,                                  // not compressed
				MsgSubmit::NoMessageClass,              // has no Message Class
				MsgSubmit::UCS2Alphabet ) );
			submit.userData( MsgSubmit::UCS2UserData( strUserData.sData.toStdWString() ) );

		}
		else // 8 bit data not supported yet
			SE_ASSERT( false );

		// Ready to make PDU 
		std::stringstream ss;
		submit.to_stream( ss );
		QString sCurrentPDU = MakeSubmitPDUFromStream( ss );
		lstResultPDUs.append( sCurrentPDU );
	}

	// if the message is empty
	if (lstResultPDUs.isEmpty())
	{
		submit.dataCodingScheme(
			MsgSubmit::GeneralDataCodingScheme(
			false,                                  // not compressed
			MsgSubmit::NoMessageClass,              // has no Message Class
			MsgSubmit::DefaultAlphabet ) );           // use Default Alphabet
		submit.userData( MsgSubmit::DefaultUserData( "" ) );
		std::stringstream ss;
		submit.to_stream( ss );
		QString sCurrentPDU = MakeSubmitPDUFromStream( ss );
		lstResultPDUs.append( sCurrentPDU );
	}
	/*}
	catch (std::exception& e)
	{
		std::cout << "An exception was caught: " << e.what( ) << std::endl;
	}*/
	return lstResultPDUs;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	Decoding
//
typedef PduLib::Message<PduLib::SmsDeliver> MsgDeliver;

// MakeHexStream
std::stringstream CPDUConverter::MakeHexStream( QString const& sInput )
{
	std::stringstream ss;
	bool bOk = false;
	int nSize = sInput.size();
	if (nSize % 2 != 0)
		// evn
		throw std::exception( "Corrupted message" );

	for (int i = 0; i < nSize; i += 2)
	{
		QString sHex = sInput.mid( i, 2 );
		int nDec = sHex.toUInt( &bOk, 16 );
		if (!bOk || nDec < 0 || nDec > 255)
			throw std::exception( "Corrupted message" );
		ss << unsigned char( nDec );
	}
	return ss;
}

// GetNormalizedTimeStamp
QDateTime CPDUConverter::GetNormalizedTimeStamp( PduLib::TimeStamp const& ts )
{
	int nYear = (int)ts.year() + 2000;
	int nMonth = (int)ts.month();
	int nDay = (int)ts.day();
	QDate oDate( nYear, nMonth, nDay );
	SE_DEBUG( QString sDate = oDate.toString() );

	int nHour = (int)ts.hour();
	int nMinute = (int)ts.minute();
	int nSecond = (int)ts.second();
	QTime oTime( nHour, nMinute, nSecond );
	SE_DEBUG( QString sTime = oTime.toString() );
	int nTimeZone = (int)ts.timezone() / 4;
	QDateTime oDateTime( oDate, oTime, QTimeZone( 60 * 60 * nTimeZone ) );

	QDateTime oLocalDateTime = oDateTime.toLocalTime();
	SE_DEBUG( QString sLocalDT = oLocalDateTime.toString() );
	return oLocalDateTime;
}

// GetNormalizedTimeStamp
QDateTime CPDUConverter::GetNormalizedTimeStamp( QString const& sDT, int nTimeZone )
{
	QDateTime oDateTime = QDateTime::fromString( sDT, "yy/MM/dd,hh:mm:ss" );
	if (!oDateTime.isValid())
		return oDateTime;

	oDateTime.addYears( 100 );
	SE_DEBUG( QString sDtTm = oDateTime.toString() );
	QDateTime oTargetDateTime( oDateTime.date(), oDateTime.time(), QTimeZone( 60 * 60 * nTimeZone ) );

	QDateTime oLocalDateTime = oDateTime.toLocalTime();
	SE_DEBUG( QString sLocalDT = oLocalDateTime.toString() );
	return oLocalDateTime;
}

// GetOriginatorAddress
QString GetOriginatorAddress( const MsgDeliver& msg )
{
	QString sResult;
	typedef PduLib::EOriginatorAddressType EAddressType;

	std::string sOriginator = msg.originatorAddress();
	EAddressType eAddressType = msg.originatorAddressType();
	if (eAddressType == EAddressType::InternationalFormat)
		sResult = QString::fromStdString( sOriginator );
	else // EAddressType::AlphaFormat
		sResult = CGsmDefaultAlphabet::Translate( sOriginator );
	return sResult;
}

// GetUserData
QString GetUserData( const MsgDeliver& msg )
{
	QString sResult;
	if (MsgDeliver::DefaultAlphabet == msg.alphabet())
	{
		std::string sData = msg.template userData<typename MsgDeliver::DefaultUserData>().userData();
		sResult = CGsmDefaultAlphabet::Translate( sData );
	}
	else if (MsgDeliver::UCS2Alphabet == msg.alphabet())
	{
		std::wstring sData = msg.template userData<typename MsgDeliver::UCS2UserData>().userData();
		sResult = QString::fromStdWString( sData );
	}
	else if (MsgDeliver::EightBitDataAlphabet == msg.alphabet())
	{
		const std::vector<unsigned char>& aData = msg.template userData<typename MsgDeliver::EightBitUserData>().userData();
		int nSize = aData.size();
		for (int i = 0; i < nSize; ++i)
		{
			uchar c = aData[i];
			sResult.append( QChar( c ) );
		}
	}
	return sResult;
}


//
typedef PduLib::Message<PduLib::SmsDeliver> MsgDeliver;
//
// ToReportAck
QString ToReportAckPDU( MsgDeliver const& oDeliver )
{
	typedef PduLib::Message<PduLib::SmsDeliverReportAck> MsgReportAck;
	MsgReportAck oReportAck;
	oReportAck.protocolIdentifier( 0 ); // Implicit Protocol Identifier

	if (MsgDeliver::DefaultAlphabet == oDeliver.alphabet())
	{
		std::string sData = oDeliver.template userData<typename MsgDeliver::DefaultUserData>().userData();
		oReportAck.dataCodingScheme(
			MsgReportAck::GeneralDataCodingScheme(
			false,                                  // not compressed
			MsgReportAck::NoMessageClass,              // has no Message Class
			MsgReportAck::DefaultAlphabet ) );           // use Default Alphabet
		oReportAck.userData( MsgReportAck::DefaultUserData( sData ) );
	}
	else if (MsgDeliver::UCS2Alphabet == oDeliver.alphabet())
	{
		std::wstring sData = oDeliver.template userData<typename MsgDeliver::UCS2UserData>().userData();
		oReportAck.dataCodingScheme(
			MsgReportAck::GeneralDataCodingScheme(
			false,                                  // not compressed
			MsgReportAck::NoMessageClass,              // has no Message Class
			MsgReportAck::UCS2Alphabet ) );
		oReportAck.userData( MsgReportAck::UCS2UserData( sData ) );
	}
	else if (MsgDeliver::EightBitDataAlphabet == oDeliver.alphabet())
	{
		const std::vector<unsigned char>& aData = oDeliver.template userData<typename MsgDeliver::EightBitUserData>().userData();
		oReportAck.dataCodingScheme(
			MsgReportAck::GeneralDataCodingScheme(
			false,                                  // not compressed
			MsgReportAck::NoMessageClass,              // has no Message Class
			MsgReportAck::EightBitDataAlphabet ) );
		oReportAck.userData( MsgReportAck::EightBitUserData( aData ) );
	}

	std::stringstream ss;
	oReportAck.to_stream( ss );
	QString sReportActPDU = CPDUConverter::MakeReportActPDUFromStream( ss );
	return sReportActPDU;
}



// Decode
t_SmsMessageSharedPtr CPDUConverter::Decode( QString const& sDeliveredPDU, QString* psReportAckPDU )
{
	SE_ASSERT( !sDeliveredPDU.isEmpty() );
	if (sDeliveredPDU.isEmpty())
		return t_SmsMessageSharedPtr();

	std::stringstream ss = MakeHexStream( sDeliveredPDU );
	MsgDeliver oDeliver( ss );
	
	// Unused
	if ( psReportAckPDU )
		*psReportAckPDU = ToReportAckPDU( oDeliver );

	QString sSender = GetOriginatorAddress( oDeliver );
	QDateTime oDelivieryTime = GetNormalizedTimeStamp( oDeliver.serviceCentreTimeStamp() );
	QString sData = GetUserData( oDeliver );
	t_SmsMessageSharedPtr pSmsMessage( new CSmsMessage( sSender, sData, oDelivieryTime ) );
	return pSmsMessage;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////
