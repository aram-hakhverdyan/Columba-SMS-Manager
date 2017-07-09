////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_atbasicparser.h"
//
//	Qt Includes
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin    
////////////////////////////////////////////////////////////////////////////////

//
//	class CATBasicParser
//
CATBasicParser::~CATBasicParser()
{
	if (m_bExtOwner && m_pExtension)
		delete m_pExtension;
	m_pExtension = nullptr;

	QList<SHandlerInfo> lstHandlers = m_hHandlers.values();
	for (int i = 0; i < lstHandlers.size(); ++i)
	{
		SHandlerInfo oHandler = lstHandlers[i];
		if (oHandler.bOwner && oHandler.pHandler)
		{
			delete oHandler.pHandler;
			oHandler.pHandler = nullptr;
		}
	}
}

// Parse
t_ATResponceSharedPtr CATBasicParser::Parse( QStringList& lstATData,
											 EParsingStatus* peStatus )
{
	if (lstATData.isEmpty())
	{
		 if( peStatus )
			 *peStatus = EParsingStatus::Processed;
		 return t_ATResponceSharedPtr();
	}

	t_ATResponceSharedPtr pResponce;
	EParsingStatus eStatus = EParsingStatus::Unrecognised;
	// Provide More Data
	for (int i = 0; i < m_lstMoreDataRequestors.size(); ++i)
	{
		IATResponceHandler* pCurrentHandler = m_lstMoreDataRequestors[i];
		SE_ASSERT( pCurrentHandler );
		try
		{
			pResponce = TryHandle( pCurrentHandler, lstATData, eStatus );
			if (eStatus == EParsingStatus::Processed)
			{
				m_lstMoreDataRequestors.removeAt( i );
				// More Data Is Provided
				return pResponce;
			}
		}
		catch (std::exception const& oErr)
		{
			// Failed to process more data so remove frome list
			m_lstMoreDataRequestors.removeAt( i );
			// rethrow
			throw oErr;
		}
		//catch (...)
		//{
		//	// Failed to process more data so remove frome list
		//	m_lstMoreDataRequestors.removeAt( i );
		//}
	}

	if (m_lstKeyLength.isEmpty())
	{
		if (peStatus)
			*peStatus = EParsingStatus::Unrecognised;
		return t_ATResponceSharedPtr();
	}

	eStatus = EParsingStatus::Unrecognised;
	int nCount = m_lstKeyLength.size();
	QString sATData = lstATData.first();
	for (int i = 0; i < nCount; ++i)
	{
		int nCurrentKeyLength = m_lstKeyLength[i];
		SE_ASSERT( nCurrentKeyLength > 0 );
		if (sATData.size() < nCurrentKeyLength)
			continue;
		QString sKey = sATData.left( nCurrentKeyLength );
		SHandlerInfo oHandler = m_hHandlers.value( sKey );
		IATResponceHandler* pHandler = oHandler.pHandler;
		if (!pHandler)
			continue;
		
		//int nATPointCount = pHandler->GetRequiredATPointCount(); // Firs is already taken
		//int nRequiredCount = nATPointCount <= lstATData.size() ? nATPointCount : lstATData.size();
		//for (int j = 1; j < nRequiredCount; ++j)
		//	sATData.append( "\r\n" + lstATData[j] );

		pResponce = TryHandle( pHandler, lstATData, eStatus );
		
		break;
	}

	// Check Extension
	if (eStatus == EParsingStatus::Unrecognised && m_pExtension)
		return m_pExtension->Parse( lstATData, peStatus );

	if (peStatus)
		*peStatus = eStatus;
	return pResponce;
}

