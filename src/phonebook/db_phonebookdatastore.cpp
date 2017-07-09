////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "db_phonebookdatastore.h"
#include "se_core_macros.h"
// Qt
#include <QSqlQuery>
#include <QVariant>
#include <QBuffer>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////


//
//	Static member Initaialization
//
const QLatin1String CPhoneBookDataStore::Tables::PhoneBook::TableName = QLatin1String( "phone_book" );
const QLatin1String CPhoneBookDataStore::Tables::PhoneBook::_ID = QLatin1String( "id" );
const QLatin1String CPhoneBookDataStore::Tables::PhoneBook::_FirstName = QLatin1String( "first_name" );
const QLatin1String CPhoneBookDataStore::Tables::PhoneBook::_LastName = QLatin1String( "last_name" );
const QLatin1String CPhoneBookDataStore::Tables::PhoneBook::_Icon = QLatin1String( "icon" );


const QLatin1String CPhoneBookDataStore::Tables::PhoneBookDetails::TableName = QLatin1String( "phone_book_details" );
const QLatin1String CPhoneBookDataStore::Tables::PhoneBookDetails::_ID = QLatin1String( "id" );
const QLatin1String CPhoneBookDataStore::Tables::PhoneBookDetails::_PhoneBookID = QLatin1String( "phone_book_id" );
const QLatin1String CPhoneBookDataStore::Tables::PhoneBookDetails::_PhoneNumber = QLatin1String( "phone_number" );
const QLatin1String CPhoneBookDataStore::Tables::PhoneBookDetails::_Type = QLatin1String( "type" );

////////////////////////////////////////////////////////////////////////////////
//
//	class CPhoneBookDataStore
//
// Initialize
void CPhoneBookDataStore::Initialize()
{
	CREATE_QUERY;
	// Create Table PhoneBook
	EXECUTE_QUERY( QString( "CREATE TABLE IF NOT EXISTS %1 ("
		"%2 INTEGER PRIMARY KEY NOT NULL, "
		"%3 TEXT    NOT NULL, "
		"%4 TEXT    NOT NULL, "
		"%5 BLOB );" )
		.arg( Tables::PhoneBook::TableName )
		.arg( Tables::PhoneBook::_ID )
		.arg( Tables::PhoneBook::_FirstName )
		.arg( Tables::PhoneBook::_LastName )
		.arg( Tables::PhoneBook::_Icon )
		);

	// Create Table PhoneBookDetails
	EXECUTE_QUERY( QString( "CREATE TABLE IF NOT EXISTS %1 ("
		"%2 INTEGER PRIMARY KEY NOT NULL, "
		"%3 INTEGER NOT NULL, "
		"%4 TEXT    NOT NULL, "
		"%5 INTEGER NOT NULL );" )
		.arg( Tables::PhoneBookDetails::TableName )
		.arg( Tables::PhoneBookDetails::_ID )
		.arg( Tables::PhoneBookDetails::_PhoneBookID )
		.arg( Tables::PhoneBookDetails::_PhoneNumber )
		.arg( Tables::PhoneBookDetails::_Type )
		);
}

