////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_sendersmodel.h"
#include "se_core.h"
#include "db_maincomponent.h"
// Qt
#include <QSize>
#include <QBrush>
#include <QFont>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSendersModel
//
CSendersModel::CSendersModel( QObject* pParent )
	: Base( pParent )
{
	UpdateAll();
}

// rowCount
int	CSendersModel::rowCount( const QModelIndex& parent ) const
{
	return m_lstSenderData.size();
}

// data
QVariant CSendersModel::data( const QModelIndex& oIndex, int role ) const
{
	if (!oIndex.isValid( ))
		return QVariant();
	int nRow = oIndex.row();
	//SE_ASSERT( nRow >= 0 && nRow < m_lstSenderData.size( ) );
	if (nRow < 0 || nRow >= m_lstSenderData.size())
		return QVariant();

	SSenderData strData = m_lstSenderData[nRow];
	bool bHasUnreadMessages = strData.nUnreadMessageCount > 0;
	QVariant vtResult;
	switch (role)
	{
	case Qt::DisplayRole:
		vtResult = strData.sTitle;
		break;
	/*case Qt::ForegroundRole:
		{
			QBrush oTextBrush;
			oTextBrush.setStyle( Qt::SolidPattern );
			if (bHasUnreadMessages)
				oTextBrush.setColor( Qt::green );
			vtResult = oTextBrush;
			break;
		}*/
	case Qt::FontRole:
		{
			QFont oFont;
			oFont.setPointSize( 12 );
			oFont.setFamily( "Calibri" );
			if (bHasUnreadMessages)
				oFont.setBold( true );
			vtResult = oFont;
			break;
		}
	case Qt::SizeHintRole:
		vtResult = QSize( -1, 40 );
		break;
	case eHasUnreadMessageRole:
		vtResult = bHasUnreadMessages;
		break;
	case eGetIDRole:
		vtResult = int( strData.nID );
		break;
	case ePhoneNumberRole:
		vtResult = GetPhoneNumber( strData.nID );
			break;
		

	default:
		break;
	}

	return vtResult;
}

// insertRows
bool CSendersModel::insertRows( int row, int count,
									 const QModelIndex& parent )
{
	return false;
}

// removeRows
bool CSendersModel::removeRows( int row, int count,
									 const QModelIndex& parent )
{
	return false;
}

void CSendersModel::UpdateAll()
{
	m_lstSenderData.clear();

	QSqlQuery oQuery;
	// make shorter
	typedef db::CMainDataDB::Tables t_Tables;
	// Select All from senders table
	EXECUTE_QUERY_EX( oQuery, QString( "SELECT * FROM %1;" )
		.arg( t_Tables::Senders::TableName ) );

	while (oQuery.next()) 
	{
		// Get Data
		int nID = oQuery.value( 0 ).toInt();
		QString sTitle = oQuery.value( 1 ).toString();
		int nFBInex = oQuery.value( 2 ).toInt();
		SE_ASSERT( nID != 0 );
		int nUnreadCount = oQuery.value( 3 ).toInt();
		// 
		// Needs data from phone book
		if (sTitle.isEmpty())
		{
			SE_ASSERT( nFBInex > 0 );
			QSqlQuery oQuery2;
			EXECUTE_QUERY_EX( oQuery2, QString( "SELECT  %1.%2, %1.%3 "
				"FROM %1 JOIN %4 "
				"ON %1.%5 == %4.%6 "
				"WHERE %4.%7 == %8; " )
				.arg( t_Tables::PhoneBook::TableName )
				.arg( t_Tables::PhoneBook::_FirstName )
				.arg( t_Tables::PhoneBook::_LastName )
				.arg( t_Tables::PhoneBookDetails::TableName )
				.arg( t_Tables::PhoneBook::_ID )
				.arg( t_Tables::PhoneBookDetails::_PhoneBookID )
				.arg( t_Tables::PhoneBookDetails::_ID )
				.arg( nFBInex )
				);

			SE_VERIFY( oQuery2.next() );
				// Get Data
			QString sFName = oQuery2.value( 0 ).toString();
			QString sLName = oQuery2.value( 1 ).toString();
			SE_ASSERT( !sFName.isEmpty() );
			SE_ASSERT( !sLName.isEmpty() );
			QString sFullName = sFName + " " + sLName;
			m_lstSenderData.append( SSenderData( nID, sFullName, nUnreadCount ) );
		}
		else
		{
			m_lstSenderData.append( SSenderData( nID, sTitle, nUnreadCount ) );
		}
	}

	/*SELECT  phone_book.first_name, phone_book.last_name
		FROM phone_book JOIN phone_book_details
		ON phone_book.id == phone_book_details.phone_book_id
		WHERE phone_book_details.id IN( 2, 4 );*/	
}

