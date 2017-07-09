#ifndef SE_AT_COMMANDS_H
#define SE_AT_COMMANDS_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_core.h"
#include "se_atcommandresponcehandler.h"
#include "se_responceobjects.h"
//	Qt Includes
#include <QByteArray>
#include <QVector>
// STL
#include <memory>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CATCommand
//
class SMSENGINE_EXPORT CATCommand
{
public:
	// Type Redefinitions
	typedef  QList<IATResponceHandler*> t_lstHandlers;
	typedef  QList<t_ATResponceHandlerUniquePtr> t_lstUniqueHandlers;

public:
	// Constructor & Destructor
	CATCommand( QString const& sCommandName,
				bool           bFullCommand = false,
				QString const& sPostATSymbol = t_csz( "+" ),
				QString const& sATDomain = t_csz( "AT" ) );
	virtual ~CATCommand() = default;

public:
	//
	//	Main Interface
	//
	//! Returns command data for the an apropriate role. 
	//! This methods is using during command execution on AT device.
	QByteArray GetData() const;
	inline QString GetCommandName() const;

	// Set/Get
	inline void  SetRole( ECommandRole eRole );
	inline ECommandRole GetRole() const;

	// Get Responce Handler
	virtual t_lstHandlers GetResponceHandler();
	
	// Get/Set status
	inline ECommandStatus GetStatus() const;
	inline void SetStatus( ECommandStatus eStatus );

protected:
	//
	//	Polimorphic Methods
	//
	// Returns AT command argument: [AT+<cmd_name>=<arg>] 
	virtual QString GetArgument() const;

private:
	//
	//	Content
	//
	// Command Data
	QString						 m_sATDomain;
	QString						 m_sPostATSymbol;
	QString						 m_sCommandName;
	// This property holds whater command name 
	// contains full command specification
	bool						 m_bFullCommand;
	// Command Attributs
	ECommandRole				 m_eRole;
	ECommandStatus               m_eStatus;
	

protected:
	// Command responce handlers
	t_ATResponceHandlerUniquePtr m_pDefaultPositiveHandler;
	t_ATResponceHandlerUniquePtr m_pDefaultNegativeHandler;
};

typedef std::shared_ptr<CATCommand> t_ATCommandPtr;
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CMGF
//
class SMSENGINE_EXPORT CAT_CMGF : public CATCommand
{
	typedef CATCommand Base;
	typedef std::shared_ptr<C_CMGF_Responce> t_ResponcePtr;

public:
	// Constructor & Destructor
	inline CAT_CMGF( ESmsMode eMode = ESmsMode::eUnknown );
	virtual ~CAT_CMGF() = default;

public:
	//
	//	Main Interface
	//
	inline C_CMGF_Responce* GetResponce() const;
	// Get/Set Mode 
	inline ESmsMode GetMode() const;
	inline void		SetMode( ESmsMode eMode );
	// 

protected:
	//
	//	Overidden Methods
	//
	// Returns AT command argument: [AT+<cmd_name>=<arg>] 
	QString GetArgument() const override;
public:
	// Get Responce Handler
	t_lstHandlers GetResponceHandler() override;

public:
	////////////////////////////////////////////////////////////////////////////
	//
	//	Responce Handlers
	//
	class CStateHandler : public CDefaultResponceHandler
	{
		typedef CDefaultResponceHandler Base;
	public:
		// Constructor
		CStateHandler()
			: Base("CMGF") {}
	protected:
		// Overidden Methods
		t_ATResponceSharedPtr ProcessResponce( QStringList const& lstResponce, 
											   EParsingStatus& eStatus ) override;
	};
	// class CSupportCheckHandler
	//class CSupportCheckHandler : public CDefaultResponceHandler
	//{
	//	typedef CDefaultResponceHandler Base;
	//public:
	//	// Constructor
	//	CSupportCheckHandler()
	//		: Base( "CMGF" )
	//	{}
	//protected:
	//	// Overidden Methods
	//	t_ATResponceSharedPtr ProcessResponce( QStringList const& lstResponce ) override;
	//};
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////

private:
	//
	//	Content
	//
	ESmsMode					 m_eMode;
	t_ResponcePtr				 m_pResponceObject;
	t_ATResponceHandlerSPtr m_pStateHandler;
};

