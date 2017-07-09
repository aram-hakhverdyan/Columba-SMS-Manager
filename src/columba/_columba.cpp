#include "_columba.h"
#include "se_core.h"
#include "se_ateventhandler.h"
#include "../smsengin/gsm_translator.h"


#include <QSerialPortInfo>
#include <QMessageBox>
#include <QVBoxLayout>

#include "gui_bubble.h"
#include "db_maincomponent.h"

#include "gui_sendersview.h"
#include "gui_sendersmodel.h"

#include "gui_conversationexplorer.h"

#include <QMovie>

columba::columba(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//QRegExp oRegExpr("^([0-9\(\)\/\+ \-]*)$");
	QRegExp oRegExpr("^\\(*\\+*[1-9]{0,3}\\)*-*[1-9]{0,3}[-. /]*\\(*[2-9]\\d{2}\\)*[-. /]*\\d{3}[-. /]*\\d{4} *e*x*t*\\.* *\\d{0,4}$");
	//QRegExp oRegExpr( "^(?:(?:\\+?1\\s*(?:[.-]\\s*)?)?(?:\\(\\s*([2-9]1[02-9]|[2-9][02-8]1|[2-9][02-8][02-9])\\s*\\)|([2-9]1[02-9]|[2-9][02-8]1|[2-9][02-8][02-9]))\\s*(?:[.-]\\s*)?)?([2-9]1[02-9]|[2-9][02-9]1|[2-9][02-9]{2})\\s*(?:[.-]\\s*)?([0-9]{4})(?:\\s*(?:#|x\\.?|ext\\.?|extension)\\s*(\\d+))?$" );


	QRegExpValidator* pValidator = new QRegExpValidator( this );
	pValidator->setRegExp( oRegExpr );
	ui.lineEdit->setValidator( pValidator );
	//oRegExpr.
	//try
	//{
	//	m_pDBConnectionManager = new db::CConnectionManager();
	//	m_pComponent = new db::CMainDataDB();
	//	m_pDBConnectionManager->AddDBComponent( t_IDBComponentSPtr( m_pComponent ) );
	//	
	//	gui::CConversationExplorer* pExplorer = new gui::CConversationExplorer();
	//	//pExplorer->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
	//	ui.horizontalLayout_2->addWidget( pExplorer );

	//	QLabel* pLabel = new QLabel();
	//	QMovie *movie = new QMovie( ":/columba/Resources/gif/sending.gif" );
	//	movie->setSpeed( 200 );
	//	pLabel->setMovie( movie );
	//	movie->start();
	//	ui.horizontalLayout_2->addWidget( pLabel );
	//	//pLabel->setMaximumSize( 120, 120 );

	//	// Connect
	//	SE_CONNECT( m_pComponent, sigMessageAdded( int, qint64 ),
	//				pExplorer,     onUpdateMessage( int, qint64 ) );

	//}
	//catch (se::CError const& oErr)
	//{
	//	QMessageBox::critical( this, "Data Base Connection Error", oErr.GetErrorMessage() );
	//}

	//gui::CBasicBubble* pBubble = new gui::CBasicBubble(
	//	QString( "Barev Aram jan..cnundd shnorhavor...arajin" 
	//	"hertin aroxjutyun em cankanum...qez shaaat hajoxutyunner"
	//	" u bazum erjanik paher...maxtum em urax tramadrutyun ev "
	//	" hacheli or))) vonc es qez zgum? Amen inch normala?((((" ) );
	//QVBoxLayout* pLayout = new QVBoxLayout();
	//
	//ui.scrollArea->setLayout( pLayout );

	//QHBoxLayout* pHLay = new QHBoxLayout();
	//QSpacerItem* pSpacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	//pHLay->addItem( pSpacer );
	//pHLay->addWidget( pBubble );
	//pBubble->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ) );
	//pLayout->addLayout( pHLay );
	/*pBubble->move( ui.scrollArea->geometry().topLeft() + QPoint( 10, 10 ) );*/

	LOG_HEADER( "Columba Started" );

	m_pDevice = new se::CSerialDevice(this);
	m_pSmsEngine = new se::CATSmsEngine( this );
	m_pSmsEngine->SetDevice( m_pDevice );

	QList<QSerialPortInfo> lstPorts = QSerialPortInfo::availablePorts();
	for each (auto oPort in lstPorts)
	{
#ifndef QT_DEBUG
		QMessageBox::information(this, "Serial Port Info", QString(
			"Description  : %1\n"
			"Serial Number: %2\n"
			"Port Name    : %3\n"
			"Product ID   : %4\n"
			"Vendor  ID   : %5\n"
			"Manufacturer : %6\n"
			"System Location: %7\n"
			"Is Valid: %8\n"
			"Is Busy : %9\n"
			).arg(
			oPort.description(),
			oPort.serialNumber(),
			oPort.portName(),
			QString::number(oPort.productIdentifier()),
			QString::number(oPort.vendorIdentifier()),
			oPort.manufacturer(),
			oPort.systemLocation(),
			QString::number(oPort.isValid()),
			QString::number(oPort.isBusy())
			));
#endif
		/*if (oPort.isValid())
		{*/
		ui.cbxPorts->addItem(QString("%1 (%2)").arg(oPort.description(), oPort.portName()));
		m_lstPorts.append(oPort);
		//}
		if (oPort.hasProductIdentifier() && oPort.productIdentifier() == 5186)
			ui.cbxPorts->setCurrentIndex(ui.cbxPorts->count() - 1);
	}

	//
	//	Configure AT Parser
	//
	m_pEventParser = new se::CATEventParser();
	m_pCommandParser = new se::CATCommandResponceParser();
	// Event Handlers
	se::CRssiEventHandler* pRssiHandler = new se::CRssiEventHandler();
	se::CATNewMessageHandler* pNewMessageHandler = new se::CATNewMessageHandler();
	//se::CDefaultOkHandler* pOkHandler = new se::CDefaultOkHandler();

	m_pEventParser->AddHandler( pRssiHandler );
	m_pEventParser ->AddHandler( pNewMessageHandler );
	//m_pEventParser ->AddHandler( pOkHandler );

	//
	//
	//
	//
	//
	//
	m_pDevice->SetEventParser( se::t_IATParserSPtr( m_pEventParser ) );
	m_pDevice->SetCommandParser( se::t_CommandResponceParserSPtr( m_pCommandParser ) );


	//
	//	Connections
	//
	//Q_ASSERT(QObject::connect(&m_oSerialPort, SIGNAL(readyRead()), this, SLOT(onReadyRead())));
	Q_ASSERT(QObject::connect(ui.btnConnect, SIGNAL(clicked()), this, SLOT(onConnect())));
	Q_ASSERT( QObject::connect( ui.btnExec, SIGNAL( clicked() ), this, SLOT( onExec() ) ) );
	//Q_ASSERT(QObject::connect(ui.btnSend, SIGNAL(clicked()), this, SLOT(onSend())));
	//Q_ASSERT( QObject::connect( &m_oSerialPort, SIGNAL( connected() ), this, SLOT( onConnected() ) ) );
	//Q_ASSERT( QObject::connect( &m_oSerialPort, SIGNAL( bytesWritten(qint64) ), this, SLOT( onBytesWritten(qint64) ) ) );
	//Q_ASSERT(QObject::connect(ui.btnSendMsg, SIGNAL(clicked()), this, SLOT(onTerminateMessage())));
	///////////*Q_ASSERT(QObject::connect(m_pDevice, SIGNAL(sigError(se::CError const&)),
	//////////	this,							    SLOT(onError(se::CError const&))));*/

	/*Q_ASSERT(QObject::connect(m_pDevice, SIGNAL(sigResponseReady(se::t_ATCommandPtr)),
		this, SLOT(onResponseReady(se::t_ATCommandPtr))));
	Q_ASSERT( QObject::connect( m_pDevice, SIGNAL( sigATEvent( QByteArray const& ) ),
		this, SLOT(								    onATEvent( QByteArray const& ) ) ) );

	Q_ASSERT( QObject::connect( m_pDevice, SIGNAL( sigATEvent(se::t_ATResponceSharedPtr) ),
		this, SLOT(                                 onATEvent(se::t_ATResponceSharedPtr) ) ) );*/
	

	SE_CONNECT( m_pSmsEngine, sigError(se::CError const&), 
				         this, onError(se::CError const&) );

	/*SE_CONNECT( m_pSmsEngine, sigRssiChanged(int),
				this,                 onRSSI(int) );*/
	
	SE_CONNECT( m_pSmsEngine, sigNewMessage(se::t_SmsMessageSharedPtr),
				this,          onNewMessage(se::t_SmsMessageSharedPtr) );
	
	SE_CONNECT( m_pSmsEngine, sigMessageWasSent(se::t_SmsMessageSharedPtr),
				this, onSmsWasSent(se::t_SmsMessageSharedPtr) );
}


