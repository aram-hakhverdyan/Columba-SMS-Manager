#ifndef SE_SERIAL_DEVICE_H
#define SE_SERIAL_DEVICE_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_ismsgateway.h"
//#include "se_at_iparser.h"
#include "se_atbasicparser.h"
//
//	Qt Includes
//
#include <QQueue>
#include <QSerialPort>
#include <QPair>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSerialDevice
//
class SMSENGINE_EXPORT CSerialDevice : public IATDevice
{
	Q_OBJECT
	// Type Redefinitions
	typedef IATDevice Base;

public:
	//! Constructor & Destructor
	inline CSerialDevice( QObject* pParent = nullptr );
	virtual ~CSerialDevice() = default;

public:
	//
	//	Main Interface
	//
	// Sets serial port and tries to open it
	void SetPort( QSerialPort     const& oPort );
	QSerialPort const& Port() const;

	void SetPort( QSerialPortInfo const& oPort );
	bool TestPort( QSerialPortInfo const& oPort,
				   QString* pMessage = nullptr );
	void ClosePort();
	void SetEventParser( t_IATParserSPtr pParser );
	void SetCommandParser( t_CommandResponceParserSPtr pParser );
	
public:
	//
	//	Overidden From Base
	//
	bool ExecuteCommand( t_ATCommandPtr pATCommand ) override;
	bool IsReady() const override;


protected:
	// Overidden from QObject
	void timerEvent( QTimerEvent *pEvent ) override;

protected slots:
	//
	//	Secure Slots
	//
	void onReadyRead();
	void onError( QSerialPort::SerialPortError eError );

signals:
	//
	//	Signals
	//
	void sigRssiChanged( int nNewValue, int nOldValue );

protected:
	//
	//	Tools
	//	
	void ExecuteFirstInQueue();
	void WriteInTempFile( QString const& sData ) const;
	inline void ResetMoreDataWaitor( t_IATParserSPtr pWaitor = nullptr );
	
	bool TryProcessCommandResponce( QStringList& lstResponce );
	bool TryProcessEventResponce( QStringList& lstResponce );
	// Unused
	bool ProcessCommandResponce( QStringList& lstResponce );
	bool ProcessResponce( t_IATParserSPtr pParser, QStringList& lstResponce );

private:
	//
	//	Content
	//
	QSerialPort					m_oSerialPort;
	QQueue<t_ATCommandPtr>		m_qExecutionQueue;
	t_IATParserSPtr				m_pEventParser;
	t_CommandResponceParserSPtr	m_pCommandParser;
	t_IATParserSPtr				m_pNeedsMoreData;
	bool						m_bIdenticResponceRead;
	int							m_nTimerId;
	t_ATResponceSharedPtr		m_pLastRssiEvent;
	bool						m_bRejectDublicates;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CSerialDeviceCreator 
//
class SMSENGINE_EXPORT CSerialDeviceCreator 
{
public:
	//! Constructor & Destructor
	CSerialDeviceCreator() = default;
	~CSerialDeviceCreator() = default;

public:
	//
	//	Static Interface
	//
	static CSerialDevice* CreateSerialDevice( QObject* pParent = nullptr );
};
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
inline void CSerialDevice::ResetMoreDataWaitor( t_IATParserSPtr pWaitor )
{
	m_pNeedsMoreData = pWaitor;
}

////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // SE_SERIAL_DEVICE_H
