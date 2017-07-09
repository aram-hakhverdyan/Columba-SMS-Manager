#ifndef APP_SMS_MANAGER_H
#define APP_SMS_MANAGER_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "app_imodule.h"
#include "se_error.h"
#include "se_smsmessage.h"
#include "db_maincomponent.h"
// Qt
#include <QSerialPort>
#include <QStatusBar>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Forward Declarations
//
namespace gui {
class CConversationExplorer; 
class CNewMessageDialog; 
}
namespace db{ class CMainDataDB; }

namespace se  {
class CSerialDevice;
class CATSmsEngine;
}

class QLabel;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace app { // application
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSmsManager
//
class CSmsManager : public IModule
{
	Q_OBJECT
	typedef IModule Base;

public:
	CSmsManager( QObject* pParent = nullptr );
	~CSmsManager();

public:
	//
	//	Own Interface
	//
	void ConnectToPort( QSerialPortInfo const& oPortInfo );
	void Disconnect();
	bool TestSerialPort( QSerialPortInfo const& oPort,
						 QString* pMessage = nullptr );
	QSerialPortInfo GetCurrentPortInfo() const;
	bool IsSmsEngineReady() const;
	//void ConfigureSmsEngine();

public:
	//
	//	IModule Interface <-
	//	
	QString		GetName() const								override;
	QString		Topic() const								override;
	QWidget*	GetWidget()									override;
	QIcon		GetIcon() const								override;
	QList<QActionGroup*> GetActionList()					override;
	t_IDBComponentSPtr GetDBComponent()						override;
	void		Initialize()								override;
	void		SaveSettings( QSettings* pSettings )		override;
	void		LoadSettings( QSettings const* pSettings )	override;

protected slots:
	//
	//	Secure SLOTs
	//
	void onError( se::CError const& );
	// Events
	void onNewMessage( se::t_SmsMessageSharedPtr pMessage );
	void onMessageWasSent( se::t_SmsMessageSharedPtr pMessage );
	void onRssiChanged( int nNewValue );
	void onSendMessage( QString const& sSenderInfo,
						QString const& sMessage );

	// New Message Dialog
	void onOpenNewMessageDialog();

signals:
	void sigStatusBarRequest( QStatusBar*& pStatusBar );
	void sigNewMessage( se::t_SmsMessageSharedPtr pMessage );
	void sigDisconnected();

private:
	//
	//	Content
	//
	// GUI
	gui::CConversationExplorer* m_pConversationExplorer;
	// SE
	se::CSerialDevice* m_pSerialDevice;
	se::CATSmsEngine*  m_pSmsEngine;
	// DB
	db::t_MainDataDBSPtr m_pDataDB;
	// Animation Label
	QLabel* m_pAnimationLabel;

	// 
	se::t_SmsMessageSharedPtr m_pLastSentSms;
	
	// New Message Action
	QActionGroup* m_pDefaultActionGroup;
	// New Message Dialog
	gui::CNewMessageDialog* m_pNewMessageDialog;

	// Sending Origine
	bool m_bSendCalledFromDialog; // or from the chat
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace application
////////////////////////////////////////////////////////////////////////////////

#endif // APP_SMS_MANAGER_H
