#ifndef APP_I_MODULE_H
#define APP_I_MODULE_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "module_global.h"
#include "db_component.h"
// Qt
#include <QString>
#include <QIcon>
#include <QDockWidget>
#include <QActionGroup>
#include <QSettings>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Forward Declarations
//
class QWidget;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace app { // application
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Common Definitions
//
enum class ENotificationType
{
	Error = 0,
	Warning,
	Information
};

//enum class EConnectionStatusOptions
//{
//	eError = 0,
//	eWarning,
//	eInformation
//};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class IModule
//
class MODULE_EXPORT IModule : public QObject
{
	Q_OBJECT

public:
	inline IModule( QObject* pParent = nullptr );
	virtual ~IModule() = default;

public:
	//
	//	Main Interface
	//	
	//virtual void SetName( QString const& );
	virtual QString				GetName() const = 0;
	virtual QString				Topic() const;
	virtual QWidget*			GetWidget();
	virtual QIcon				GetIcon() const;
	virtual int					GetDockWidgetsList( QList<QDockWidget*>& );
	virtual QList<QActionGroup*>GetActionList();
	virtual t_IDBComponentSPtr	GetDBComponent();
	virtual void				Initialize();
	// Save / Load Settings
	virtual void SaveSettings( QSettings* pSettings );
	virtual void LoadSettings( QSettings const* pSettings );

signals:
	void sigNotify( QString const& sTitle, 
				    QString const& sText, 
				    ENotificationType eType );
	// Null string will mean to turn off the status report
	void sigReportStatus( QString const& sStatus = QString() );

	//virtual bool packData( QDataStream&, int = 0 );
	//virtual bool unpackData( QDataStream&, int = 0 );
	//virtual void setWinManager( WindowManager* );
	//virtual bool isSerializable();
	//virtual void setParentModule( SYMCoreInterrogating* pParent );
	//virtual SYMCoreInterrogating* parentModule();
	//virtual void setSettingsManager( ISettingsManager* pSettingsManager );
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
inline IModule::IModule( QObject* pParent ) : QObject( pParent ) 
{}
////////////////////////////////////////////////////////////////////////////////
} // namespace application
////////////////////////////////////////////////////////////////////////////////

#endif // APP_I_MODULE_H