// MarkMessagesAsRead
void CSendersModel::MarkMessagesAsRead( int nSenderID )
{
	SE_ASSERT( nSenderID );
	if (nSenderID <= 0)
		return;

	QSqlQuery oQuery;
	// make shorter
	typedef db::CMainDataDB::Tables t_Tables;
	// Select All from senders table
	EXECUTE_QUERY_EX( oQuery, QString( "UPDATE %1 " 
		"SET %2=0 "
		"WHERE %3=%4; ")
		.arg( t_Tables::Senders::TableName )
		.arg( t_Tables::Senders::_UnreadCount)
		.arg( t_Tables::Senders::_ID )
		.arg( nSenderID )
		);

	//! Note: "conversation" table is not updated. If it will be necesery consider this fact.

	// Update member data
	int nIdx = IndexFromID( nSenderID );
	SE_ASSERT( nIdx >= 0 && nIdx < m_lstSenderData.size() );
	if (nIdx >=0 && nIdx < m_lstSenderData.size())
	{
		m_lstSenderData[nIdx].nUnreadMessageCount = 0;
		// Update
		QModelIndex oIndex = index( nIdx );
		emit Base::dataChanged( oIndex, oIndex );
	}
	//UpdateItem( nSenderID );
}

// UpdateItem
void CSendersModel::UpdateItem( int nSenderID )
{
	SE_ASSERT( nSenderID );
	if (nSenderID <= 0)
		return;

	QSqlQuery oQuery;
	// make shorter
	typedef db::CMainDataDB::Tables t_Tables;
	// Select All from senders table
	EXECUTE_QUERY_EX( oQuery, QString( "SELECT * FROM %1 "
		"WHERE %2 == %3; " )
		.arg( t_Tables::Senders::TableName )
		.arg( t_Tables::Senders::_ID )
		.arg( nSenderID )
		);

	//QString sPhoneBookIndices;
	if (oQuery.next())
	{
		// Get Data
		int nID = oQuery.value( 0 ).toInt();
		QString sTitle = oQuery.value( 1 ).toString();
		int nFBInex = oQuery.value( 2 ).toInt();
		SE_ASSERT( nID != 0 );
		int nUnreadCount = oQuery.value( 3 ).toInt();
		// 
		SSenderData oSenderData( nID, sTitle, nUnreadCount );
		// Needs data from phone book
		if (sTitle.isEmpty())
		{
			//
			//	Get Phone Book Data
			//
			SE_ASSERT( nFBInex > 0 );
			/*SELECT  phone_book.first_name, phone_book.last_name
			FROM phone_book JOIN phone_book_details
			ON phone_book.id == phone_book_details.phone_book_id
			WHERE phone_book_details.id IN( 2, 4 );*/

			EXECUTE_QUERY_EX( oQuery, QString( "SELECT  %1.%2, %1.%3 "
				"FROM %1 JOIN %4 "
				"ON %1.%5 == %4.%6 "
				"WHERE %4.%7 == %8; " )
				.arg( t_Tables::PhoneBook::TableName )
				.arg( t_Tables::PhoneBook::_FirstName )
				.arg( t_Tables::PhoneBook::_LastName )
				.arg( t_Tables::PhoneBookDetails::TableName )
				.arg( t_Tables::PhoneBook::_ID )
				.arg( t_Tables::PhoneBookDetails::_PhoneBookID )
				.arg( t_Tables::PhoneBookDetails::_ID )
				.arg( nFBInex )
				);


			SE_VERIFY( oQuery.next() );
			// Get Data
			QString sFName = oQuery.value( 0 ).toString();
			QString sLName = oQuery.value( 1 ).toString();
			SE_ASSERT( !sFName.isEmpty() );
			SE_ASSERT( !sLName.isEmpty() );
			oSenderData.sTitle = sFName + " " + sLName;
		}
			
		int nIdx = IndexFromID( nSenderID );
		if (nIdx == -1)
		{
			// Add new sender data
			AddSenderData( oSenderData );
		}
		else
		{
			m_lstSenderData[nIdx] = oSenderData;
			// Update
			QModelIndex oIndex = index( nIdx );
			emit dataChanged( oIndex, oIndex );
		}	
	}
}


