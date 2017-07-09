#ifndef PB_CONTACT_H
#define PB_CONTACT_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "pb_phonenumber.h"
// Qt
#include <QString>
#include <QPixmap>
// STD
#include <memory>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CContactID
//
typedef int t_ContactID;
typedef QList<t_ContactID> t_ContactIdList;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
namespace pb { // Phone Book
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CContactHeader
//
class CContactHeader
{
public:
	// Constructor & Destructor
	inline CContactHeader();
	inline CContactHeader( QString sFirstName,
						   QString sLastName,
						   QPixmap oIcon = QPixmap(),
						   int	   nID = -1 );
	~CContactHeader() = default;

public:
	// 
	//	Main Interface
	//
	inline void SetFirstName( QString const& sFirstName );
	inline QString GetFirstName() const;

	inline void SetLastName( QString const& sLastName );
	inline QString GetLastName() const;

	inline void SetIcon( QPixmap const& oIcon );
	inline QPixmap GetIcon() const;

	inline void SetID( t_ContactID nID );
	inline t_ContactID  GetID() const;

	inline bool IsEmpty() const;
	inline bool IsValid() const;
	inline void Clear();


private:
	//
	//	Content
	//
	QString m_sFirstName;
	QString m_sLastName;
	QPixmap m_oIcon;
	t_ContactID		m_nID;
};
typedef std::shared_ptr<CContactHeader>			t_ContactHeaderSPtr;
typedef QMap<t_ContactID, pb::CContactHeader>	t_IdToContactHeaderMap;
typedef QList<pb::CContactHeader>				t_ContactHeaderList;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	enum EPhoneType
//
enum class EPhoneType
{
	Undefined = 0,
	Mobile,
	Home,
	Work,
	Other
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CPhoneNumberEntry
//! pair of phone number and phone type
class CPhoneNumberEntry
{
public:
	inline CPhoneNumberEntry( CPhoneNumber const&	oPhoneNumber, 
							  EPhoneType			ePhoneType 
							 /* int					nID = -1*/ );
	inline ~CPhoneNumberEntry() = default;

public:
	//
	//	Accessors
	//
	// ID
	/*inline void SetID( int nID );
	inline int  GetID() const;*/
	// Phone Number
	inline CPhoneNumber& PhoneNumber();
	inline CPhoneNumber const& PhoneNumber() const;
	// Phone Type
	inline EPhoneType& PhoneType();
	inline EPhoneType const& PhoneType() const;

private:
	//
	//	Content
	//
	// Entry ID. 
	// Note: In current implementation this is the ID from PhoneBookDetiles table
	//int				m_nID; 
	CPhoneNumber	m_oPhoneNumber;
	EPhoneType		m_ePhoneType;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CPhoneNumberEntryList
//
typedef QList<CPhoneNumberEntry> t_PhoneNumberEntryList;
//class CPhoneNumberEntryList
//{
//public:
//	// Constructor & Destructor
//	inline CPhoneNumberEntryList();
//	~CPhoneNumberEntryList() = default;
//
//public:
//	// 
//	//	Main Interface
//	//
//	// Phone Number Entry Modifiers
//	inline void AddEntry( CPhoneNumberEntry const& strEntry );
//	inline void AddEntry( CPhoneNumber const& oPhoneNumber, EPhoneType ePhoneType );
//	inline void RemoveEntryAt( int nIndex );
//	inline void Clear() const;
//	// Phone Number Entry Accessors
//	inline CPhoneNumber&		EntryAt( int nIndex );
//	inline CPhoneNumber const&	EntryAt( int nIndex ) const;
//	inline int					GetEntryCount() const;
//	inline bool					IsEmpty() const;
//
//private:
//	//
//	//	Content
//	//
//	QList<CPhoneNumberEntry> m_lstEntries;
//};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CContact
//
class CContact
{
public:
	// Constructor & Destructor
	inline CContact();
	explicit inline CContact( CContactHeader const& oHeader, 
					 t_PhoneNumberEntryList const& lstEntries = t_PhoneNumberEntryList() );
	
	inline CContact( QString sFirstName,
					 QString sLastName,
					 QPixmap oIcon = QPixmap(),
					 int	   nID = -1,
					 t_PhoneNumberEntryList const& lstEntries = t_PhoneNumberEntryList() );

