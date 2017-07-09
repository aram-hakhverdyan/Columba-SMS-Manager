#ifndef GUI_CONVERSATION_AREA_H
#define GUI_CONVERSATION_AREA_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_bubble.h"
// Qt
#include <QCheckBox>
#include <QScrollArea>
#include <QSet>
#include <QVBoxLayout>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CConversationItem
//
class CConversationItem : public QWidget
{
	Q_OBJECT

public:
	CConversationItem( CMessageInfo const& oMessageInfo,
					   QWidget* pParent = nullptr );

	//CConversationItem( CBasicBubble* pBubble );
	virtual ~CConversationItem() = default;

public:
	//
	//	Main Interface
	//
	inline void SetID( qint64 nID );
	inline qint64 GetID() const;
	inline CMessageInfo const& MessageInfo() const;

	inline bool IsSelected() const;
	inline void SetSelected( bool bSelect = true );
	inline bool IsSelectorVisible() const;

public slots:
	//
	//	Public Slots
	//
	void onShowSelector();
	void onHideSelector();

private slots:
	void onSelectionChanged( int nState );
	void onBubbleClicked();
	void onShowBubbleContextMenu( QPoint const& );
	void onCopyBubbleText();
	void onMenuAboutToBeHide();
	void onRemoveClicked();

signals:
	void sigSelectionChanged( bool bSelected );
	void sigRemoveMesssageRequested();
	void sigParentContexMenuRequested( QPoint const& );
	void sigRequestViewport( QWidget*& pViewport );

private:
	// Tools
	void Init( QString const& sMessage,
			   QString const& sTime,
			   EBubbleOrientation eOrientation,
			   qint64 nId );
private:
	//
	//	Content
	//
	CTimeBubble*	m_pBubble;
	QCheckBox*		m_pCheckBox;
	CMessageInfo    m_oMessageInfo;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CConversationArea
//
class CConversationArea : public QScrollArea
{
	Q_OBJECT

	typedef QScrollArea Base;

public:
	CConversationArea( QWidget* pParent = nullptr );
	virtual ~CConversationArea() = default;

public:
	//
	//	Main Interface
	//
	void Append( CMessageInfo const& oMessage );
	void Append( QList<CMessageInfo> lstMessages );

	void Prepend( CMessageInfo const& oMessage );
	void Prepend( QList<CMessageInfo> const& lstMessages );

	QList<CMessageInfo> GetSelection() const;
	bool IsSelectorsVisible() const;

	int GetMessageCount() const;

public slots:
	void onShowSelectors(); 
	void onHideSelectors();
	void onSelectAll();
	void onUnselectAll();
	void onRemoveSelectedItems();
	void onMoveScrollBarToBottom();
	void onMoveScrollBarToBottomDelaied( int nInterval = 300 );

public:
	// Overidden
	//bool event( QEvent* pevent ) override;
	// Overidden from QObject
	void timerEvent( QTimerEvent *pEvent ) override;

private slots:
	//
	//	Secure SLOTs
	//
	void onItemSelectionChanged( bool bSelected );
	void onSliderRangeChanged( int nMin, int nMax );
	void onShowContextMenu( QPoint const& );
	// confirmation could be taken from bool argument
	void onConfirmAndRemoveSelectedMessages();
	//
	//	Context Menu SLOTs
	//
	void onFinishEditingClicked();
	// for conversation item correct context menu position mappings
	void onRequestViewport( QWidget*& pViewport );

signals:
	void sigItemsGoingToBeRemoved( QList<CMessageInfo> const& );
	void sigAllRemoved();
	// This signal is emiting when the remove action is triggered 
	// (bad :( it will be better to move remove action to conversation explorer class)
	void sigRemoveMessagesClicked();

private:
	// Opens message box to confirm message removing
	bool ConfirmMessageRemoving( int nMessageCount );

private:
	//
	//	Content
	//
	QVBoxLayout*				m_pCentralLayout;
	QList<CConversationItem*>	m_lstConversationItems;
	QSet<CConversationItem*>	m_setSelection;
	bool						m_bScrollBarUpdateEnable;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////
inline void CConversationItem::SetID( qint64 nID )
{ m_oMessageInfo.SetID( nID ); }
inline qint64 CConversationItem::GetID( ) const
{ return m_oMessageInfo.GetID(); }
inline CMessageInfo const& CConversationItem::MessageInfo() const
{ return m_oMessageInfo; }
inline bool CConversationItem::IsSelected() const
{ return m_pCheckBox->checkState(); }
inline void CConversationItem::SetSelected( bool bSelect )
{
	m_pCheckBox->setChecked( bSelect );
	m_pBubble->SetSelected( bSelect );
}
inline bool CConversationItem::IsSelectorVisible() const
{ return m_pCheckBox->isVisible(); }


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_CONVERSATION_AREA_H
