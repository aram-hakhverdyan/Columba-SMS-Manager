////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "pb_profilephotoviewer.h"
#include "se_core_macros.h"
// Qt
#include <QVBoxLayout>

#include <QPainter>
#include <QPaintEvent>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace pb {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CPhotoButton
//
CPhotoButton::CPhotoButton( QWidget* pParent )
	: Base( pParent )
{
	setStyleSheet( QLatin1String( "QPushButton:hover{image: url(:/phonebook/Resources/camera2.png);}\n"
		"\n"
		"QPushButton { \n"
		"		image: url(:/phonebook/Resources/camera.png); \n"
		"		border: none;\n"
		"		}\n"
		"	\n"
		"		QPushButton:pressed {\n"
		"		        padding-top: 1px;\n"
		"		        padding-left: 2px;\n"
		"		}\n"
		"\n"
		"\n" ) );
}

QSize CPhotoButton::sizeHint() const
{
	return QSize( 50, 50 );
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CProfilePhotoViewer
//
CProfilePhotoViewer::CProfilePhotoViewer( QWidget* pParent )
	: Base( pParent ),
	m_pPhotoLabel( nullptr ),
	m_pbtnSelectPhoto( nullptr )
{
	// Configure Photo Label
	m_pPhotoLabel = new QLabel( this );
	//pLay->addWidget( m_pPhotoLabel );
	m_pPhotoLabel->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
	//m_pPhotoLabel->setScaledContents( true );

	// Configure Frame
	m_pPhotoLabel->setFrameStyle( QFrame::Box | QFrame::Sunken);
	m_pPhotoLabel->setLineWidth( 2 );
	m_pPhotoLabel->setMidLineWidth( 1 );

	// Configure Photo Button
	m_pbtnSelectPhoto = new CPhotoButton( this );
	m_pbtnSelectPhoto->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	m_pbtnSelectPhoto->setToolTip( tr( "Change Profile Photo" ) );
	m_pbtnSelectPhoto->SetShowDuration( 500 );
	m_pbtnSelectPhoto->SetHideDuration( 500 );
	SE_CONDUCT( m_pbtnSelectPhoto, clicked(), this, sigSelectPhotoClicked() );


	// Layout Photo Button
	QVBoxLayout* pVerticalLayout = new QVBoxLayout( this );
	pVerticalLayout->setMargin( m_cnButtonMargin );
	// Vertical Spicer
	QSpacerItem* pVerticalSpacer = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
	pVerticalLayout->addItem( pVerticalSpacer );

	QHBoxLayout* pHorizontalLayout = new QHBoxLayout();
	pHorizontalLayout->setMargin( m_cnButtonMargin );
	// Horizontal Spicer
	QSpacerItem* pHorizontalSpacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

	pHorizontalLayout->addItem( pHorizontalSpacer );
	pHorizontalLayout->addWidget( m_pbtnSelectPhoto );
	pVerticalLayout->addLayout( pHorizontalLayout );

	m_pbtnSelectPhoto->hide();

}

// SetPhoto
void CProfilePhotoViewer::SetPhoto( QPixmap const& oPhoto )
{
	SE_ASSERT( m_pPhotoLabel );
	//
	//setFixedSize( -1, -1 );
	m_pPhotoLabel->setPixmap( oPhoto );
	m_pPhotoLabel->adjustSize();
	setFixedSize( m_pPhotoLabel->size() );

	/*int nButtonMargin = 8;
	int nXRight = m_pPhotoLabel->frameRect().right();
	int nXWidth = m_pbtnSelectPhoto->width();

	int nBtnX = nXRight - nButtonMargin - nXWidth;
	int nBtnY = m_pPhotoLabel->rect().top() + m_pPhotoLabel->frameRect().bottom() - nButtonMargin - m_pbtnSelectPhoto->height();*/
	//m_pbtnSelectPhoto->move( nBtnX, nBtnY );
}

// GetPhoto
QPixmap const* CProfilePhotoViewer::GetPhoto() const
{
	SE_ASSERT( m_pPhotoLabel );
	return m_pPhotoLabel->pixmap();
}

// resizeEvent
void CProfilePhotoViewer::enterEvent( QEvent* pEvent )
{
	SE_ASSERT( m_pbtnSelectPhoto );
	m_pbtnSelectPhoto->ShowAnimated();
}

// paintEvent
void CProfilePhotoViewer::leaveEvent( QEvent* pEvent )
{
	SE_ASSERT( m_pbtnSelectPhoto );
	m_pbtnSelectPhoto->HideAnimated();
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
