////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "app_smsmanager.h"
// GUI
#include "gui_conversationexplorer.h"
#include "gui_newmessage_dlg.h"
// SE
#include "se_serialdevice.h"
#include "se_at_smsengine.h"
#include "se_phonenumber.h"
// DB
#include "db_maincomponent.h"
// Qt
#include <QSerialPortInfo>
#include <QMovie>
#include <QLabel>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace app {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSmsManager
//
CSmsManager::CSmsManager( QObject* pParent )
	: Base( pParent ),
	m_pConversationExplorer( nullptr ),
	m_pSerialDevice( nullptr ),
	m_pSmsEngine( nullptr ),
	m_pAnimationLabel( nullptr ),
	m_pDefaultActionGroup( nullptr ),
	m_pNewMessageDialog( nullptr ),
	m_bSendCalledFromDialog( false )
{
	// DB
	m_pDataDB.reset( new db::CMainDataDB() );

	
}

//! Destructor
CSmsManager::~CSmsManager()
{
	if (m_pConversationExplorer && !m_pConversationExplorer->parent())
	{	// Hasn't parent, so delete
		delete m_pConversationExplorer;
		m_pConversationExplorer = nullptr;
	}
}


void CSmsManager::Initialize()
{
	// Conversation Explorer
	m_pConversationExplorer = new gui::CConversationExplorer();
	SE_CONNECT( m_pConversationExplorer, sigSend( QString const&, QString const& ),
				this, onSendMessage( QString const&, QString const& ) );

	m_pConversationExplorer->SetDataDB( m_pDataDB.get() );

	/*SE_CONNECT( m_pConversationExplorer, sigConversationGoingToBeRemoved(int),
				m_pDataDB.get(),	        RemoveConversation(int) );*/

	// Serial Device
	m_pSerialDevice = se::CSerialDeviceCreator::CreateSerialDevice( this );
	SE_ASSERT( m_pSerialDevice );
	// Setup SMS Engine
	m_pSmsEngine = new se::CATSmsEngine( this );
	m_pSmsEngine->SetDevice( m_pSerialDevice );
	// Make Connections
	SE_CONNECT( m_pSmsEngine, sigError( se::CError const& ),
				this, onError( se::CError const& ) );

	SE_CONNECT( m_pSmsEngine, sigRssiChanged( int ),
				this, onRssiChanged( int ) );

	SE_CONNECT( m_pSmsEngine, sigNewMessage( se::t_SmsMessageSharedPtr ),
				this, onNewMessage( se::t_SmsMessageSharedPtr ) );

	SE_CONNECT( m_pSmsEngine, sigMessageWasSent( se::t_SmsMessageSharedPtr ),
				this, onMessageWasSent( se::t_SmsMessageSharedPtr ) );

	// Setup Actions
	m_pDefaultActionGroup = new QActionGroup( this );
	QIcon oNewMessageIcon( ":/columba/Resources/send_message2_.png" );
	QAction* pNewMessageAction = new QAction( oNewMessageIcon, tr( "New Message" ), this );
	pNewMessageAction->setActionGroup( m_pDefaultActionGroup );
	SE_CONNECT( pNewMessageAction, triggered(), this, onOpenNewMessageDialog() );

	// New Message Dialog
	m_pNewMessageDialog = new gui::CNewMessageDialog( m_pConversationExplorer );
	m_pNewMessageDialog->setWindowIcon( oNewMessageIcon );
	// Connections :TODO
	SE_CONNECT( m_pNewMessageDialog, sigSend(QString const&, QString const&),
				this, onSendMessage(QString const&, QString const&) );
}

// SaveSettings
void CSmsManager::SaveSettings( QSettings* pSettings )
{
	// TODO

}

// LoadSettings
void CSmsManager::LoadSettings( QSettings const* pSettings )
{
	// TODO
}


QString CSmsManager::GetName() const
{
	return tr( "SMS Manager" );
}

QString CSmsManager::Topic() const
{
	return tr( "Manages reading and writeing SMS messages" );
}

QWidget* CSmsManager::GetWidget()
{
	return m_pConversationExplorer;
}

QIcon CSmsManager::GetIcon() const
{
	return QIcon();
}

