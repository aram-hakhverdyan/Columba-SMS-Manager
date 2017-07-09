////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "db_connectionmanager.h"

// Qt
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////

//extern QList<t_IDBComponentSPtr> g_lstDBComponents;

//
//  Static Member Initialization
//
const QLatin1String CConnectionManager::m_csSQLiteDBPath = QLatin1String( "/" );
const QLatin1String CConnectionManager::m_csDataFileName = QLatin1String( "data.sqlite" );

////////////////////////////////////////////////////////////////////////////////
//
//	class CConnectionManager
//
// Initialize
void CConnectionManager::InitDefaultDataBase( )
{
	m_bNeedsTableInit = false;
	// Chack data file existance
	QDir oDataFileDir;
	QString sFilePath = m_sSQLiteDBPath + m_sDataFileName;
	if (!oDataFileDir.exists( sFilePath ))
	{
		// Create path
		if (!oDataFileDir.mkpath( m_sSQLiteDBPath ))
			throw se::CError( se::CError::DBError,
			QCoreApplication::translate( "DBQueryError", "Unable to create path" ) );
		m_bNeedsTableInit = true;
	}

	// Create SQlite connection
	m_oDefaultDB = QSqlDatabase::addDatabase( "QSQLITE" );
	m_oDefaultDB.setDatabaseName( sFilePath );
	if (!m_oDefaultDB.open())
	{
		QString sErrMsg = m_oDefaultDB.lastError().driverText();
		throw se::CError( se::CError::DBError,
							 QCoreApplication::translate( "DBQueryError",
							 "Unable to establish database connection: " ).append( sErrMsg ) );
	}

	/*if (m_bNeedsTableInit)
		for (t_IDBComponentSPtr& pComponent : m_lstDBComponents)
			pComponent->Initialize();*/
}

// AddDBComponent
void CConnectionManager::AddDBComponent( t_IDBComponentSPtr pComponent, bool bUseDefaultDB )
{
	if ( pComponent && !m_lstDBComponents.contains( pComponent ))
	{
		if (bUseDefaultDB)
			pComponent->SetDatabase( m_oDefaultDB );
		m_lstDBComponents.append( pComponent );
		if (m_bNeedsTableInit)
			pComponent->Initialize();
	}
}

//// CreateDBInfrastructure
//void CConnectionManager::CreateDBInfrastructure()
//{
//	QSqlQuery oQuery;
//	// Create Table user_info
//	oQuery.exec( "CREATE TABLE user_info ("
//				 "user_id    INTEGER PRIMARY KEY NOT NULL, "
//				 "first_name TEXT    NOT NULL, "
//				 "last_name  TEXT    NOT NULL, "
//				 "birthday   DATE    NOT NULL, "
//				 "gender     INTEGER NOT NULL, "
//				 "avatar     BLOB);" );
//	checkExecution( oQuery );
//
//	// Create Auth Table
//	oQuery.exec( "CREATE TABLE auth_info ("
//				 "user_id  INTEGER PRIMARY KEY AUTOINCREMENT,"
//				 "username TEXT    NOT NULL UNIQUE, "
//				 "password INTEGER NOT NULL);"
//				 );
//	checkExecution( oQuery );
//
//	// Create Friendship Table
//	oQuery.exec( "CREATE TABLE friendship ("
//				 "friendship_id INTEGER PRIMARY KEY AUTOINCREMENT, "
//				 "user_id       INTEGER NOT NULL, "
//				 "friend_id     INTEGER NOT NULL);" );
//	checkExecution( oQuery );
//
//	// Create Conversation Table
//	oQuery.exec( "CREATE TABLE conversation ("
//				 "conv_id    INTEGER PRIMARY KEY AUTOINCREMENT, "
//				 "table_name TEXT NOT NULL);" );
//	checkExecution( oQuery );
//
//	// Create Conversation Info Table
//	oQuery.exec( "CREATE TABLE conversation_info ("
//				 "user_id INTEGER NOT NULL, "
//				 "conv_id INTEGER NOT NULL);" );
//	checkExecution( oQuery );
//
//	// Create Messages Table
//	oQuery.exec( "CREATE TABLE messages ("
//				 "conv_id   INTEGER  NOT NULL, "
//				 "user_id   INTEGER  NOT NULL, "
//				 "message   TEXT     NOT NULL, "
//				 "send_time DATETIME );" );
//	checkExecution( oQuery );
//}
////////////////////////////////////////////////////////////////////////////////
	
////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
