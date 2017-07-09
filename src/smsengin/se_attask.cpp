////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_attask.h"
//	Qt Includes
#include <QCoreApplication>
#include <QTimerEvent>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin    
////////////////////////////////////////////////////////////////////////////////

//
//	class CSingleCommandExecution
//

// PrepareCommand
t_ATCommandPtr   CSingleCommandExecution::PrepareCommand( t_ATCommandPtr pPrevious )
{
	return m_pATCommand;
}

// ProcessResponce
ITaskStep::STaskStepResult CSingleCommandExecution::ProcessResponce( t_ATCommandPtr pResponce )
{
	// Nothing to do
	STaskStepResult oResult;
	oResult.eStatus = pResponce ? ETaskStatus( pResponce->GetStatus() ) : ETaskStatus::Failed;
	return oResult;
}



//
//	Class CTaskRunner
//
bool CTaskRunner::Start( ITaskStep* pTask, bool bOwner )
{
	ResetCurrentTask( pTask );
	m_bOwner = bOwner;

	if (!m_pCurrentTask || !m_pDevice)
		return false;
	m_pCurrentATCommand = m_pCurrentTask->PrepareCommand( t_ATCommandPtr() );
	if (!m_pCurrentATCommand)
		return false;
	if (m_pDevice->ExecuteCommand( m_pCurrentATCommand ))
	{
		m_anIsRunning = 1;
		return true;
	}
	else
		return false;
}

// SetATDevice
void CTaskRunner::SetATDevice( IATDevice* pDevice )
{
	if (m_pDevice)
	{
		// Disconnect from old device
		SE_DISCONNECT( m_pDevice, sigResponseReady(se::t_ATCommandPtr),
					this,          onResponceReady(se::t_ATCommandPtr) );
	}
	if (pDevice)
	{
		SE_CONNECT( pDevice, sigResponseReady(se::t_ATCommandPtr),
					this,     onResponceReady(se::t_ATCommandPtr) );
	}
	m_pDevice = pDevice;
}

// Finish
void CTaskRunner::Finish( ETaskStatus eStatus,
			 t_ATCommandPtr pLastCommand,
			 QString sErrorMessage )
{
	ResetCurrentTask();
	// Kill Timmer if any
	if (m_nTimmerID != -1)
	{
		killTimer( m_nTimmerID );
		m_nTimmerID = -1;
	}
	STaskResult oTaskResult;
	oTaskResult.pLastCommand = pLastCommand;
	oTaskResult.eStatus = eStatus;
	oTaskResult.sErrorMessage = sErrorMessage;
	m_strLastResult = oTaskResult;
	m_anIsRunning = 0;
	emit sigResponceReady( oTaskResult );
}


// onResponceReady
void CTaskRunner::onResponceReady( se::t_ATCommandPtr pResponceCommand )
{
	//SE_ASSERT( m_pCurrentTask );
	if (!m_pCurrentTask)
		return;
	if (pResponceCommand != m_pCurrentATCommand)
	{
		//////////////////////////////////////////////////////////////////////////////////////////
		LOG( "Foreign AT Command Responce: " + (pResponceCommand ? pResponceCommand->GetData() : "Null Pointer") );
		//////////////////////////////////////////////////////////////////////////////////////////
		return;
	}
 	ITaskStep::STaskStepResult oStepResult = m_pCurrentTask->ProcessResponce( pResponceCommand );
	if (oStepResult.eStatus == ETaskStatus::Succeeded)
	{
		ResetCurrentTask( oStepResult.pNextTaskStep ); // Will reset m_pCurrentTask
		if (m_pCurrentTask)
		{
			if (m_pDevice)
			{
				m_pCurrentATCommand = m_pCurrentTask->PrepareCommand( pResponceCommand );
				if (m_pCurrentATCommand)
					m_pDevice->ExecuteCommand( m_pCurrentATCommand );
				else
				{
					// Fatal Error
					SE_ASSERT( false );
					//ResetCurrentTask();
					Finish( ETaskStatus::Failed, 
							pResponceCommand, 
							tr( "Invalid command" ) );
				}
			}
			else
			{
				// Fatal Error
				SE_ASSERT( false );
				//ResetCurrentTask();
				// Create responce object
				Finish( ETaskStatus::Failed, 
						pResponceCommand, 
						tr( "Unexpected miss of serial device" ) );
			}
		}
		else
		{
			// Task finished
			// Notifie responce object
			//ResetCurrentTask();
			Finish( oStepResult.eStatus,
					pResponceCommand,
					oStepResult.sErrorMesage );
		}
	}
} 

// timerEvent
void CTaskRunner::timerEvent( QTimerEvent* pEvent )
{
	SE_ASSERT( m_nTimmerID == pEvent->timerId() );
	killTimer( pEvent->timerId() );
	m_nTimmerID = -1;
	//Finish( ETaskStatus::Failed, m_pCurrentATCommand, tr( "Timed out" ) );
	if(!m_pCurrentATCommand)
		Finish( ETaskStatus::Failed, m_pCurrentATCommand, tr( "Timed out" ) );
	else
	{
		m_pCurrentATCommand->SetStatus( ECommandStatus::Failed );
		//onResponceReady( m_pCurrentATCommand );
		Finish( ETaskStatus::Failed, m_pCurrentATCommand, tr( "Timed out" ) );
	}
}


// WaitForFinish
void CTaskRunner::WaitForFinish( qint64 nMsec )
{
	if (nMsec > 0)
		m_nTimmerID = startTimer( nMsec );
	while (m_anIsRunning != 0)
	{
		QCoreApplication::processEvents();
	}
}

////////////////////////////////////////////////////////////////////////////////
//
//	class CSmsMessageInitializer
//! Decleares QMetaType for SMS Message class
class CTaskResultInitializer
{
public:
	CTaskResultInitializer() { qRegisterMetaType<STaskResult>( "se::STaskResult" ); }
};
static CTaskResultInitializer oMessagePtrInit;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////
