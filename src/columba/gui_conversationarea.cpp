////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_conversationarea.h"
#include "gui_colorprovoder.h"
#include "se_core.h"
// Qt
#include <QHBoxLayout>
#include <QTextEdit>
#include <QScrollBar>
#include <QEvent>
#include <QMenu>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CConversationItem
//
CConversationItem::CConversationItem( CMessageInfo const& oMessageInfo,
									  QWidget* pParent )
									  : QWidget( pParent ),
									  m_pBubble( nullptr ),
									  m_pCheckBox( nullptr ),
									  m_oMessageInfo( oMessageInfo )
{
	Init( oMessageInfo.GetMessage(),
		  oMessageInfo.GetDateTime().toString( "HH:mm  dd MMM yyyy" ),
		  oMessageInfo.IsDelivered() ? EBubbleOrientation::Left : EBubbleOrientation::Right,
		  oMessageInfo.GetID() );
}

void CConversationItem::Init( QString const& sMessage,
							  QString const& sTime,
							  EBubbleOrientation eOrientation,
							  qint64 nId )
{
	Q_ASSERT( !sMessage.isEmpty() );
	// Prepere Bubble
	m_pBubble = new CTimeBubble( sMessage, sTime, eOrientation, this );
	SE_CONNECT( m_pBubble, sigClicked(), this, onBubbleClicked() );
	// configure context menu settings
	m_pBubble->setContextMenuPolicy( Qt::CustomContextMenu );
	SE_CONNECT( m_pBubble, customContextMenuRequested( const QPoint& ),
				this, onShowBubbleContextMenu( const QPoint& ) );

	QBrush oBubbleBrush = CColorProvider::GetBubbleBrush( eOrientation );
	m_pBubble->SetBubbleBrush( oBubbleBrush );
	QColor oTextColor = CColorProvider::GetBubbleTextColor( eOrientation );
	m_pBubble->SetTextColor( oTextColor );
	//m_pBubble->SetTimeColor( Qt::white );
	// Setup layout
	QHBoxLayout* pHLay = new QHBoxLayout();
	pHLay->setContentsMargins( 0, 0, 0, 0 );

	QSpacerItem* pSpacer = new QSpacerItem( 20, 20, QSizePolicy::Maximum,
											QSizePolicy::Minimum );
	// Setup check box
	m_pCheckBox = new QCheckBox();
	SE_CONNECT( m_pCheckBox, stateChanged( int ),
				this, onSelectionChanged( int ) );
	pHLay->addWidget( m_pCheckBox );
	pHLay->setAlignment( m_pCheckBox, Qt::AlignTop );
	// Hide by default
	m_pCheckBox->hide();

	// Dummy Widget
	QSpacerItem* pDummyWidget = new QSpacerItem( 40, 20, QSizePolicy::Fixed,
												 QSizePolicy::Minimum );
	if (eOrientation == EBubbleOrientation::Right)
	{
		pHLay->addItem( pDummyWidget );
		pHLay->addItem( pSpacer );
		pHLay->addWidget( m_pBubble );
	}
	else // Left
	{
		pHLay->addWidget( m_pBubble );
		pHLay->addItem( pSpacer );
		pHLay->addItem( pDummyWidget );
	}
	setLayout( pHLay );
}

// onBubbleClicked
void CConversationItem::onShowBubbleContextMenu( QPoint const& oPos )
{
	SE_ASSERT( m_pBubble );
	//QPoint oBubblePos = mapTo( m_pBubble, oPos );
	if (!m_pBubble->Contains( oPos ) || IsSelectorVisible() /*Not show bubble context menu in editing mode*/)
	{
		QPoint oInThisPos = m_pBubble->mapToParent( oPos );
		QPoint oParentPos;
		//************* Get parent scrall area to map the position point to area's viewport -- But it Not good (
		QWidget* pViewport = nullptr;
		emit sigRequestViewport( pViewport );
		if (pViewport)
			oParentPos = mapTo( pViewport, oInThisPos );
		emit sigParentContexMenuRequested( oParentPos );
		return;
	}

	m_pBubble->SetSelected();
	//SetSelected();

	QMenu oMenu;
	SE_CONNECT( &oMenu, aboutToHide(), this, onMenuAboutToBeHide() );
	QAction* pCopyAction = oMenu.addAction( tr( "Copy Text" ), this, SLOT( onCopyBubbleText() ) );
	// Set Icon
	QIcon oCopyIcon( ":/columba/Resources/copy.png" );
	pCopyAction->setIcon( oCopyIcon );

	oMenu.addSeparator();
	QAction* pRemoveAction = oMenu.addAction( tr( "Remove" ), this, SLOT( onRemoveClicked() ) );
	// Set Icon
	QIcon oRemoveIcon( ":/columba/Resources/remove.png" );
	pRemoveAction->setIcon( oRemoveIcon );

	QPoint oGlobalPos = mapToGlobal( oPos );
	// Show context menu at handling position
	oMenu.exec( oGlobalPos );
}