int CSendersModel::IndexFromID( int nSenderID )
{
	int nIdx = -1;
	for (int i = 0; i < m_lstSenderData.size(); ++i)
		if (m_lstSenderData[i].nID == nSenderID)
		{
			nIdx = i;
			break;
		}
	return nIdx;
}

// AddSenderData
void CSendersModel::AddSenderData( SSenderData const& strData )
{
	int nLast = m_lstSenderData.size();
	beginInsertRows( QModelIndex(), nLast, nLast );
	m_lstSenderData.append( strData );
	endInsertRows();
	//! Not Tested
	// TODO
}

// GetPhoneNumber
QString CSendersModel::GetPhoneNumber( int nSenderID ) const
{
	SE_ASSERT( nSenderID );
	if (nSenderID <= 0)
		return QString();

	QSqlQuery oQuery;
	// make shorter
	typedef db::CMainDataDB::Tables t_Tables;
	// Select All from senders table
	EXECUTE_QUERY_EX( oQuery, QString( "SELECT %1, %2 "
		"FROM %3 "
		"WHERE %4 == %5; " )
		.arg( t_Tables::Senders::_Info )
		.arg( t_Tables::Senders::_PhoneBookDetailesID )
		.arg( t_Tables::Senders::TableName )
		.arg( t_Tables::Senders::_ID )
		.arg( nSenderID )
		);

	QString sPhone;
	if (oQuery.next())
	{
		// Get Data
		int nPhoneBookDetailedID = oQuery.value( 1 ).toInt();
		if (nPhoneBookDetailedID == -1)
		{
			sPhone = oQuery.value( 0 ).toString();
			SE_ASSERT( !sPhone.isEmpty() );
		}
		else if (nPhoneBookDetailedID > 0) // Get from Phone Book Detiles
		{
			EXECUTE_QUERY_EX( oQuery, QString( "SELECT %1 "
				"FROM %2 "
				"WHERE %3 == %4; " )
				.arg( t_Tables::PhoneBookDetails::_PhoneNumber )
				.arg( t_Tables::PhoneBookDetails::TableName )
				.arg( t_Tables::PhoneBookDetails::_ID )
				.arg( nPhoneBookDetailedID )
				);
			SE_VERIFY( oQuery.next() );
			sPhone = oQuery.value( 0 ).toString();
			SE_ASSERT( !sPhone.isEmpty() );
		}
	}
	return sPhone;
}

// RemoveSenderItem
void CSendersModel::RemoveSenderItem( int nSenderID )
{
	int nIdx = IndexFromID( nSenderID );
	if (nIdx == -1 || nIdx > m_lstSenderData.size())
		return;
	m_lstSenderData.removeAt( nIdx );
	// Update
	QModelIndex oIndex = index( nIdx );
	emit Base::dataChanged( oIndex, oIndex );
}

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
