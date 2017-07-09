#ifndef APP_MAIN_WINDOW_H
#define APP_MAIN_WINDOW_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "app_smsmanager.h"
#include "app_phonebook.h"

#include "db_connectionmanager.h"
#include "ui_columba.h"
// Qt
#include <QMainWindow>
#include <QSerialPortInfo>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Forward Declarations
//
namespace gui { 
class CConversationExplorer; 
class CSettingsDialog;
class CStatusView;
enum class EConnectionStatus;
}
namespace db{ class CMainDataDB; }

namespace se  {
class CSerialDevice;
class CATSmsEngine;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace app { // application
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CColumbaMainWindow
//
class CColumbaMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	CColumbaMainWindow( QWidget *pParent = nullptr );
	~CColumbaMainWindow();
	
public:
	//
	//	Main Interface
	//
	bool TryConnectToPort( QSerialPortInfo oPortInfo, QString* pMessage = nullptr );
	void Disconnect();


public slots:
	//
	//	Public Slots
	//
	void onNotify( QString const& sTitle,
				   QString const& sText,
				   ENotificationType eType );
	// Null string will mean to turn off the status report
	void onReportStatus( QString const& sStatus = QString() );
	// Used by SMS Manager
	void onStatusBarRequest( QStatusBar*& pBar );


protected slots:
	//
	//	Secure Slots
	//
	// Own Actions
	// Settings Dialog
	void onOpenSettingsDialog();
	void onCloseSettingsDialog();
	void onSettingsAccepted();
	void onTryConnect( int nIdx, bool& bResult, QString* pMessage );
	void onTryDisconnect( int nIdx, bool& bResult, QString* pMessage );
	void onUpdateSettings();
	// 
	void onNewMessage( se::t_SmsMessageSharedPtr pMessage );
	void onDisconnected();

protected:
	//
	//	Tools
	//
	void AddModule( IModule* pModule );
	void CloseModule( IModule* pModule );
	void UpdateSerialPortInfo();
	QStringList GetSerialPortNames() const;
	void SetConnectionStatus( gui::EConnectionStatus eStatus );

	//
	void LoadSettings();
	void SaveSettings();

	QString MakeSerialPortName( QSerialPortInfo const& oPortInfo ) const;
	// 
	QSerialPortInfo SerialPortInfoByName( QString const& sPortName, int* pIdx = nullptr );

	// Overidden From Base
	bool eventFilter( QObject* pWatched, QEvent* pEvent );

	QString GetDBPath();

private:
	//
	//	Content
	//
	// UI
	Ui::ColumbaMainWindowUI ui;
	// DB Manager
	db::t_ConnectionManagerSPtr m_pDBManager;
	// SMS Manager
	CSmsManager* m_pSmsManager;
	// Phone Book
	CPhoneBook*  m_pPhoneBook;
	// List Of Moduls
	QList<IModule*> m_lstModules; // Sould be incapsulated in ModulManager class
	// Connection Status Widget
	gui::CStatusView* m_pStatusWidget;

	//
	//	Settings
	//
	// Settings Dialog
	gui::CSettingsDialog* m_pSettingsDialog;
	// Serial Port List
	QList<QSerialPortInfo> m_lstPorts;
	// Settings
	QSettings* m_pSettings;
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

#endif // APP_MAIN_WINDOW_H
