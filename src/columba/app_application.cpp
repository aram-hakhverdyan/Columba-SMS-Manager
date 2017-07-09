////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "app_application.h"
#include "se_core.h"
// Qt
#include <QStyleFactory>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace app {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CColumbaApplication
//
CColumbaApplication::CColumbaApplication( int& nArgc, char** cArgv )
	: QApplication( nArgc, cArgv ),
	m_pMainWindow( nullptr )
{
	setApplicationName( "Columba" );
	setOrganizationName( "ADCompany" );
	setApplicationVersion( "1.0.0" );
	setStyle( QStyleFactory::create( "Fusion" ) );

	SE_CONNECT( this, lastWindowClosed(), this, quit() );

	m_pMainWindow.reset( new CColumbaMainWindow() );
	m_pMainWindow->show();
}

// notify
bool CColumbaApplication::notify( QObject* pReceiver, QEvent* pEvent )
{
	bool bResult = false;
	try
	{
		bResult = Base::notify( pReceiver, pEvent );
	}
	catch (se::CError const& oErr)
	{
		QString sTitle = "Error"; // TODO
		if( m_pMainWindow ) 
			m_pMainWindow->onNotify( sTitle, oErr.GetErrorMessage(), app::ENotificationType::Error );
		return false;
	}
	catch (std::exception const& oErr)
	{	
		QString sTitle = "Error"; // TODO
		if (m_pMainWindow)
			m_pMainWindow->onNotify( sTitle, oErr.what(), app::ENotificationType::Error );
		return false;
	}
	catch (...)
	{
		QString sTitle = "Error"; // TODO
		if (m_pMainWindow)
			m_pMainWindow->onNotify( sTitle, tr( "Unhandled exception" ), app::ENotificationType::Error );
		return false;
	}
	return bResult;
}

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
} // namespace app
////////////////////////////////////////////////////////////////////////////////
