#ifndef GUI_BUBBLE_H
#define GUI_BUBBLE_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_common.h"
// Qt
#include <QWidget>
#include <QStaticText>
#include <QLabel>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CBasicBubble
//
class CBasicBubble : public QWidget
{
	Q_OBJECT

public:
	// Constructor & Destructor
	CBasicBubble( QString const& sText, 
				  EBubbleOrientation eOrientation = EBubbleOrientation::Left,
				  QWidget* pParent = nullptr );
	virtual ~CBasicBubble() = default;

public:
	//
	//	Set/Get Methods
	// 
	inline void SetText( QString const& sText );
	inline QString GetText() const;
	
	inline void SetMaxWidth( int nWidth );
	inline int GetMaxWidth() const;

	inline void SetFont( QFont const& oFont );
	inline QFont GetFont() const;

	inline void SetTextColor( QColor const& oColor );
	inline QColor GetTextColor() const;

	inline void SetBubbleBrush( QBrush const& oBrush );
	inline QBrush GetBubbleBrush() const;

	inline void SetOrientation( EBubbleOrientation eOrientation );
	inline EBubbleOrientation GetOrientation() const;

	inline void SetSelected( bool bSelected = true );
	inline bool IsSelected() const;

	inline bool Contains( QPoint const& oPos ) const;

public:
	// 
	//	Overidden Methods
	//
	QSize sizeHint() const override;

signals:
	void sigClicked();

protected:
	void paintEvent( QPaintEvent* pEvent ) override;
	void resizeEvent( QResizeEvent* pEvent ) override;
	void mousePressEvent( QMouseEvent* pEvent );
	//void mouseReleaseEvent( QMouseEvent* pEvent );

protected:
	//
	//	Polimorphic Methods
	//
	// Returns margin between widget and text rectangle
	inline /*virtual*/ void GetMargin( int& nLeft,
							int& nTop,
							int& nRight,
							int& nBottom ) const;
	QRectF GetTextRect() const;
	// Draws bubbles background
	virtual void DrawBackground( QPainter& oPainter,
							 QRect const& oWidgetRect,
							 QRect const& oTextRect );

	virtual bool Contains( QPoint const& oPos,
						   QRect const& oWidgetRect,
						   QRect const& oTextRect ) const;

protected:
	//
	//	Tools
	//
	void InitUI();
	void UpdateSizeHint();
	QRectF GetBackgroundVisualRect( QRect const& oWidgetRect,
									QRect const& oTextRect ) const;

private:
	//
	//	Content
	//
	int		m_nMaxWidth;
	EBubbleOrientation m_eOrientation;
	QFont	m_oFont;
	QColor	m_oTextColor;
	QBrush	m_oBubbleBrush;
	QSize   m_oSizeHint;
	QStaticText m_oStaticText;
	bool	m_bIsSelected;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CTimeBubble
//
class CTimeBubble : public QWidget
{
	Q_OBJECT

public:
	// Constructor & Destructor
	CTimeBubble( QString const& sText,
				 QString const& sTime,
				 EBubbleOrientation eOrientation = EBubbleOrientation::Left,
				 QWidget* pParent = nullptr );
	virtual ~CTimeBubble( ) = default;

public:
	//
	//	Main Interface
	//
/*	inline CBasicBubble& Bubble();
	inline CBasicBubble const& Bubble( ) const;
*/
	inline void SetText( QString const& sText );
	inline QString GetText( ) const;
	
	inline void SetFont( QFont const& oFont );
	inline QFont GetFont( ) const;

	inline void SetTextColor( QColor const& oColor );
	inline QColor GetTextColor( ) const;

	inline void SetBubbleBrush( QBrush const& oBrush );
	inline QBrush GetBubbleBrush( ) const;

	inline void SetOrientation( EBubbleOrientation eOrientation );
	inline EBubbleOrientation GetOrientation( ) const;

	inline void SetSelected( bool bSelected = true );
	inline bool IsSelected() const;

	inline void SetTime( QString const& sTime );
	inline QString GetTime() const; 

	inline void SetTimeColor( QColor const& oColor );
	inline QColor GetTimeColor() const;

