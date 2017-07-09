////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_core.h"
#include "se_serialdevice.h"
#include "se_atbasicparser.h"

#include "se_ateventhandler.h"
//	Qt Includes
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QTimerEvent>

#include <QVariant>
////////////////////////////////////////////////////////////////////////////////
							   
////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin    
////////////////////////////////////////////////////////////////////////////////

//
//	Class CSerialDevice
//

CSerialDevice::CSerialDevice(QObject* pParent)
	: Base(pParent),
	  m_pEventParser( nullptr ),
	  m_pCommandParser( nullptr ),
	  m_pNeedsMoreData( nullptr ),
	  m_bIdenticResponceRead( true ),
	  m_nTimerId( -1 ),
	  m_bRejectDublicates( true )
{
	//
	//	Make Connections
	//
	SE_CONNECT(&m_oSerialPort, readyRead(), this, onReadyRead());
	SE_CONNECT(&m_oSerialPort, error(QSerialPort::SerialPortError), 
		               this, onError(QSerialPort::SerialPortError));

}

// ExecuteFirstInQueue
void CSerialDevice::ExecuteFirstInQueue()
{
	if (m_qExecutionQueue.isEmpty())
		return;
	t_ATCommandPtr pAtCommand = m_qExecutionQueue.head();
	QByteArray aData = pAtCommand->GetData();
	SE_ASSERT( !aData.isEmpty() );
	pAtCommand->SetStatus( ECommandStatus::Executing );
	// LOG /////////////////////////////////
	LOG( "Executing: " + aData );
	////////////////////////////////////////
	qint64 nWritten = m_oSerialPort.write( aData );
	SE_ASSERT( nWritten == aData.size() );
	m_bIdenticResponceRead = false;
	m_pCommandParser->SetListeningATCommand( pAtCommand );
}

// ExecuteCommand
bool CSerialDevice::ExecuteCommand( t_ATCommandPtr pATCommand )
{
	SE_ASSERT( pATCommand );
	if (!pATCommand)
		return false;
	if (!m_oSerialPort.isOpen())
	{
		if (!m_oSerialPort.open( QIODevice::ReadWrite ))
			// Error will be emitted
			return false;
	}
	
	bool bIsEmptyQueue = m_qExecutionQueue.isEmpty();
	m_qExecutionQueue.enqueue( pATCommand );
	if (bIsEmptyQueue)
		ExecuteFirstInQueue();
	return true;
}

// IsReady
bool CSerialDevice::IsReady() const 
{
	return m_oSerialPort.isOpen();
}

