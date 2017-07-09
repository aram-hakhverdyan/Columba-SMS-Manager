#ifndef SE_AT_COMMAND_RESPONCE_HANDLER_H
#define SE_AT_COMMAND_RESPONCE_HANDLER_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_atbasichandler.h"
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

class CATCommand;
////////////////////////////////////////////////////////////////////////////////
//
//	 class CATCommandResponceHandler
//
class CATCommandResponceHandler : public CBaseicHandler
{
	typedef CBaseicHandler Base;

public:
	// Constructor & Destructor
	inline   CATCommandResponceHandler( QString const& sCommandKey );
	virtual ~CATCommandResponceHandler() = default;

public:
	//
	//	Main Interface
	//
	inline void SetParentCommand( CATCommand* pCommand );

protected:
	// Returns AT command role
	ECommandRole GetRole() const;
	inline CATCommand* GetParentCommand() const;

protected:
	//
	//	Content
	//
	CATCommand* m_pATCommand;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	 class CDefaultOkHandler
//! Handles "OK" responce
class SMSENGINE_EXPORT CDefaultOkHandler : public CATCommandResponceHandler
{
	typedef CATCommandResponceHandler Base;

public:
	// Constructor & Destructor
	inline   CDefaultOkHandler();
	virtual ~CDefaultOkHandler() = default;

public:
	//
	//	Overidden Methods
	//
	t_ATResponceSharedPtr Handle( QString sData, 
								  EParsingStatus& eStatus ) override;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	 class CDefaultErrorHandler
//! Handles "ERROR" responce
class CDefaultErrorHandler : public CATCommandResponceHandler
{
	typedef CATCommandResponceHandler Base;

public:
	// Constructor & Destructor
	inline   CDefaultErrorHandler();
	virtual ~CDefaultErrorHandler() = default;

public:
	//
	//	Overidden Methods
	//
	t_ATResponceSharedPtr Handle( QString sData, 
								  EParsingStatus& eStatus ) override;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	 class CDefaultOkHandler
//! Handles "+<Command Name>"-like responces
class CDefaultResponceHandler : public CATCommandResponceHandler
{
	typedef CATCommandResponceHandler Base;

public:
	// Constructor & Destructor
	inline   CDefaultResponceHandler( QString const& sComandName );
	virtual ~CDefaultResponceHandler() = default;

public:
	//
	//	Overidden Methods
	//
	t_ATResponceSharedPtr Handle( QString sData, 
								  EParsingStatus& eStatus ) override final;

protected:
	//
	//	Polimorphic methods
	//
	// This method should be overriden by derived classes to process AT responce
	//! Note: The lstResponce argument contains comma splitted responce data
	virtual t_ATResponceSharedPtr ProcessResponce( QStringList const& lstResponce, 
												   EParsingStatus& eStatus ) = 0;

};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CCMSErrorHandler
//
class SMSENGINE_EXPORT CCMSErrorHandler : public CDefaultResponceHandler
{
	typedef CDefaultResponceHandler Base;

public:
	// Constructor & Destructor
	CCMSErrorHandler();
	virtual ~CCMSErrorHandler() = default;

public:
	//
	//	Overidden Methods
	//
	t_ATResponceSharedPtr ProcessResponce( QStringList const& lstResponce,
												   EParsingStatus& eStatus ) override;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
//
//	class CATCommandResponceHandler
//	
inline CATCommandResponceHandler::CATCommandResponceHandler( QString const& sEventKey )
	: Base( sEventKey ),
	  m_pATCommand( nullptr )
{}

// SetParentCommand
inline void CATCommandResponceHandler::SetParentCommand( CATCommand* pCommand )
{ m_pATCommand = pCommand; }

// GetParentCommand
inline CATCommand* CATCommandResponceHandler::GetParentCommand() const
{ return m_pATCommand; }

//
//	class CDefaultOkHandler
//	
inline CDefaultOkHandler::CDefaultOkHandler()
	: Base( "OK" )
{}

//
//	class CDefaultErrorHandler
//	
inline CDefaultErrorHandler::CDefaultErrorHandler()
	: Base( "ERROR" )
{}


//
//	class CDefaultResponceHandler
//	
inline CDefaultResponceHandler::CDefaultResponceHandler( QString const& sCommandName )
	: Base( "+" + sCommandName )
{
	SE_ASSERT( !sCommandName.isEmpty() );
}
////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // SE_AT_COMMAND_RESPONCE_HANDLER_H
