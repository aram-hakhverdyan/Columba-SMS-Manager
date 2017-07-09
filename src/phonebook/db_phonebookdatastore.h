#ifndef DB_PHONE_BOOK_DATA_STORE_H
#define DB_PHONE_BOOK_DATA_STORE_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "db_component.h"
#include "pb_contact.h"
// Qt includes
#include <QLatin1String>
#include <QHash>
#include <QMap>

#include <QAbstractListModel>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////

typedef pb::t_ContactHeaderList		t_ContactHeaderList;
typedef pb::t_IdToContactHeaderMap	t_IdToContactHeaderMap;
typedef pb::t_ContactList			t_ContactList;
typedef pb::t_IdToContactMap		t_IdToContactMap;


////////////////////////////////////////////////////////////////////////////////
//
//	class CPhoneBookDataStore
//
class CPhoneBookDataStore : public QObject, public IDBComponent
{
	Q_OBJECT

public:// Constructors
	inline CPhoneBookDataStore() = default;
	virtual ~CPhoneBookDataStore() = default;

public:
	//
	//	IDBComponent Interface
	//
	void Initialize() override;

public:
	//
	//	Own Interface
	//
	// Removes cached data and loads all data from database
	void Reload();
	//
	void ClearCache();
	//
	t_ContactHeaderList		GetContactHeadersList() const;
	t_IdToContactHeaderMap	GetContactHeadersMap() const;
	//
	t_ContactList			GetContactsList() const;
	t_IdToContactMap		GetContactsMap() const;
	//
	t_ContactIdList			GetContactIds() const;

	// Returns contact by specified ID. 
	// If there is not a contact by specified ID then returns invalid contact
	pb::CContact		GetContact( t_ContactID nContactID ) const;
	// Returns contact header by specified ID. 
	// If there is not a contact by specified ID then returns invalid contact header
	pb::CContactHeader	GetContactHeader( t_ContactID nContactID ) const;
	// 
	int					GetContactCount() const;
	//
	bool				IsCacheEmpty() const;
	//
	//	Modifiers
	//
	// adds specified contact info to storage (database) and returns assigned ID
	t_ContactID			AddContact( pb::CContact const& oContact );
	// Removes specified contact from the chache and database 
	void				RemoveContact( t_ContactID nContactID );

public:
	////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Literal Definitions
	//! This literals are the same as the literals in "MainDBComponent" in columba module.
	//! The literals are defined here to support free usage of phonebook moudle outside of Columba project
	struct Tables
	{
		struct PhoneBook
		{
			static const QLatin1String TableName;
			static const QLatin1String _ID;
			static const QLatin1String _FirstName;
			static const QLatin1String _LastName;
			static const QLatin1String _Icon;

		};

		struct PhoneBookDetails
		{
			static const QLatin1String TableName;
			static const QLatin1String _ID;
			static const QLatin1String _PhoneBookID;
			static const QLatin1String _PhoneNumber;
			static const QLatin1String _Type;
		};
	};
	////////////////////////////////////////////////////////////////////////////////

signals:
	//
	//	Signals
	//
	// This signal is emitted after every reloading
	void sigReloaded();
	void sigAboutToBeCleared();

	void sigContactAdded( pb::CContact const& oContact );
	void sigContactAboutToBeRemoved( t_ContactID nContactID );

protected:
	//
	//	Tools
	//
	int FetchFieldLength( QString const& sTable, QString const& sField );
	void AddPhoneNumberEntry( int nPhoneBookID, pb::CPhoneNumberEntry const& oEntry );

private:
	//
	//	Content
	//
	// ID to contact cache
	QHash<t_ContactID, pb::t_ContactSPtr> m_hshCache;
};

typedef std::shared_ptr<CPhoneBookDataStore> t_PhoneBookDBSPtr;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////

#endif // DB_PHONE_BOOK_DATA_STORE_H