typedef std::shared_ptr<CAT_CMGF> t_AT_CMGF_Ptr;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CSCA
//! SMS Service Center Information
class SMSENGINE_EXPORT CAT_CSCA : public CATCommand
{
	typedef CATCommand Base;
	typedef std::shared_ptr<C_CSCA_State> t_StateDataPtr;

public:
	// Constructor & Destructor
	//! Note Works Only in "Check State" mode (Yet
	inline CAT_CSCA();
	virtual ~CAT_CSCA() = default;

public:
	//
	//	Main Interface
	//
	inline C_CSCA_State* GetResponce() const; 

protected:
	//
	//	Overidden Methods
	//
	// Returns AT command argument: [AT+<cmd_name>=<arg>] 
	//QString GetArgument() const override;
public:
	// Get Responce Handler
	t_lstHandlers GetResponceHandler() override;

public:
	////////////////////////////////////////////////////////////////////////////
	//
	//	Responce Handlers
	//
	class CStateHandler : public CDefaultResponceHandler
	{
		typedef CDefaultResponceHandler Base;
	public:
		// Constructor
		CStateHandler()
			: Base( "CSCA" )
		{}
	protected:
		// Overidden Methods
		t_ATResponceSharedPtr ProcessResponce( QStringList const& lstResponce,
											   EParsingStatus& eStatus ) override;
	};
	////////////////////////////////////////////////////////////////////////////

private:
	//
	//	Content
	//
	t_StateDataPtr				 m_pResponceObject;
	t_ATResponceHandlerSPtr		 m_pStateHandler;
};

typedef std::shared_ptr<CAT_CSCA> t_AT_CSCA_Ptr;
////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CNMI
//
class SMSENGINE_EXPORT CAT_CNMI : public CATCommand
{
	typedef CATCommand Base;
	typedef std::shared_ptr<C_CNMI_Responce> t_ResponcePtr;

public:
	// Constructor & Destructor
	inline CAT_CNMI( int nMode = 0,
					 int nMt = 0,
					 int nBm = 0,
					 int nDs = 0,
					 int nBfr = 0 );
	virtual ~CAT_CNMI() = default;

public:
	//
	//	Main Interface
	//
	inline C_CNMI_Responce* GetResponce() const;
	
protected:
	//
	//	Overidden Methods
	//
	// Returns AT command argument: [AT+<cmd_name>=<arg>] 
	QString GetArgument() const override;
public:
	// Get Responce Handler
	t_lstHandlers GetResponceHandler() override;

public:
	////////////////////////////////////////////////////////////////////////////
	//
	//	Responce Handlers
	//
	class CStateHandler : public CDefaultResponceHandler
	{
		typedef CDefaultResponceHandler Base;
	public:
		// Constructor
		CStateHandler()
			: Base( "CNMI" ),
			  m_bWaitForOk(false)
		{}
	protected:
		// Overidden Methods
		t_ATResponceSharedPtr ProcessResponce( QStringList const& lstResponce, 
											   EParsingStatus& eStatus ) override;
	private:
		// Content
		bool m_bWaitForOk;
		t_ResponcePtr m_pResponce;
	};
	////////////////////////////////////////////////////////////////////////////

private:
	//
	//	Content
	//
	QString				         m_sSettings;   // Mode,Mt,Bm,Ds,Bfr
	t_ResponcePtr				 m_pResponceObject;
	t_ATResponceHandlerSPtr m_pStateHandler;
};

typedef std::shared_ptr<CAT_CNMI> t_AT_CNMI_Ptr;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CMGS
//
class SMSENGINE_EXPORT CAT_CMGS_Step1 : public CATCommand
{
	typedef CATCommand Base;
	typedef std::shared_ptr<CCMSErrorCode>		t_CMSErrorCodePtr;

