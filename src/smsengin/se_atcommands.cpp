////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_atcommands.h"
#include "se_atcommandresponcehandler.h"
//
//	Qt Includes
//
#include <QMetaType>
////////////////////////////////////////////////////////////////////////////////
							   
////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin    
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CATCommand
//
//! Constructor
CATCommand::CATCommand( QString const& sCommandName,
						bool           bFullCommand,
						QString const& sPostATSymbol,
						QString const& sATDomain )
						: m_sATDomain( sATDomain ),
						  m_sPostATSymbol( sPostATSymbol ),
						  m_sCommandName( sCommandName ),
						  m_bFullCommand( bFullCommand ),
						  m_eRole( ECommandRole::ExecuteCommand ),
						  m_eStatus( ECommandStatus::Undefined )
{
	CDefaultOkHandler* pOkHandler = new CDefaultOkHandler();
	pOkHandler->SetParentCommand( this );
	CDefaultErrorHandler* pErrorHandler = new CDefaultErrorHandler();
	pErrorHandler->SetParentCommand( this );
	m_pDefaultPositiveHandler.reset( pOkHandler );
	m_pDefaultNegativeHandler.reset( pErrorHandler );
}

// GetData
QByteArray CATCommand::GetData() const
{
	if (m_bFullCommand)
	{
		SE_ASSERT( !m_sCommandName.isEmpty() );
		return  m_sCommandName.toLatin1()/*.append( '\r' )*/;
	}

	QByteArray aResult = m_sATDomain.toLatin1();
	if (!m_sCommandName.isEmpty())
	{
		aResult.append( m_sPostATSymbol + m_sCommandName );

		switch (m_eRole)
		{
			case se::ECommandRole::ExecuteCommand:
				{
					aResult.append( '=' );
					QByteArray aArgument = GetArgument().toLatin1();
					SE_ASSERT( !aArgument.isEmpty() );
					aResult.append( aArgument );
					break;
				}
			case se::ECommandRole::eCheckState:
				aResult.append( '?' );
				break;
			case se::ECommandRole::eCheckPossibleValues:
				aResult.append( "=?" );
				break;

			default:
				SE_ASSERT( false );
				break;
		}
	}
	aResult.append( '\r' );
	return aResult;
}


// GetArgument         [Default Implementation]
QString CATCommand::GetArgument() const { return QString(); }
// ProcessResponceData [Default Implementation]

