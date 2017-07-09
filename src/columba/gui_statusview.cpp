////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_statusview.h"
// Qt
#include <QPainter>
#include <QPaintEvent>
#include <QFormLayout>
#include <QCoreApplication>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


QString toString( EConnectionStatus eStatus )
{
	QString sResult;
	switch (eStatus)
	{
		case gui::EConnectionStatus::Undefined:
			break;
		case gui::EConnectionStatus::FailedToConnect:
			sResult = QCoreApplication::translate( "EConnectionStatus", "Not connected" );
			break;
		case gui::EConnectionStatus::Connected:
			sResult = QCoreApplication::translate( "EConnectionStatus", "Connected" );
			break;
		case gui::EConnectionStatus::Connecting:
			sResult = QCoreApplication::translate( "EConnectionStatus", "Connecting" );
			break;
		case gui::EConnectionStatus::Disconnecting:
			sResult = QCoreApplication::translate( "EConnectionStatus", "Disconnecting" );
			break;
		case gui::EConnectionStatus::Disconnected:
			sResult = QCoreApplication::translate( "EConnectionStatus", "Disconnected" );
			break;
		default:
			break;
	}
	return sResult;
}

////////////////////////////////////////////////////////////////////////////////
//
//	class CStatusCircle
//
CStatusCircle::CStatusCircle( EConnectionStatus eReady, QWidget* pParent )
	: QWidget( pParent ),
	m_nPadding( 1 )
{}


// paintEvent
void CStatusCircle::paintEvent( QPaintEvent* pEvent )
{
	if (m_eStatus == EConnectionStatus::Undefined)
		// Not draw undefined status
		return;
	QPainter oPainter( this );
	// Set Brush
	QBrush oBrush = oPainter.brush();
	oBrush.setStyle( Qt::SolidPattern );
	if (m_eStatus == EConnectionStatus::Connected)
		oBrush.setColor( Qt::green );
	else if (m_eStatus == EConnectionStatus::FailedToConnect)
		oBrush.setColor( Qt::red );
	else if (m_eStatus == EConnectionStatus::Disconnected)
		oBrush.setColor( QColor( 225, 175, 65 ) );
	else if (m_eStatus == EConnectionStatus::Connecting ||
			 m_eStatus == EConnectionStatus::Disconnecting)
		oBrush.setColor( Qt::cyan /*QColor( 225, 175, 65 )*/ );
	else // Undefined
		// For undefined status not draw anything, just return
		return;

	oPainter.setBrush( oBrush );
	// Setup Pen
	QPen oPen = oPainter.pen();
	oPen.setColor( oBrush.color() );
	//oPen.setWidth( 1 );
	oPainter.setPen( oPen );
	// Set Render Hint
	oPainter.setRenderHint( QPainter::RenderHint::Antialiasing );

	QRect oEventRect = pEvent->rect();
	//QPoint oTopLeft = oEventRect.topLeft() + QPoint( m_nPadding, m_nPadding );
	int nRadius = qMin( oEventRect.width(), oEventRect.height() )/2 - m_nPadding;
	//QRect oCircleRect( oTopLeft.x(), oTopLeft.y(), nWidth, nWidth );
	// Draw Circle
	QPoint oCenter = oEventRect.center();
	oPainter.drawEllipse( oCenter, nRadius, nRadius );
}

//QSize CStatusCircle::sizeHint() const
//{
//	return QSize( 10, 10 );
//}


////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	class CStatusView
//
CStatusView::CStatusView( QWidget* pParent )
	: QWidget( pParent ),
	m_pStatus( nullptr ),
	m_pTextLabel( nullptr )
{
	QFormLayout* pPormLay = new QFormLayout();
	m_pStatus = new CStatusCircle( EConnectionStatus::Undefined, this );
	m_pStatus->setFixedSize( 12, 12 );
	// Text Label
	m_pTextLabel = new QLabel();
	//m_pTextLabel->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Fixed );

	pPormLay->setWidget( 0, QFormLayout::FieldRole, m_pTextLabel );
	pPormLay->setWidget( 0, QFormLayout::LabelRole, m_pStatus );
	setLayout( pPormLay );
}

void CStatusView::SetReady( QString const& sText )
{
	
	m_pStatus->SetStatus( EConnectionStatus::Connected );
	//m_pTextLabel->setStyleSheet( "color: green" );
	m_pTextLabel->setText(sText);
}

void CStatusView::SetFailed( QString const& sText )
{
	m_pStatus->SetStatus( EConnectionStatus::FailedToConnect );
	//m_pTextLabel->setStyleSheet( "color: red" );
	m_pTextLabel->setText( sText );
}

void CStatusView::SetDisconneced( QString const& sText )
{
	m_pStatus->SetStatus( EConnectionStatus::Disconnected );
	//m_pTextLabel->setStyleSheet( "color: red" );
	m_pTextLabel->setText( sText );
}

void CStatusView::SetWaiting( QString const& sText )
{
	m_pStatus->SetStatus( EConnectionStatus::Connecting );
	//m_pTextLabel->setStyleSheet( "color: black" );
	m_pTextLabel->setText( sText );
}

void CStatusView::SetUndefined( QString const& sText )
{
	m_pStatus->SetStatus( EConnectionStatus::Undefined );
	//m_pTextLabel->setStyleSheet( "color: black" );
	m_pTextLabel->setText( sText );
}


////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
