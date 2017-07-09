#ifndef SE_AT_CATEGORY_HANDLER_H
#define SE_AT_CATEGORY_HANDLER_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_core.h"
#include "se_atbasicparser.h"
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CBaseicHandler
//
class SMSENGINE_EXPORT CBaseicHandler : public IATResponceHandler
{
public:
	// Constructor & Destructor
	inline   CBaseicHandler( QString const& sKey );
	virtual ~CBaseicHandler() = default;

public:
	//
	//	Overidden Methodes
	//
	QString               GetKey() const  override;
	bool                  HasResponce()   override;
	t_ATResponceUniquePtr TakeResponce();

protected:
	// Safe set responce data
	void SetResponce( CATResponce* pResponce );

private:
	//
	//	Content
	//
	QString m_sKey;
	t_ATResponceUniquePtr m_upResponce;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CCaretGroupHandler
//
class SMSENGINE_EXPORT CATEventHandler : public CBaseicHandler
{
	typedef CBaseicHandler Base;

public:
	// Constructor & Destructor
	inline   CATEventHandler( QString const& sEventKey );
	virtual ~CATEventHandler() = default;

};
////////////////////////////////////////////////////////////////////////////////


//
//	Forward Declaration
//
class CATCommand;
////////////////////////////////////////////////////////////////////////////////
//
//	class CCaretGroupHandler
//
class SMSENGINE_EXPORT CATCommandResponceHandler : public CBaseicHandler
{
	typedef CBaseicHandler Base;

public:
	// Constructor & Destructor
	inline   CATCommandResponceHandler( QString const& sCommandKey );
	virtual ~CATCommandResponceHandler( ) = default;

public:
	//
	//	Main Interface
	//
	void SetParentCommand( CATCommand* pCommand );
	// Returns AT command role
	ECommandRole GetRole() const;

private:
	//
	//	Content
	//
	CATCommand* m_pATCommand;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
//
//	class CATEventHandler
//	
inline CATEventHandler::CATEventHandler( QString const& sEventKey )
	: Base( "^" + sEventKey )
{}
////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // SE_AT_CATEGORY_HANDLER_H
