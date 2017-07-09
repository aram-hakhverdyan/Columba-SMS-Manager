#ifndef SE_AT_TASK_H
#define SE_AT_TASK_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_atcommands.h"
#include "se_ismsgateway.h"
//	Qt Includes
#include <QObject>
#include <QAtomicInt>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Interface ITaskStep
//
class SMSENGINE_EXPORT ITaskStep
{
public:
	//
	//	struct STaskStepResult
	//
	struct STaskStepResult
	{
		STaskStepResult( ETaskStatus eSt = ETaskStatus::Undefined, ITaskStep* pNextTask = nullptr )
			: eStatus(eSt),
			  pNextTaskStep(pNextTask)
		{}
		// Content
		ETaskStatus eStatus;
		ITaskStep*  pNextTaskStep;
		QString     sErrorMesage;
	};

public:
	inline ITaskStep() = default;
	virtual ~ITaskStep() = default;

public:
	//
	//	Main Interface
	//
	virtual t_ATCommandPtr   PrepareCommand( t_ATCommandPtr pPrevious ) = 0;
	virtual STaskStepResult ProcessResponce( t_ATCommandPtr pResponce ) = 0;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CSingleCommandExecution
//
class SMSENGINE_EXPORT CSingleCommandExecution : public ITaskStep
{
public:
	CSingleCommandExecution( t_ATCommandPtr pATCommand )
		: m_pATCommand(pATCommand) {}
	virtual ~CSingleCommandExecution() = default;

public:
	//
	//	Main Interface
	//
	inline void SetCommand( t_ATCommandPtr pATCommand );
	inline t_ATCommandPtr GetCommand() const;
		 
public:
	//
	//	Overidden Methods
	//
	t_ATCommandPtr   PrepareCommand( t_ATCommandPtr pPrevious ) override;
	STaskStepResult ProcessResponce( t_ATCommandPtr pResponce ) override;

private:
	//
	//	Content
	//
	t_ATCommandPtr m_pATCommand;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	struct STaskResult
//
struct STaskResult
{
	STaskResult()
		: eStatus( ETaskStatus::Undefined )
	{}
	ETaskStatus    eStatus;
	t_ATCommandPtr pLastCommand;
	QString		   sErrorMessage; // If failed
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CTaskRunner
//
class SMSENGINE_EXPORT CTaskRunner : public QObject
{
	Q_OBJECT

public:
	inline CTaskRunner( QObject* pParent = nullptr );
	virtual ~CTaskRunner() = default;

public:
	//
	//	Main Interface
	//
	bool Start( ITaskStep* pTask, bool bOwner = true );
	void SetATDevice( IATDevice* pDevice );
	// if -1, forever
	void WaitForFinish( qint64 nMsec = 30000 );
	inline bool IsRuning() const;
	inline STaskResult GetResult() const;

signals:
	//
	//	Signals
	//
	void sigResponceReady( STaskResult const& strResult );

protected slots:
	//
	//	Secure Slots
	//
	void onResponceReady( se::t_ATCommandPtr );
protected:
	//
	//	Tools
	//
	inline void ResetCurrentTask( ITaskStep* pTask = nullptr );
	// Terminates task execution and emits responce
	void Finish( ETaskStatus eStatus, 
				 t_ATCommandPtr pLastCommand, 
				 QString sErrorMessage = QString() );
protected:
	// Overidden
	void timerEvent( QTimerEvent* pEvent ) override;


private:
	//
	//	Content
	//
	IATDevice*		m_pDevice;
	ITaskStep*		m_pCurrentTask;
	t_ATCommandPtr	m_pCurrentATCommand;
	bool			m_bOwner;
	QAtomicInt		m_anIsRunning;
	STaskResult		m_strLastResult;
	int             m_nTimmerID;
};
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
// [ISmsGetway] Default Constructor
inline CTaskRunner::CTaskRunner( QObject* pParent )
	: QObject( pParent ),
	  m_pDevice( nullptr ),
	  m_pCurrentTask(nullptr),
	  m_bOwner( false ),
	  m_nTimmerID( -1 )

{}

// ResetCurrentTask
inline void CTaskRunner::ResetCurrentTask( ITaskStep* pTask )
{
	if (m_bOwner && m_pCurrentTask)
		delete m_pCurrentTask;
	m_pCurrentTask = pTask;
	if (!pTask)
		m_pCurrentATCommand.reset();
}

// IsRuning
inline bool CTaskRunner::IsRuning() const
{
	return bool( m_anIsRunning );
}

// GetResult
inline STaskResult CTaskRunner::GetResult() const
{ return m_strLastResult; }


//
//	class CSingleCommandExecution
//
// SetCommand
inline void CSingleCommandExecution::SetCommand( t_ATCommandPtr pATCommand )
{ m_pATCommand = pATCommand; }

// GetCommand
inline t_ATCommandPtr CSingleCommandExecution::GetCommand() const
{ return m_pATCommand; }
////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // SE_SMS_GATEWAY_H