	friend class CCMSErrorHandler;
public:
	// Constructs a CMGS object whick will run in Text mode
	inline CAT_CMGS_Step1( QString const& sPhoneNumber );
	// Constructs a CMGS object whick will run in PDU mode
	inline CAT_CMGS_Step1( int nLength );
	// Destructor
	virtual ~CAT_CMGS_Step1() = default;

public:
	//
	//	Main Interface
	//
	// Get/Set SMS Mode
	inline ESmsMode GetWorkingMode() const;

public:
	//
	//	Overidden Methods
	//
	// Get Responce Handler
	t_lstHandlers GetResponceHandler() override;

protected:
	// Returns AT command argument: [AT+<cmd_name>=<arg>] 
	QString GetArgument() const override;
	
protected:
	////////////////////////////////////////////////////////////////////////////
	//
	//	Responce Handlers
	//
	class CFirstStageHandler : public CATCommandResponceHandler
	{
		typedef CATCommandResponceHandler Base;
	public:
		// Constructor
		CFirstStageHandler()
			: Base( ">" )
		{}
	protected:
		// Overidden Methods
		t_ATResponceSharedPtr Handle( QString sData, EParsingStatus& eStatus ) override;
	};
	////////////////////////////////////////////////////////////////////////////

private:
	//
	//	Content
	//
	QString m_sPhoneNumber;
	int		m_nLength;
	t_ATResponceHandlerSPtr m_pHandler;
	t_CMSErrorCodePtr		m_pCMSErrorCode;
	t_ATResponceHandlerSPtr m_pCMSErrorHandler;
};

typedef std::shared_ptr<CAT_CMGS_Step1> t_AT_CMGS_Step1_Ptr;
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CMGS
//
class SMSENGINE_EXPORT CAT_CMGS_Step2 : public CATCommand
{
	typedef CATCommand Base;
	typedef std::shared_ptr<C_CMGS_Responce>	t_ResponcePtr;
	typedef std::shared_ptr<CCMSErrorCode>		t_CMSErrorCodePtr;

	friend class CCMSErrorHandler;
public:
	// Constructs a CMGS object whick will run in Text mode
	inline CAT_CMGS_Step2( QString const& sData );
	// Destructor
	virtual ~CAT_CMGS_Step2() = default;

public:
	//
	//	Main Interface
	//
	
public:
	//
	//	Overidden Methods
	//
	// Get Responce Handler
	t_lstHandlers GetResponceHandler() override;
	t_ResponcePtr GetResponce() const;

protected:
	////////////////////////////////////////////////////////////////////////////
	//
	//	Responce Handlers
	//
	class CCMGSHandler: public CDefaultResponceHandler
	{
		typedef CDefaultResponceHandler Base;
	public:
		// Constructor
		CCMGSHandler( )
			: Base( "CMGS" ),
			m_bWaitForOk( false )
		{}
	protected:
		// Overidden Methods
		t_ATResponceSharedPtr ProcessResponce( QStringList const& lstResponce,
											   EParsingStatus& eStatus ) override;
	private:
		// Content
		bool m_bWaitForOk;
		t_ResponcePtr m_pResponce;
	};
	////////////////////////////////////////////////////////////////////////////

private:
	//
	//	Content
	//
	t_ResponcePtr			m_pResponce;
	t_ATResponceHandlerSPtr m_pCMGSHandler;
	t_CMSErrorCodePtr		m_pCMSErrorCode;
	t_ATResponceHandlerSPtr m_pCMSErrorHandler;
};

typedef std::shared_ptr<CAT_CMGS_Step2> t_AT_CMGS_Step2_Ptr;
////////////////////////////////////////////////////////////////////////////////










////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CATCommand
//
// GetCommandName
inline QString CATCommand::GetCommandName() const
{ return m_sCommandName; }

// GetRole
inline ECommandRole CATCommand::GetRole() const
{ return m_eRole; }

// SetRole
inline void  CATCommand::SetRole( ECommandRole eRole )
{ m_eRole = eRole; }

// GetStatus
inline ECommandStatus CATCommand::GetStatus() const
{ return m_eStatus; }

inline void CATCommand::SetStatus( ECommandStatus eStatus )
{ m_eStatus = eStatus; }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CMGF
//
// GetCommandName
inline CAT_CMGF::CAT_CMGF( ESmsMode eMode )
	: Base( t_sz( "CMGF" ) ),
	  m_eMode( eMode )
{
	CStateHandler* pStateHandler = new CStateHandler();
	pStateHandler->SetParentCommand( this );
	m_pStateHandler.reset( pStateHandler );
}