columba::~columba()
{

}


void columba::onRSSI( int nValue )
{
	ui.textEdit->append( QString( "Yo RSSI: %1" ).arg(nValue) );
}

void columba::onConnect()
{ 
	int nIndex = ui.cbxPorts->currentIndex();
	QSerialPortInfo oInfo = m_lstPorts[nIndex];
	m_pDevice->SetPort(oInfo);
	/*QString sCmd = "AT^CURC=0";
	m_pSmsEngine->ExecATCommand( sCmd );*/
}

void columba::onError(se::CError const& oErr)
{
	QString sMsg = oErr.GetErrorMessage();
	//QMessageBox::critical(this, "Error", sMsg);
	ui.textEdit->append( "Error: " + sMsg );
}


void columba::onResponseReady(se::t_ATCommandPtr pAT)
{
	QString sName = pAT->GetCommandName();
	se::ECommandStatus eStatus = pAT->GetStatus();
	QString sStatus = eStatus == se::ECommandStatus::Succeeded ? "Succeeded" : "Failed";
	ui.textEdit->append( "AT Responce Ready: " + sName + "\nStatus: " + sStatus );
	
	int l = 9;
}

void columba::onATEvent( QByteArray const& oData )
{
	QString sMsg = oData;
	//ui.textEdit->append( "AT Event: " + sMsg );
	//QMessageBox::information( this, "At Event", sMsg );
}

