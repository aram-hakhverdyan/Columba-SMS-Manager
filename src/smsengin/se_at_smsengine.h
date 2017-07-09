#ifndef SE_AT_SMS_ENGINE_H
#define SE_AT_SMS_ENGINE_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_ismsengine.h"
#include "se_attask.h"
//
//	Qt Includes
//
////////////////////////////////////////////////////////////////////////////////

//
//	Forward Declarations
//
class QThread;

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Interface CATSmsEngine
//! Implementation class of IATSmsEngine interface
//! Provides high-level SMS transmission 
class SMSENGINE_EXPORT CATSmsEngine : public IATSmsEngine
{
	Q_OBJECT

	typedef IATSmsEngine Base;
	typedef std::unique_ptr<CTaskRunner> t_TaskRunnerPtr;

public:
	CATSmsEngine( QObject* pParent = nullptr );
	virtual ~CATSmsEngine();

public:
	//
	//	ISmsEngine Interface
	//
	// Send SMS message in none blocking mode
	// By default uses checked SMSC number
	void SendMessage( CSmsMessage const& sMessage,
					  QString const& sSMSCNumber = QString() ) override;
	// Returns SMS Device
	ISmsGetway* GetSmsGetway() override;

public:
	//
	//	IATSmsEngine Interface
	//
	// Set/Get SMS message transmission mode (TEXT | PDU)
	void     SetMode( ESmsMode eMode ) override;
	ESmsMode GetMode() const           override;
	// Get/Set AT Device
	IATDevice* GetDevice() const       override;
	void       SetDevice( IATDevice* ) override;

public:
	//
	//	Own Interface
	//
	// Checks current SMS mode in device and updates mode member
	ESmsMode	CheckCurrentMode( qint64 nMsecs = 5000 );
	QString		CheckSMSCNumber( qint64 nMsecs = 5000 );

	bool		Configure();
	t_ATCommandPtr ExecuteATCommand( QString sATCommand, qint64 nMsecs = 5000 );

	inline bool		IsConfigured() const;
	inline bool		IsReady() const;
	inline QString	GetSMSCNumber() const;
	inline bool		IsInSendingState() const;

protected slots:
	//
	//	Secure Slots
	//
	void onATEvent( se::t_ATResponceSharedPtr pEvent );
	void onTaskFinished( STaskResult const& strResponce );

protected:
	//
	//	Tools
	//
	// Creates TaskRunner instance and makes apropriate signal-slot connectes 
	CTaskRunner* CreateTaskRunner();

protected:
	////////////////////////////////////////////////////////////////////////////
	//
	//	Tasks
	//
	////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////
	//
	//	Set SMS Mode 
	//
	class CSetSMSMode : public ITaskStep
	{
	public:
		CSetSMSMode( CATSmsEngine& oEngine, ESmsMode eMode )
			: m_oEngine( oEngine ),
			  m_eMode( eMode ) { SE_ASSERT( eMode != ESmsMode::eUnknown ); }

	public:
		//	Overidden Methods
		t_ATCommandPtr   PrepareCommand( t_ATCommandPtr pPrevious ) override;
		STaskStepResult ProcessResponce( t_ATCommandPtr pResponce ) override;

	private:
		//	Content
		CATSmsEngine& m_oEngine;
		ESmsMode m_eMode;
	};
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	//
	//	Check SMS Mode
	//
	class CCheckSMSMode : public ITaskStep
	{
	public:
		CCheckSMSMode( CATSmsEngine& oEngine )
			: m_oEngine( oEngine ) {}

	public:
		//	Overidden Methods
		t_ATCommandPtr   PrepareCommand( t_ATCommandPtr pPrevious ) override;
		STaskStepResult ProcessResponce( t_ATCommandPtr pResponce ) override;

	private:
		//	Content
		CATSmsEngine& m_oEngine;
	};
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	//
	//	Reset Message Receiving Configuration
	//
	class CResetMessageReceivingConfig: public ITaskStep
	{
	public:
		CResetMessageReceivingConfig( CATSmsEngine& oEngine, 
									  bool bIsReseted = false )
			: m_oEngine( oEngine ),
			m_bIsReseted( bIsReseted )
		{}

	public:
		//	Overidden Methods
		t_ATCommandPtr   PrepareCommand( t_ATCommandPtr pPrevious ) override;
		STaskStepResult ProcessResponce( t_ATCommandPtr pResponce ) override;

	private:
		//	Content
		CATSmsEngine& m_oEngine;
		bool		  m_bIsReseted; // Is Reset State Discovered
	};
	////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////
	//
	//	Send SMS Message
	//
	class CSendSMS_Step1 : public ITaskStep
	{
	public:
		CSendSMS_Step1( CATSmsEngine& oEngine,
						QStringList const& lstsPDU )
						: m_oEngine( oEngine ),
						m_lstPDU( lstsPDU )
		{}

	public:
		//	Overidden Methods
		t_ATCommandPtr   PrepareCommand( t_ATCommandPtr pPrevious ) override;
		STaskStepResult ProcessResponce( t_ATCommandPtr pResponce ) override;

	private:
		//	Content
		CATSmsEngine&	m_oEngine;
		QStringList		m_lstPDU;
	};
	// Step 2
	class CSendSMS_Step2 : public ITaskStep
	{
	public:
		CSendSMS_Step2( CATSmsEngine& oEngine,
						QStringList const& lstPDU )
						: m_oEngine( oEngine ),
						m_lstPDU( lstPDU )
		{}

	public:
		//	Overidden Methods
		t_ATCommandPtr   PrepareCommand( t_ATCommandPtr pPrevious ) override;
		STaskStepResult ProcessResponce( t_ATCommandPtr pResponce ) override;

	private:
		//	Content
		CATSmsEngine&	m_oEngine;
		QStringList		m_lstPDU;
	};
	////////////////////////////////////////////////////////////////////////////

private:
	//
	//	Content
	//
	IATDevice*      m_pDevice;
	ESmsMode		m_eMode;
	bool			m_bIsConfigured;
	QString			m_sSMSCNumber;
	//CTaskRunner*    m_pTaskRunner;
	//QThread*		m_pWorkerThread;
	// Stores the count of the calls of SendMessage method: 
	// After the finidh of send message task the count is decrementing
	int				m_nSendingCount;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
//
//	class CATSmsEngine
//
inline bool CATSmsEngine::IsConfigured() const { return m_bIsConfigured; }
inline bool	CATSmsEngine::IsReady() const
{ return m_pDevice && m_pDevice->IsReady() && IsConfigured(); }
inline QString CATSmsEngine::GetSMSCNumber() const { return m_sSMSCNumber; }
inline bool		CATSmsEngine::IsInSendingState() const { return m_nSendingCount > 0 ? true : false; }

////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // SE_AT_SMS_ENGINE_H
