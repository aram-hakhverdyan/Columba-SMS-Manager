#ifndef DB_COMPONENT_H
#define DB_COMPONENT_H

////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "module_global.h"
#include "se_error.h"
// Qt
#include <QSqlDataBase>
#include <QSqlError>
#include <QSqlQuery>
#include <QCoreApplication>
// STD
#include <memory>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// DB Macros
//

#define CREATE_QUERY			QSqlQuery __oQuery = IDBComponent::CreateQuery(); 
#define DECL_QUERY(_query_)		QSqlQuery _query_ = IDBComponent::CreateQuery(); 

#define EXECUTE_QUERY(_cmd_)            \
if (!__oQuery.exec( _cmd_ ))                 \
	throw se::CError( se::CError::DBError, QCoreApplication::translate( "DBQueryError", "DB Query Failed: " ) \
	.append( __oQuery.lastError( ).databaseText( ) ) );

// If execution fails throws an exception with error message
#define EXECUTE_QUERY_EX(_query_, _cmd_)            \
if (!_query_.exec( _cmd_ ))                      \
	throw se::CError( QCoreApplication::translate( "DBQueryError", "DB Query Failed: " ) \
	.append( _query_.lastError( ).databaseText( ) ) );

#define EXEC_QUERY(_query_)            \
if (!_query_.exec())                      \
	throw se::CError( QCoreApplication::translate( "DBQueryError", "DB Query Failed: " ) \
	.append( _query_.lastError( ).databaseText( ) ) );
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace db {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// DB Macros
//
//#define ADD_DB_COMPONENT( _pComponent_ ) g_lstDBComponents.append( _pComponent_ );
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	Interface IDBComponent
//
class IDBComponent
{
public:// Constructors
	inline IDBComponent( QSqlDatabase& oDB = QSqlDatabase() );
	virtual ~IDBComponent() = default;

public:
	//
	//	Interface Methodes
	//
	virtual void Initialize() = 0;
	// Get/Set database
	inline QSqlDatabase GetDatabase();
	inline void SetDatabase( QSqlDatabase const& );

protected:
	//
	//	Helper methods for derived classes
	//
	// Creates query object on member database
	inline QSqlQuery CreateQuery( QString const& sQuery = QString() );
	inline QSqlDatabase& Database();
	inline QSqlDatabase const& Database() const;
	// Query execution
	//inline void ExecQuery( QSqlQuery& oQuery, QString const& sQuery );
	inline QSqlQuery ExecQuery( QString const& sQuery );

private:
	//
	//	Content
	//
	QSqlDatabase m_oDB;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
// Constructor
inline IDBComponent::IDBComponent(QSqlDatabase& oDB) : m_oDB(oDB) {}
// Methods
inline QSqlDatabase IDBComponent::GetDatabase() { return m_oDB; }
inline void IDBComponent::SetDatabase( QSqlDatabase const& oDB ) { m_oDB = oDB; }
// for subclasses
inline QSqlQuery IDBComponent::CreateQuery( QString const& sQuery ) { return QSqlQuery( sQuery, m_oDB ); }
inline QSqlDatabase& IDBComponent::Database() { return m_oDB; }
inline QSqlDatabase const& IDBComponent::Database() const { return m_oDB; }
// queries
inline QSqlQuery IDBComponent::ExecQuery( QString const& sQuery )
{
	QSqlQuery oQuery = CreateQuery();
	EXECUTE_QUERY_EX( oQuery, sQuery );
	return oQuery;
}
////////////////////////////////////////////////////////////////////////////////
} // namespace db
////////////////////////////////////////////////////////////////////////////////
typedef std::shared_ptr<db::IDBComponent> t_IDBComponentSPtr;
////////////////////////////////////////////////////////////////////////////////
#endif // DB_COMPONENT_H