void columba::onExec()
{
	/*QString sMode = ui.cbxSmsMode->currentText();
	se::ESmsMode eMode = sMode == "PDU" ? se::ESmsMode::ePDUMode : se::ESmsMode::eTextMode;
	m_pSmsEngine->SetMode( eMode );*/


	/*se::ESmsMode eMode = m_pSmsEngine->CheckCurrentMode();
	ui.textEdit->append( "Current mode is: " + QString::number( int( eMode ) ) );*/
	
	
	/*QString sCmd = ui.textEdit->toPlainText();
	if (sCmd.isEmpty())
		return;*/
	
	//m_pSmsEngine->ExecATCommand( QString() );
	
	
	
	
	
	se::ESmsMode eMode = m_pSmsEngine->CheckCurrentMode();
	ui.textEdit->append( QString::number( int( eMode ) ) );
	
	/*QString sMsg = ui.textEdit->toPlainText();
	QString sPhone = ui.lineEdit->text();

	se::CSmsMessage oMsg( sPhone, sMsg );
	m_pSmsEngine->SendMessage( oMsg, "+37493297333" );*/
	
	//////////////QString sSender = ui.lineEdit->text();
	//////////////QString sMsg = ui.textEdit->toPlainText();
	//////////////QDateTime oDateTime = QDateTime::currentDateTime();

	//////////////m_pComponent->StoreMessage( sSender, false, sMsg, oDateTime );

	/*se::t_lstUserData lst = se::CGsmDefaultAlphabet::MakeUserData( sMsg );
	int k = 9;
*/
	//QString sCmd = ui.lineEdit->text();
	///*if (sCmd.isEmpty())
	//	return;*/
	//se::t_ATCommandPtr pAT( new se::CATCommand( sCmd ) );
	//ui.textEdit->append( "Exec: " + pAT->GetData() );
	//m_pDevice->ExecuteCommand( pAT );
}

// onATEvent
void columba::onATEvent( se::t_ATResponceSharedPtr pEvent )
{
	if (!pEvent)
		return;
	//ui.textEdit->append( "AT EVENT" );
	if (pEvent->GetType() == se::EATResponceType::InvalidEvent)
	{
		se::CInvalidResponce* pRssiValue = dynamic_cast<se::CInvalidResponce*>(pEvent.get());
		if (pRssiValue)
			ui.textEdit->append( QString( "Invalid Responce :(: %1" ).arg( pRssiValue->GetMessage() ) );
	}
}

void columba::onNewMessage( se::t_SmsMessageSharedPtr pMessage )
{
	if (!pMessage)
		return;
	std::wstring sMsg = pMessage->GetMessage().toStdWString();
	int nSize = sMsg.size();
	ushort* pUtf16 = new ushort[nSize];
	for (int i = 0; i < sMsg.size(); ++i)
	{
		short t = sMsg[i];
		ushort c = t;
		QString s = QString::fromUtf16( &c, 1 );
		pUtf16[i] = c;
	}
	SE_ASSERT( pMessage );
	ui.textEdit->append( QString( "Wow!!! New Message!\nFrom: %1\nTime: %2\nMessage: %3\n" )
						 .arg( pMessage->GetPhoneNumber() )
						 .arg( pMessage->GetDateTime().toString() )
						 .arg( QString::fromUtf16( pUtf16, nSize ) ) );

	// Testing
	int nDur = 5000;
	//startTimer(nDur);
	//ui.textEdit->append( QString( "\nTimer Started: Dur: %1 msec" ).arg( nDur ) );
}


// timerEvent
void columba::timerEvent( QTimerEvent *pEvent )
{
	ui.textEdit->append( QString( "Executing CNMI Config Task" ));
	m_pSmsEngine->Configure();
	ui.textEdit->append( QString( "CNMI Config Task Executed" ) );
	int nId = pEvent->timerId();
	QObject::killTimer( nId );
}

void columba::onSmsWasSent( se::t_SmsMessageSharedPtr pMessage )
{
	ui.textEdit->append( QString( "WOW!!! SMS Was Sent" ) );
}