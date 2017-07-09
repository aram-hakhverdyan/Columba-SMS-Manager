#ifndef DB_CONNECTION_MANAGER_H
#define DB_CONNECTION_MANAGER_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "module_global.h"
#include "se_error.h"
#include "db_component.h"
// Qt includes
#include <QSqlError>
#include <QDir>
#include <QCoreApplication>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace db { // Data Base
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CConnectionManager
//
class MODULE_EXPORT CConnectionManager
{
public:
	inline CConnectionManager( QString const& sSQLiteDBPath = m_csSQLiteDBPath,
							   QString const& sDBFileName = m_csDataFileName );
	inline ~CConnectionManager() = default;

public:
	//
	//  Main Interface
	//
	inline QString GetSQLiteDatabasePath() const;
	inline QString GetDatabaseFileName() const;
	// Adds specified DB component to component list,
	// If bUseDefaultDB is true then the default database will be set to specified db component
	void AddDBComponent( t_IDBComponentSPtr pComponent, bool bUseDefaultDB = true );
	void RemoveDBComponent( t_IDBComponentSPtr pComponent );

private:
	// Creates and opens Default DB connection
	void InitDefaultDataBase();

private:
	//
	//	Content
	//
	QSqlDatabase    m_oDefaultDB;
	bool			m_bNeedsTableInit;
	QList<t_IDBComponentSPtr> m_lstDBComponents;
	// Usefull Constants
	QString m_sDataFileName;
	QString m_sSQLiteDBPath;

	// String Literals
	static const QLatin1String m_csSQLiteDBPath;
	static const QLatin1String m_csDataFileName;
};
typedef std::shared_ptr<CConnectionManager> t_ConnectionManagerSPtr;
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
inline CConnectionManager::CConnectionManager( QString const& sSQLiteDBPath,
											   QString const& sDBFileName )
											   : m_sDataFileName( sDBFileName ), 
											     m_sSQLiteDBPath( sSQLiteDBPath )
{
	Q_ASSERT( !sSQLiteDBPath.isEmpty() );
	Q_ASSERT( !m_sDataFileName.isEmpty() );
	InitDefaultDataBase();
}

inline QString CConnectionManager::GetSQLiteDatabasePath() const
{ return m_sSQLiteDBPath; }
inline QString CConnectionManager::GetDatabaseFileName() const
{ return m_sDataFileName; }

////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////

#endif // DB_CONNECTION_MANAGER_H