// TryHandle
t_ATResponceSharedPtr CATBasicParser::TryHandle( IATResponceHandler* pHandler, QStringList& lstATData, EParsingStatus& eStatus )
{
	SE_ASSERT( pHandler );
	SE_ASSERT( !lstATData.isEmpty() );

	t_ATResponceSharedPtr pResult;
	EParsingStatus eHandlingStatus;
	QString sATData = lstATData.first();
	//////////////////////////////////////////////////////////////////////////////////////////
	LOG( "Processing: " + sATData + ":::: Handler Key: " + pHandler->GetKey() );
	//////////////////////////////////////////////////////////////////////////////////////////

	
	pResult = pHandler->Handle( sATData, eHandlingStatus );
	while (eHandlingStatus == EParsingStatus::NeedMoreData)
	{
		// Remove processed items from AT data list
		lstATData.removeFirst();
		if (!lstATData.isEmpty())
		{
			//////////////////////////////////////////////////////////////////////////////////////////
			LOG( "Provideing More Data To AT " + (pResult ? ToString( pResult->GetMajorType() ) : "") + ": " + lstATData.first() + ":::: Handler Key: " + pHandler->GetKey() );
			//////////////////////////////////////////////////////////////////////////////////////////

			sATData = lstATData.first();
			pResult = pHandler->Handle( sATData, eHandlingStatus );
		}
		else // No data is avilable currently
		{
			//////////////////////////////////////////////////////////////////////////////////////////
			LOG( "No More Data: Waiting:::: Handler Key: " + pHandler->GetKey() );
			//////////////////////////////////////////////////////////////////////////////////////////
			// Add more data requestor
			m_lstMoreDataRequestors.append( pHandler );
			eStatus = EParsingStatus::NeedMoreData;
			break;
		}
	}

	if (eHandlingStatus == EParsingStatus::Processed)
	{
		eStatus = EParsingStatus::Processed;
		// Remove processed items from AT data list
		lstATData.removeFirst();
		//////////////////////////////////////////////////////////////////////////////////////////
		LOG( "AT Event Processing Succeded: " + sATData + ":::: Handler Key: " + pHandler->GetKey() );
		//////////////////////////////////////////////////////////////////////////////////////////
	}
	else if (eHandlingStatus == EParsingStatus::Failed || eHandlingStatus == EParsingStatus::Unrecognised)
	{
		eStatus = EParsingStatus::Failed /*Internal Error*/;
		//////////////////////////////////////////////////////////////////////////////////////////
		LOG( "AT Event Processing Failed: " + sATData + ":::: Handler Key: " + pHandler->GetKey() );
		//////////////////////////////////////////////////////////////////////////////////////////
	}
	return pResult;
}

// AddHandler
void CATBasicParser::AddHandler( IATResponceHandler* pHandler, bool bOwner )
{
	if (!pHandler)
		return;
	QString sKey = pHandler->GetKey();
	SE_ASSERT( !sKey.isEmpty() );
	if (sKey.isEmpty())
		return;
	SHandlerInfo oHandler;
	oHandler.pHandler = pHandler;
	oHandler.bOwner = bOwner;

	m_hHandlers.insert( sKey, oHandler );
	int nKeyLength = sKey.size();
	InsertKeyLength( nKeyLength );
}

// Extend
void CATBasicParser::Extend( IATParser* pExtension, bool bOwner )
{
	if (m_bExtOwner && m_pExtension)
		delete m_pExtension;
	m_pExtension = pExtension;
	m_bExtOwner = bOwner;
}

void CATBasicParser::Clear()
{

	QList<SHandlerInfo> lstHandlers = m_hHandlers.values();
	for (int i = 0; i < lstHandlers.size(); ++i)
	{
		SHandlerInfo oHandler = lstHandlers[i];
		if (oHandler.bOwner && oHandler.pHandler)
		{
			delete oHandler.pHandler;
			oHandler.pHandler = nullptr;
		}
	}

	m_hHandlers.clear();
	m_lstKeyLength.clear();
}

// InsertKeyLength
void CATBasicParser::InsertKeyLength( int nLength )
{
	m_lstKeyLength.append( nLength );
}


//
//	class CATEventParser
//
// InsertKeyLength
void CATEventParser::InsertKeyLength( int nLength )
{
	if (Base::KeyLengths().isEmpty())
		Base::KeyLengths().append( nLength );
	else
	{
		int i = 0;
		// add length if not exists by kepping acending sorting order
		//! should be optimized using binary search logistics
		QList<int>& lstKeyLengths = Base::KeyLengths();
		while (i < lstKeyLengths.size() && nLength > lstKeyLengths.at( i ))
			++i;
		if (i == lstKeyLengths.size())
			lstKeyLengths.append( nLength );
		else if( nLength < lstKeyLengths.at( i ))
			lstKeyLengths.insert( i, nLength );
	}
}

//
//	class CATCommandResponceParser
//
// SetListeningATCommand
void CATCommandResponceParser::SetListeningATCommand( t_ATCommandPtr pATCommand )
{
	Base::Clear();
	if (pATCommand)
	{
		QList<IATResponceHandler*> lstHandlers = pATCommand->GetResponceHandler();
		for (IATResponceHandler* pHandler : lstHandlers)
			Base::AddHandler( pHandler );
	}
}
////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////
