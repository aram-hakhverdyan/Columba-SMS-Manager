////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "pb_animatedbutton.h"
#include "se_core_macros.h"
// Qt
#include <QGraphicsOpacityEffect>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace pb {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CAnimatedButton
//
CAnimatedButton::CAnimatedButton( QWidget* pParent )
	: Base( pParent ),
	m_pShowAnimation( nullptr ),
	m_pHideAnimation( nullptr )
{
	// Setup Opacity Effect on button
	// QWidget takes ownership of effect.
	QGraphicsOpacityEffect* pOpacityEffect = new QGraphicsOpacityEffect(); 
	setGraphicsEffect( pOpacityEffect );

	// Setup Show Animation
	m_pShowAnimation = new QPropertyAnimation( pOpacityEffect, "opacity", this );
	m_pShowAnimation->setStartValue( 0 );
	m_pShowAnimation->setEndValue( 1 );
	m_pShowAnimation->setEasingCurve( QEasingCurve::OutQuart );

	// Setup Hide Animation
	m_pHideAnimation = new QPropertyAnimation( pOpacityEffect, "opacity", this );
	m_pHideAnimation->setStartValue( 1 );
	m_pHideAnimation->setEndValue( 0 );
	m_pHideAnimation->setEasingCurve( QEasingCurve::OutQuart );
	SE_CONNECT( m_pHideAnimation, finished(), this, hide() );
}

void CAnimatedButton::SetShowDuration( int nShowDuration )
{
	SE_ASSERT( m_pShowAnimation );
	SE_ASSERT( nShowDuration >= 0 );
	m_pShowAnimation->setDuration( nShowDuration );
}

int CAnimatedButton::GetShowDuration() const
{
	SE_ASSERT( m_pShowAnimation );
	return m_pShowAnimation->duration();
}

void CAnimatedButton::SetHideDuration( int nHideDuration )
{
	SE_ASSERT( m_pHideAnimation );
	SE_ASSERT( nHideDuration >= 0 );
	m_pHideAnimation->setDuration( nHideDuration );
}

int CAnimatedButton::GetHideDuration() const
{
	SE_ASSERT( m_pHideAnimation );
	return m_pHideAnimation->duration();
}

// ShowAnimated
void CAnimatedButton::ShowAnimated()
{
	SE_ASSERT( m_pShowAnimation );
	SE_ASSERT( m_pHideAnimation );

	if (isVisible())
	{
		// Check if hide animation is running
		if (m_pHideAnimation->state() == QAbstractAnimation::State::Running //||
			/* m_pHideAnimation->state() == QAbstractAnimation::State::Paused*/)
		{
			// Perform switching to the Show animation
			SE_ASSERT( m_pHideAnimation->totalDuration() > 0 );
			if (m_pHideAnimation->totalDuration() <= 0)
				return;
			double dCurrentProgressPercent = double( m_pHideAnimation->currentTime() ) / m_pHideAnimation->totalDuration();

			int nCurrentShowTime = int( dCurrentProgressPercent * (m_pShowAnimation->totalDuration()) );
			// Stop Hide Animation
			// Disconnect to avoid from hide() slot call
			SE_DISCONNECT( m_pHideAnimation, finished(), this, hide() );
			m_pHideAnimation->stop();
			// Reconnect
			SE_CONNECT( m_pHideAnimation, finished(), this, hide() );

			m_pShowAnimation->start();
			m_pShowAnimation->setCurrentTime( nCurrentShowTime );
		}
	}
	else // not visible
	{
		show();
		m_pShowAnimation->start();
	}
}

// HideAnimated
void CAnimatedButton::HideAnimated()
{
	if (!isVisible())
		return;

	SE_ASSERT( m_pShowAnimation );
	SE_ASSERT( m_pHideAnimation );
	// Start animation
	
	// After the finish the hide slot will be called

	// Check if show animation is running
	if (m_pShowAnimation->state() == QAbstractAnimation::State::Running)
	{
		// Perform switching to the Show animation
		SE_ASSERT( m_pShowAnimation->totalDuration() > 0 );
		if (m_pShowAnimation->totalDuration() <= 0)
			return;
		double dCurrentProgressPercent = double( m_pShowAnimation->currentTime() ) / m_pShowAnimation->totalDuration();

		int nCurrentHideTime = int( dCurrentProgressPercent * (m_pShowAnimation->totalDuration()) );
		// Stop Show Animation
		m_pShowAnimation->stop();
		m_pHideAnimation->setCurrentTime( nCurrentHideTime );
	}	

	m_pHideAnimation->start();
}

void CAnimatedButton::showEvent( QShowEvent* pShowEvent )
{

}

void CAnimatedButton::hideEvent( QHideEvent* pHideEvent )
{

}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
