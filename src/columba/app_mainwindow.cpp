////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "app_mainwindow.h"
#include "se_core.h"
#include "gui_settings_dlg.h"
// Qt
#include <QMessageBox>
#include <QStatusBar>
#include <QSound>
#include <QStandardPaths>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace app {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CColumbaMainWindow
//
CColumbaMainWindow::CColumbaMainWindow( QWidget *pParent )
	: QMainWindow( pParent ),
	m_pSmsManager( nullptr ),
	m_pSettingsDialog( nullptr ),
	m_pSettings( nullptr )
{
	ui.setupUi( this );
	// Add event filter to ignore context menu events on Tool Bar
	ui.mainToolBar->installEventFilter( this );
	QString sDBPath = GetDBPath();
	m_pDBManager.reset(new db::CConnectionManager(sDBPath, "data.cdb"));

	// Init Settings
	m_pSettings = new QSettings( this );

	//
	//	Add SMS Manager
	//
	m_pSmsManager = new CSmsManager( this );
	SE_CONNECT( m_pSmsManager, sigStatusBarRequest(QStatusBar*&),
				this,           onStatusBarRequest(QStatusBar*&) );
	
	SE_CONNECT( m_pSmsManager, sigNewMessage(se::t_SmsMessageSharedPtr),
				this,           onNewMessage(se::t_SmsMessageSharedPtr) );

	SE_CONNECT( m_pSmsManager, sigDisconnected(),
				this, onDisconnected() );

	AddModule( m_pSmsManager );
	
	
	//
	//	Add Phone Book
	//
	m_pPhoneBook = new CPhoneBook( this );
	// Make Connections: TODO
	AddModule( m_pPhoneBook );
	
	QWidget* pPhoneBookWidget = m_pPhoneBook->GetWidget();
	if (pPhoneBookWidget)
		pPhoneBookWidget->show();
	
	// Set Sms Manager Widget 
	QWidget* pSmsManagerWidget = m_pSmsManager->GetWidget();
	setCentralWidget( pSmsManagerWidget );

	// Status Widget
	m_pStatusWidget = new gui::CStatusView();
	statusBar()->addPermanentWidget( m_pStatusWidget );
	SetConnectionStatus( gui::EConnectionStatus::FailedToConnect );
	// Update Serial Ports
	UpdateSerialPortInfo();
	// Setup Settings Dialog Action
	QIcon oSettingsIcon( ":/columba/Resources/settings2.png" );
	QAction* pSettingsAction = new QAction( oSettingsIcon, tr( "Settings" ), this );
	// Make Connectios
	SE_CONNECT( pSettingsAction, triggered(), this, onOpenSettingsDialog() );
	// Add Action to  ToolBar
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addAction( pSettingsAction );
	// Settings Dialog
	m_pSettingsDialog = new gui::CSettingsDialog( this );
	QStringList lstPorts = GetSerialPortNames();
	m_pSettingsDialog->SetPortItems( lstPorts );
	// Make Dialog Connections
	SE_CONNECT( m_pSettingsDialog, sigTryConnect(int, bool&, QString*),
				this,               onTryConnect(int, bool&, QString*));
	SE_CONNECT( m_pSettingsDialog, sigTryDisconnect(int, bool&, QString*),
				this,               onTryDisconnect(int, bool&, QString*) );
	SE_CONNECT( m_pSettingsDialog, sigUpdate(), this, onUpdateSettings() );
	SE_CONNECT( m_pSettingsDialog, accepted(), this, onSettingsAccepted() );
	SE_CONNECT( m_pSettingsDialog, rejected(), this, onCloseSettingsDialog() );

	// Load Columba Settings
	LoadSettings();
	if (!m_pSmsManager->IsSmsEngineReady())
		onOpenSettingsDialog();
}
		
CColumbaMainWindow::~CColumbaMainWindow()
{
	SaveSettings();
	for (int i = 0; i < m_lstModules.size(); ++i)
	{
		CloseModule( m_lstModules[i] );
	}
}

void CColumbaMainWindow::CloseModule( IModule* pModule )
{
	if (!pModule)
		return;
	QString sModulePrefix = pModule->GetName();
	sModulePrefix.simplified();
	sModulePrefix.replace( " ", "" );
	SE_ASSERT( m_pSettings );
	m_pSettings->beginGroup( sModulePrefix );
	pModule->SaveSettings( m_pSettings );
	m_pSettings->endGroup();
}