// Reload
void CPhoneBookDataStore::Reload()
{
	// Clear Cache
	ClearCache();

	DECL_QUERY( oQuery );
	// Fetch whole table PhoneBook
	EXECUTE_QUERY_EX(oQuery, QString( "SELECT * FROM %1;" )
				   .arg( Tables::PhoneBook::TableName ) );
	// Add PhoneBook table content to cache
	while (oQuery.next())
	{
		// Get Data
		int nContactID = oQuery.value( 0 ).toInt();
		QString sFirstName = oQuery.value( 1 ).toString();
		SE_ASSERT( !sFirstName.isEmpty() );
		QString sLastName =  oQuery.value( 2 ).toString();
		// Get image data back from database
		QByteArray aIconBuffer = oQuery.value( 3 ).toByteArray();
		QPixmap oIcon = QPixmap();
		oIcon.loadFromData( aIconBuffer ); 
		// TODO: Take in a count null image case. Default image should be set.

		// Crete contact object
		pb::t_ContactSPtr pContact = pb::CContact::Create( sFirstName, sLastName, oIcon, nContactID );
		SE_ASSERT( pContact );
		
		// Add to cache
		m_hshCache.insert( nContactID, pContact );
	}

	//
	//	Marge phone numbers
	//
	// Fetch whole table PhoneBookDetails
	EXECUTE_QUERY_EX( oQuery, QString( "SELECT * FROM %1;" )
					  .arg( Tables::PhoneBookDetails::TableName ) );
	// Marge phone numbers
	while (oQuery.next())
	{
		// Get Data
		int nPhoneNumberID = oQuery.value( 0 ).toInt();
		int nContactID = oQuery.value( 1 ).toInt();
		QString sPhoneNumber = oQuery.value( 2 ).toString();
		SE_ASSERT( !sPhoneNumber.isEmpty() );
		int nPhoneType = oQuery.value( 3 ).toInt();
		pb::EPhoneType ePhoneType = static_cast<pb::EPhoneType>(nPhoneType);

		// Find contact
		auto it = m_hshCache.find( nContactID );
		if (it != m_hshCache.end())
		{
			pb::t_ContactSPtr pContact = it.value();
			SE_ASSERT( pContact );
			pContact->AddPhoneNumberEntry( pb::CPhoneNumber( sPhoneNumber ), ePhoneType/*, nPhoneNumberID*/ );
		}
	}
	//	
	emit sigReloaded();
}

// ClearCache
void CPhoneBookDataStore::ClearCache()
{
	emit sigAboutToBeCleared();
	m_hshCache.clear();
}

t_ContactHeaderList		CPhoneBookDataStore::GetContactHeadersList() const
{
	t_ContactHeaderList oResult;
	for (auto it = m_hshCache.begin(); it != m_hshCache.end(); ++it)
	{
		pb::t_ContactSPtr pContact = it.value();
		SE_ASSERT( pContact );
		t_ContactID nID = it.key();
		//!-->Can bring performance issues: Should Be Tested
		oResult.append( pContact->Header() );
	}
	return oResult;
}

// GetContactHeaders
t_IdToContactHeaderMap CPhoneBookDataStore::GetContactHeadersMap() const
{
	t_IdToContactHeaderMap oResult;
	for (auto it = m_hshCache.begin(); it != m_hshCache.end(); ++it)
	{
		pb::t_ContactSPtr pContact = it.value();
		SE_ASSERT( pContact );
		t_ContactID nID = it.key();
		//!-->Can bring performance issues: Should Be Tested
		oResult.insert( nID, pContact->Header() );
	}
	return oResult;
}

// GetContactsList
t_ContactList			CPhoneBookDataStore::GetContactsList() const
{
	t_ContactList oResult;
	for (auto it = m_hshCache.begin(); it != m_hshCache.end(); ++it)
	{
		pb::t_ContactSPtr pContact = it.value();
		SE_ASSERT( pContact );
		t_ContactID nID = it.key();
		// Make copy contact
		//!-->Can bring performance issues: Should Be Tested
		oResult.append( *pContact );
	}
	return oResult;
}

// GetContacts
t_IdToContactMap	CPhoneBookDataStore::GetContactsMap() const
{
	t_IdToContactMap oResult;
	for (auto it = m_hshCache.begin(); it != m_hshCache.end(); ++it)
	{
		pb::t_ContactSPtr pContact = it.value();
		SE_ASSERT( pContact );
		t_ContactID nID = it.key();
		// Make copy contact
		//!-->Can bring performance issues: Should Be Tested
		oResult.insert( nID, *pContact );
	}
	return oResult;
}

t_ContactIdList	CPhoneBookDataStore::GetContactIds() const
{
	return m_hshCache.keys();
}


// GetContact
pb::CContact		CPhoneBookDataStore::GetContact( t_ContactID nContactID ) const
{
	pb::t_ContactSPtr pContact = m_hshCache.value( nContactID );
	SE_ASSERT( pContact );
	return *pContact;
}

// GetContactHeader
pb::CContactHeader	CPhoneBookDataStore::GetContactHeader( t_ContactID nContactID ) const
{
	pb::t_ContactSPtr pContact = m_hshCache.value( nContactID );
	SE_ASSERT( pContact );
	return pContact->Header();
}

// GetContactCount
int					CPhoneBookDataStore::GetContactCount() const
{
	return m_hshCache.size();
}

