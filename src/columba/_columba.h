#ifndef COLUMBA_H
#define COLUMBA_H

#include <QtWidgets/QMainWindow>
#include "ui_test.h"
#include <QSerialPortInfo>
//
#include "se_serialdevice.h"
#include "se_atbasicparser.h"
#include "se_at_smsengine.h"
//#include "se_ismsgateway.h"
//#include "se_ismsengine.h"
//#include "se_smsmessage.h"
//#include "se_error.h"

#include "db_connectionmanager.h"
#include "db_maincomponent.h"


class columba : public QMainWindow
{
	Q_OBJECT

public:
	columba(QWidget *parent = 0);
	~columba();

public slots:
	void onConnect();
	void onError(se::CError const&);
	void onResponseReady(se::t_ATCommandPtr pAT);
	void onATEvent( QByteArray const& aData );
	void onATEvent( se::t_ATResponceSharedPtr pEvent );
	void onExec();
	// Events
	void onRSSI( int );
	void onNewMessage( se::t_SmsMessageSharedPtr pMessage );
	void onSmsWasSent( se::t_SmsMessageSharedPtr pMessage );

protected:
	void timerEvent( QTimerEvent *pEvent );

private:
	Ui::TestWindow ui;
	QList<QSerialPortInfo> m_lstPorts;
	se::CSerialDevice* m_pDevice;
	se::CATEventParser* m_pEventParser;
	se::CATCommandResponceParser* m_pCommandParser;

	se::CATSmsEngine*   m_pSmsEngine;

	db::CConnectionManager* m_pDBConnectionManager;
	db::CMainDataDB* m_pComponent;
};

#endif // COLUMBA_H
