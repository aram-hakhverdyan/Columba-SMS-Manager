////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_conversationexplorer.h"
#include "gui_sendersview.h"
#include "gui_sendersmodel.h"
#include "gui_conversationdata.h"
#include "gui_conversationarea.h"
#include "gui_messageeditor.h"
#include "se_core.h"
// Qt
#include <QSplitter>
#include <QStackedWidget>
#include <QMessageBox>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CChatPage
//
CChatPage::CChatPage( QWidget* pParent )
	: Base( pParent ),
	m_pConversationArea( nullptr ),
	m_pMessageEditor( nullptr )
{
	// Setup UI
	m_pConversationArea = new CConversationArea();
	SE_CONDUCT( m_pConversationArea, sigItemsGoingToBeRemoved(QList<CMessageInfo> const&), 
				this,             sigMessagesGoingToBeRemoved(QList<CMessageInfo> const&) );

	m_pMessageEditor = new CMessageEditor();
	QSplitter* pVSplitter = new QSplitter( Qt::Vertical );

	pVSplitter->addWidget( m_pConversationArea );
	// Editor Section
	QWidget* pEditerSection = new QWidget();
	// set black background
	QPalette oPal = pEditerSection->palette();
	oPal.setColor( QPalette::Background, Qt::white );
	pEditerSection->setAutoFillBackground( true );
	pEditerSection->setPalette( oPal );
	QHBoxLayout* pHlay = new QHBoxLayout( pEditerSection );
	pHlay->setMargin( 3 );
	pHlay->addWidget( m_pMessageEditor );

	// Setup Send Button
	m_pSendButton = new CSendButton();
	m_pSendButton->setFixedHeight( 70 );
	SE_CONDUCT( m_pSendButton, clicked(), this, sigSendClicked() );
	pHlay->addWidget( m_pSendButton );
	pHlay->setAlignment( m_pSendButton, Qt::AlignBottom );

	pVSplitter->addWidget( pEditerSection );
	pVSplitter->setStretchFactor( 0, 35 );
	pVSplitter->setStretchFactor( 1, 1 );

	QVBoxLayout* pVLay = new QVBoxLayout( this );
	pVLay->setSpacing( 0 );
	pVLay->setMargin( 0 );
	pVLay->addWidget( pVSplitter );
}

// onFreeze
void CChatPage::onFreeze()
{
	m_pMessageEditor->SetReadOnly();
	m_pSendButton->setDisabled( true );
}

// onUnfreeze
void CChatPage::onUnfreeze()
{
	m_pMessageEditor->SetReadOnly( false );
	m_pSendButton->setEnabled( true );
}

