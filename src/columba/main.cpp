////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "_columba.h"
#include "app_application.h"
// Qt
#include <QMessageBox>
////////////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[])
{
	app::CColumbaApplication oApp( argc, argv );
	try
	{
		return oApp.exec();
	}
	catch (se::CError const& oErr)
	{
		QMessageBox oMsgBox;
		oMsgBox.setIcon( QMessageBox::Critical );
		oMsgBox.setWindowTitle( "Fatal Error" );
		oMsgBox.setText( "Columba terminated abnormally! : " + oErr.GetErrorMessage() );
		return oMsgBox.exec();
	}
	catch (std::exception const& oErr)
	{
		QMessageBox oMsgBox;
		oMsgBox.setIcon( QMessageBox::Critical );
		oMsgBox.setWindowTitle( "Columba: Fatal Error" );
		oMsgBox.setText( QString( "Columba terminated abnormally! : %1" ).arg( oErr.what() ) );
		return oMsgBox.exec();
	}
	catch (...)
	{
		QMessageBox oMsgBox;
		oMsgBox.setIcon( QMessageBox::Critical );
		oMsgBox.setWindowTitle( "Columba: Fatal Error" );
		oMsgBox.setText( "Columba terminated abnormally!" );
		return oMsgBox.exec();
	}
}