void CColumbaMainWindow::AddModule( IModule* pModule )
{
	SE_ASSERT( pModule );
	if (!pModule)
		return;
	// Get And Add DB Component
	t_IDBComponentSPtr pDBComponent = pModule->GetDBComponent();
	if (pDBComponent)
		m_pDBManager->AddDBComponent( pDBComponent, true );
	pModule->Initialize();

	SE_CONNECT( pModule, sigNotify(QString const&, QString const&, ENotificationType),
				this,     onNotify(QString const&, QString const&, ENotificationType) );
	SE_CONNECT( pModule, sigReportStatus(QString const&),
				this,     onReportStatus(QString const&) );

	// Load Settings
	QString sModulePrefix = pModule->GetName();
	sModulePrefix.simplified();
	sModulePrefix.replace( " ", "" );
	SE_ASSERT( m_pSettings );
	m_pSettings->beginGroup( sModulePrefix );
	pModule->LoadSettings( m_pSettings );
	m_pSettings->endGroup();

	// Get Actions
	QList<QActionGroup*> lstActionGroupss = pModule->GetActionList();
	for (int i = 0; i < lstActionGroupss.size(); ++i)
	{
		QActionGroup* pActionGroup = lstActionGroupss[i];
		if (!pActionGroup)
			continue;
		QList<QAction*> lstActions = pActionGroup->actions();
		ui.mainToolBar->addActions( lstActions );
	}
}

void CColumbaMainWindow::LoadSettings()
{
	SE_ASSERT( m_pSettings );
	m_pSettings->beginGroup( "columba" );
	// Load last use serial port
	QString sPortName = m_pSettings->value( "last_serial_port" ).toString();
	int nIdx = -1;
	QSerialPortInfo oPortInfo = SerialPortInfoByName( sPortName, &nIdx );
	bool bConnected = TryConnectToPort( oPortInfo );
	SE_ASSERT( m_pSettingsDialog );
	gui::EConnectionStatus eStatus = bConnected ? 
		gui::EConnectionStatus::Connected : 
		gui::EConnectionStatus::FailedToConnect;
	m_pSettingsDialog->SetConnectionStatusAt( nIdx, eStatus );

	m_pSettings->endGroup();
}

void CColumbaMainWindow::SaveSettings()
{
	SE_ASSERT( m_pSettings );
	m_pSettings->beginGroup( "columba" );
	// Save using serial port
	QString sPortName = MakeSerialPortName( m_pSmsManager->GetCurrentPortInfo() );
	m_pSettings->setValue( "last_serial_port", sPortName );

	m_pSettings->endGroup();
}

// SerialPortInfoByName
QSerialPortInfo CColumbaMainWindow::SerialPortInfoByName( QString const& sPortName, int* pIdx )
{
	if (pIdx)
		*pIdx = -1;
	if (sPortName.isEmpty())
		return QSerialPortInfo();

	QSerialPortInfo oResult;
	for (int i = 0; i < m_lstPorts.size(); ++i)
	{
		QSerialPortInfo const& oPortInfo = m_lstPorts[i];
		QString sName = MakeSerialPortName( oPortInfo );
		if (sName == sPortName)
		{
			oResult = oPortInfo;
			if (pIdx)
				*pIdx = i;
			break;
		}
	}
	return oResult;
}
//
//	SLOTS
//
// onNotify
void CColumbaMainWindow::onNotify( QString const& sTitle,
								   QString const& sText_,
								   ENotificationType eType )
{
	if (sTitle.isEmpty())
		return;
	QString sText = sText_;
	if (sText.isEmpty())
		sText = sTitle;

	switch (eType)
	{
		case ENotificationType::Error:
			QMessageBox::critical( this, sTitle, sText );
			break;
		case ENotificationType::Warning:
			QMessageBox::warning( this, sTitle, sText );
			break;
		case ENotificationType::Information:
			QMessageBox::information( this, sTitle, sText );
			break;
		default:
			break;
	}
}

// onReportStatus
void CColumbaMainWindow::onReportStatus( QString const& sStatus )
{
	if (sStatus.isNull())
		statusBar()->clearMessage();
	else
		statusBar()->showMessage( sStatus, 0 );
}

// onStatusBarRequest
void CColumbaMainWindow::onStatusBarRequest( QStatusBar*& pBar )
{
	pBar = statusBar();
}

