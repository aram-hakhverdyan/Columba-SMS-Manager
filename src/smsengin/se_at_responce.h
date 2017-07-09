#ifndef SE_AT_RESPONCE_H
#define SE_AT_RESPONCE_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_core.h"
// STD
#include <memory>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CATResponce
//! Base class for all AT responces which store responce data
class SMSENGINE_EXPORT CATResponce
{
public:
	// Constructor & Destructor
	inline CATResponce( EATResponceType eType );
	virtual ~CATResponce() = default;

public:
	//
	//	Main Interface
	//
	inline EATResponceType GetType() const;
	inline EATResponceMajorType GetMajorType() const;

private:
	//
	//	Content
	//
	EATResponceType m_eType;
};
// 
typedef std::unique_ptr<CATResponce>  t_ATResponceUniquePtr;
typedef std::shared_ptr<CATResponce>  t_ATResponceSharedPtr;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CInvalidResponce
//
class SMSENGINE_EXPORT CInvalidResponce : public CATResponce
{
	typedef CATResponce Base;
public:
	// Constructor & Destructor
	inline CInvalidResponce( EATResponceMajorType eMajorType, QString const& sMsg )
		: Base( eMajorType == EATResponceMajorType::ATCommandResponce? 
		  EATResponceType::InvalidCommandResponce : EATResponceType::InvalidEvent ),
		  m_sMessage( sMsg )
	{}
	virtual ~CInvalidResponce() = default;

public:
	//
	//	Main Interface
	// 
	inline QString GetMessage() const { return m_sMessage; }

private:
	//
	//	Content
	//
	QString m_sMessage;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CInvalidCommandResponce
//
class SMSENGINE_EXPORT CInvalidCommandResponce : public CInvalidResponce
{
	typedef CInvalidResponce Base;
public:
	// Constructor & Destructor
	inline CInvalidCommandResponce( QString const& sMsg )
		: Base( EATResponceMajorType::ATCommandResponce, sMsg )
	{}
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CInvalidCommandResponce
//
class SMSENGINE_EXPORT CInvalidEventResponce : public CInvalidResponce
{
	typedef CInvalidResponce Base;
public:
	// Constructor & Destructor
	inline CInvalidEventResponce( QString const& sMsg )
		: Base( EATResponceMajorType::ATEvent, sMsg )
	{}
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CIntValue
//
class SMSENGINE_EXPORT CIntValueResponce : public CATResponce
{
	typedef CATResponce Base;
public:
	// Constructor & Destructor
	inline CIntValueResponce( int nValue, EATResponceType eType )
		: Base( eType ),
		m_nValue( nValue )
	{}
	virtual ~CIntValueResponce() = default;

public:
	//
	//	Main Interface
	// 
	inline int GetValue() const { return m_nValue; }

private:
	//
	//	Content
	//
	int m_nValue;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
//
//	class CATResponce
//
inline CATResponce::CATResponce( EATResponceType eType )
	: m_eType( eType )
{}

// GetType
inline EATResponceType CATResponce::GetType() const
{
	return m_eType;
}

// GetMajorType
inline EATResponceMajorType CATResponce::GetMajorType() const
{
	EATResponceMajorType eMajotType = EATResponceMajorType( quint16( m_eType ) & 0x7 );
	return eMajotType;
}
////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // SE_AT_RESPONCE_H