	~CContact() = default;

public:
	// Static Creator
	static std::shared_ptr<CContact> Create( QString sFirstName,
											 QString sLastName,
											 QPixmap oIcon = QPixmap(),
											 int	 nID = -1,
											 t_PhoneNumberEntryList const& lstEntries = t_PhoneNumberEntryList() );

public:
	//
	//	Main Interface
	//	Get/Set Phone number entries
	//
	inline t_PhoneNumberEntryList GetPhoneNumberEntryList() const;
	inline void SetPhoneNumberEntryList( t_PhoneNumberEntryList const& lstEntries );
	//! Phone Number Entry List Methods for the comfort use
	// Phone Number Entry Modifiers
	inline void AddPhoneNumberEntry( CPhoneNumberEntry const& strEntry );
	inline void AddPhoneNumberEntry( CPhoneNumber const& oPhoneNumber, 
									 EPhoneType ePhoneType = EPhoneType::Mobile
									/* int nEntryID = -1*/ );
	inline void RemovePhoneNumberEntryAt( int nIndex );
	// Phone Number Entry Accessors
	inline CPhoneNumberEntry& PhoneNumberEntryAt( int nIndex );
	inline CPhoneNumberEntry const& PhoneNumberEntryAt( int nIndex ) const;
	inline int	GetPhoneNumberEntryCount() const;
	//
	//	Get/Set Header Information
	//
	inline void SetHeader( CContactHeader const& oHeader );
	// Returns the copy of the header
	inline CContactHeader GetHeader() const;
	// Ref accessors
	inline CContactHeader& Header();
	inline CContactHeader const& Header() const;