// onMenuAboutToBeHide
void CConversationItem::onMenuAboutToBeHide()
{
	m_pBubble->SetSelected( false );
}

// onRemoveClicked
void CConversationItem::onRemoveClicked()
{
	SetSelected();
	emit sigRemoveMesssageRequested();
	SetSelected( false );
}

// onCopyBubbleText
void CConversationItem::onCopyBubbleText()
{
	QString sText = m_pBubble->GetText();
	QApplication::clipboard()->setText( sText );
}

// onBubbleClicked
void CConversationItem::onBubbleClicked()
{
	if (IsSelectorVisible())
		SetSelected( !IsSelected() );
}

// onSelectionChanged
void CConversationItem::onSelectionChanged( int nState )
{
	bool bSelected = static_cast<bool>(nState);
	m_pBubble->SetSelected( bSelected );
	emit sigSelectionChanged( bSelected );
}

// onShowSelector
void CConversationItem::onShowSelector( )
{
	m_pCheckBox->show();
}

// onHideSelector
void CConversationItem::onHideSelector( )
{
	m_pCheckBox->hide();
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CConversationArea
//
CConversationArea::CConversationArea( QWidget* pParent )
	: QScrollArea( pParent ),
	m_pCentralLayout( nullptr ),
	m_bScrollBarUpdateEnable( true )
{
	m_pCentralLayout = new QVBoxLayout();
	//m_pCentralLayout->setSpacing( 20 );
	QSpacerItem* pVerticalSpacer = new QSpacerItem( 20, 40,
													QSizePolicy::Minimum, 
													QSizePolicy::Expanding );
	m_pCentralLayout->addItem( pVerticalSpacer );
	// Setup central widget
	QWidget* pCentralWidget = new QWidget();
	pCentralWidget->setLayout( m_pCentralLayout );
	// Set Central widget
	setWidget( pCentralWidget );
	setWidgetResizable( true );

	// Connect Vertical Sllider
	/*QScrollBar* pVerticalBar = verticalScrollBar();
	SE_CONNECT( pVerticalBar, rangeChanged(int, int),
				this, onSliderRangeChanged(int, int));
	*/

	// Set Widget Background
	QPalette oPal( palette( ) );

	// set black background
	oPal.setColor( QPalette::Background, Qt::white );
	setAutoFillBackground( true );
	setPalette( oPal );

	// configure context menu settings
	setContextMenuPolicy( Qt::CustomContextMenu );
	SE_CONNECT( this, customContextMenuRequested( const QPoint& ),
				this, onShowContextMenu( const QPoint& ) );

	onMoveScrollBarToBottomDelaied();
}

// Append [1]
void CConversationArea::Append( CMessageInfo const& oMessage )
{
	CConversationItem* pItem = new CConversationItem( oMessage );
	SE_CONNECT( pItem, sigSelectionChanged(bool),
				this,  onItemSelectionChanged(bool) );
	// For Context Menu
	SE_CONNECT( pItem, sigRemoveMesssageRequested(),
				this, onConfirmAndRemoveSelectedMessages() );

	SE_CONNECT( pItem, sigParentContexMenuRequested( QPoint const& ),
				this, onShowContextMenu( QPoint const& ) );
	
	SE_CONNECT( pItem, sigRequestViewport(QWidget*&),
				this, onRequestViewport(QWidget*&) );

	m_pCentralLayout->addWidget( pItem );
	m_lstConversationItems.append( pItem );
	if (IsSelectorsVisible())
		pItem->onShowSelector();
}

// Append [2]
void CConversationArea::Append( QList<CMessageInfo> lstMessages )
{
	for (CMessageInfo const& oCurrentInfo : lstMessages)
		Append( oCurrentInfo );
}

// Prepend [1]
void CConversationArea::Prepend( CMessageInfo const& oMessage )
{
	CConversationItem* pItem = new CConversationItem( oMessage );
	SE_CONNECT( pItem, sigSelectionChanged( int ),
				this, onItemSelectionChanged( int ) );
	m_pCentralLayout->insertWidget( 0, pItem );
	m_lstConversationItems.prepend( pItem );
}

// Prepend [2]
void CConversationArea::Prepend( QList<CMessageInfo> const& lstMessages )
{
	for (CMessageInfo const& oCurrentInfo : lstMessages)
		Prepend( oCurrentInfo );
}


// GetSelection
QList<CMessageInfo> CConversationArea::GetSelection( ) const
{
	QList<CMessageInfo> lstResult;
	for (auto it = m_setSelection.begin(); it != m_setSelection.end(); ++it)
	{
		CConversationItem* pItem = *it;
		SE_ASSERT( pItem );
		lstResult.append( pItem->MessageInfo() );
	}
	return lstResult;
}


//
//	SLOTS
//
// onShowSelectors
void CConversationArea::onShowSelectors()
{
	for (CConversationItem* pItem : m_lstConversationItems)
	{
		SE_ASSERT( pItem );
		pItem->onShowSelector();
	}
}

// onHideSelectors
void CConversationArea::onHideSelectors()
{
	for (CConversationItem* pItem : m_lstConversationItems)
	{
		SE_ASSERT( pItem );
		pItem->onHideSelector();
	}
}

// onSelectAll
void CConversationArea::onSelectAll( )
{
	for (CConversationItem* pItem : m_lstConversationItems)
	{
		SE_ASSERT( pItem );
		pItem->SetSelected( true );
	}
}

// onUnselectAll
void CConversationArea::onUnselectAll( )
{
	for (CConversationItem* pItem : m_lstConversationItems)
	{
		SE_ASSERT( pItem );
		pItem->SetSelected( false );
	}
}

// onRemoveSelectedItems
void CConversationArea::onRemoveSelectedItems()
{
	QList<CMessageInfo> lstSelection = GetSelection();
	emit sigItemsGoingToBeRemoved( lstSelection );
	// Remove
	for (auto it = m_setSelection.begin( ); it != m_setSelection.end( ); ++it)
	{
		CConversationItem* pItem = *it;
		SE_ASSERT( pItem );
		m_lstConversationItems.removeOne( pItem );
		pItem->deleteLater();
	}
	m_setSelection.clear();
	if (m_lstConversationItems.isEmpty())
		emit sigAllRemoved();
}


// onItemSelectionChanged
void CConversationArea::onItemSelectionChanged( bool bSelected )
{
	CConversationItem* pItem = dynamic_cast<CConversationItem*>(sender());
	if (pItem)
	{
		if (bSelected)
			m_setSelection.insert( pItem );
		else
			m_setSelection.remove( pItem );
	}
}

void CConversationArea::onSliderRangeChanged( int nMin, int nMax )
{
	if (m_bScrollBarUpdateEnable)
	{
		QScrollBar* pVerticalBar = verticalScrollBar();
		pVerticalBar->setValue( nMax );
	}
}

// onMoveScrollBarToBottom
void CConversationArea::onMoveScrollBarToBottom()
{
	/*if (m_bScrollBarUpdateEnable)
	{*/
		QScrollBar* pVerticalBar = verticalScrollBar();
		int nMax = pVerticalBar->maximum();
		pVerticalBar->setValue( nMax );
	/*}*/
}

// onMoveScrollBarToBottom
void CConversationArea::onMoveScrollBarToBottomDelaied( int nInterval )
{
	// Turn on Timer after what move scrall bars to bottom
	QObject::startTimer( nInterval );
}

void CConversationArea::timerEvent( QTimerEvent *pEvent )
{
	QObject::killTimer( pEvent->timerId() );
	onMoveScrollBarToBottom();
}

// IsSelectorsVisible
bool CConversationArea::IsSelectorsVisible() const
{
	if (m_lstConversationItems.isEmpty())
		return false;
	CConversationItem* pFirstItem = m_lstConversationItems.first();
	if (!pFirstItem)
		return false;
	return pFirstItem->IsSelectorVisible();
}

// GetMessageCount
int CConversationArea::GetMessageCount() const
{
	return m_lstConversationItems.size();
}

// onShowContextMenu
void CConversationArea::onShowContextMenu( QPoint const& oPos )
{
	// Create menu and insert actions
	QMenu oMenu;
	if (!IsSelectorsVisible())
	{
		if (!m_lstConversationItems.isEmpty())
		{
			QAction* pEditAction = oMenu.addAction( tr( "Edit" ), this, SLOT( /*onEditClicked*/onShowSelectors() ) );
			// Set Icon
			QIcon oEditIcon( ":/columba/Resources/edit.png" );
			pEditAction->setIcon( oEditIcon );
		}
	}
	else
	{
		// Action Select All
		QAction* pSelectAllAction = oMenu.addAction( tr( "Select All" ), this, SLOT( onSelectAll() ) );
		QIcon oSelectAllIcon( ":/columba/Resources/select.png" );
		pSelectAllAction->setIcon( oSelectAllIcon );

		// Action Unselect All
		QAction* pUnselectAllAction = oMenu.addAction( tr( "Unselect All" ), this, SLOT( onUnselectAll() ) );
		QIcon oUnselectAllIcon( ":/columba/Resources/deselect.png" );
		pUnselectAllAction->setIcon( oUnselectAllIcon );

		oMenu.addSeparator();

		// Action Remove Selected Items
		QAction* pRemoveSelectedItemsAction = oMenu.addAction( tr( "Remove Selected Messages" ), this, SLOT( onConfirmAndRemoveSelectedMessages() ) );
		QIcon oCopyIcon( ":/columba/Resources/remove.png" );
		pRemoveSelectedItemsAction->setIcon( oCopyIcon );
		if (m_setSelection.isEmpty())
		{
			pUnselectAllAction->setDisabled( true );
			pRemoveSelectedItemsAction->setDisabled( true );
		}
		else if (m_lstConversationItems.size() == m_setSelection.size())
			// All items is selected
			pSelectAllAction->setDisabled( true );
		
		oMenu.addSeparator();

		// Action Finish Editing
		QAction* pFinishEditingAction = oMenu.addAction( tr( "Finish Editing" ), this, SLOT( onFinishEditingClicked() ) );
		QIcon oOkIcon( ":/columba/Resources/ok.png" );
		pFinishEditingAction->setIcon( oOkIcon );
	}

	QPoint oGlobalPos = mapToGlobal( oPos );
	// Show context menu at handling position
	oMenu.exec( oGlobalPos );
}

//
//	Context Menu Slots
//
// onFinishEditingClicked
void CConversationArea::onFinishEditingClicked()
{
	onUnselectAll();
	onHideSelectors();
}

// onRequestViewport
void CConversationArea::onRequestViewport( QWidget*& pViewport )
{
	pViewport = viewport();
}

// ConfirmMessageRemoving
bool CConversationArea::ConfirmMessageRemoving( int nMessageCount )
{
	QString sMessageCountInfo;
	if (nMessageCount >= 0)
		sMessageCountInfo = QString::number( nMessageCount );
	else
		sMessageCountInfo = "selected";

	QMessageBox oMsgBox;
	oMsgBox.setWindowTitle( tr( "Confirm Removing" ) );
	oMsgBox.setText( tr( "You are going to remove %1 message(s)." ).arg( sMessageCountInfo ) );
	oMsgBox.setInformativeText( tr( "Do you want to confirm removing?" ) );
	oMsgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
	oMsgBox.setDefaultButton( QMessageBox::Yes );
	QPixmap oQuestionIcon( ":/columba/Resources/question.png" );
	oMsgBox.setIconPixmap( oQuestionIcon );
	//oMsgBox.setIcon( QMessageBox::Question );
	int nPressedButton = oMsgBox.exec();

	return nPressedButton == QMessageBox::Yes;
}

// onConfirmAndRemoveSelectedMessages
void CConversationArea::onConfirmAndRemoveSelectedMessages()
{
	int nMessageCount = m_setSelection.size();
	if (nMessageCount <= 0)
		return;
	// Confirm removing
	if (ConfirmMessageRemoving( nMessageCount ))
		onRemoveSelectedItems();
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
