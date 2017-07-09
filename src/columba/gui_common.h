#ifndef GUI_COMMON_H
#define GUI_COMMON_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
// Qt
#include <QDateTime>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Enum EBubbleOrientation
//
enum class EBubbleOrientation
{
	Left = 0,
	Right
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CMessageInfo
//
class CMessageInfo
{
public:
	inline CMessageInfo();
	inline CMessageInfo( QString const& sMessage,
						 QDateTime const& oDateTime,
						 bool bIsDelivered,
						 qint64 nID = -1 );
	~CMessageInfo() = default;

public:
	//
	//	Main Interface
	//	
	inline QString GetMessage() const;
	inline void SetMessage( QString const& sMessage );

	inline QDateTime GetDateTime() const;
	inline void SetDateTime( QDateTime const& oDateTime );

	inline bool IsDelivered() const;
	inline void SetDelivered( bool bDelevered = true );

	inline qint64 GetID() const;
	inline void SetID( qint64 nID );

private:
	//
	//	Content
	//	
	QString m_sMessage;
	QDateTime m_oDateTime;
	bool m_bIsDelivered;
	qint64 m_nID;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
inline CMessageInfo::CMessageInfo( QString const& sMessage,
								   QDateTime const& oDateTime,
								   bool bIsDelivered,
								   qint64 nID )
								   : m_sMessage( sMessage ),
								   m_oDateTime( oDateTime ),
								   m_bIsDelivered( bIsDelivered ),
								   m_nID( nID )
{}

inline CMessageInfo::CMessageInfo()
	: m_bIsDelivered( false ),
	  m_nID( -1 )
{}

inline QString CMessageInfo::GetMessage() const
{
	return m_sMessage;
}

inline void CMessageInfo::SetMessage( QString const& sMessage )
{
	m_sMessage = sMessage;
}

inline QDateTime CMessageInfo::GetDateTime() const
{
	return m_oDateTime;
}

inline void CMessageInfo::SetDateTime( QDateTime const& oDateTime )
{
	m_oDateTime = oDateTime;
}

inline bool CMessageInfo::IsDelivered() const
{
	return m_bIsDelivered;
}

inline void CMessageInfo::SetDelivered( bool bDelevered )
{
	m_bIsDelivered = bDelevered;
}

inline qint64 CMessageInfo::GetID() const
{
	return m_nID;
}

inline void CMessageInfo::SetID( qint64 nID )
{
	m_nID = nID;
}

	
////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_COMMON_H
