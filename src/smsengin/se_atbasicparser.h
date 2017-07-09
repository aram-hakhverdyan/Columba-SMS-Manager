#ifndef SE_AT_BASIC_PARSER_H
#define SE_AT_BASIC_PARSER_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_at_iparser.h"
// 
#include "se_atcommands.h"
//
//	Qt Includes
//
#include <QObject>
#include <QHash>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CATBasicParser
//
class SMSENGINE_EXPORT CATBasicParser : public IATParser
{
public:
	// Constructor & Destructor
	inline CATBasicParser();
	virtual ~CATBasicParser();

public:
	//
	//	Main Interface
	//
	t_ATResponceSharedPtr Parse( QStringList& lstATData,
								 EParsingStatus* peStatus = nullptr )    override;
	void AddHandler( IATResponceHandler* pHandler, bool bOwner = false ) override;
	void Extend( IATParser* pExtension, bool bOwner = false )	override;
	void Clear() override;

protected:
	//
	//	Internal Tools
	//
	virtual void InsertKeyLength( int nLength ); // appends by default
	// Accessors
	inline QList<int>& KeyLengths();
	inline QList<int> const& KeyLengths() const;
	t_ATResponceSharedPtr TryHandle( IATResponceHandler* pHandler,
									 QStringList& lstATData,
									 EParsingStatus& eStatus );

private:
	//
	//	Content
	//
	struct SHandlerInfo
	{
		SHandlerInfo() : pHandler( nullptr ), bOwner( false ) {};

		IATResponceHandler* pHandler;
		bool bOwner;
	};
	QHash<QString, SHandlerInfo>		m_hHandlers;
	QList<int>						    m_lstKeyLength;
	IATParser*							m_pExtension;
	bool								m_bExtOwner;
	QList<IATResponceHandler*>			m_lstMoreDataRequestors;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CATEventParser
//
class SMSENGINE_EXPORT CATEventParser : public CATBasicParser
{
	typedef CATBasicParser Base;

public:
	// Constructor & Destructor
	CATEventParser() = default;
	virtual ~CATEventParser() = default;

protected:
	//
	//	Internal Tools
	//
	virtual void InsertKeyLength( int nLength ); // appends by default
};
////////////////////////////////////////////////////////////////////////////////
typedef std::shared_ptr<CATEventParser> t_ATEventParserSPtr;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CATEventParser
//
class SMSENGINE_EXPORT CATCommandResponceParser : public CATBasicParser
{
	typedef CATBasicParser Base;

public:
	// Constructor & Destructor
	CATCommandResponceParser() = default;
	virtual ~CATCommandResponceParser() = default;

public:
	//
	//	Own Interface
	//
	//! Sets currently listening command. 
	//! Resets handler stuff by specified command's handlers
	void SetListeningATCommand( t_ATCommandPtr pATCommand );
};
////////////////////////////////////////////////////////////////////////////////
typedef std::shared_ptr<CATCommandResponceParser> t_CommandResponceParserSPtr;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
inline CATBasicParser::CATBasicParser()
	: m_pExtension(nullptr),
	  m_bExtOwner( false )
{}
inline QList<int>& CATBasicParser::KeyLengths() { return m_lstKeyLength; }
inline QList<int> const& CATBasicParser::KeyLengths() const { return m_lstKeyLength; }
////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // SE_AT_BASIC_PARSER_H