bool CChatPage::IsFrozen() const { return m_pMessageEditor->IsReadOnly(); }
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSendersModel
//
CConversationExplorer::CConversationExplorer( QWidget* pParent )
	: Base( pParent ),
	  m_pConversationStackWidget( nullptr ),
	  m_pSendersView( nullptr ),
	  m_pMainDataDB( nullptr )
{
	// Conv Data
	m_pConversationData.reset( new CConversationData() );
	// Setup UI
	QSplitter* pHSplitter = new QSplitter( Qt::Horizontal );
	// Senders Model
	//! Can throw an exception
	m_pSendersModel = new CSendersModel( this );
	// Senders View
	m_pSendersView = new CSendersView();
	m_pSendersView->setModel( m_pSendersModel );
	pHSplitter->addWidget( m_pSendersView );
	pHSplitter->setChildrenCollapsible( false );
	SE_CONNECT( m_pSendersView->selectionModel(), currentChanged(const QModelIndex&, const QModelIndex&),
				this, onSenderItemClicked(QModelIndex const&) );
	SE_CONNECT( m_pSendersView, sigRemoveItem(QModelIndex const&, int),
				this,      onRemoveSenderItem(QModelIndex const&, int) );
	
	// Stacked Widget
	m_pConversationStackWidget = new QStackedWidget();
	// Add "No Message" Label Widget
	QLabel* pNoMessageLbl = new QLabel( tr( "No Messages" ) );
	pNoMessageLbl->setAlignment( Qt::AlignCenter );
	// Font
	QFont oFont = pNoMessageLbl->font();
	oFont.setPointSize( 30 );
	pNoMessageLbl->setStyleSheet( "color: rgb(161,161,161)" );
	pNoMessageLbl->setFont( oFont );

	m_pConversationStackWidget->addWidget( pNoMessageLbl );

	// Right Side Widget
	//QSplitter* pVSplitter = new QSplitter( Qt::Vertical );
	//pVSplitter->addWidget( m_pConversationStackWidget );
	//
	//// Setup Message Editing Section
	//m_pMessageEditor = new CMessageEditor();
	//QWidget* pEditerSection = new QWidget();
	//// set black background
	//QPalette oPal = pEditerSection->palette();
	//oPal.setColor( QPalette::Background, Qt::white );
	//pEditerSection->setAutoFillBackground( true );
	//pEditerSection->setPalette( oPal );
	//QHBoxLayout* pHlay = new QHBoxLayout( pEditerSection );
	//pHlay->setMargin( 3 );
	//pHlay->addWidget( m_pMessageEditor );
	//// Setup Send Button
	//CSendButton* pSendButton = new CSendButton();
	//pSendButton->setFixedHeight( 70 );
	//SE_CONNECT( pSendButton, clicked(), this, onSendClicked() );
	//pHlay->addWidget( pSendButton );
	//pHlay->setAlignment( pSendButton, Qt::AlignBottom );
	//pVSplitter->addWidget( pEditerSection );
	//pVSplitter->setStretchFactor( 0, 10 );
	//pVSplitter->setStretchFactor( 1, 1 );

	pHSplitter->addWidget( m_pConversationStackWidget );
	pHSplitter->setStretchFactor( 0, 2 );
	pHSplitter->setStretchFactor( 1, 5 );

	// Setup Central Layout
	QVBoxLayout* pCentralLayout = new QVBoxLayout();
	pCentralLayout->addWidget( pHSplitter );
	setLayout( pCentralLayout );

	// Select First Item
	QModelIndex oFirst = m_pSendersModel->index( 0 );
	//pSendersView->setSelectionBehavior( QAbstractItemView::SelectRows );
	if (oFirst.isValid())
	{
		m_pSendersView->setCurrentIndex( oFirst );
		onSenderItemClicked( oFirst );
	}
}


// onSenderItemClicked
void CConversationExplorer::onSenderItemClicked( QModelIndex const& oIndex )
{
	SE_ASSERT( oIndex.isValid() );
	if (!oIndex.isValid())
		return;
	int nID = oIndex.data( CSendersModel::eGetIDRole ).toInt();
	SE_ASSERT( nID > 0 );
	auto it = m_hshSenderIdToChat.find( nID );
	if (it != m_hshSenderIdToChat.end())
	{
		QWidget* pCurrentWidget = it.value();
		m_pConversationStackWidget->setCurrentWidget( pCurrentWidget );
	}
	else
	{
		// Create New One
		QList<CMessageInfo> lstMsgInfo = m_pConversationData->GetMessageInfoList( nID );
		//SE_ASSERT( !lstMsgInfo.isEmpty() );
		// Create Chat Page
		CChatPage* pChatPage = new CChatPage();
		SE_CONNECT( pChatPage, sigSendClicked(), this, onSendClicked() );
		SE_CONNECT( pChatPage, sigMessagesGoingToBeRemoved(QList<CMessageInfo> const&), 
					this,                 onRemoveMessages(QList<CMessageInfo> const&) );
		// Set ConversationArea Data
		CConversationArea* pChatArea = pChatPage->GetConversationArea();
		SE_ASSERT( pChatArea );
		pChatArea->Append( lstMsgInfo );
		int nIdx = m_pConversationStackWidget->addWidget( pChatPage );
		m_pConversationStackWidget->setCurrentIndex( nIdx );
		m_pConversationStackWidget->update();
		//pChatArea->ensureVisible( pChatArea->rect().bottomRight().x(), pChatArea->rect().bottomRight().y() );
		//pChatArea->onMoveScrollBarToBottom();

		// Add to hash
		m_hshSenderIdToChat.insert( nID, pChatPage );
	}

	// Mark unread messages as read (if any)
	bool bHasUnreadMessage = oIndex.data( CSendersModel::eHasUnreadMessageRole ).toBool();
	if (bHasUnreadMessage)
	{
		SE_ASSERT( m_pSendersModel );
		m_pSendersModel->MarkMessagesAsRead( nID );
	}
}


