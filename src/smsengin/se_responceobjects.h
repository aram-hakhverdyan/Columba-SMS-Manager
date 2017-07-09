#ifndef SE_AT_RESPONCE_OBJECTS_H
#define SE_AT_RESPONCE_OBJECTS_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_at_responce.h"
// Qt
#include <QList>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	class CDefaultOK
//
class SMSENGINE_EXPORT CDefaultOK : public CATResponce
{
	typedef CATResponce Base;
public:
	// Constructor & Destructor
	inline CDefaultOK()
		: Base( EATResponceType::DefaultOK )
	{}
	virtual ~CDefaultOK() = default;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CDefaultOK
//
class SMSENGINE_EXPORT CDefaultError : public CATResponce
{
	typedef CATResponce Base;
public:
	// Constructor & Destructor
	inline CDefaultError()
		: Base( EATResponceType::DefaultError )
	{}
	virtual ~CDefaultError() = default;
};
////////////////////////////////////////////////////////////////////////////////

//! Not used
////////////////////////////////////////////////////////////////////////////////
//
//	class CEmptyResponce
//
class SMSENGINE_EXPORT CEmptyResponce : public CATResponce
{
	typedef CATResponce Base;
public:
	// Constructor & Destructor
	inline CEmptyResponce()
		: Base( EATResponceType::EmptyResponce )
	{}
	virtual ~CEmptyResponce() = default;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CCMSErrorCode
//
class SMSENGINE_EXPORT CCMSErrorCode : public CIntValueResponce
{
	typedef CIntValueResponce Base;
public:
	// Constructor & Destructor
	inline CCMSErrorCode( int nErrorCode )
		: Base( nErrorCode, EATResponceType::CMD_CMSError )
	{}
	virtual ~CCMSErrorCode() = default;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class C_CMGF_Responce
//
class SMSENGINE_EXPORT C_CMGF_Responce : public CATResponce
{
	typedef CATResponce Base;
public:
	// Constructor & Destructor
	inline C_CMGF_Responce( ECommandRole eRole = ECommandRole::ExecuteCommand )
		: Base( EATResponceType::CMD_CMGF ),
		  m_eType( eRole )
	{}
	virtual ~C_CMGF_Responce() = default;
public:
	ECommandRole GetType() const { return m_eType; }

private:
	//	Content
	ECommandRole m_eType;
};

class SMSENGINE_EXPORT C_CMGF_State : public C_CMGF_Responce
{
	typedef C_CMGF_Responce Base;
public:
	// Constructor & Destructor
	inline C_CMGF_State( int nMode )
		: Base( ECommandRole::eCheckState ),
		m_eMode( ESmsMode::eUnknown )
	{
		if (nMode == 0)
			m_eMode = ESmsMode::ePDUMode;
		else if (nMode == 1)
			m_eMode = ESmsMode::eTextMode;
		else
		{ SE_ASSERT(false); }
	}
	virtual ~C_CMGF_State() = default;

public:
	//
	//	Main Interface
	//
	inline ESmsMode GetMode() const { return m_eMode; };

private:
	//	Content
	ESmsMode m_eMode;
};

class SMSENGINE_EXPORT C_CMGF_PossibleValues : public C_CMGF_Responce
{
	typedef C_CMGF_Responce Base;
public:
	typedef QList<ESmsMode> lstModes;
public:
	// Constructor & Destructor
	inline C_CMGF_PossibleValues( QList<int> lstSupportedModes )
		: Base( ECommandRole::eCheckPossibleValues )
	{
		for (int i = 0; i < lstSupportedModes.size(); ++i)
			m_eModes.append( ESmsMode( lstSupportedModes[i] ) ); // BAD Solution, but should be enough
	}
	virtual ~C_CMGF_PossibleValues() = default;

public:
	//
	//	Main Interface
	//
	inline lstModes GetSupportedModes() const { return m_eModes; };

private:
	//	Content
	lstModes m_eModes;
};
////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////
//
//	class C_CSCA_State
//
class SMSENGINE_EXPORT C_CSCA_State : public CATResponce
{
	typedef CATResponce Base;
public:
	// Constructor & Destructor
	inline C_CSCA_State( QString const& sSMSCNUmber, int nValue = -1 )
		: Base( EATResponceType::CMD_CSCA ),
		m_SMSCNumber( sSMSCNUmber ),
		m_nValue( nValue )
	{}
	virtual ~C_CSCA_State() = default;
public:
	// Interface
	QString GetServiceCenterNumber() const { return m_SMSCNumber; }
	int GetServiceCenterValue() const { return m_nValue; }

private:
	//	Content
	QString m_SMSCNumber;
	int		m_nValue;
};
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	class C_CMGF_Responce
//
class SMSENGINE_EXPORT C_CNMI_Responce : public CATResponce
{
	typedef CATResponce Base;
public:
	// Constructor & Destructor
	inline C_CNMI_Responce( ECommandRole eRole = ECommandRole::ExecuteCommand )
		: Base( EATResponceType::CMD_CMGF ),
		m_eType( eRole )
	{}
	virtual ~C_CNMI_Responce() = default;
public:
	ECommandRole GetType() const { return m_eType; }

private:
	//	Content
	ECommandRole m_eType;
};

class SMSENGINE_EXPORT C_CNMI_State : public C_CNMI_Responce
{
	typedef C_CNMI_Responce Base;
public:
	// Constructor & Destructor
	inline C_CNMI_State( int nMode, 
						 int nMt, 
						 int nBm, 
						 int nDs, 
						 int nBfr)
		: Base( ECommandRole::eCheckState ),
		m_nMode( nMode ),
		m_nMt( nMt ),
		m_nBm( nBm ),
		m_nDs( nDs ),
		m_nBfr( nBfr )
	{}
	virtual ~C_CNMI_State() = default;

public:
	//
	//	Main Interface
	//
	inline int GetMode() const { return m_nMode; };
	inline int GetMT() const { return m_nMt; };
	inline int GetBM() const { return m_nBm; };
	inline int GetDS() const { return m_nDs; };
	inline int GetBFR() const { return m_nBfr; };

private:
	//	Content
	int m_nMode;
	int m_nMt;
	int m_nBm;
	int m_nDs;
	int m_nBfr;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class C_CMGF_Responce
//
class SMSENGINE_EXPORT C_CMGS_Responce : public CIntValueResponce
{
	typedef CIntValueResponce Base;
public:
	// Constructor & Destructor
	inline C_CMGS_Responce( int nValue )
		: Base( nValue, EATResponceType::CMD_CMGS )
	{}
	virtual ~C_CMGS_Responce( ) = default;
};


////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // SE_AT_RESPONCE_OBJECTS_H