// GetResponce
inline C_CMGF_Responce* CAT_CMGF::GetResponce() const
{ return m_pResponceObject.get(); }

// GetMode
inline ESmsMode CAT_CMGF::GetMode() const
{ return m_eMode; }

// SetMode
inline void CAT_CMGF::SetMode( ESmsMode eMode )
{ m_eMode = eMode; }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CSCA
//
// GetCommandName
inline CAT_CSCA::CAT_CSCA()
	: Base( t_sz( "CSCA" ) )
{
	CStateHandler* pStateHandler = new CStateHandler();
	pStateHandler->SetParentCommand( this );
	m_pStateHandler.reset( pStateHandler );
	Base::SetRole( ECommandRole::eCheckState );
}


// GetResponce
inline C_CSCA_State* CAT_CSCA::GetResponce() const
{
	return m_pResponceObject.get();
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CNMI
//
// CAT_CNMI
inline CAT_CNMI::CAT_CNMI( int nMode,
				 int nMt,
				 int nBm,
				 int nDs,
				 int nBfr )
				 : Base( t_sz( "CNMI" ) )
{
	m_sSettings.append( QString::number( nMode ).append(',') );
	m_sSettings.append( QString::number( nMt ).append( ',' ) );
	m_sSettings.append( QString::number( nBm ).append( ',' ) );
	m_sSettings.append( QString::number( nDs ).append( ',' ) );
	m_sSettings.append( QString::number( nBfr ) );

	CStateHandler* pStateHandler = new CStateHandler();
	pStateHandler->SetParentCommand( this );
	m_pStateHandler.reset( pStateHandler );
}


// GetResponce
inline C_CNMI_Responce* CAT_CNMI::GetResponce() const
{ return m_pResponceObject.get(); }

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CMGS_Step1
//
inline CAT_CMGS_Step1::CAT_CMGS_Step1( QString const& sPhoneNumber )
	: Base(t_sz( "CMGS" )),
	m_sPhoneNumber( sPhoneNumber ),
	m_nLength( -1 )
{
	CFirstStageHandler* pHandler = new CFirstStageHandler();
	pHandler->SetParentCommand( this );
	m_pHandler.reset( pHandler );

	// Create default CMS Error Handler
	CCMSErrorHandler* pErrorHandler = new CCMSErrorHandler();
	pErrorHandler->SetParentCommand( this );
	m_pCMSErrorHandler.reset( pErrorHandler );
}

inline CAT_CMGS_Step1::CAT_CMGS_Step1( int nLength )
	: Base( t_sz( "CMGS" ) ),
	m_nLength( nLength )

{
	CFirstStageHandler* pHandler = new CFirstStageHandler( );
	pHandler->SetParentCommand( this );
	m_pHandler.reset( pHandler );

	// Create default CMS Error Handler
	CCMSErrorHandler* pErrorHandler = new CCMSErrorHandler();
	pErrorHandler->SetParentCommand( this );
	m_pCMSErrorHandler.reset( pErrorHandler );
}


// GetWorkingMode
inline ESmsMode CAT_CMGS_Step1::GetWorkingMode() const
{ return m_sPhoneNumber.isEmpty()? ESmsMode::ePDUMode : ESmsMode::eTextMode; }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CMGS_Step2
//
inline CAT_CMGS_Step2::CAT_CMGS_Step2( QString const& sData )
	: Base( sData + QString( QChar( '\032' ) ), true )
{
	// Create default CMGS Handler
	CCMGSHandler* pCMGSHandler = new CCMGSHandler();
	pCMGSHandler->SetParentCommand( this );
	m_pCMGSHandler.reset( pCMGSHandler );
	
	// Create default CMS Error Handler
	CCMSErrorHandler* pErrorHandler = new CCMSErrorHandler();
	pErrorHandler->SetParentCommand( this );
	m_pCMSErrorHandler.reset( pErrorHandler );
}


////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Metatype Declarations
//
Q_DECLARE_METATYPE( se::t_ATCommandPtr );
////////////////////////////////////////////////////////////////////////////////


#endif // SE_AT_COMMANDS_H