// onUpdateMessage
void CConversationExplorer::onUpdateMessage( int nSenderID, qint64 nMessageID )
{
	SE_ASSERT( nSenderID > 0 && nMessageID > 0 );
	if (nSenderID <= 0)
		return;
	m_pSendersModel->UpdateItem( nSenderID );

	auto it = m_hshSenderIdToChat.find( nSenderID );
	if (it != m_hshSenderIdToChat.end())
	{
		SE_ASSERT( nMessageID > 0 );
		if (nMessageID > 0)
		{
			CMessageInfo oInfo = m_pConversationData->GetMessage( nMessageID );
			CChatPage* pChatPage = it.value();
			SE_ASSERT( pChatPage );
			CConversationArea* pCurrentArea = pChatPage->GetConversationArea();
			SE_ASSERT( pCurrentArea );
			pCurrentArea->Append( oInfo );
			pCurrentArea->update();
			//pCurrentArea->onMoveScrollBarToBottom();
		}
	}
}

// onSendClicked
void CConversationExplorer::onSendClicked()
{
	SE_ASSERT( m_pSendersModel );
	SE_ASSERT( m_pSendersView );
	QModelIndex oIndex = m_pSendersView->currentIndex();
	if (!oIndex.isValid())
		return;
	
	CChatPage* pChatPage = qobject_cast<CChatPage*>(m_pConversationStackWidget->currentWidget());
	if (!pChatPage)
		return;
	CMessageEditor* pMessageEditor = pChatPage->GetMessageEditor();
	SE_ASSERT( pMessageEditor );
	QString sMessage = pMessageEditor->GetText();
	if (sMessage.isEmpty())
		return;
	
	// Get Phone Number
	QString sPhoneNumber = m_pSendersModel->data( oIndex, CSendersModel::ePhoneNumberRole ).toString();

	emit sigSend( sPhoneNumber, sMessage );
}

// onFreeze
void CConversationExplorer::onFreeze()
{
	/*QList<QWidget*> lstChatPages = m_hshSenderIdToChat.values();
	for (QWidget* pCurrentWidget : lstChatPages)
	{*/
		CChatPage* pChatPage = qobject_cast<CChatPage*>(m_pConversationStackWidget->currentWidget());
		if (!pChatPage)
			return;
		pChatPage->onFreeze();
	/*}*/
}

// MoveChatScrollBarToBottom
void CConversationExplorer::MoveChatScrollBarToBottom()
{
	CChatPage* pChatPage = qobject_cast<CChatPage*>(m_pConversationStackWidget->currentWidget());
	if (!pChatPage)
		return;
	CConversationArea* pArea = pChatPage->GetConversationArea();
	if (!pArea)
		return;
	pArea->onMoveScrollBarToBottomDelaied();
}

// onUnfreeze
void CConversationExplorer::onUnfreeze()
{
	QList<CChatPage*> lstChatPages = m_hshSenderIdToChat.values();
	for (CChatPage* pChatPage : lstChatPages)
	{
		if (!pChatPage)
			return;
		pChatPage->onUnfreeze();
	}
}

