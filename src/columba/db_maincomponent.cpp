////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "db_maincomponent.h"
#include "se_core.h"
// Qt
#include <QSqlQuery>
#include <QVariant>
#include <QVector>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////


	//
	//	Static member Initaialization
	//
	const QLatin1String CMainDataDB::Tables::PhoneBook::TableName  = QLatin1String( "phone_book" );
	const QLatin1String CMainDataDB::Tables::PhoneBook::_ID        = QLatin1String( "id" );
	const QLatin1String CMainDataDB::Tables::PhoneBook::_FirstName = QLatin1String( "first_name" );
	const QLatin1String CMainDataDB::Tables::PhoneBook::_LastName  = QLatin1String( "last_name" );
	const QLatin1String CMainDataDB::Tables::PhoneBook::_Icon	   = QLatin1String( "icon" );


	const QLatin1String CMainDataDB::Tables::PhoneBookDetails::TableName	   = QLatin1String( "phone_book_details" );
	const QLatin1String CMainDataDB::Tables::PhoneBookDetails::_ID		   = QLatin1String( "id" );
	const QLatin1String CMainDataDB::Tables::PhoneBookDetails::_PhoneBookID = QLatin1String( "phone_book_id" );
	const QLatin1String CMainDataDB::Tables::PhoneBookDetails::_PhoneNumber = QLatin1String( "phone_number" );
	const QLatin1String CMainDataDB::Tables::PhoneBookDetails::_Type		   = QLatin1String( "type" );

	const QLatin1String CMainDataDB::Tables::Senders::TableName			  = QLatin1String( "senders" );
	const QLatin1String CMainDataDB::Tables::Senders::_ID				  = QLatin1String( "id" );
	const QLatin1String CMainDataDB::Tables::Senders::_PhoneBookDetailesID = QLatin1String( "phone_book_detailes_id" );
	const QLatin1String CMainDataDB::Tables::Senders::_Info				  = QLatin1String( "info" );
	const QLatin1String CMainDataDB::Tables::Senders::_UnreadCount		  = QLatin1String( "unread_count" );

	const QLatin1String CMainDataDB::Tables::Conversations::TableName    = QLatin1String( "conversations" );
	const QLatin1String CMainDataDB::Tables::Conversations::_ID			= QLatin1String( "id" );
	const QLatin1String CMainDataDB::Tables::Conversations::_SenderID	= QLatin1String( "sender_id" );
	const QLatin1String CMainDataDB::Tables::Conversations::_IsDelivered = QLatin1String( "is_delivered" );
	const QLatin1String CMainDataDB::Tables::Conversations::_Status	= QLatin1String( "status" );
	const QLatin1String CMainDataDB::Tables::Conversations::_Message		= QLatin1String( "message" );
	const QLatin1String CMainDataDB::Tables::Conversations::_Date		= QLatin1String( "date" );
	const QLatin1String CMainDataDB::Tables::Conversations::_Time		= QLatin1String( "time" );

