#ifndef GUI_CONVERSATION_EXPLORER_H
#define GUI_CONVERSATION_EXPLORER_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_conversationdata.h"
#include "gui_sendersview.h"
#include "db_maincomponent.h"
// Qt
#include <QWidget>
#include <QHash>
// STD
#include <memory>
////////////////////////////////////////////////////////////////////////////////

//
//	Forward Declarations
//
class QStackedWidget;

////////////////////////////////////////////////////////////////////////////////
namespace gui { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////


//
//	Forward Declarations
//
class CSendersModel;
class CMessageEditor;
class CConversationArea;
class CSendButton;
////////////////////////////////////////////////////////////////////////////////
//
//	struct SSenderInfo
//
struct SSenderInfo
{
	QString sName;
	QString sPhoneNumber;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	class CChatPage
//
class CChatPage : public QWidget
{
	Q_OBJECT
	typedef QWidget Base;

public:
	CChatPage( QWidget* pParent = nullptr );
	~CChatPage() = default;

public:
	//
	//	Main Interface
	//
	inline CMessageEditor* GetMessageEditor();
	inline CMessageEditor const* GetMessageEditor() const;

	inline CConversationArea* GetConversationArea();
	inline CConversationArea const* GetConversationArea() const;
	bool IsFrozen() const;

public slots:
	//
	//	SLOTS
	//
	void onFreeze();
	void onUnfreeze();

signals:
	// Signals
	void sigSendClicked();
	void sigMessagesGoingToBeRemoved( QList<CMessageInfo> const& );

private:
	//
	//	Content
	//
	CConversationArea*	m_pConversationArea;
	CMessageEditor*		m_pMessageEditor;
	CSendButton*		m_pSendButton;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CConversationExplorer
//
class CConversationExplorer : public QWidget
{
	Q_OBJECT
	typedef QWidget Base;
public:
	// Constructor & Destructor
	CConversationExplorer( QWidget* pParent = nullptr );
	virtual ~CConversationExplorer() = default;

public:
	//
	//	Main Interface
	//
	// Sender Info
	int GetSenderCount() const;
	int GetCurrentSenderIndex() const;
	SSenderInfo GetSenderInfoAt( int nIndex ) const;

	// Message Editor
	QString GetMessageText() const;
	void ClearCurrentMessageText();

	// Updates all stuff. Reloads data from DB
	void Reset(); // Not implemented so there was no need of this method
	void MoveChatScrollBarToBottom();
	// Check Frozen State
	bool IsFrozen() const;

	// Data DB
	void SetDataDB( db::CMainDataDB* pDataDB );
	inline db::CMainDataDB* GetDataDB();

public slots:
	//
	//	SLOTS
	//
	// Updates added message by specified indicators
	void onUpdateMessage( int nSenderID, qint64 nMessageID );
	void onFreeze();
	void onUnfreeze();

protected slots:
	//
	//	Secure Slots
	//
	void onSenderItemClicked( QModelIndex const& oIndex );
	void onSendClicked();
	void onRemoveSenderItem( QModelIndex const& oIndex, int nSenderID );
	void onRemoveMessages( QList<CMessageInfo> const& );

signals:
	void sigSend( QString const& sSenderInfo,
				  QString const& sMessage );
	//void sigConversationGoingToBeRemoved( int nSenderID );

private:
	//
	//	Internal Tools
	//
	// Opens message box to confirm conversation removing
	bool ConfirmConversationRemoving( QString const& sSenderTitle, 
									  int nMessageCount );

private:
	//
	//	Content
	//
	QStackedWidget*						m_pConversationStackWidget;
	QHash<int, CChatPage*>				m_hshSenderIdToChat;
	std::unique_ptr<CConversationData>	m_pConversationData;
	CSendersModel*						m_pSendersModel;
	CSendersView*						m_pSendersView;
	db::CMainDataDB*					m_pMainDataDB;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////

//
//	class CChatPage 
//
inline CMessageEditor* CChatPage::GetMessageEditor() { return m_pMessageEditor; }
inline CMessageEditor const* CChatPage::GetMessageEditor() const { return m_pMessageEditor; }

inline CConversationArea* CChatPage::GetConversationArea() { return m_pConversationArea; }
inline CConversationArea const* CChatPage::GetConversationArea() const { return m_pConversationArea; }

//
//	class CConversationExplorer
//
inline db::CMainDataDB* CConversationExplorer::GetDataDB() { return m_pMainDataDB; }


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_CONVERSATION_EXPLORER_H
