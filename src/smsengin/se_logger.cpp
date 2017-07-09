////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "se_logger.h"
//	Qt Includes
#include <QDir>
#include <QDateTime>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin    
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CATSmsEngine
//
//! Constructor
CLoger::CLoger( QString const& sDomain,
				QString const& sDefaultFileName )
				: m_sDomain( sDomain ),
				m_sDefaultFileName(sDefaultFileName)
{
	if (sDomain.isEmpty() || sDefaultFileName.isEmpty())
		return;

	QDir oTemp = QDir::temp();
	oTemp.mkdir( sDomain );
	oTemp.cd( sDomain );
	if (oTemp.exists())
	{
		QString sFile = oTemp.path() + "/" + sDefaultFileName;
		QFile oLogFile( sFile );
	}
}

void CLoger::Log( QString const& sData )
{
	QString sFile = QDir::tempPath() + "/" + m_sDomain + "/" + m_sDefaultFileName;
	QFile oLogFile( sFile );
	if (!sData.isEmpty() && oLogFile.open( QIODevice::Append ))
	{
		QString sSep( 81, QChar( '-' ) );
		oLogFile.write( sSep.toLocal8Bit() + "\n" );
		oLogFile.write( sData.toLatin1() );
		oLogFile.write( "\nDateTime: " +
						QDateTime::currentDateTime().toString().toLatin1() + "\n" );
		oLogFile.write( sSep.toLocal8Bit() + "\n" );
	}
}

void CLoger::StartHeader( QString const& sHeader )
{

	QString sFile = QDir::tempPath() + "/" + m_sDomain + "/" + m_sDefaultFileName;
	QFile oLogFile( sFile );
	if (!sHeader.isEmpty() && oLogFile.open( QIODevice::Append ))
	{
		QString sSep( 81, QChar( '#' ) );
		oLogFile.write( sSep.toLocal8Bit() + "\n" );
		oLogFile.write( "#\n" );
		oLogFile.write( "#\n	" + sHeader.toLatin1() + "\n" );
		oLogFile.write( "#\n" );

		oLogFile.write( "\nDateTime: " +
						QDateTime::currentDateTime().toString().toLatin1() + "\n" );
		oLogFile.write( sSep.toLocal8Bit() + "\n" );
	}
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////