	//! Contact Heder Methods For The Comfort Use
	// Get/Set ID
	//! Note: The contact's ID is the same as contact header's ID
	inline void SetID( t_ContactID nID );
	inline t_ContactID  GetID() const;
	// Get/Set First Name
	inline void SetFirstName( QString const& sFirstName );
	inline QString GetFirstName() const;
	// Get/Set Last Name
	inline void SetLastName( QString const& sLastName );
	inline QString GetLastName() const;
	// Get/Set Icon
	inline void SetIcon( QPixmap const& oIcon );
	inline QPixmap GetIcon() const;
	// Empty State
	inline bool IsEmpty() const;
	inline bool IsValid() const;
	inline void Clear();

private:
	//
	//	Content
	//
	CContactHeader			m_oHeader;
	t_PhoneNumberEntryList	m_lstPhoneNumberEntries;
};
typedef std::shared_ptr<CContact> t_ContactSPtr;
typedef QMap<t_ContactID, pb::CContact>			t_IdToContactMap;
typedef QList<pb::CContact>						t_ContactList;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
//
//	CContactHeader
//
inline CContactHeader::CContactHeader()
	: m_nID( -1 )
{
}

inline CContactHeader::CContactHeader( QString sFirstName,
									   QString sLastName,
									   QPixmap oIcon,
									   int	   nID )
									   : m_sFirstName( sFirstName ),
									   m_sLastName( sLastName ),
									   m_oIcon( oIcon ),
									   m_nID( nID )
{
}

inline void CContactHeader::SetFirstName( QString const& sFirstName )	{ m_sFirstName = sFirstName; }
inline QString CContactHeader::GetFirstName() const						{ return m_sFirstName; }

inline void CContactHeader::SetLastName( QString const& sLastName )		{ m_sLastName = sLastName; }
inline QString CContactHeader::GetLastName() const						{ return m_sLastName; }

inline void CContactHeader::SetIcon( QPixmap const& oIcon )				{ m_oIcon = oIcon; }
inline QPixmap CContactHeader::GetIcon() const							{ return m_oIcon; }

inline void CContactHeader::SetID( t_ContactID nID ) { m_nID = nID; }
inline t_ContactID  CContactHeader::GetID()	const { return m_nID; }

inline bool CContactHeader::IsEmpty() const								{ return m_sFirstName.isEmpty() && m_sLastName.isEmpty(); }
inline bool CContactHeader::IsValid() const								{ return !m_sFirstName.isEmpty(); }

inline void CContactHeader::Clear() { m_nID = -1; m_sFirstName.clear(); m_sLastName.clear(); }



//
//	CPhoneNumberEntry
//
inline CPhoneNumberEntry::CPhoneNumberEntry( CPhoneNumber const& oPhoneNumber, EPhoneType ePhoneType/*, int nID*/ )
	: //m_nID( nID ),
	m_oPhoneNumber( oPhoneNumber ),
	m_ePhoneType( ePhoneType )
{}
// Phone Number
inline CPhoneNumber& CPhoneNumberEntry::PhoneNumber() { return m_oPhoneNumber; }
inline CPhoneNumber const& CPhoneNumberEntry::PhoneNumber() const { return m_oPhoneNumber; }
// Phone Type
inline EPhoneType& CPhoneNumberEntry::PhoneType() { return m_ePhoneType; }
inline EPhoneType const& CPhoneNumberEntry::PhoneType() const { return m_ePhoneType; }


//inline void CPhoneNumberEntry::SetID( int nID ) { m_nID = nID; }
//inline int  CPhoneNumberEntry::GetID() const { return m_nID; };

//
//	CContact
//
//! Default Constructor
inline CContact::CContact() {}
inline CContact::CContact( CContactHeader const& oHeader,
				 t_PhoneNumberEntryList const& lstEntries )
				 : m_oHeader( oHeader ),
				   m_lstPhoneNumberEntries( lstEntries )
{}


inline CContact::CContact( QString sFirstName,
				 QString sLastName,
				 QPixmap oIcon,
				 int	   nID,
				 t_PhoneNumberEntryList const& lstEntries )
				 : m_oHeader( sFirstName, sLastName, oIcon, nID ),
				 m_lstPhoneNumberEntries( lstEntries )
{}

inline t_ContactSPtr CContact::Create( QString sFirstName,
											 QString sLastName,
											 QPixmap oIcon,
											 int	   nID,
											 t_PhoneNumberEntryList const& lstEntries )
{
	return t_ContactSPtr( new CContact( sFirstName, sLastName, oIcon, nID, lstEntries ) );
}

inline t_PhoneNumberEntryList CContact::GetPhoneNumberEntryList() const									
{ return m_lstPhoneNumberEntries; }
inline void CContact::SetPhoneNumberEntryList( t_PhoneNumberEntryList const& lstEntries )				
{ m_lstPhoneNumberEntries = lstEntries; }
	//! Phone Number Entry List Methods for the comfort use
	// Phone Number Entry Modifiers
inline void CContact::AddPhoneNumberEntry( CPhoneNumberEntry const& strEntry )							
{ m_lstPhoneNumberEntries.append( strEntry ); }
inline void CContact::AddPhoneNumberEntry( CPhoneNumber const& oPhoneNumber, EPhoneType ePhoneType/*, int nEntryID*/ )
{ AddPhoneNumberEntry( CPhoneNumberEntry( oPhoneNumber, ePhoneType/*, nEntryID*/ ) ); }
inline void CContact::RemovePhoneNumberEntryAt( int nIndex )
{ m_lstPhoneNumberEntries.removeAt( nIndex ); }
	// Phone Number Entry Accessors
inline CPhoneNumberEntry& CContact::PhoneNumberEntryAt( int nIndex ) { return m_lstPhoneNumberEntries[nIndex]; }
inline CPhoneNumberEntry const& CContact::PhoneNumberEntryAt( int nIndex ) const { return m_lstPhoneNumberEntries.at( nIndex ); }
inline int	CContact::GetPhoneNumberEntryCount() const { return m_lstPhoneNumberEntries.size(); }
	//
	//	Get/Set Header Information
	//
inline void CContact::SetHeader( CContactHeader const& oHeader ) { m_oHeader = oHeader; }
inline CContactHeader CContact::GetHeader() const { return m_oHeader; }
// Header
inline CContactHeader& CContact::Header() { return m_oHeader; }
inline CContactHeader const& CContact::Header() const { return m_oHeader; }
	//! Contact Heder Methods For The Comfort Use
	// Get/Set ID
	//! Note: The contact's ID is the same as contact header's ID
inline void CContact::SetID( int nID ) { m_oHeader.SetID( nID ); }
inline int  CContact::GetID() const { return m_oHeader.GetID(); }
	// Get/Set First Name
inline void CContact::SetFirstName( QString const& sFirstName ) { m_oHeader.SetFirstName( sFirstName ); }
inline QString CContact::GetFirstName() const					{ return m_oHeader.GetFirstName(); }
	// Get/Set Last Name
inline void CContact::SetLastName( QString const& sLastName )	{ m_oHeader.SetLastName( sLastName ); }
inline QString CContact::GetLastName() const					{ return m_oHeader.GetLastName(); }
	// Get/Set Icon
inline void CContact::SetIcon( QPixmap const& oIcon ) { m_oHeader.SetIcon( oIcon ); }
inline QPixmap CContact::GetIcon() const { return m_oHeader.GetIcon(); }
	// Empty State
inline bool CContact::IsEmpty() const { return m_oHeader.IsEmpty() || m_lstPhoneNumberEntries.isEmpty(); }
inline bool CContact::IsValid() const { return m_oHeader.IsValid(); }

inline void CContact::Clear() { m_oHeader.Clear(); m_lstPhoneNumberEntries.clear(); }
////////////////////////////////////////////////////////////////////////////////
} // namespace pb
////////////////////////////////////////////////////////////////////////////////

#endif // PB_CONTACT_H
