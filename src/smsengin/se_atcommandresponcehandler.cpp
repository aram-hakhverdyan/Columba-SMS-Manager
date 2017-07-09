////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_atcommandresponcehandler.h"
#include "se_responceobjects.h"
#include "se_atcommands.h"
//
//	Qt Includes
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin    
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 
//	class CATCommandResponceHandler
//
// GetRole
ECommandRole CATCommandResponceHandler::GetRole() const
{
	if (m_pATCommand)
		return m_pATCommand->GetRole();
	return ECommandRole::ExecuteCommand;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 
//	class CDefaultOkHandler
//
//! Handle
t_ATResponceSharedPtr CDefaultOkHandler::Handle( QString sData, EParsingStatus& eStatus )
{
	t_ATResponceSharedPtr pResult;
	QString sTrimmedData = sData.trimmed();
	if (sTrimmedData == t_csz("OK"))
	{
		pResult.reset( new CDefaultOK() );
		if (m_pATCommand)
			m_pATCommand->SetStatus( ECommandStatus::Succeeded );
		eStatus = EParsingStatus::Processed;
	}
	else
		eStatus = EParsingStatus::Unrecognised;
	return pResult;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 
//	class CDefaultErrorHandler
//
//! Handle
t_ATResponceSharedPtr CDefaultErrorHandler::Handle( QString sData, EParsingStatus& eStatus )
{
	t_ATResponceSharedPtr pResult;
	QString sTrimmedData = sData.trimmed();
	if (sTrimmedData == t_csz( "ERROR" ))
	{
		pResult.reset( new CDefaultError() );
		if (m_pATCommand)
			m_pATCommand->SetStatus( ECommandStatus::Failed );
		eStatus = EParsingStatus::Processed;
	}
	else
		eStatus = EParsingStatus::Unrecognised;
	return pResult;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// 
//	class CDefaultResponceHandler
//
//! Handle
t_ATResponceSharedPtr CDefaultResponceHandler::Handle( QString sData, EParsingStatus& eStatus )
{
	SE_ASSERT( !sData.isEmpty() );
	if (sData.isEmpty())
		return t_ATResponceSharedPtr();
	// Remove " : "
	int nIdx = sData.indexOf( ":" );
	sData.remove( 0, nIdx + 1 );
	// Remove whitespaces
	sData = sData.trimmed();
	QStringList lstData = sData.split( ',' );
	t_ATResponceSharedPtr pResponce = ProcessResponce( lstData, eStatus );
	return pResponce;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 
//	class CCMSErrorHandler
//

// CRssiEventHandler
CCMSErrorHandler::CCMSErrorHandler()
	: Base( "CMS ERROR" )
{}

// ProcessResponce [+CMS ERROR]
t_ATResponceSharedPtr CCMSErrorHandler::ProcessResponce( QStringList const& lstResponce,
														 EParsingStatus& eStatus )
{
	/*if (lstResponce.size() != 1)
	{
		eStatus = EParsingStatus::Unrecognised;
		return t_ATResponceSharedPtr();
	}*/
	std::shared_ptr<CCMSErrorCode> pResponce;
	QString sData = lstResponce.first();
	bool bOk = true;
	int nErrorCode = sData.toInt( &bOk );
	if (bOk)
	{
		pResponce.reset( new CCMSErrorCode( nErrorCode ) );
	}

	CATCommand* pParent = GetParentCommand();
	if (pParent)
	{
		pParent->SetStatus( ECommandStatus::Failed );
		
		// if the parent is CMGS then store Error code
		CAT_CMGS_Step1* pCMGSParent1 = dynamic_cast<CAT_CMGS_Step1*>(GetParentCommand());
		if (pCMGSParent1)
			pCMGSParent1->m_pCMSErrorCode = pResponce;
		CAT_CMGS_Step2* pCMGSParent2 = dynamic_cast<CAT_CMGS_Step2*>(GetParentCommand());
		if (pCMGSParent2)
			pCMGSParent2->m_pCMSErrorCode = pResponce;
	}

	eStatus = EParsingStatus::Processed;
	return pResponce;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////