// UpdateSerialPortInfo
void CColumbaMainWindow::UpdateSerialPortInfo()
{
	m_lstPorts = QSerialPortInfo::availablePorts();

	// Update Settnings Dialog
	if (m_pSettingsDialog)
	{
		QStringList lstPortNames = GetSerialPortNames();
		m_pSettingsDialog->SetPortItems( lstPortNames );
	}
}

// MakeSerialPortName
QString CColumbaMainWindow::MakeSerialPortName( QSerialPortInfo const& oPortInfo ) const
{
	if (oPortInfo.isNull())
		return QString();
	QString sName = QString( "%1 (%2)" ).arg( oPortInfo.description(), oPortInfo.portName() );
	return sName;
}


// GetSerialPortNames
QStringList CColumbaMainWindow::GetSerialPortNames() const
{
	QStringList lstPorts;
	for (int i = 0; i < m_lstPorts.size(); ++i)
	{
		QSerialPortInfo const& oPortInfo = m_lstPorts[i];
		QString sName = MakeSerialPortName( oPortInfo );
		lstPorts.append( sName );
	}
	return lstPorts;
}

//
//	Settings Dialog SLOTS
//
// onOpenSettingsDialog
void CColumbaMainWindow::onOpenSettingsDialog()
{
	SE_ASSERT( m_pSettingsDialog );
	//m_pSettingsDialog->Reset();
	/*QStringList lstPortNames = GetSerialPortNames();
	m_pSettingsDialog->SetPortItems( lstPortNames );*/
	m_pSettingsDialog->show();
}

// onCloseSettingsDialog
void CColumbaMainWindow::onCloseSettingsDialog()
{
	 // Nothig to do
	//m_pSettingsDialog->hide();
}

// onSettingsAccepted
void CColumbaMainWindow::onSettingsAccepted()
{
	SE_ASSERT( m_pSettingsDialog );
	int nPortIndex = m_pSettingsDialog->GetCurrentPortIndex();
	if (nPortIndex < 0 || nPortIndex >= m_lstPorts.size())
	{
		SetConnectionStatus( gui::EConnectionStatus::FailedToConnect );
		return;
	}

	//gui::EConnectionStatus eTestStatus = m_pSettingsDialog->GetConnectionStatusAt( nPortIndex );
	//if (eTestStatus == gui::EConnectionStatus::FailedToConnect)
	//{
	//	SetConnectionStatus( gui::EConnectionStatus::FailedToConnect );
	//}
	//else if (eTestStatus == gui::EConnectionStatus::Connected)
	//{
	//	SE_ASSERT( m_pSmsManager );
	//	QSerialPortInfo oPortInfo = m_lstPorts[nPortIndex];
	//	m_pSmsManager->SetSerialPort( oPortInfo );
	//	SetConnectionStatus( gui::EConnectionStatus::Connected );

	//}
	//else // Undefined
	//{
	//	// Try Connect
	//	SetConnectionStatus( gui::EConnectionStatus::Connecting );
	//	bool bResult = false;
	//	QString sMsg;
	//	onTryConnect( nPortIndex, bResult, &sMsg );
	//	gui::EConnectionStatus eStatus = bResult ? gui::EConnectionStatus::Connected : gui::EConnectionStatus::FailedToConnect;
	//	SetConnectionStatus( eStatus );
	//	if (eStatus == gui::EConnectionStatus::Connected)
	//	{
	//		SE_ASSERT( m_pSmsManager );
	//		QSerialPortInfo oPortInfo = m_lstPorts[nPortIndex];
	//		m_pSmsManager->SetSerialPort( oPortInfo );
	//	}	
	//	else
	//	{
	//		statusBar()->showMessage( tr( "Failed to connect" ), 3000 );
	//	}
	//}
	
}


// TryConnectToPort
bool CColumbaMainWindow::TryConnectToPort( QSerialPortInfo oPortInfo, QString* pMessage )
{
	SE_ASSERT( m_pSmsManager );
	QString sCurrentPortName = MakeSerialPortName( m_pSmsManager->GetCurrentPortInfo() );
	QString sPortName = MakeSerialPortName( oPortInfo );
	if (sCurrentPortName == sPortName && m_pSmsManager->IsSmsEngineReady())
	{
		if (pMessage)
			*pMessage = tr( "Already connected" );
		return true;
	}

	SetConnectionStatus( gui::EConnectionStatus::Connecting );
	bool bResult = m_pSmsManager->TestSerialPort( oPortInfo, pMessage );
	if (bResult)
	{
		m_pSmsManager->ConnectToPort( oPortInfo );
		SetConnectionStatus( gui::EConnectionStatus::Connected );
	}
	else
	{
		//m_pSmsManager->SetSerialPort( oPortInfo ); // TODO
		if (!m_pSmsManager->IsSmsEngineReady())
			SetConnectionStatus( gui::EConnectionStatus::FailedToConnect );
		else
			SetConnectionStatus( gui::EConnectionStatus::Connected );
	}
	return bResult;
}

