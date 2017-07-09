////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_conversationdata.h"
#include "se_core.h"
// Qt
#include <QVariant>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSendersModel
//
QList<CMessageInfo> CConversationData::GetMessageInfoList( int nID ) 
{
	SE_ASSERT( nID > 0 );
	QSqlQuery oQuery;
	// make shorter
	typedef db::CMainDataDB::Tables t_Tables;
	/*SELECT id, is_delivered, message, date, time
		FROM conversations
		WHERE sender_id == 4;*/
	EXECUTE_QUERY_EX( oQuery, QString( "SELECT %1, %2, %3, %4, %5 "
		"FROM %6 "
		"WHERE %7 == %8; " )
					  .arg( t_Tables::Conversations::_ID )
					  .arg( t_Tables::Conversations::_IsDelivered )
					  .arg( t_Tables::Conversations::_Message )
					  .arg( t_Tables::Conversations::_Date )
					  .arg( t_Tables::Conversations::_Time )
					  .arg( t_Tables::Conversations::TableName )
					  .arg( t_Tables::Conversations::_SenderID )
					  .arg( nID )
					  );

	QList<CMessageInfo> lstMessageInfo;
	while (oQuery.next( ))
	{
		// Get Data
		int nID = oQuery.value( 0 ).toInt();
		SE_ASSERT( nID > 0 );
		bool bIsDelivered = oQuery.value( 1 ).toBool();
		QString sMessage = oQuery.value( 2 ).toString();
		SE_ASSERT( !sMessage.isEmpty() );
		// Fetch Date Time
		QString sDate = oQuery.value( 3 ).toString();
		QString sTime = oQuery.value( 4 ).toString();

		QDateTime oDateTime = QDateTime::fromString( sDate + " " + sTime,
													 "dd MMM yyyy HH:mm" );
		lstMessageInfo.append( gui::CMessageInfo( sMessage, oDateTime, bIsDelivered, nID ) );
	}

	return lstMessageInfo;
}

// GetMessage
CMessageInfo CConversationData::GetMessage( qint64 nMessageID )
{
	SE_ASSERT( nMessageID > 0 );
	if (nMessageID < 0)
		return CMessageInfo();
	QSqlQuery oQuery;
	// make shorter
	typedef db::CMainDataDB::Tables t_Tables;
	/*SELECT id, is_delivered, message, date, time
	FROM conversations
	WHERE sender_id == 4;*/
	EXECUTE_QUERY_EX( oQuery, QString( "SELECT %1, %2, %3, %4 "
		"FROM %5 "
		"WHERE %6 == %7; " )
		.arg( t_Tables::Conversations::_IsDelivered )
		.arg( t_Tables::Conversations::_Message )
		.arg( t_Tables::Conversations::_Date )
		.arg( t_Tables::Conversations::_Time )
		.arg( t_Tables::Conversations::TableName )
		.arg( t_Tables::Conversations::_ID)
		.arg( nMessageID )
		);

	if (oQuery.first())
	{
		// Get Data
		bool bIsDelivered = oQuery.value( 0 ).toBool();
		QString sMessage = oQuery.value( 1 ).toString();
		SE_ASSERT( !sMessage.isEmpty() );
		// Fetch Date Time
		QString sDate = oQuery.value( 2 ).toString();
		QString sTime = oQuery.value( 3 ).toString();

		QDate oDate = QDate::fromString( sDate, "dd MMM yyyy" );
		QTime oTime = QTime::fromString( sTime, "HH:mm" );
		SE_ASSERT( oDate.isValid() );
		SE_ASSERT( oTime.isValid() );

		QDateTime oDateTime( oDate, oTime );  // "dd MMM yyyy HH:mm" 
		/*QDateTime oDateTime = QDateTime::fromString( sDate + " " + sTime,
													 "dd MMM yyyy HH:mm" );*/
		return gui::CMessageInfo( sMessage, oDateTime, bIsDelivered, nMessageID );
	}

	return CMessageInfo();
}

// GetMessageCount
//int	CConversationData::GetMessageCount( int nSenderID )
//{
//	QList<CMessageInfo> lstMessages = GetMessageInfoList( nSenderID );
//	return lstMessages.size();
//}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