// GetResponceHandler
CATCommand::t_lstHandlers CATCommand::GetResponceHandler()
{
	t_lstHandlers lstHandlers;
	lstHandlers.append( m_pDefaultPositiveHandler.get() );
	lstHandlers.append( m_pDefaultNegativeHandler.get() );
	return lstHandlers;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CMGF
//
// GetArgument
QString CAT_CMGF::GetArgument() const
{
	return QString::number( int( m_eMode ) );
}

// GetResponceHandler
CATCommand::t_lstHandlers CAT_CMGF::GetResponceHandler()
{
	t_lstHandlers lstHanders;
	ECommandRole eRole = Base::GetRole();
	switch (eRole)
	{
		case ECommandRole::ExecuteCommand:
			lstHanders = Base::GetResponceHandler();
			break;
		case ECommandRole::eCheckState:
			lstHanders.append( m_pStateHandler.get() );
			lstHanders.append( m_pDefaultNegativeHandler.get() );
			break;
		/*case ECommandRole::eCheckPossibleValues:
			break;*/
		default:
			SE_ASSERT( false );
			break;
	}
	return lstHanders;
}

//
//	State Handler
//
t_ATResponceSharedPtr CAT_CMGF::CStateHandler::ProcessResponce( QStringList const& lstResponce, 
																EParsingStatus& eStatus )
{
	eStatus = EParsingStatus::Unrecognised;
	if (lstResponce.isEmpty())
		return t_ATResponceSharedPtr();
	QString sState = lstResponce.first();
	bool bOk = false;
	int nState = sState.toInt( &bOk );
	SE_ASSERT( bOk );
	if (!bOk)
		return t_ATResponceSharedPtr();
	CAT_CMGF::t_ResponcePtr pResponce( new C_CMGF_State( nState ) );
	// Set responce if has a parent command
	CAT_CMGF* pAT_CMGF = dynamic_cast<CAT_CMGF*>( GetParentCommand());
	if (pAT_CMGF)
	{
		pAT_CMGF->m_pResponceObject = pResponce;
		pAT_CMGF->SetStatus( ECommandStatus::Succeeded );
	}
	eStatus = EParsingStatus::Processed;
	return pResponce;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CSCA
//
// GetArgument
//QString CAT_CSCA::GetArgument() const
//{
//	return QString::number( int( m_eMode ) );
//}

// GetResponceHandler
CATCommand::t_lstHandlers CAT_CSCA::GetResponceHandler()
{
	t_lstHandlers lstHanders;
	ECommandRole eRole = Base::GetRole();
	switch (eRole)
	{
		case ECommandRole::ExecuteCommand:
			SE_ASSERT( false );
			lstHanders = Base::GetResponceHandler();
			break;
		case ECommandRole::eCheckState:
			lstHanders.append( m_pStateHandler.get() );
			lstHanders.append( m_pDefaultNegativeHandler.get() );
			break;
			/*case ECommandRole::eCheckPossibleValues:
			break;*/
		default:
			SE_ASSERT( false );
			break;
	}
	return lstHanders;
}

//
//	State Handler
//
t_ATResponceSharedPtr CAT_CSCA::CStateHandler::ProcessResponce( QStringList const& lstResponce,
																EParsingStatus& eStatus )
{
	eStatus = EParsingStatus::Unrecognised;
	if (lstResponce.isEmpty())
		return t_ATResponceSharedPtr();
	
	// Parse SMSC Number
	QString sSMSCNumber;
	QString sDirtySMSC = lstResponce.first();
	if (!sDirtySMSC.isEmpty())
	{
		// Remove ""s
		if (sDirtySMSC.at( 0 ) == '"')
			sDirtySMSC.remove( 0, 1 );
		int nDirtySize = sDirtySMSC.size();
		if ( !sDirtySMSC.isEmpty() && sDirtySMSC.at( nDirtySize - 1 ) == '"')
			sDirtySMSC.remove( nDirtySize - 1, 1 );

		sSMSCNumber = sDirtySMSC;
	}
	
	// Parse SMSC Value
	int nSMSCValue = -1;
	if (lstResponce.size() >= 2)
	{
		QString sSMSCValue = lstResponce[1];
		bool bOk = false;
		int nValue = sSMSCValue.toInt( &bOk );
		if (bOk)
			nSMSCValue = nValue;
	}

	CAT_CSCA::t_StateDataPtr pResponce( new C_CSCA_State( sSMSCNumber, nSMSCValue ) );
	// Set responce if has a parent command
	
	CAT_CSCA* pAT_CSCA = dynamic_cast<CAT_CSCA*>(GetParentCommand());
	if (pAT_CSCA)
	{
		pAT_CSCA->m_pResponceObject = pResponce;
		pAT_CSCA->SetStatus( ECommandStatus::Succeeded );
	}

	eStatus = EParsingStatus::Processed;
	return pResponce;
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CNMI
//
// GetArgument
QString CAT_CNMI::GetArgument() const
{
	return m_sSettings;
}

// GetResponceHandler
CATCommand::t_lstHandlers CAT_CNMI::GetResponceHandler()
{
	t_lstHandlers lstHanders;
	ECommandRole eRole = Base::GetRole();
	switch (eRole)
	{
		case ECommandRole::ExecuteCommand:
			lstHanders = Base::GetResponceHandler();
			break;
		case ECommandRole::eCheckState:
			lstHanders.append( m_pStateHandler.get() );
			lstHanders.append( m_pDefaultNegativeHandler.get() );
			break;
		/*case ECommandRole::eCheckPossibleValues:
			break;*/
		default:
			SE_ASSERT( false );
			break;
	}
	return lstHanders;
}

//
//	State Handler
//
t_ATResponceSharedPtr CAT_CNMI::CStateHandler::ProcessResponce( QStringList const& lstResponce, 
																EParsingStatus& eStatus )
{
	eStatus = EParsingStatus::Unrecognised;
	if (!m_bWaitForOk)
	{
		SE_ASSERT( lstResponce.size() >= 5 );
		if (lstResponce.size() < 5)
			return t_ATResponceSharedPtr();

		bool bOk = false;
		// Fetch Mode
		int nMode = lstResponce[0].toInt( &bOk );
		SE_ASSERT( bOk );
		if (!bOk)
			return t_ATResponceSharedPtr();
		// Fetch MT
		int nMT = lstResponce[1].toInt( &bOk );
		SE_ASSERT( bOk );
		if (!bOk)
			return t_ATResponceSharedPtr();
		// Fetch BM
		int nBM = lstResponce[2].toInt( &bOk );
		SE_ASSERT( bOk );
		if (!bOk)
			return t_ATResponceSharedPtr();
		// Fetch DS
		int nDS = lstResponce[3].toInt( &bOk );
		SE_ASSERT( bOk );
		if (!bOk)
			return t_ATResponceSharedPtr();
		// Fetch BFR
		int nBFR = lstResponce[4].toInt( &bOk );
		SE_ASSERT( bOk );
		if (!bOk)
			return t_ATResponceSharedPtr();

		m_pResponce.reset( new C_CNMI_State( nMode, nMT, nBM, nDS, nBFR ) );
		// Set responce if has a parent command
		CAT_CNMI* pAT_CNMI = dynamic_cast<CAT_CNMI*>(GetParentCommand());
		if (pAT_CNMI)
		{
			pAT_CNMI->m_pResponceObject = m_pResponce;
			pAT_CNMI->SetStatus( ECommandStatus::Succeeded );
		}
		eStatus = EParsingStatus::NeedMoreData;
		m_bWaitForOk = true;
		return nullptr;
	}
	else
	{
		if (lstResponce.size() != 1)
		{
			eStatus = EParsingStatus::Failed;
			return nullptr;
		}
		QString sOk = lstResponce.first();
		if (sOk.trimmed() != "OK")
		{
			eStatus = EParsingStatus::Failed;
			return nullptr;
		}
		eStatus = EParsingStatus::Processed;
		CATCommand* pParent = GetParentCommand();
		if (pParent)
			pParent->SetStatus( ECommandStatus::Succeeded );
		// Ok is read
		m_bWaitForOk = false;
		return m_pResponce;
	}
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CAT_CMGS_Step1
//
// GetArgument
QString CAT_CMGS_Step1::GetArgument() const
{
	if (GetWorkingMode() == ESmsMode::eTextMode)
		return QString( "\"%1\"" ).arg( m_sPhoneNumber );
	else // PDU Mode
		return QString::number( m_nLength );
}

// GetResponceHandler
CATCommand::t_lstHandlers CAT_CMGS_Step1::GetResponceHandler()
{
	ECommandRole eRole = Base::GetRole();
	switch (eRole)
	{
		case ECommandRole::ExecuteCommand:
			return t_lstHandlers() << m_pHandler.get() << m_pCMSErrorHandler.get();
			/*case ECommandRole::eCheckState:
			break;
			case ECommandRole::eCheckPossibleValues:
			break;*/
		default:
			SE_ASSERT( false );
			return t_lstHandlers();
	}
}

//
//	Handlers
//
t_ATResponceSharedPtr CAT_CMGS_Step1::CFirstStageHandler::Handle( QString sData, 
																  EParsingStatus& eStatus )
{
	if (sData.trimmed() != ">")
	{
		eStatus = EParsingStatus::Unrecognised;
		return t_ATResponceSharedPtr();
	}
	eStatus = EParsingStatus::Processed;
	CATCommand* pParent = GetParentCommand();
	if (pParent)
		pParent->SetStatus( ECommandStatus::Succeeded );
	return t_ATResponceSharedPtr( new CEmptyResponce() );
}


// ProcessResponce
t_ATResponceSharedPtr CAT_CMGS_Step2::CCMGSHandler::ProcessResponce( QStringList const& lstResponce,
																	 EParsingStatus& eStatus )
{
	if (lstResponce.size() != 1)
	{
		eStatus = EParsingStatus::Unrecognised;
		return t_ATResponceSharedPtr();
	}
	
	if (!m_bWaitForOk)
	{
		QString sVal = lstResponce.first();
		bool bOk;
		int nVal = sVal.toInt( &bOk );
		if (!bOk)
		{
			eStatus = EParsingStatus::Unrecognised;
			return t_ATResponceSharedPtr();
		}
		m_pResponce.reset( new C_CMGS_Responce( nVal ) );
		eStatus = EParsingStatus::NeedMoreData;
		m_bWaitForOk = true;
		return t_ATResponceSharedPtr();
	}
	else
	{
		QString sOk = lstResponce.first( );
		if (sOk.trimmed( ) != "OK")
		{
			eStatus = EParsingStatus::Unrecognised;
			return nullptr;
		}
		eStatus = EParsingStatus::Processed;
		CAT_CMGS_Step2* pParent = dynamic_cast<CAT_CMGS_Step2*>(GetParentCommand( ));
		if (pParent)
		{
			pParent->SetStatus( ECommandStatus::Succeeded );
			pParent->m_pResponce = this->m_pResponce;
		}
		// Ok is read
		m_bWaitForOk = false;
		return m_pResponce;
	}
}

CATCommand::t_lstHandlers CAT_CMGS_Step2::GetResponceHandler( )
{
	ECommandRole eRole = Base::GetRole( );
	switch (eRole)
	{
	case ECommandRole::ExecuteCommand:
		return t_lstHandlers() << m_pCMGSHandler.get() << m_pCMSErrorHandler.get();
		/*case ECommandRole::eCheckState:
		break;
		case ECommandRole::eCheckPossibleValues:
		break;*/
	default:
		SE_ASSERT( false );
		return t_lstHandlers( );
	}
}

CAT_CMGS_Step2::t_ResponcePtr CAT_CMGS_Step2::GetResponce( ) const
{
	return m_pResponce;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSmsMessageInitializer
//! Decleares QMetaType for SMS Message class
//class CATCommandInitializer
//{
//public:
//	CATCommandInitializer() { qRegisterMetaType<t_ATCommandPtr>("se::t_ATCommandPtr"); }
//};
//static CATCommandInitializer soCommandInitializer;
////////////////////////////////////////////////////////////////////////////////

#define SE_REGISTER_META_TYPE( _type_ )  class CInit_##_type_	{			\
public:																		\
		 CInit_##_type_() { qRegisterMetaType<_type_>( "se::##_type_" ); }	\
};																			\

SE_REGISTER_META_TYPE(t_ATCommandPtr);


////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////
