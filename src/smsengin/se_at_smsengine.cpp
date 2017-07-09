////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_at_smsengine.h"
#include "se_atcommands.h"
#include "se_ateventobjects.h"
#include "se_pduconverter.h"
//	Qt Includes
#include <QThread>
////////////////////////////////////////////////////////////////////////////////

//#include <windows.h>
//#include <io.h>
//#include <Lm.h>
//#include <Lmwksta.h>
//#include <sys/types.h>
//#include <sys/utime.h>
//#include <sys/stat.h>
//#include <malloc.h>

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin    
////////////////////////////////////////////////////////////////////////////////

//!
//!	Tasks
//!
////////////////////////////////////////////////////////////////////////////////
//
//	nested class CSetSMSMode
//
// PrepareCommand
t_ATCommandPtr CATSmsEngine::CSetSMSMode::PrepareCommand( t_ATCommandPtr pPrevious ) 
{
	t_ATCommandPtr pCMGF( new CAT_CMGF( m_eMode ) );
	return pCMGF;
}

// ProcessResponce
ITaskStep::STaskStepResult CATSmsEngine::CSetSMSMode::ProcessResponce( t_ATCommandPtr pResponce )
{
	STaskStepResult oResult;
	SE_ASSERT( pResponce );
	if (!pResponce)
	{
		oResult.eStatus = ETaskStatus::Failed;
		oResult.sErrorMesage = tr( "Unexpected Result" );
		return oResult;
	}
	oResult.eStatus = static_cast<ETaskStatus>( pResponce->GetStatus());
	if (oResult.eStatus == ETaskStatus::Succeeded)
	{
		// Applie engine mode
		m_oEngine.m_eMode = this->m_eMode;
		t_ATCommandPtr pCNMI( new CATCommand( "AT+CNMI=1,2", true ) );
		oResult.pNextTaskStep = new CSingleCommandExecution( pCNMI );
	}
	else if (oResult.eStatus == ETaskStatus::Failed)
	{
		QString sMode = m_eMode == ESmsMode::eTextMode ? tr( "Text" ) : tr( "PDU" );
		oResult.sErrorMesage = tr( "%1 mode is not supported" ).arg( sMode );

		// Notify
		CError oErr( CError::eNotSupported, oResult.sErrorMesage );
		emit m_oEngine.sigError( oErr );
	}
	return oResult;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	nested class CCheckSMSMode
//
t_ATCommandPtr   CATSmsEngine::CCheckSMSMode::PrepareCommand( t_ATCommandPtr pPrevious )
{
	t_ATCommandPtr pCMGF( new CAT_CMGF() );
	pCMGF->SetRole( ECommandRole::eCheckState );
	return pCMGF;
}

ITaskStep::STaskStepResult CATSmsEngine::CCheckSMSMode::ProcessResponce( t_ATCommandPtr pResponce )
{
	STaskStepResult oResult;
	SE_ASSERT( pResponce );
	if (!pResponce)
	{
		oResult.eStatus = ETaskStatus::Failed;
		oResult.sErrorMesage = tr( "Unexpected Result" );
		return oResult;
	}
	oResult.eStatus = static_cast<ETaskStatus>(pResponce->GetStatus());
	if (oResult.eStatus == ETaskStatus::Succeeded)
	{
		CAT_CMGF* pCMGF = dynamic_cast<CAT_CMGF*>(pResponce.get());
		if (pCMGF)
		{
			C_CMGF_State* pState = dynamic_cast<C_CMGF_State*>(pCMGF->GetResponce());
			if (pState)
			{
				ESmsMode eMode = pState->GetMode();
				// Set Engine Mode
				m_oEngine.m_eMode = eMode;
			}
		}
	}
	else if (oResult.eStatus == ETaskStatus::Failed)
	{
		oResult.sErrorMesage = tr( "Failed to check current SMS mode" );
		CError oErr( CError::eStateCheckFailed, oResult.sErrorMesage );
		//m_oEngine.m_eMode = ESmsMode::eUnknown;
		emit m_oEngine.sigError( oErr );
	}
	return oResult;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	nested class CResetMessageReceivingConfig
//
t_ATCommandPtr   CATSmsEngine::CResetMessageReceivingConfig::PrepareCommand( t_ATCommandPtr pPrevious )
{
	t_ATCommandPtr pCNMI;
	if (m_bIsReseted)
	{
		pCNMI.reset( new CAT_CNMI( 1, 2 ) );
	}
	else
	{
		pCNMI.reset( new CAT_CNMI() );
		pCNMI->SetRole( ECommandRole::eCheckState );
	}
	return pCNMI;
}

ITaskStep::STaskStepResult CATSmsEngine::CResetMessageReceivingConfig::ProcessResponce( t_ATCommandPtr pResponce )
{
	STaskStepResult oResult;
	SE_ASSERT( pResponce );
	if (!pResponce)
	{
		oResult.eStatus = ETaskStatus::Failed;
		oResult.sErrorMesage = tr( "Unexpected Result" );
		return oResult;
	}
	oResult.eStatus = static_cast<ETaskStatus>(pResponce->GetStatus());
	if (oResult.eStatus == ETaskStatus::Succeeded)
	{
		CAT_CNMI* pCNMI_ChaeckState = dynamic_cast<CAT_CNMI*>(pResponce.get());
		if (pCNMI_ChaeckState)
		{
			C_CNMI_State* pState = dynamic_cast<C_CNMI_State*>(pCNMI_ChaeckState->GetResponce());
			if (pState)
			{
				int nMT = pState->GetMT();
				if (nMT == 0)
				{
					// Reseted
					CResetMessageReceivingConfig* pResetTask = new CResetMessageReceivingConfig( m_oEngine, true );
					oResult.pNextTaskStep = pResetTask;
				}
				else
				{
					if (!m_bIsReseted)
					{
						//m_oEngine.m_pWorkerThread->sleep( 100 );
						for (int i = 0; i < 100000000; ++i);
						CResetMessageReceivingConfig* pReCheckTask = new CResetMessageReceivingConfig( m_oEngine );
						oResult.pNextTaskStep = pReCheckTask;
					}
					else
						oResult.pNextTaskStep = nullptr;
				}
			}
		}
	}
	else if (oResult.eStatus == ETaskStatus::Failed)
	{
		oResult.sErrorMesage = tr( "Failed to check current SMS receiving configuration" );
		CError oErr( CError::eStateCheckFailed, oResult.sErrorMesage );
		emit m_oEngine.sigError( oErr );
	}
	return oResult;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	nested class CSendSMS_Step1
//
t_ATCommandPtr   CATSmsEngine::CSendSMS_Step1::PrepareCommand( t_ATCommandPtr pPrevious )
{
	SE_ASSERT( !m_lstPDU.isEmpty() );
	int nPduLength = CPDUConverter::CalculatePduLengthToSubmit( m_lstPDU.first() );
	return t_ATCommandPtr( new CAT_CMGS_Step1( nPduLength ) );
}

ITaskStep::STaskStepResult CATSmsEngine::CSendSMS_Step1::ProcessResponce( t_ATCommandPtr pResponce )
{
	STaskStepResult oResult;
	SE_ASSERT( pResponce );
	if (!pResponce)
	{
		oResult.eStatus = ETaskStatus::Failed;
		oResult.sErrorMesage = tr( "Unexpected Result" );
		m_oEngine.m_nSendingCount--;
		return oResult;
	}
	oResult.eStatus = static_cast<ETaskStatus>(pResponce->GetStatus( ));
	if (oResult.eStatus == ETaskStatus::Succeeded)
	{
		CSendSMS_Step2* pSend2 = new CSendSMS_Step2( m_oEngine, m_lstPDU );
		oResult.pNextTaskStep = pSend2;
	}
	else if (oResult.eStatus == ETaskStatus::Failed)
	{
		oResult.sErrorMesage = tr( "Failed to send message" );
		CError oErr( CError::eSmsSendFailed, oResult.sErrorMesage );
		m_oEngine.m_nSendingCount--;
		emit m_oEngine.sigError( oErr );
	}
	return oResult;
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	nested class CSendSMS_Step2
//
t_ATCommandPtr   CATSmsEngine::CSendSMS_Step2::PrepareCommand( t_ATCommandPtr pPrevious )
{
	SE_ASSERT( !m_lstPDU.isEmpty() );
	return t_ATCommandPtr( new CAT_CMGS_Step2( m_lstPDU.first() ) );
}

ITaskStep::STaskStepResult CATSmsEngine::CSendSMS_Step2::ProcessResponce( t_ATCommandPtr pResponce )
{
	STaskStepResult oResult;
	SE_ASSERT( pResponce );
	if (!pResponce)
	{
		oResult.eStatus = ETaskStatus::Failed;
		oResult.sErrorMesage = tr( "Unexpected Result" );
		return oResult;
	}
	oResult.eStatus = static_cast<ETaskStatus>(pResponce->GetStatus( ));
	if (oResult.eStatus == ETaskStatus::Succeeded)
	{
		m_lstPDU.removeFirst();
		if (!m_lstPDU.isEmpty())
			oResult.pNextTaskStep = new CSendSMS_Step1( m_oEngine, m_lstPDU );
		else
		{
			m_oEngine.sigMessageWasSent( nullptr /*TODO*/ );
			m_oEngine.m_nSendingCount--;
		}
	}
	else if (oResult.eStatus == ETaskStatus::Failed)
	{
		oResult.sErrorMesage = tr( "Failed to send message" );
		CError oErr( CError::eSmsSendFailed, oResult.sErrorMesage );
		m_oEngine.m_nSendingCount--;
		emit m_oEngine.sigError( oErr );
	}
	return oResult;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//	class CATSmsEngine
//
//! CATSmsEngine
CATSmsEngine::CATSmsEngine( QObject* pParent )
	: Base( pParent ),
	m_pDevice( nullptr ),
	m_eMode( ESmsMode::eUnknown ),
	m_bIsConfigured( false ),
	m_nSendingCount( 0 )
	//m_pWorkerThread(nullptr)
	//m_pTaskRunner( nullptr )
{
	//m_pTaskRunner = new CTaskRunner( this );
	//// Make Connections
	//SE_CONNECT( m_pTaskRunner, sigResponceReady(STaskResult const&), 
	//			this,			 onTaskFinished(STaskResult const&) );
}

// Destructor
CATSmsEngine::~CATSmsEngine()
{
	//if (m_pWorkerThread)
	//{
	//	// closing worker thread 
	//	m_pWorkerThread->quit();
	//	SE_VERIFY( m_pWorkerThread->wait( 5000 ) );
	//	delete m_pWorkerThread;
	//	m_pWorkerThread = nullptr;
	//}
}
// SendMessage
void CATSmsEngine::SendMessage( CSmsMessage const& sMessage,
								QString const& sSMSC )
{
	// Check SMSC
	QString sSMSCNumber = sSMSC;
	if (sSMSCNumber.isEmpty())
	{
		if (IsConfigured())
			sSMSCNumber = GetSMSCNumber();
		else
			// Check SMSC
			sSMSCNumber = CheckSMSCNumber();

		if (sSMSCNumber.isEmpty())
		{
			CError eErr( CError::eSmsSendFailed, tr( "Invalid SMS service senter number" ) );
			emit sigError( eErr );
			return;
		}
	}

	QStringList lstPDUs;
	try 
	{
		lstPDUs = CPDUConverter::Encode( sMessage, sSMSCNumber );
	}
	catch (std::exception& oErr)
	{
		CError eErr( CError::eSmsSendFailed, tr( "Encoding failed: %1" ).arg( oErr.what() ) );
		emit sigError( eErr );
		return;
	}

	if (lstPDUs.isEmpty())
	{
		// TODO
		return;
	}
	CTaskRunner* pTaskRunner = CreateTaskRunner( );
	SE_ASSERT( pTaskRunner );
	CSendSMS_Step1* pSendSMS = new CSendSMS_Step1( *this, lstPDUs );
	pTaskRunner->Start( pSendSMS );

	//
	m_nSendingCount++;
}


// GetSmsGetway
ISmsGetway* CATSmsEngine::GetSmsGetway()
{
	return m_pDevice;
}

// SetMode
void CATSmsEngine::SetMode( ESmsMode eMode )
{
	if (eMode == ESmsMode::eUnknown || m_eMode == eMode)
		return;
	CTaskRunner* pTaskRunner = CreateTaskRunner();
	SE_ASSERT( pTaskRunner );
	if (!pTaskRunner)
	{
		CError eErr( CError::eInvalidInternalConfiguration, tr( "Task runner is missing." ) );
		emit sigError( eErr );
		return;
	}

	CSetSMSMode* pSetModeTask = new CSetSMSMode( *this, eMode );
	bool bOk = pTaskRunner->Start( pSetModeTask );
	if (!bOk)
	{
		CError eErr( CError::eInvalidInternalConfiguration, tr( "Request Failed" ) );
		emit sigError( eErr );
	}
}

// GetMode
ESmsMode CATSmsEngine::GetMode() const
{
	return m_eMode;
}


ESmsMode CATSmsEngine::CheckCurrentMode( qint64 nMsecs )
{
	CTaskRunner* pTaskRunner = CreateTaskRunner();
	SE_ASSERT( pTaskRunner );
	CCheckSMSMode* pCheckModeTask = new CCheckSMSMode( *this );
	
	m_eMode = ESmsMode::eUnknown;
	bool bOk = pTaskRunner->Start( pCheckModeTask );
	if (!bOk)
	{
		CError eErr( CError::eInvalidInternalConfiguration, tr( "Request Failed" ) );
		sigError( eErr );
		return m_eMode;
	}
	
	// Sync
	pTaskRunner->WaitForFinish( nMsecs );
	// Mode should already be updated
	return m_eMode;
}


// GetDevice
IATDevice* CATSmsEngine::GetDevice() const
{
	return m_pDevice;
}

// SetDevice
void CATSmsEngine::SetDevice( IATDevice* pDevice )
{
	if (m_pDevice == pDevice)
		return;
	m_pDevice = pDevice;
	m_bIsConfigured = false;
	/*SE_ASSERT( m_pTaskRunner );
	m_pTaskRunner->SetATDevice( pDevice );*/
	// Make Connections
	SE_CONNECT( pDevice, sigATEvent(se::t_ATResponceSharedPtr),
				this,     onATEvent(se::t_ATResponceSharedPtr) );
	SE_CONDUCT(pDevice, sigError(se::CError const&),
				this,   sigError(se::CError const&) );

	// By default set PDU mode
	//SetMode( ESmsMode::ePDUMode );
}

// onATEvent
void CATSmsEngine::onATEvent( se::t_ATResponceSharedPtr pEvent )
{
	SE_ASSERT( pEvent );
	if (!pEvent)
		return;
	EATResponceType eType = pEvent->GetType();
	switch (eType)
	{
		case EATResponceType::RSSI:
			{
				CRssiData* pRSSI = dynamic_cast<CRssiData*>(pEvent.get());
				SE_ASSERT( pRSSI );
				if (pRSSI)
				{
					int nValue = pRSSI->GetValue();
					emit sigRssiChanged( nValue );
				}
				break;
			}
		case EATResponceType::NewMessage:
			{
				CNewMessageData* pNewMessage = dynamic_cast<CNewMessageData*>(pEvent.get());
				SE_ASSERT( pNewMessage );
				if (pNewMessage)
				{
					// Notify about new SMS message
					t_SmsMessageSharedPtr pSMS = pNewMessage->GetMessage();
					emit sigNewMessage( pSMS );


					/////////////////// CONFIGURATION
					/*m_pWorkerThread = new QThread( this );
					*/
					/*CTaskRunner* pRunner = CreateTaskRunner();
					CResetMessageReceivingConfig* pSetConfig = new CResetMessageReceivingConfig( *this, true );
					pRunner->Start( pSetConfig );
					pRunner->WaitForFinish();

					CTaskRunner* pTaskRunner = CreateTaskRunner();
					SE_ASSERT( pTaskRunner );
				
					CResetMessageReceivingConfig* pConfigureCNMI = new CResetMessageReceivingConfig( *this );
					bool bOk = pTaskRunner->Start( pConfigureCNMI );
					if (bOk)
					{
						pTaskRunner->WaitForFinish();
						STaskResult strResuly = pTaskRunner->GetResult();
						bOk &= strResuly.eStatus == ETaskStatus::Succeeded;
					}
					else
					{
						CError eErr( CError::eUnexpectedResult, tr( "Unable to configure message receiving policy!" ) );
						emit sigError( eErr );
					}*/

					ExecuteATCommand( "AT+CNMA=1" );
				}
				break;
			}
		default:
			break;
	}
}

// onTaskFinished
void CATSmsEngine::onTaskFinished( STaskResult const& strResponce )
{
	
	//QString sStatus = strResponce.eStatus == ETaskStatus::Succeeded ? "Succeeded" : "Failed";
	//CError eErr( CError::eUnexpectedResult, "Last Task Finished: Status: " + sStatus );
	//emit sigError( eErr );

	emit sigLastTaskFinished( strResponce );

	// Delete Task Runner
	CTaskRunner* pTaskRunner = qobject_cast<CTaskRunner*>(sender());
	SE_ASSERT( pTaskRunner );
	if (pTaskRunner)
		pTaskRunner->deleteLater();
}

// ExecuteATCommand
t_ATCommandPtr CATSmsEngine::ExecuteATCommand( QString sATCommand, qint64 nMsecs )
{
	if (sATCommand.isEmpty())
		return t_ATCommandPtr();

	if (sATCommand[sATCommand.size() - 1] != '\r')
		sATCommand.append( '\r' );

	t_ATCommandPtr pATCommand( new CATCommand( sATCommand, true ) );
	CSingleCommandExecution* pTask = new CSingleCommandExecution( pATCommand );
	
	// Create Task Runner
	CTaskRunner* pTaskRunner = CreateTaskRunner();
	SE_ASSERT( pTaskRunner );
	pTaskRunner->Start( pTask );
	// Sync
	pTaskRunner->WaitForFinish( nMsecs );
	return pATCommand;
}

// CheckSMSCNumber
QString CATSmsEngine::CheckSMSCNumber( qint64 nMsecs )
{
	// Create Task Runner
	CTaskRunner* pTaskRunner = CreateTaskRunner();
	SE_ASSERT( pTaskRunner );

	//
	// Get SMSC Number
	//
	// Create SMSC Getting AT command
	t_AT_CSCA_Ptr pCSCA( new CAT_CSCA );
	pCSCA->SetRole( ECommandRole::eCheckState );
	CSingleCommandExecution* pGetSMSCTask = new CSingleCommandExecution( pCSCA );
	if (pTaskRunner->Start( pGetSMSCTask ))
	{
		pTaskRunner->WaitForFinish( nMsecs );
		STaskResult strResuly = pTaskRunner->GetResult();
		if (strResuly.eStatus == ETaskStatus::Succeeded)
		{
			SE_ASSERT( strResuly.pLastCommand == pCSCA );
			C_CSCA_State* pSMSCState = pCSCA->GetResponce();
			SE_ASSERT( pSMSCState );
			if (pSMSCState)
				m_sSMSCNumber = pSMSCState->GetServiceCenterNumber();
		}
		else
		{
			m_sSMSCNumber.clear();
			// ..... Chemnal petkyi te che..
			//CError eErr( CError::eStateCheckFailed, tr( "Unable to check current SMS serive senter information" ) );
			//emit sigError( eErr );
		}
	}
	else
	{
		CError eErr( CError::eUnexpectedResult, tr( "Unable to check current SMS serive senter information" ) );
		emit sigError( eErr );
	}

	return m_sSMSCNumber;
}

// Configure
bool CATSmsEngine::Configure()
{
	//
	//	Check SMSC number
	//
	QString sSMSC = CheckSMSCNumber();
	bool bOk = !sSMSC.isNull();
	// Create Task Runner
	CTaskRunner* pTaskRunner = CreateTaskRunner();
	SE_ASSERT( pTaskRunner );

	//
	// Configure CNMI
	//
	CResetMessageReceivingConfig* pConfigureCNMI = new CResetMessageReceivingConfig( *this, true );
	bOk &= pTaskRunner->Start( pConfigureCNMI );
	if (bOk)
	{
		pTaskRunner->WaitForFinish( 5000 );
		STaskResult strResuly = pTaskRunner->GetResult();
		bOk &= strResuly.eStatus == ETaskStatus::Succeeded;
	}
	else
	{
		CError eErr( CError::eUnexpectedResult, tr( "Unable to configure message receiving policy!" ) );
		emit sigError( eErr );
	}

	
	m_bIsConfigured = bOk;
	return bOk;
}

// CreateTaskRunner
CTaskRunner* CATSmsEngine::CreateTaskRunner()
{
	if (!m_pDevice)
		return nullptr;
	CTaskRunner* pTaskRunner = new CTaskRunner( /*this*/ );
	SE_ASSERT( pTaskRunner );
	pTaskRunner->SetATDevice( m_pDevice );
	// Make Connections
	SE_CONNECT( pTaskRunner, sigResponceReady(STaskResult const&),
				this, onTaskFinished(STaskResult const&) );
	return pTaskRunner;
}


////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////