// SetDataDB
void CConversationExplorer::SetDataDB( db::CMainDataDB* pDataDB )
{
	m_pMainDataDB = pDataDB;
	SE_CONNECT( pDataDB, sigMessageAdded(int, qint64),
				this,    onUpdateMessage(int, qint64) );
}

// ClearCurrentMessageText
void CConversationExplorer::ClearCurrentMessageText()
{
	CChatPage* pChatPage = qobject_cast<CChatPage*>(m_pConversationStackWidget->currentWidget());
	if (!pChatPage)
		return;
	CMessageEditor* pMessageEditor = pChatPage->GetMessageEditor();
	SE_ASSERT( pMessageEditor );
	pMessageEditor->onClear();
}

// ConfirmConversationRemoving
bool CConversationExplorer::ConfirmConversationRemoving( QString const& sSenderTitle,
														 int nMessageCount )
{
	QString sMessageCountInfo;
	if (nMessageCount >= 0)
		sMessageCountInfo = QString( "( %1 messages )" ).arg( nMessageCount );

	QMessageBox oMsgBox;
	oMsgBox.setWindowTitle( tr( "Confirm Removing" ) );
	oMsgBox.setText( tr( "You are going to remove the conversation with \"%1\" %2" ).arg( sSenderTitle ).arg( sMessageCountInfo ) );
	oMsgBox.setInformativeText( tr( "Do you want to confirm removing?" ) );
	oMsgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
	oMsgBox.setDefaultButton( QMessageBox::Yes );
	//oMsgBox.setIcon( QMessageBox::Warning );
	QPixmap oQuestionIcon( ":/columba/Resources/question.png" );
	oMsgBox.setIconPixmap( oQuestionIcon );
	int nPressedButton = oMsgBox.exec();

	return nPressedButton == QMessageBox::Yes;
}

// onRemoveSenderItem
void CConversationExplorer::onRemoveSenderItem( QModelIndex const& oIndex, int nSenderID )
{
	if (nSenderID <= 0)
		return;
	SE_ASSERT( m_pSendersModel );
	SE_ASSERT( m_pConversationData );
	SE_ASSERT( m_pConversationStackWidget );

	//
	//	Confirm Removing
	//
	QString sSenderTitle = m_pSendersModel->data( oIndex, Qt::DisplayRole ).toString();
	int nMessageCount = -1;
	CChatPage* pChatPage = m_hshSenderIdToChat.value( nSenderID );
	SE_ASSERT( pChatPage );
	if (pChatPage)
	{
		CConversationArea* pArea = pChatPage->GetConversationArea();
		SE_ASSERT( pArea );
		nMessageCount = pArea->GetMessageCount();
	}

	if (ConfirmConversationRemoving( sSenderTitle, nMessageCount ))
	{
		// Remove Conversation From DB
		//emit sigConversationGoingToBeRemoved( nSenderID );
		if (m_pMainDataDB)
			m_pMainDataDB->RemoveConversation( nSenderID );
		// Remove Chat Page
		QWidget* pChatPage = m_hshSenderIdToChat.value( nSenderID );
		if (pChatPage)
		{
			m_pConversationStackWidget->removeWidget( pChatPage );
			m_hshSenderIdToChat.remove( nSenderID );
		}

		// Remove Sender Item
		m_pSendersModel->RemoveSenderItem( nSenderID );
	}
}

// onRemoveMessages
void CConversationExplorer::onRemoveMessages( QList<CMessageInfo> const& lstMessages )
{
	if (lstMessages.isEmpty())
		return;
	// Confirm message removing
	int nMessageCount = lstMessages.size();	
	// Remove From DB
	QVector<qint64> aMessageIDs( nMessageCount );
	for (int i = 0; i < nMessageCount; ++i)
		aMessageIDs[i] = lstMessages[i].GetID();
	if (m_pMainDataDB)
		m_pMainDataDB->RemoveMessages( aMessageIDs );
}


////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