QList<QActionGroup*>  CSmsManager::GetActionList()
{
	QList<QActionGroup*> lstResult;
	lstResult.append( m_pDefaultActionGroup );
	return lstResult;
}
 

t_IDBComponentSPtr CSmsManager::GetDBComponent()
{
	return m_pDataDB;
}





//
//	Own Interface
//
void CSmsManager::ConnectToPort( QSerialPortInfo const& oPortInfo )
{
	SE_ASSERT( m_pSerialDevice );
	SE_ASSERT( m_pSmsEngine );
	m_pSerialDevice->SetPort( oPortInfo );
	m_pSmsEngine->Configure();
}

// Disconnect
void CSmsManager::Disconnect()
{
	SE_ASSERT( m_pSerialDevice );
	m_pSerialDevice->ClosePort();
}

// 
bool CSmsManager::IsSmsEngineReady() const
{
	SE_ASSERT( m_pSmsEngine );
	return m_pSmsEngine->IsReady();
}

bool CSmsManager::TestSerialPort( QSerialPortInfo const& oPortInfo,
								  QString* pMessage )
{
	// Serial Device
	se::CSerialDevice* pSerialDevice = se::CSerialDeviceCreator::CreateSerialDevice( this );
	SE_ASSERT( pSerialDevice );
	if (!pSerialDevice->TestPort( oPortInfo, pMessage ))
		return false;
	// Setup SMS Engine
	pSerialDevice->SetPort( oPortInfo );
	se::CATSmsEngine* pSmsEngine = new se::CATSmsEngine( this );
	pSmsEngine->SetDevice( pSerialDevice );
	se::ESmsMode eMode = pSmsEngine->CheckCurrentMode( 2000 );
	
	delete pSerialDevice;
	delete pSmsEngine;
	if (eMode == se::ESmsMode::eUnknown)
		return false;
	else
		return true;
}

QSerialPortInfo CSmsManager::GetCurrentPortInfo() const
{
	SE_ASSERT( m_pSerialDevice );
	QSerialPortInfo oPortInfo( m_pSerialDevice->Port() );
	return oPortInfo;
}

//
//	SLOTS
//
// onNewMessage
void CSmsManager::onNewMessage( se::t_SmsMessageSharedPtr pMessage )
{
	if (!pMessage)
		return;
	QString sPhoneNumber = pMessage->GetPhoneNumber();
	QString sMessage = pMessage->GetMessage();
	QDateTime oDateTime = pMessage->GetDateTime();
	SE_DEBUG( QString sDateTime = oDateTime.toString() );
	m_pDataDB->StoreMessage( sPhoneNumber, true, sMessage, oDateTime );
	// Move Scrall Bar to Bottom
	m_pConversationExplorer->MoveChatScrollBarToBottom();
	emit sigNewMessage( pMessage );
}

// onRssiChanged
void CSmsManager::onRssiChanged( int nNewValue )
{
	// TODO
}

// onError
void CSmsManager::onError( se::CError const& oErr )
{
	se::CError::EType eErrorType = oErr.GetErrorType();

	if (eErrorType == se::CError::eSerialPortError)
	{
		emit sigDisconnected();
		// TODO: Disconnect sms engine properly
	}
	else if (oErr.GetErrorType() == se::CError::eSmsSendFailed && m_pAnimationLabel)
	{
		// BAD (
		QStatusBar* pStatusBar = nullptr;
		emit sigStatusBarRequest( pStatusBar );
		if (pStatusBar && m_pAnimationLabel)
		{
			pStatusBar->removeWidget( m_pAnimationLabel );
			m_pAnimationLabel = nullptr;
		}
		
		if (m_bSendCalledFromDialog)
			m_pNewMessageDialog->onUnfreeze();
		else
			m_pConversationExplorer->onUnfreeze();

	}
	QString sErr = oErr.GetErrorMessage();
	emit sigNotify( "Error" /*TODO*/, sErr, ENotificationType::Error );
}