	inline bool Contains( QPoint const& oPos ) const;

signals:
	void sigClicked();

private:
	//
	//	Content
	//
	CBasicBubble* m_pBubble;
	QLabel*       m_pTimeLabel;
};
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////


// GetMargin
inline void CBasicBubble::GetMargin( int& nLeft, int& nTop,
							  int& nRight, int& nBottom ) const
{
	nLeft = 5;
	nTop = 5;
	nRight = 5;
	nBottom = 5;
}

inline void CBasicBubble::SetText( QString const& sText )
{
	m_oStaticText.setText( sText );
	UpdateSizeHint();
}

inline QString CBasicBubble::GetText( ) const
{ return m_oStaticText.text(); }

inline void CBasicBubble::SetMaxWidth( int nWidth )
{
	m_nMaxWidth = nWidth < 20 ? 20 : nWidth;
	UpdateSizeHint();
}

inline int CBasicBubble::GetMaxWidth( ) const
{ return m_nMaxWidth; }

inline void CBasicBubble::SetFont( QFont const& oFont )
{
	m_oFont = oFont;
	m_oStaticText.prepare( QTransform( ), m_oFont );
	UpdateSizeHint();
}


inline QFont CBasicBubble::GetFont() const
{ return m_oFont; }

inline void CBasicBubble::SetTextColor( QColor const& oColor )
{ m_oTextColor = oColor; }

inline QColor CBasicBubble::GetTextColor() const
{ return m_oTextColor; }

inline void CBasicBubble::SetBubbleBrush( QBrush const& oBrush )
{ m_oBubbleBrush = oBrush; }

inline QBrush CBasicBubble::GetBubbleBrush() const
{ return m_oBubbleBrush; }

inline void CBasicBubble::SetOrientation( EBubbleOrientation eOrientation )
{ m_eOrientation = eOrientation; }

inline EBubbleOrientation CBasicBubble::GetOrientation() const
{ return m_eOrientation; }

inline void CBasicBubble::SetSelected( bool bSelected )
{
	m_bIsSelected = bSelected; 
	update();
}

inline bool CBasicBubble::IsSelected() const
{ return m_bIsSelected; }

// Contains
inline bool CBasicBubble::Contains( QPoint const& oPos ) const
{
	QRect oWidgetRect = rect();
	QRect oTextRect = GetTextRect().toRect();
	return Contains( oPos, oWidgetRect, oTextRect );
}
//
//	class CTimeBubble
//
//inline CBasicBubble& CTimeBubble::Bubble()
//{
//	Q_ASSERT( m_pBubble );
//	return *m_pBubble;
//}
//
//inline CBasicBubble const& CTimeBubble::Bubble( ) const
//{
//	Q_ASSERT( m_pBubble );
//	return *m_pBubble;
//}

inline void CTimeBubble::SetTime( QString const& sTime )
{ m_pTimeLabel->setText( sTime ); }

inline QString CTimeBubble::GetTime() const
{ return m_pTimeLabel->text(); }

inline void CTimeBubble::SetFont( QFont const& oFont )
{ m_pBubble->SetFont( oFont ); }

inline QFont CTimeBubble::GetFont() const
{ return m_pBubble->GetFont(); }

inline void CTimeBubble::SetTextColor( QColor const& oColor )
{ m_pBubble->SetTextColor( oColor ); }

inline QColor CTimeBubble::GetTextColor() const
{ return m_pBubble->GetTextColor(); }

inline void CTimeBubble::SetBubbleBrush( QBrush const& oBrush )
{ m_pBubble->SetBubbleBrush( oBrush ); }
inline QBrush CTimeBubble::GetBubbleBrush() const
{ return m_pBubble->GetBubbleBrush(); }

inline void CTimeBubble::SetOrientation( EBubbleOrientation eOrientation )
{
	m_pBubble->SetOrientation( eOrientation );
	Qt::Alignment eAlg = eOrientation == EBubbleOrientation::Left ?
		Qt::AlignLeft : Qt::AlignRight;
	m_pTimeLabel->setAlignment( eAlg );
}

inline EBubbleOrientation CTimeBubble::GetOrientation() const
{ return m_pBubble->GetOrientation(); }

inline void CTimeBubble::SetSelected( bool bSelected )
{ m_pBubble->SetSelected( bSelected ); }
inline bool CTimeBubble::IsSelected() const
{ return m_pBubble->IsSelected(); }


inline void CTimeBubble::SetTimeColor( QColor const& oColor )
{
	m_pTimeLabel->setStyleSheet( QString( "color: rgb(%1, %2, %3)" )
								 .arg( oColor.red() )
								 .arg( oColor.green() )
								 .arg( oColor.blue() ) );
}

inline QColor CTimeBubble::GetTimeColor( ) const
{
	Q_ASSERT( false );
	return QColor();
}

inline bool CTimeBubble::Contains( QPoint const& oPos ) const
{
	return m_pBubble->Contains( oPos );
}

inline void CTimeBubble::SetText( QString const& sText )
{
	Q_ASSERT( m_pBubble );
	m_pBubble->SetText( sText );
}

inline QString CTimeBubble::GetText( ) const
{
	Q_ASSERT( m_pBubble );
	return m_pBubble->GetText();
}

////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_BUBBLE_H
