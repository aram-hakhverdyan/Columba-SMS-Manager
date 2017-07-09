////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_messageeditor.h"
// Qt
#include <QVBoxLayout>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CMessageEditor
//
CMessageEditor::CMessageEditor( QWidget* pParent )
	: Base( pParent ),
	  m_pEditor( nullptr )
{
	m_pEditor = new QPlainTextEdit();
	QVBoxLayout* pVLay = new  QVBoxLayout( this );
	pVLay->setMargin( 0 );
	pVLay->addWidget( m_pEditor );
	m_pEditor->setFrameStyle( 0 );
	m_pEditor->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

	// Font
	QFont oFont;
	oFont.setPointSize( 11 );
	oFont.setFamily( "Calibri" );
	setFont( oFont );
}

// onClear
void CMessageEditor::onClear()
{
	Q_ASSERT( m_pEditor );
	m_pEditor->clear();
}

// CSendButton
CSendButton::CSendButton( QWidget* pParent )
	: Base( pParent )
{
	setToolTip( tr( "Send" ) );
	//setText( tr( "Send" ) );
	//setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum ) );
	QSizePolicy sizePolicy4( QSizePolicy::Minimum, QSizePolicy::Preferred );
	/*sizePolicy4.setHorizontalStretch( 0 );
	sizePolicy4.setVerticalStretch( 0 );
	sizePolicy4.setHeightForWidth( sizePolicy().hasHeightForWidth() );*/
	setSizePolicy( sizePolicy4 );
	setStyleSheet( QLatin1String( "QPushButton:hover{ background: #71B7FE; }\n"
		"\n"
		"QPushButton {\n"
		"background: #3F9FFE;\n"
		"border: none;\n"
		"}\n"
		"\n"
		"QPushButton:pressed {\n"
		"        background: rgb(53,134,203);\n"
		"        padding-top: 2px;\n"
		"        padding-left: 3px;\n"
		"}\n"
		"\n"
		"QPushButton:on {\n"
		"        background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 :   1, stop :   0.0 #5AA72D,\n"
		"        stop :   0.5 #B3E296, stop :   0.55 #B3E296, stop :   1.0 #f5f9ff);\n"
		"        padding-top: 2px;\n"
		"        padding-left: 3px;\n"
		"}\n"
		"\n"
		"QPushButton:disabled {\n"
		"        background: rgb(140,140,140);\n"
		"        color: black;\n"
		"}\n"
		"" ) );
	/*QIcon icon;
	icon.addFile( QStringLiteral( ":/columba/Resources/ebd6b3a4b793ae3c20c1d719077ad8bf.png" ), QSize(), QIcon::Normal, QIcon::Off );*/
	QIcon icon;
	icon.addFile( QStringLiteral( ":/columba/Resources/sendo.png" ), QSize(), QIcon::Normal, QIcon::Off );

	setIcon( icon );
	setIconSize( QSize( 25, 40) );
	//setAutoDefault( true );
	//setFixedHeight( 70 );
}

//QSize CSendButton::sizeHint() const
//{
//	return QSize( 40, 60 );
//}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