// ProcessCommandResponce
bool CSerialDevice::ProcessCommandResponce( QStringList& lstResponce )
{
	if (m_qExecutionQueue.isEmpty())
		return false;

	t_ATCommandPtr pAtCommand = m_qExecutionQueue.head();
	QString sResponce = lstResponce.first();
	// Check is it an identic responce
	if (!m_bIdenticResponceRead)
	{
		QString sData = pAtCommand->GetData();
		if (sData == sResponce)
		{
			m_bIdenticResponceRead = true;
			lstResponce.removeFirst();
			return true;
		}
	}

	if (!m_pCommandParser)
		// Unable to parse
		return false;

	m_pCommandParser->SetListeningATCommand( pAtCommand );
	EParsingStatus eStatus;
	t_ATResponceSharedPtr pResult = m_pCommandParser->Parse( lstResponce, &eStatus );
	if (eStatus == EParsingStatus::Processed)
	{
		emit sigResponseReady( pAtCommand );
		// Dequeue AT command
		if (!m_qExecutionQueue.isEmpty())
		{
			m_qExecutionQueue.dequeue();
			m_pCommandParser->Clear();
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		LOG( "Processing Succedded: " + sResponce + ":::: AT Command: " + pAtCommand->GetData() );
		//////////////////////////////////////////////////////////////////////////////////////////
					
		// Suspicious responce is recognized so stop timer
		if (m_nTimerId != -1)
		{
			QObject::killTimer( m_nTimerId );
			//////////////////////////////////////////////////////////////////////////////////////////
			LOG( "Timmer Killed: ID: " + QString::number(m_nTimerId) + sResponce + ":::: AT Command: " + pAtCommand->GetData() );
			//////////////////////////////////////////////////////////////////////////////////////////
			m_nTimerId = -1;
		}

		ExecuteFirstInQueue();
		return true;
	}
	else if (eStatus == EParsingStatus::NeedMoreData)
	{
		ResetMoreDataWaitor( m_pCommandParser );
		return true;
	}

	///////////////////////////////////////////////
	LOG( "Processing Failed: " + sResponce );
	///////////////////////////////////////////////

	return false;
}

// ProcessResponce
bool CSerialDevice::ProcessResponce( t_IATParserSPtr pParser, QStringList& lstResponce )
{
	SE_ASSERT( pParser );
	SE_ASSERT( !lstResponce.isEmpty() );
	if (lstResponce.isEmpty())
		return true;
	// Start Parsing
	EParsingStatus eStatus;
	t_ATResponceSharedPtr pResponce = pParser->Parse( lstResponce, &eStatus );
	if (eStatus == EParsingStatus::Processed)
	{
		EATResponceMajorType eMajorType = pResponce->GetMajorType();
		if (eMajorType == EATResponceMajorType::ATCommandResponce)
		{
			SE_ASSERT( !m_qExecutionQueue.isEmpty() );
			t_ATCommandPtr pAtCommand = m_qExecutionQueue.head();
			emit sigResponseReady( pAtCommand );
			// Dequeue AT command
			if (!m_qExecutionQueue.isEmpty())
			{
				m_qExecutionQueue.dequeue();
				m_pCommandParser->Clear();
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			//LOG( "Processing Succedded: " + sResponce + ":::: AT Command: " + pAtCommand->GetData() );
			//////////////////////////////////////////////////////////////////////////////////////////

			// Suspicious responce is recognized so stop timer
			if (m_nTimerId != -1)
			{
				QObject::killTimer( m_nTimerId );
				//////////////////////////////////////////////////////////////////////////////////////////
				//LOG( "Timmer Killed: ID: " + QString::number( m_nTimerId ) + sResponce + ":::: AT Command: " + pAtCommand->GetData() );
				//////////////////////////////////////////////////////////////////////////////////////////
				m_nTimerId = -1;
			}

			ExecuteFirstInQueue();
			return true;
		}
		else if (eMajorType == EATResponceMajorType::ATEvent)
		{
			// If the event is RSSI info then emit only the last value
			if (pResponce->GetType() == EATResponceType::RSSI)
				m_pLastRssiEvent = pResponce;
			else
				emit sigATEvent( pResponce );
			return true;
		}
		else // eMajorType == EATResponceMajorType::Unknown
		{
			SE_ASSERT( false );
			return true;
		}
	}
	else if (eStatus == EParsingStatus::NeedMoreData)
	{
		ResetMoreDataWaitor( pParser );
		return true;
	}
	else if (eStatus == EParsingStatus::Unrecognised)
	{
		WriteInTempFile( QString( "Unrecognized : %1" ).arg( lstResponce.first() ) );
		return m_qExecutionQueue.isEmpty()? true : false;
	}
	else
	{
		WriteInTempFile( QString( "ParsingFailed: %1" ).arg( lstResponce.first() ) );
		return m_qExecutionQueue.isEmpty() ? true : false;
	}
}

// TryProcessCommandResponce
bool CSerialDevice::TryProcessCommandResponce( QStringList& lstResponce )
{
	SE_ASSERT( m_pCommandParser );
	if (!m_pCommandParser)
		return false;
	SE_ASSERT( !lstResponce.isEmpty() );
	if (lstResponce.isEmpty())
		return true;
	// Start Parsing
	EParsingStatus eStatus;
	QString sResponce = lstResponce.first();
	t_ATResponceSharedPtr pResponce = m_pCommandParser->Parse( lstResponce, &eStatus );
	if (eStatus == EParsingStatus::Processed)
	{
		SE_ASSERT( pResponce );
		EATResponceMajorType eMajorType = pResponce->GetMajorType();
		if (eMajorType == EATResponceMajorType::ATCommandResponce)
		{
			SE_ASSERT( !m_qExecutionQueue.isEmpty() );
			// Dequeue AT command
			/*if (!m_qExecutionQueue.isEmpty())
			{*/
			t_ATCommandPtr pAtCommand = m_qExecutionQueue.head();
			emit sigResponseReady( pAtCommand );
			m_qExecutionQueue.dequeue();
			m_pCommandParser->Clear();
			//}
			//////////////////////////////////////////////////////////////////////////////////////////
			LOG( "Processing Succedded: " + sResponce + ":::: AT Command: " + pAtCommand->GetData() );
			//////////////////////////////////////////////////////////////////////////////////////////

			// Suspicious responce is recognized so stop timer
			if (m_nTimerId != -1)
			{
				QObject::killTimer( m_nTimerId );
				//////////////////////////////////////////////////////////////////////////////////////////
				LOG( "Timmer Killed: ID: " + QString::number( m_nTimerId ) + sResponce + ":::: AT Command: " + pAtCommand->GetData() );
				//////////////////////////////////////////////////////////////////////////////////////////
				m_nTimerId = -1;
			}

			ExecuteFirstInQueue();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (eStatus == EParsingStatus::NeedMoreData)
	{
		SE_ASSERT( m_qExecutionQueue.isEmpty() );
		ResetMoreDataWaitor( m_pCommandParser );
		return true;
	}
	/*else if (eStatus == EParsingStatus::Unrecognised)
	{
		WriteInTempFile( QString( "Unrecognized : %1" ).arg( lstResponce.first() ) );
		return m_qExecutionQueue.isEmpty() ? true : false;
	}
	else
	{
		WriteInTempFile( QString( "ParsingFailed: %1" ).arg( lstResponce.first() ) );
		return m_qExecutionQueue.isEmpty() ? true : false;
	}*/
	else
	{
		return false;
	}
}

// TryProcessCommandResponce
bool CSerialDevice::TryProcessEventResponce( QStringList& lstResponce )
{
	SE_ASSERT( m_pEventParser );
	if (!m_pEventParser)
		return false;
	SE_ASSERT( !lstResponce.isEmpty() );
	if (lstResponce.isEmpty())
		return true;
	// Start Parsing
	EParsingStatus eStatus;

	////////////// Bad Solution: SHOULD BE CORRECTED AS SOON AS POSSIBLE
	t_ATResponceSharedPtr pResponce;
	try
	{
		pResponce = m_pEventParser->Parse( lstResponce, &eStatus );
	}
	catch (std::exception const& oErr)
	{
		ExecuteCommand( t_ATCommandPtr( new CATCommand( "AT+CNMA=2\r", true ) ) );
		emit sigError( CError( tr( "Failed to receive message: %1" ).arg( oErr.what() ) ) );
		return false;
	}

	//////////////

	if (eStatus == EParsingStatus::Processed)
	{
		SE_ASSERT( pResponce );
		EATResponceMajorType eMajorType = pResponce->GetMajorType();
		if (eMajorType == EATResponceMajorType::ATEvent)
		{
			// If the event is RSSI info then emit only the last value
			if (pResponce->GetType() == EATResponceType::RSSI)
				m_pLastRssiEvent = pResponce;
			else
				emit sigATEvent( pResponce );
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (eStatus == EParsingStatus::NeedMoreData)
	{
		ResetMoreDataWaitor( m_pCommandParser );
		return true;
	}
	else
	{
		return false;
	}
}

// onReadyRead
void CSerialDevice::onReadyRead()
{
	QByteArray aData = m_oSerialPort.readAll();
	SE_ASSERT( !aData.isEmpty() );
	if (aData.isEmpty())
		return;

	//////////////////////////////
	// Prepere AT Item List
	QString sData( aData );
	QStringList lstResponce = sData.split( "\r\n", QString::SkipEmptyParts );
	

#ifdef SE_LOGGING_ON
	// Ignore unused periodic evvents [Hardcoded]
	for (int i = 0; i < lstResponce.size(); ++i)
	{
		QString sCurrent = lstResponce[i];
		if (sCurrent.startsWith( "^DSFLOWRPT" ) || sCurrent.startsWith( "^RSSI" ))
			lstResponce.removeAt( i );
	}
	QString sLog = lstResponce.join( "\r\n" );
	// LOG
	if (!sLog.trimmed().isEmpty())
		LOG( "Ready Read: " + sLog );
#endif
	
	while (!lstResponce.isEmpty())
	{
		if (!m_qExecutionQueue.isEmpty())
		{
			if (!m_bRejectDublicates && !m_bIdenticResponceRead)
			{
				// Check is it an identic responce
				SE_ASSERT( !m_qExecutionQueue.isEmpty() );
				QString sResponce = lstResponce.first();
				t_ATCommandPtr pAtCommand = m_qExecutionQueue.head();
				QString sData = pAtCommand->GetData();
				if (sData == sResponce)
				{
					m_bIdenticResponceRead = true;
					lstResponce.removeFirst();
					continue;
				}
			}
			else
			{
				if (TryProcessCommandResponce( lstResponce ))
					continue;
			}
		}

		// If IP is here it means that the responce should be an event or unrecugnized token.
		if (TryProcessEventResponce( lstResponce ))
			continue;
		//
		//	Unrecognized
		//
		QString sCurrentResponce = lstResponce.first();
		lstResponce.removeFirst();
		// LOG
		WriteInTempFile( QString( "Unrecognized %1: %2" )
						 .arg( m_qExecutionQueue.isEmpty() ? "Event" : "Command" )
						 .arg( sCurrentResponce ) );
		// Start timmer to avoid from unsupported responce traffics
		if (!m_qExecutionQueue.isEmpty() && m_nTimerId == -1)
		{
			// LOG //////////////////////////////////////////////////////////
			QString sHeadName;
			if (m_qExecutionQueue.head())
				sHeadName = m_qExecutionQueue.head()->GetData();
			LOG( "AT Trafic Jam: Timer started. Queue Head: " + sHeadName );
			//////////////////////////////////////////////////////////////////
			m_nTimerId = QObject::startTimer( 8000 );
		}
	}

	// Emit last RSSI value
	if (m_pLastRssiEvent)
	{
		emit sigATEvent( m_pLastRssiEvent );
		m_pLastRssiEvent.reset();
	}
}



// WriteInTempFile
void CSerialDevice::WriteInTempFile( QString const& sData ) const
{
	QDir oTemp = QDir::temp( );
	oTemp.mkdir( "ColumbaTemp" );
	oTemp.cd( "ColumbaTemp" );
	if (oTemp.exists( ))
	{
		QString sFile = oTemp.path() + "/UnrecognizedCommands.dat";
		QFile oFile( sFile );
		if (oFile.open( QIODevice::Append ))
		{
			oFile.write( "Command: " + sData.toLatin1( ) );
			oFile.write( " DateTime: " +
						 QDateTime::currentDateTime( ).toString( ).toLatin1( ) + "\n" );
		}
	}
}

// timerEvent
void CSerialDevice::timerEvent( QTimerEvent *pEvent )
{
	if (!m_qExecutionQueue.isEmpty())
	{
		t_ATCommandPtr pATCmd = m_qExecutionQueue.dequeue();
		// LOG /////////////////////////////////
		LOG( "Time Out: " + pATCmd->GetData() );
		////////////////////////////////////////
		pATCmd->SetStatus( ECommandStatus::Failed );
		emit sigResponseReady( pATCmd );
		ExecuteFirstInQueue();
	}

	SE_ASSERT( pEvent );
	if (pEvent)
	{
		int nId = pEvent->timerId();
		SE_ASSERT( nId == m_nTimerId );
		QObject::killTimer( nId );
		m_nTimerId = -1;
	}
}

QSerialPort const& CSerialDevice::Port() const
{
	return m_oSerialPort;
}

// SetPort
void CSerialDevice::SetPort( QSerialPortInfo const& oPort )
{
	m_oSerialPort.setPort( oPort );
	// Try to open the serial port [SHOULD BE TESTED]
	bool bOk = m_oSerialPort.open( QIODevice::ReadWrite );
	int i = 9;
	// If port opening will fail the apropriate error signal will be emitted
}

// ClosePort
void CSerialDevice::ClosePort()
{
	m_oSerialPort.close();
}

// TestPort
bool CSerialDevice::TestPort( QSerialPortInfo const& oPort,
							  QString* pMessage )
{
	QSerialPort oTestPort;
	oTestPort.setPort( oPort );
	bool bOk = oTestPort.open( QIODevice::ReadWrite );
	if (!bOk && pMessage)
		*pMessage = oTestPort.errorString();

	return bOk;
}

// SetParser
void CSerialDevice::SetEventParser( t_IATParserSPtr pParser )
{
	m_pEventParser = pParser;
}

// SetParser
void CSerialDevice::SetCommandParser( t_CommandResponceParserSPtr pParser )
{
	m_pCommandParser = pParser;
}

// onError
void CSerialDevice::onError( QSerialPort::SerialPortError eError )
{
	if (eError == QSerialPort::NoError)
		return;
	QString sErrMsg = m_oSerialPort.errorString();
	CError oError( CError::eSerialPortError, sErrMsg );
	emit sigError( oError );
}



////////////////////////////////////////////////////////////////////////////////
//
//	CSerialDeviceCreator
//

// Create
CSerialDevice* CSerialDeviceCreator::CreateSerialDevice( QObject* pParent )
{
	//
	//	Configure AT Parser
	//
	t_ATEventParserSPtr pEventParser( new CATEventParser() );
	t_CommandResponceParserSPtr pCommandParser( new CATCommandResponceParser() );
	// Event Handlers
	se::CRssiEventHandler* pRssiHandler = new se::CRssiEventHandler();
	se::CATNewMessageHandler* pNewMessageHandler = new se::CATNewMessageHandler();
	//se::CCMSErrorHandler* pCMSErrorHandler = new se::CCMSErrorHandler();
	//se::CDefaultOkHandler* pOkHandler = new se::CDefaultOkHandler();

	pEventParser->AddHandler( pRssiHandler, true );
	pEventParser->AddHandler( pNewMessageHandler, true );
	//pEventParser->AddHandler( pCMSErrorHandler, true );
	//m_pEventParser ->AddHandler( pOkHandler );
	// Create Device
	CSerialDevice* pDevice = new CSerialDevice( pParent );
	pDevice->SetEventParser( pEventParser );
	pDevice->SetCommandParser( pCommandParser );

	return pDevice;
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////
