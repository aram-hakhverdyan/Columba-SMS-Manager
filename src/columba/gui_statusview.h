#ifndef GUI_STATUS_VIEW_H
#define GUI_STATUS_VIEW_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
// Qt
#include <QLabel>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

enum class EConnectionStatus
{
	Undefined = 0,
	FailedToConnect,
	Connected,
	Connecting,
	Disconnecting,
	Disconnected
};
QString toString( EConnectionStatus eStatus );

////////////////////////////////////////////////////////////////////////////////
//
//	class CStatusCircle
//
class CStatusCircle : public QWidget
{
public:
	

public:
	// Constructor & Destructor
	CStatusCircle( EConnectionStatus eReady = EConnectionStatus::Undefined, QWidget* pParent = nullptr );
	virtual ~CStatusCircle() = default;

public:
	//
	//	Set/Get Methods
	// 
	inline void SetStatus( EConnectionStatus eStstus );
	inline EConnectionStatus GetStatus() const;

protected:
	void paintEvent( QPaintEvent* pEvent ) override;
	//QSize sizeHint() const override;

private:
	//
	//	Content
	//
	EConnectionStatus m_eStatus;
	int m_nPadding;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CStatusView
//
class CStatusView : public QWidget
{
public:
	// Constructor & Destructor
	CStatusView( QWidget* pParent = nullptr );
	virtual ~CStatusView() = default;

public:
	//
	//	Set/Get Methods
	// 
	void SetReady( QString const& sText = QString() );
	void SetFailed( QString const& sText = QString() );
	void SetDisconneced( QString const& sText = QString() );
	void SetWaiting( QString const& sText = QString() );
	void SetUndefined( QString const& sText = QString() );

	inline EConnectionStatus GetStatus() const;
	inline QString GetText() const;

private:
	//
	//	Content
	//
	CStatusCircle* m_pStatus;
	QLabel*		   m_pTextLabel;
};
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
inline void CStatusCircle::SetStatus( EConnectionStatus eStstus )
{
	m_eStatus = eStstus;
	QString sStatus = toString( eStstus );
	setToolTip( sStatus );
	update();
}

inline EConnectionStatus CStatusCircle::GetStatus() const
{ return m_eStatus; }

//
//
//
inline EConnectionStatus CStatusView::GetStatus() const { return m_pStatus->GetStatus(); }
inline QString CStatusView::GetText() const { return m_pTextLabel->text(); }

// GetMargin
////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_STATUS_VIEW_H
