////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_bubble.h"
#include "se_core.h"
// Qt
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPaintEvent>
#include <QVBoxLayout>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CBasicBubble
//
CBasicBubble::CBasicBubble( QString const& sText, 
							EBubbleOrientation eOrientation,
							QWidget* pParent )
	: QWidget( pParent, Qt::FramelessWindowHint ),
	  m_nMaxWidth( 400 ),
	  m_eOrientation( eOrientation ),
	  m_bIsSelected( false )
{
	InitUI();
	setSizePolicy( QSizePolicy( 
		QSizePolicy::Expanding, 
		QSizePolicy::Minimum ) );
	// Set Static text
	m_oStaticText.setPerformanceHint( QStaticText::AggressiveCaching );
	m_oStaticText.setTextWidth( m_nMaxWidth );
	m_oStaticText.setText( sText );
	UpdateSizeHint();
	resize( m_oSizeHint );
}

// InitUI
void CBasicBubble::InitUI()
{
	// Set Widget Blur
	//setAttribute( Qt::WA_TranslucentBackground );
	/*QGraphicsDropShadowEffect* pEffect = new QGraphicsDropShadowEffect( );
	pEffect->setBlurRadius( 50 );
	pEffect->setOffset( 3, 3 );
	setGraphicsEffect( pEffect );*/

	// Init Brush
	m_oBubbleBrush.setColor( QColor( 242, 242, 242/*233, 255, 222*/ ) );
	m_oBubbleBrush.setStyle( Qt::SolidPattern );

	// Init Font
	QFont oFont;
	oFont.setPointSize( 11 );
	oFont.setFamily( "Calibri" );
	SetFont( oFont );

	// Init Text Color
	m_oTextColor = Qt::black;
}

// sizeHint
QSize CBasicBubble::sizeHint() const
{
	return m_oSizeHint;
}

// GetTextRect
QRectF CBasicBubble::GetTextRect() const
{
	// Get Margins
	int nLeftMargin = 0, nTopMargin = 0, nRightMargin = 0, nBottomMargin = 0;
	GetMargin( nLeftMargin, nTopMargin, nRightMargin, nBottomMargin );

	QRect oWidgetRect = rect();
	QSize oTextSize = m_oStaticText.size().toSize();
	QRect oTextRect;
	if (m_eOrientation == EBubbleOrientation::Left)
	{

		oTextRect.setRect( oWidgetRect.left() + nLeftMargin,
						   oWidgetRect.top() + nTopMargin,
						   oTextSize.width(),
						   oTextSize.height() );
	}
	else // Right
	{
		oTextRect.setRect( oWidgetRect.right() - nRightMargin - oTextSize.width(),
						   oWidgetRect.top() + nTopMargin,
						   oTextSize.width(),
						   oTextSize.height() );
	}
	return oTextRect;
}

// paintEvent
void CBasicBubble::paintEvent( QPaintEvent* pEvent )
{
	QPainter oPainter( this );
	// Set Brush
	QBrush oBrush = m_oBubbleBrush;
	if (m_bIsSelected)
	{
		QColor oSelectionColor = oBrush.color().darker( 130 );
		oBrush.setColor( oSelectionColor );
	}
	oPainter.setBrush( oBrush );
	// Setup Pen
	QPen oPen = oPainter.pen( );
	oPen.setColor( m_oBubbleBrush.color() );
	//oPen.setWidth( 1 );
	oPainter.setPen( oPen );
	// Set Font
	oPainter.setFont( m_oFont );
	// Set Render Hint
	oPainter.setRenderHint( QPainter::RenderHint::Antialiasing );

	QRect oEventRect = /*pEvent->rect();*/ rect();
	
	//

	
	QRect oTextRect = GetTextRect().toRect();
	// Drow Background
	oPainter.save();
	DrawBackground( oPainter, oEventRect, oTextRect );
	oPainter.restore();
	
	// Set Text Color
	oPen = oPainter.pen();
	oPen.setColor( m_oTextColor );
	oPainter.setPen( oPen );
	// Draw Text
	oPainter.drawStaticText( oTextRect.topLeft(), m_oStaticText );
}

//// GetMargin
//void CBasicBubble::GetMargin( int& nLeft, int& nTop,
//							  int& nRight, int& nBottom )
//{
//	nLeft = 5;
//	nTop = 5;
//	nRight = 5;
//	nBottom = 5;
//}
	