// IsEmpty
bool				CPhoneBookDataStore::IsCacheEmpty() const
{
	return m_hshCache.isEmpty();
}

// AddContact
t_ContactID			CPhoneBookDataStore::AddContact( pb::CContact const& oContact )
{
	QString sFirstName = oContact.GetFirstName();
	QString sLastName = oContact.GetLastName();

	QPixmap oIcon = oContact.GetIcon();
	QByteArray aIconByteArray;
	QBuffer oIconBuffer( &aIconByteArray );
	oIconBuffer.open( QIODevice::WriteOnly );
	oIcon.save( &oIconBuffer, "PNG" ); // write inPixmap into inByteArray in PNG format

	if (sFirstName.isEmpty() && sLastName.isEmpty())
	{
		//! Invalid contact
		throw se::CError( se::CError::eInvalidContactInfo, tr( "Invalid contact info" ) );
	}

	DECL_QUERY( oQuery );
	oQuery.prepare( QString( "INSERT INTO %1 ( %2, %3, %4 ) "
		"VALUES( :fname, :lname, :icon )" )
		.arg( Tables::PhoneBook::TableName )
		.arg( Tables::PhoneBook::_FirstName )
		.arg( Tables::PhoneBook::_LastName )
		.arg( Tables::PhoneBook::_Icon )
		);

	oQuery.bindValue( ":fname", sFirstName );
	oQuery.bindValue( ":lname", sLastName );
	oQuery.bindValue( ":icon",  aIconByteArray );
	// Exec
	EXEC_QUERY( oQuery );

	// Fetch Contaxt ID
	int nContactID = FetchFieldLength( Tables::PhoneBook::TableName,
									   Tables::PhoneBook::_ID );
	//
	//	Add phone numbers to database
	//
	pb::t_PhoneNumberEntryList lstPhoneNumbers = oContact.GetPhoneNumberEntryList();
	for (int i = 0; i < lstPhoneNumbers.size(); ++i)
		AddPhoneNumberEntry( nContactID, lstPhoneNumbers[i] );

	// Make copy contact
	pb::t_ContactSPtr pContact( new pb::CContact( oContact ) );
	// Set contact ID
	// Add contact to cache
	m_hshCache.insert( nContactID, pContact );
	// Notify
	emit sigContactAdded( *pContact );
	// 
	return nContactID;
}

// RemoveContact
void				CPhoneBookDataStore::RemoveContact( t_ContactID nContactID ) 
{
	pb::CContact oContact;
	emit sigContactAboutToBeRemoved( -1 );

	// TODO
}

//
//	Tools
//
int CPhoneBookDataStore::FetchFieldLength( QString const& sTable, QString const& sField )
{
	// Fetch ID
	DECL_QUERY( oQuery );
	EXECUTE_QUERY_EX( oQuery, QString( "SELECT COUNT(%1) FROM %2; " )
					  .arg( sField )
					  .arg( sTable ) );
	SE_VERIFY( oQuery.first() );
	int nID = oQuery.value( 0 ).toInt();
	return nID;
}

// AddPhoneNumberEntry
void CPhoneBookDataStore::AddPhoneNumberEntry( int nPhoneBookID, pb::CPhoneNumberEntry const& oEntry )
{
	SE_ASSERT( nPhoneBookID > 0 );
	QString sNumber = oEntry.PhoneNumber().ToString();
	SE_ASSERT( !sNumber.isEmpty() );
	pb::EPhoneType ePhoneType = oEntry.PhoneType();
	int nPhoneType = static_cast<int>(ePhoneType);

	DECL_QUERY( oQuery );
	oQuery.prepare( QString( "INSERT INTO %1 ( %2, %3, %4 ) "
		"VALUES( :phonebookid, :phonenumber, :phonetype )" )
		.arg( Tables::PhoneBookDetails::TableName )
		.arg( Tables::PhoneBookDetails::_PhoneBookID )
		.arg( Tables::PhoneBookDetails::_PhoneNumber )
		.arg( Tables::PhoneBookDetails::_Type )
		);

	oQuery.bindValue( ":phonebookid", nPhoneBookID );
	oQuery.bindValue( ":phonenumber", sNumber );
	oQuery.bindValue( ":phonetype", nPhoneType );
	// Exec
	EXEC_QUERY( oQuery );
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