// onSendMessage
void CSmsManager::onSendMessage( QString const& sSenderInfo,
								 QString const& sMessage )
{
	SE_ASSERT( m_pSmsEngine );
	if (!IsSmsEngineReady())
	{
		sigNotify( tr( "Failed to send" ), tr( "Not connected" ), ENotificationType::Error );
		return;
	}

	if (sSenderInfo.isEmpty())
	{
		sigNotify( tr( "Empty phone number" ), tr( "Phone number is not specified" ), ENotificationType::Error );
		return;
	}

	if (sMessage.isEmpty())
	{
		// TODO
		sigNotify( tr( "Empty message" ), tr( "Empty SMS message" ), ENotificationType::Warning );
		return;
	}

	if (m_pSmsEngine->IsInSendingState())
	{
		// TODO
		sigNotify( tr( "Another message is in sending" ), tr( "Please wait until the last message will be sent" ), ENotificationType::Warning );
		return;
	}

	// Validate phone number
	se::CPhoneNumber oPhoneNumber( sSenderInfo );
	if (!oPhoneNumber.IsValid())
	{
		sigNotify( tr( "Invalid phone number" ), tr( "Specified phone number is not valid.\nPlease use international format (+...)" ), ENotificationType::Error );
		return;
	}

	// Update sending origine
	gui::CNewMessageDialog* pNewMessageDlg = dynamic_cast<gui::CNewMessageDialog*>(sender());
	if (pNewMessageDlg)
	{
		pNewMessageDlg->onFreeze();
		m_bSendCalledFromDialog = true;
	}
	else
	{
		m_pConversationExplorer->onFreeze();
		m_bSendCalledFromDialog = false;
	}

	QString sPhoneNumber = oPhoneNumber.GetPhoneNumber();
	SE_ASSERT( !sPhoneNumber.isEmpty() );

	// Get status bar Set Animation
	QStatusBar* pStatusBar = nullptr;
	emit sigStatusBarRequest( pStatusBar );
	if (pStatusBar)
	{
		m_pAnimationLabel = new QLabel();
		m_pAnimationLabel->setText( tr( "Sending..." ) );
		QMovie* pMovie = new QMovie( ":/columba/Resources/gif/sending.gif" );
		pMovie->setSpeed( 200 );
		m_pAnimationLabel->setMovie( pMovie );
		pStatusBar->addWidget( m_pAnimationLabel );
		pMovie->start();
	}

	QDateTime oDateTime = QDateTime::currentDateTime();
	m_pLastSentSms.reset( new se::CSmsMessage( sPhoneNumber, sMessage, oDateTime ) );
	m_pSmsEngine->SendMessage( *m_pLastSentSms/* Use checked SMSC number ||| Viva Cell: "+37493297333"*/ );
}


// onMessageWasSent
void CSmsManager::onMessageWasSent( se::t_SmsMessageSharedPtr pMessage )
{
	QStatusBar* pStatusBar = nullptr;
	emit sigStatusBarRequest( pStatusBar );
	if (pStatusBar && m_pAnimationLabel)
	{
		pStatusBar->removeWidget( m_pAnimationLabel );
		m_pAnimationLabel = nullptr;
	}

	// Add to database
	SE_ASSERT( m_pLastSentSms );
	if (m_pLastSentSms)
	{
		QString sPhoneNumber = m_pLastSentSms->GetPhoneNumber();
		QString sMessage = m_pLastSentSms->GetMessage();
		QDateTime oDateTime = m_pLastSentSms->GetDateTime();
		m_pDataDB->StoreMessage( sPhoneNumber, false, sMessage, oDateTime );

		m_pLastSentSms.reset();
	}

	if (m_bSendCalledFromDialog)
	{
		m_pNewMessageDialog->onUnfreeze();
		if (m_pNewMessageDialog)
			m_pNewMessageDialog->accept();
	}
	else
	{
		m_pConversationExplorer->onUnfreeze();
		m_pConversationExplorer->ClearCurrentMessageText();
		// Move Scrall Bar to Bottom
		m_pConversationExplorer->MoveChatScrollBarToBottom();
	}
}

// onOpenNewMessageDialog
void CSmsManager::onOpenNewMessageDialog()
{
	SE_ASSERT( m_pNewMessageDialog );
	m_pNewMessageDialog->Clear();
	m_pNewMessageDialog->show();
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
} // namespace app
////////////////////////////////////////////////////////////////////////////////