////////////////////////////////////////////////////////////////////////////////
//
//	class CMainDataDB
//
// Initialize
void CMainDataDB::Initialize()
{
	CREATE_QUERY;
	// Create Table PhoneBook

	EXECUTE_QUERY( QString( "CREATE TABLE %1 ("
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
	EXECUTE_QUERY( QString( "CREATE TABLE %1 ("
		"%2 INTEGER PRIMARY KEY NOT NULL, "
		"%3 INTEGER NOT NULL, "
		"%4 TEXT    NOT NULL, "
		"%5 INTEGER NOT NULL );" )
		.arg( Tables::PhoneBookDetails::TableName )
		.arg( Tables::PhoneBookDetails::_ID )
		.arg( Tables::PhoneBookDetails::_PhoneBookID )
		.arg( Tables::PhoneBookDetails::_PhoneNumber )
		.arg( Tables::PhoneBookDetails::_Type)
		);

	// Create Senders Table
	EXECUTE_QUERY( QString( "CREATE TABLE %1 ("
		"%2 INTEGER PRIMARY  KEY NOT NULL, "
		"%3 TEXT			 DEFAULT '', "
		"%4 INTEGER NOT NULL DEFAULT -1, " 
		"%5 INTEGER DEFAULT 0 ); " )
		.arg( Tables::Senders::TableName )
		.arg( Tables::Senders::_ID )
		.arg( Tables::Senders::_Info )
		.arg( Tables::Senders::_PhoneBookDetailesID )
		.arg( Tables::Senders::_UnreadCount )
		);

	// Create Conversations Table
	EXECUTE_QUERY( QString( "CREATE TABLE %1 ("
		"%2 INTEGER PRIMARY KEY NOT NULL, "
		"%3 INTEGER NOT NULL, "
		"%4 BOOLEAN NOT NULL, "
		"%5 BOOLEAN NOT NULL, "
		"%6 TEXT, "
		"%7 TEXT    NOT NULL, "
		"%8 TEXT    NOT NULL ); " )
		.arg( Tables::Conversations::TableName )
		.arg( Tables::Conversations::_ID )
		.arg( Tables::Conversations::_SenderID )
		.arg( Tables::Conversations::_IsDelivered )
		.arg( Tables::Conversations::_Status )
		.arg( Tables::Conversations::_Message )
		.arg( Tables::Conversations::_Date )
		.arg( Tables::Conversations::_Time )
		);
}

// GetSenderIDFromInfo
int CMainDataDB::GetSenderIDFromInfo( QString const& sInfo,
									  bool bAddIfNotExsists )
{
	SE_ASSERT( !sInfo.isEmpty() );
	if (sInfo.isEmpty())
		return -1;
	QSqlQuery oQuery = CreateQuery();
	// Check in unregisterd senders
	// SELECT id FROM senders WHERE info=='+37477355665'
	EXECUTE_QUERY_EX( oQuery, QString( "SELECT %1 FROM %2 WHERE %3=='%4';" )
					  .arg( Tables::Senders::_ID ) 
					  .arg( Tables::Senders::TableName )
					  .arg( Tables::Senders::_Info )
					  .arg( sInfo )
					  );

	if (oQuery.first())
	{
		// Get ID
		int nID = oQuery.value( 0 ).toInt();
		SE_ASSERT( nID > 0 );
		return nID;
	};

	// Check in registered senders
	/*SELECT senders.id
		FROM senders JOIN phone_book_details
		ON senders.phone_book_detailes_id == phone_book_details.id AND senders.phone_book_detailes_id != -1
		WHERE phone_book_details.phone_number == '+37494443460'*/

	EXECUTE_QUERY_EX( oQuery, QString( "SELECT %1.%2 "
		"FROM %1 JOIN %3 "
		"ON %1.%4 == %3.%5 AND %1.%4 != -1 "
		"WHERE %3.%6 == '%7'; " )
		.arg( Tables::Senders::TableName )
		.arg( Tables::Senders::_ID )
		.arg( Tables::PhoneBookDetails::TableName )
		.arg( Tables::Senders::_PhoneBookDetailesID )
		.arg( Tables::PhoneBookDetails::_ID )
		.arg( Tables::PhoneBookDetails::_PhoneNumber )
		.arg( sInfo )
		);

	if (oQuery.first())
	{
		// Get ID
		int nID = oQuery.value( 0 ).toInt();
		SE_ASSERT( nID > 0 );
		return nID;
	};

	if (!bAddIfNotExsists)
		return -1;
	else
	{
		//
		// Not Found So Add
		//
		// At first check in phone book
		EXECUTE_QUERY_EX( oQuery, QString( "SELECT %1 FROM %2 "
			"WHERE %3 == '%4'; " )
			.arg( Tables::PhoneBookDetails::_ID )
			.arg( Tables::PhoneBookDetails::TableName )
			.arg( Tables::PhoneBookDetails::_PhoneNumber )
			.arg( sInfo )
			);
		if (oQuery.first())
		{
			// Get Phone book detiled ID
			int nPhoneBookDtlID = oQuery.value( 0 ).toInt();
			SE_ASSERT( nPhoneBookDtlID > 0 );
			// Insert new item in senders table
			EXECUTE_QUERY_EX( oQuery, QString( "INSERT INTO %1( %2 ) "
				"VALUES( %3 ); " )
				.arg( Tables::Senders::TableName )
				.arg( Tables::Senders::_PhoneBookDetailesID )
				.arg( nPhoneBookDtlID )
				);
		}
		else
		{
			// Insert new item in senders table
			EXECUTE_QUERY_EX( oQuery, QString( "INSERT INTO %1 (%2) " 
				"VALUES('%3'); " )
				.arg( Tables::Senders::TableName )
				.arg( Tables::Senders::_Info )
				.arg( sInfo )
				);
		}
		// Fetch ID
		EXECUTE_QUERY_EX( oQuery, QString( "SELECT COUNT(%1) FROM %2; " )
						  .arg( Tables::Senders::_ID )
						  .arg( Tables::Senders::TableName ) );
		SE_VERIFY( oQuery.first() );
		int nID = oQuery.value( 0 ).toInt();
		return nID;
	}
}

// Returns added message ID
qint64 CMainDataDB::AddChatMessage( int nSenderID,
									 bool bIsDelivered,
									 QString const& sMesage,
									 QDateTime const& oDateTime )
{
	SE_ASSERT( nSenderID > 0 );
	if (nSenderID <= 0)
		return -1;
	SE_ASSERT( oDateTime.isValid() );
	SE_ASSERT( !sMesage.isEmpty() );
	if (!oDateTime.isValid())
		return -1;
	QSqlQuery oQuery = CreateQuery();
	EXECUTE_QUERY_EX( oQuery, QString( "INSERT INTO %1 ( %2, %3, %4, %5, %6, %7 ) "
		"VALUES( %8, %9, %10, '%11', '%12', '%13' ); " )
		/*1*/.arg( Tables::Conversations::TableName )
		/*2*/.arg( Tables::Conversations::_SenderID )
		/*3*/.arg( Tables::Conversations::_IsDelivered )
		/*4*/.arg( Tables::Conversations::_Status )
		/*5*/.arg( Tables::Conversations::_Message )
		/*6*/.arg( Tables::Conversations::_Date )
		/*7*/.arg( Tables::Conversations::_Time )

		/*8*/.arg( nSenderID )
		/*9*/.arg( bIsDelivered )
		/*10*/.arg( true )
		/*11*/.arg( sMesage )
		/*12*/.arg( oDateTime.date().toString( "dd MMM yyyy" ) )
		/*13*/.arg( oDateTime.time().toString("HH:mm") )
		);

	if (bIsDelivered)
	{
		//
		//	Mark as unread message in senders table
		//
		// Get Current count
		EXECUTE_QUERY_EX( oQuery, QString( "SELECT %1 FROM %2 "
			"WHERE %3 == %4; " )
			.arg( Tables::Senders::_UnreadCount )
			.arg( Tables::Senders::TableName )
			.arg( Tables::Senders::_ID )
			.arg( nSenderID ) );
		SE_VERIFY( oQuery.first() );
		int nUnreadCount = oQuery.value( 0 ).toInt();

		EXECUTE_QUERY_EX( oQuery, QString( "UPDATE %1 SET %2 = 3 WHERE %3 = %4; " )
						  .arg( Tables::Senders::TableName )
						  .arg( Tables::Senders::_UnreadCount )
						  .arg( Tables::Senders::_ID )
						  .arg( nSenderID ) );
	}


	// Fetch ID
	EXECUTE_QUERY_EX( oQuery, QString( "SELECT COUNT(%1) FROM %2; " )
					  .arg( Tables::Conversations::_ID )
					  .arg( Tables::Conversations::TableName ) );
	SE_VERIFY( oQuery.first() );
	int nID = oQuery.value( 0 ).toInt();
	return nID;
}


// StoreMessage
void CMainDataDB::StoreMessage( QString const& sSender,
								 bool bIsDelivered,
								 QString const& sMessage,
								 QDateTime const& oDateTime )
{
	SE_ASSERT( !sSender.isEmpty() );
	SE_ASSERT( oDateTime.isValid());

	int nSenderID = GetSenderIDFromInfo( sSender, true );
	SE_ASSERT( nSenderID > 0 );
	qint64 nMessageID = AddChatMessage( nSenderID, bIsDelivered, sMessage, oDateTime );
	emit sigMessageAdded( nSenderID, nMessageID );
}

// RemoveConversation
void CMainDataDB::RemoveConversation( int nSenderID )
{
	// DELETE FROM conversations WHERE conversations.sender_id == 2;

	SE_ASSERT( nSenderID > 0 );
	if (nSenderID <= 0)
		return;

	// Delete records from Conversations table
	QSqlQuery oQuery = CreateQuery();
	EXECUTE_QUERY_EX( oQuery, QString( "DELETE FROM %1 WHERE %2 == %3;" )
		/*1*/.arg( Tables::Conversations::TableName )
		/*2*/.arg( Tables::Conversations::_SenderID )
		/*3*/.arg( nSenderID )
		);

	// Delete records from Senders table
	//! DELETE FROM senders WHERE id==1;
	EXECUTE_QUERY_EX( oQuery, QString( "DELETE FROM %1 WHERE %2==%3;" )
					  .arg( Tables::Senders::TableName )
					  .arg( Tables::Senders::_ID )
					  .arg( nSenderID ) 
					  );
}

// RemoveMessages
void CMainDataDB::RemoveMessages( QVector<qint64> aMessageIDs )
{
	if (aMessageIDs.isEmpty())
		return;
	QString sINarg;
	for (qint64 nID : aMessageIDs)
	{
		SE_ASSERT( nID > 0 );
		sINarg.append( QString( "%1," ).arg( nID ) );
	}
	// remove last comma (,)
	sINarg.remove( sINarg.size() - 1, 1 );
	// DELETE FROM conversations WHERE conversations.id IN(1,2,5);

	// Delete records from Conversations table
	QSqlQuery oQuery = CreateQuery();
	EXECUTE_QUERY_EX( oQuery, QString( "DELETE FROM %1 WHERE %2 IN(%3);" )
					  /*1*/.arg( Tables::Conversations::TableName )
					  /*2*/.arg( Tables::Conversations::_ID )
					  /*3*/.arg( sINarg )
					  );
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