// GetBackgroundVisualRect
QRectF CBasicBubble::GetBackgroundVisualRect( QRect const& oWidgetRect,
											  QRect const& oTextRect ) const
{
	// Get Margins
	int nLeftMargin = 0, nTopMargin = 0, nRightMargin = 0, nBottomMargin = 0;
	GetMargin( nLeftMargin, nTopMargin, nRightMargin, nBottomMargin );
	QRect oBkRect;
	if (m_eOrientation == EBubbleOrientation::Left)
	{
		oBkRect.setTopLeft( oWidgetRect.topLeft() );
		oBkRect.setBottomRight( oTextRect.bottomRight() + QPoint( nRightMargin, nBottomMargin ) );
	}
	else // Right
	{
		oBkRect.setTop( oWidgetRect.top() );
		oBkRect.setLeft( oWidgetRect.right() - nRightMargin - nLeftMargin - oTextRect.width() );
		oBkRect.setBottomRight( oWidgetRect.bottomRight() );
	}
	return oBkRect;
}

bool CBasicBubble::Contains( QPoint const& oPos,
							 QRect const& oWidgetRect,			  
							 QRect const& oTextRect ) const
{
	// Get Margins
	
	QRectF oBkRect = GetBackgroundVisualRect( oWidgetRect, oTextRect );
	return oBkRect.contains( oPos );
}

// DrawBackground
void CBasicBubble::DrawBackground( QPainter& oPainter,
					 QRect const& oWidgetRect,
					 QRect const& oTextRect )
{
	QRectF oBkRect = GetBackgroundVisualRect( oWidgetRect, oTextRect );
	oPainter.drawRoundRect( oBkRect, 5, 15 );



	/*oPainter.setBrush( QBrush() );
	QPen oPen = oPainter.pen();
	oPen.setColor( Qt::red );
	oPen.setWidth( 3 );
	oPainter.setPen( oPen );

	oPainter.drawRect( geometry() );

	oPen = oPainter.pen( );
	oPen.setColor( Qt::blue );
	oPainter.setPen( oPen );

	oPainter.drawRect( oWidgetRect );
*/
}

void CBasicBubble::resizeEvent( QResizeEvent* pEvent )
{
	int nLeftMargin = 0, nTopMargin = 0, nRightMargin = 0, nBottomMargin = 0;
	GetMargin( nLeftMargin, nTopMargin, nRightMargin, nBottomMargin );
	int nWidth = pEvent->size().width();
	int nMaxWidth = nWidth - nLeftMargin - nRightMargin;
	SetMaxWidth( nMaxWidth );
}


// mousePressEvent
void CBasicBubble::mousePressEvent( QMouseEvent* pEvent )
{
	if (pEvent->button() == Qt::LeftButton && Contains( pEvent->pos() ))
		emit sigClicked();
}

// mouseReleaseEvent
//void CBasicBubble::mouseReleaseEvent( QMouseEvent* pEvent )
//{
//	
//}

// UpdateSizeHint
void CBasicBubble::UpdateSizeHint( )
{
	m_oStaticText.setTextWidth( -1 );
	QSize oTextSize = m_oStaticText.size( ).toSize( );
	if (oTextSize.width( ) > m_nMaxWidth)
	{/*093214908 18 89*/
		m_oStaticText.setTextWidth( m_nMaxWidth );
		oTextSize = m_oStaticText.size( ).toSize( );
	}
	// Get Margins
	int nLeftMargin = 0, nTopMargin = 0, nRightMargin = 0, nBottomMargin = 0;
	GetMargin( nLeftMargin, nTopMargin, nRightMargin, nBottomMargin );
	// Calc Min Size
	int nBubbleMinWidth = nLeftMargin + nRightMargin + oTextSize.width( );
	int nBubbleMinHeight = nTopMargin + nBottomMargin + oTextSize.height( );
	// Set Min Size
	QSize oMinSize( nBubbleMinWidth, nBubbleMinHeight );
	setMinimumHeight( oMinSize.height( ) );
	// Control Minimum Width
	if (oTextSize.width() > m_nMaxWidth)
		setMinimumWidth( oMinSize.width() );
	// Set Min Size as size hint
	m_oSizeHint = oMinSize;
	//setMinimumSize( oMinSize );
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CBasicBubble
//
CTimeBubble::CTimeBubble( QString const& sText,
			 QString const& sTime,
			 EBubbleOrientation eOrientation,
			 QWidget* pParent )
{
	m_pBubble = new CBasicBubble( sText, eOrientation );
	QVBoxLayout* pVLay = new QVBoxLayout();
	pVLay->setContentsMargins( 4, 4, 4, 4 );
	pVLay->addWidget( m_pBubble );
	m_pTimeLabel = new QLabel( sTime );
	QFont oFont = m_pBubble->GetFont( );
	oFont.setPointSize( oFont.pointSize( ) - 2 );
	m_pTimeLabel->setFont( oFont );
	Qt::Alignment eAlg = eOrientation == EBubbleOrientation::Left ?
		Qt::AlignLeft : Qt::AlignRight;
	m_pTimeLabel->setAlignment( eAlg );
	m_pTimeLabel->setStyleSheet( "color: #5C5C5C" );
	pVLay->addWidget( m_pTimeLabel );
	setLayout( pVLay );

	SE_CONDUCT( m_pBubble, sigClicked(), this, sigClicked() );
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
