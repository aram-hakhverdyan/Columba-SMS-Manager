#ifndef DB_MAIN_COMPONENT_H
#define DB_MAIN_COMPONENT_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "db_component.h"
// Qt includes
#include <QLatin1String>
#include <QDateTime>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CMainDataDB
//
class CMainDataDB : public QObject, public IDBComponent
{
	Q_OBJECT

public:// Constructors
	inline CMainDataDB() = default;
	virtual ~CMainDataDB() = default;

public:
	//
	//	Interface Methodes
	//
	void Initialize() override;

public:
	////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Literal Definitions
	//	
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

		struct Senders
		{
			static const QLatin1String TableName;
			static const QLatin1String _ID;
			static const QLatin1String _Info;
			static const QLatin1String _PhoneBookDetailesID;
			static const QLatin1String _UnreadCount;
		};

		struct Conversations
		{
			static const QLatin1String TableName;
			static const QLatin1String _ID;
			static const QLatin1String _SenderID;
			static const QLatin1String _IsDelivered;
			static const QLatin1String _Status; // renamed from _IsUnread
			static const QLatin1String _Message;
			static const QLatin1String _Date;
			static const QLatin1String _Time;
		};

	};
	////////////////////////////////////////////////////////////////////////////////

public:
	// Returns sender's ID matching to the specified info. 
	// By default adds new item if not exsists.  
	int GetSenderIDFromInfo( QString const& sInfo, 
							 bool bAddIfNotExsists = true );
	// Returns added message ID
	qint64 AddChatMessage( int nSenderID,
						   bool bIsDelivered,
						   QString const& sMesage,
						   QDateTime const& oDateTime );
public slots:
	//
	//	Main Interface
	//
	void StoreMessage( QString const& sSender,
					   bool bIsDelivered,
					   QString const& sMessage,
					   QDateTime const& oDateTime);
	
	// Removes all conversation with specified sender
	void RemoveConversation( int nSinderID );
	// Removes messages by specified IDs
	void RemoveMessages( QVector<qint64> aMessageIDs );
	// Removes messages by specified ID
	//void RemoveMessage( qint64 nMessageID );
	


signals:
	void sigMessageAdded( int nSenderID, qint64 nMessageID );

};

typedef std::shared_ptr<CMainDataDB> t_MainDataDBSPtr;
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////

#endif // DB_MAIN_COMPONENT_H