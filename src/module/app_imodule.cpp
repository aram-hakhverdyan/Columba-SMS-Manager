////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "app_imodule.h"

// Qt
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace app {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class IModule
//
QString IModule::Topic() const
{
	return GetName();
}

QWidget* IModule::GetWidget()
{
	return nullptr;
}

QIcon    IModule::GetIcon() const
{
	return QIcon();

}
int  IModule::GetDockWidgetsList( QList<QDockWidget*>& )
{
	return 0;
}

QList<QActionGroup*> IModule::GetActionList()
{
	return QList<QActionGroup*>();
}

t_IDBComponentSPtr IModule::GetDBComponent()
{
	return t_IDBComponentSPtr();
}

void IModule::Initialize()
{
	// Nothing to do
}

void IModule::SaveSettings( QSettings* pSettings )
{
	Q_UNUSED( pSettings );
}

void IModule::LoadSettings( QSettings const* pSettings )
{
	Q_UNUSED( pSettings );
}

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
} // namespace app
////////////////////////////////////////////////////////////////////////////////