#ifndef GUI_SENDERS_MODEL_H
#define GUI_SENDERS_MODEL_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
// Qt
#include <QAbstractListModel>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSendersModel
//
class CSendersModel : public QAbstractListModel
{
	typedef QAbstractListModel Base;

public:
	//
	//	Custom Roles
	//
	enum ECustomRoles
	{
		eHasUnreadMessageRole = Qt::UserRole,
		eGetIDRole,
		ePhoneNumberRole
	};

	//
	//	struct	SSenderData
	//
	struct SSenderData
	{
		SSenderData( int nID, QString const& sTitle, int nUnreadMessageCount = 0 )
		: nID( nID ),
		sTitle( sTitle ),
		nUnreadMessageCount( nUnreadMessageCount )
		{}

		int nID;
		QString sTitle;
		int nUnreadMessageCount;
	};

public:
	// Constructor & Destructor
	CSendersModel( QObject* pParent = nullptr );
	~CSendersModel() = default;

public:
	//
	//	QAbstractListModel Interface <-
	// 
	int		 rowCount( const QModelIndex& parent = QModelIndex( ) ) const override;
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
	bool	 insertRows( int row, int count,
						const QModelIndex& parent = QModelIndex( ) ) override;
	bool	 removeRows( int row, int count,
						const QModelIndex& parent = QModelIndex( ) ) override;
public:
	//
	//	Own Interface
	//
	void UpdateAll();
	void UpdateItem( int nSenderID );
	void MarkMessagesAsRead( int nSenderID );
	void AddSenderData( SSenderData const& strData );
	void RemoveSenderItem( int nSenderID );

protected:
	// Returns List index by specified ID 
	int IndexFromID( int nSenderID );
	QString GetPhoneNumber( int nSenderId ) const;

private:
	//
	//	Content
	//
	typedef QList<SSenderData> t_lstSenderData;
	t_lstSenderData m_lstSenderData;
	
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_SENDERS_MODEL_H