// Disconnect
void CColumbaMainWindow::Disconnect()
{
	SE_ASSERT( m_pSmsManager );
	SetConnectionStatus( gui::EConnectionStatus::Disconnecting );
	m_pSmsManager->Disconnect();
	SetConnectionStatus( gui::EConnectionStatus::Disconnected );
}

// onTestPortItem
void CColumbaMainWindow::onTryConnect( int nIdx, bool& bResult, QString* pMessage )
{
	SE_ASSERT( nIdx >= 0 && nIdx < m_lstPorts.size() );
	if (nIdx < 0 || nIdx >= m_lstPorts.size())
	{
		bResult = false;
		return;
	}
	QSerialPortInfo const& oPortInfo = m_lstPorts[nIdx];
	bResult = TryConnectToPort( oPortInfo, pMessage );
}

// onTryDisconnect
void CColumbaMainWindow::onTryDisconnect( int nIdx, bool& bResult, QString* pMessage )
{
	SE_ASSERT( nIdx >= 0 && nIdx < m_lstPorts.size() );
	if (nIdx < 0 || nIdx >= m_lstPorts.size())
	{
		bResult = false;
		return;
	}
	Disconnect();
	bResult = true;
}

// onUpdateSettings
void CColumbaMainWindow::onUpdateSettings()
{
	UpdateSerialPortInfo();
	//QStringList lstPortNames = GetSerialPortNames();
	//m_pSettingsDialog->SetPortItems( lstPortNames );
}

void CColumbaMainWindow::SetConnectionStatus( gui::EConnectionStatus eStatus )
{
	SE_ASSERT( m_pStatusWidget );
	switch (eStatus)
	{
		case gui::EConnectionStatus::FailedToConnect:
		case gui::EConnectionStatus::Disconnected:
			m_pStatusWidget->SetFailed( tr( "Not Connected" ) );
			break;
		case gui::EConnectionStatus::Connected:
			m_pStatusWidget->SetReady( tr( "Connected" ) );
			break;
		case gui::EConnectionStatus::Connecting:
			m_pStatusWidget->SetWaiting( tr( "Connecting..." ) );
			break;
		case gui::EConnectionStatus::Disconnecting:
			m_pStatusWidget->SetWaiting( tr( "Disconnecting..." ) );
			break;
		default:
			break;
	}
}

// onNewMessage
void CColumbaMainWindow::onNewMessage( se::t_SmsMessageSharedPtr pMessage )
{
	qApp->alert( this );
	QSound::play( ":/columba/Resources/ring.wav" );
}

// onDisconnected
void CColumbaMainWindow::onDisconnected()
{
	SetConnectionStatus( gui::EConnectionStatus::FailedToConnect );
}


// eventFilter
bool CColumbaMainWindow::eventFilter( QObject* pWatched, QEvent* pEvent )
{
	if (pWatched == ui.mainToolBar)
	{
		if (pEvent->type() == QEvent::ContextMenu)
			// Skip Context Menu Events
			return true;
	}
	return false;
}

QString CColumbaMainWindow::GetDBPath()
{
	// Cunfigure DB Manager
	QString sAppDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QDir oDir(sAppDataPath);
	if (!oDir.exists())
	{
		oDir.mkpath(sAppDataPath);
		oDir = QDir(sAppDataPath);
	}
	SE_ASSERT(oDir.exists());
	if (!oDir.exists())
	{
		QMessageBox::critical(this, tr("Unexpected Error"), tr("Failed to locate application data."));
		qApp->quit();
		return QString();
	}

	if (!oDir.cd("data"))
	{
		oDir.mkdir("data");
		oDir.cd("data");
	}

	QString sDBPath = oDir.absolutePath();
	SE_ASSERT(!sDBPath.isEmpty());
	if (sDBPath.right(1) != QDir::separator())
		sDBPath.append(QDir::separator());
	return sDBPath;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace app
////////////////////////////////////////////////////////////////////////////////
