////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "pb_contactheadermodel.h"
#include "se_core_macros.h"
#include "db_phonebookdatastore.h"
// Qt
#include <QSize>
#include <QBrush>
#include <QFont>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace pb {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CContactHeaderModel
//
CContactHeaderModel::CContactHeaderModel( QObject* pParent )
	: Base( pParent ),
	  m_pDataStore( nullptr )
{
	
}

void CContactHeaderModel::SetDataStore( db::CPhoneBookDataStore const* pDataStore )
{
	// Disconnect from old data store
	SE_DISCONNECT( m_pDataStore, sigReloaded(), this, onUpdateData() );
	SE_DISCONNECT( m_pDataStore, sigAboutToBeCleared(), this, onStoreAboutToBeCleared() );
	SE_DISCONNECT( m_pDataStore, sigContactAdded( pb::CContact const& ),
				   this, onContactAdded( pb::CContact const& ) );
	SE_DISCONNECT( m_pDataStore, sigContactAboutToBeRemoved( t_ContactID ),
				   this, onContactAboutToBeRemoved( t_ContactID ) );

	m_pDataStore = pDataStore;
	if (m_pDataStore)
	{
		SE_CONNECT( m_pDataStore, sigReloaded(), this, onUpdateData() );
		SE_CONNECT( m_pDataStore, sigAboutToBeCleared(), this, onStoreAboutToBeCleared() );
		SE_CONNECT( m_pDataStore, sigContactAdded( pb::CContact const& ), 
					this, onContactAdded( pb::CContact const& ) );
		SE_CONNECT( m_pDataStore, sigContactAboutToBeRemoved( t_ContactID ), 
					this, onContactAboutToBeRemoved( t_ContactID ) );
	}

	onUpdateData();
}

db::CPhoneBookDataStore const* CContactHeaderModel::GetDataStore() const
{
	return m_pDataStore;
}

// rowCount
int	CContactHeaderModel::rowCount( const QModelIndex& oParent ) const
{
	SE_CHECK_AND_RETURN_VAL( m_pDataStore, 0 );
	return m_pDataStore->GetContactCount();
}

// data
QVariant CContactHeaderModel::data( const QModelIndex& oIndex, int role ) const
{
	SE_CHECK_AND_RETURN_VAL( m_pDataStore, QVariant() );

	if (!oIndex.isValid())
		return QVariant();
	int nRow = oIndex.row();
	int nContactCount = m_lstHeaders.size();
	SE_ASSERT( nRow >= 0 && nRow < nContactCount );
	if (nRow < 0 || nRow >= nContactCount )
		return QVariant();

	CContactHeader const& oHeader = m_lstHeaders[nRow];

	QVariant vtResult;
	switch (role)
	{
		case Qt::DisplayRole:
			{
				QString sDisplayName = oHeader.GetFirstName() + " " + oHeader.GetLastName();
				vtResult = sDisplayName;
				break;
				/*case Qt::ForegroundRole:
				{
				QBrush oTextBrush;
				oTextBrush.setStyle( Qt::SolidPattern );
				if (bHasUnreadMessages)
				oTextBrush.setColor( Qt::green );
				vtResult = oTextBrush;
				break;
				}*/
			}
		case Qt::DecorationRole:
			{
				QPixmap oIcon = oHeader.GetIcon();
				vtResult = oIcon.scaled( 50, 50, Qt::KeepAspectRatio );
				break;
			}
		case Qt::FontRole:
			{
				QFont oFont;
				oFont.setPointSize( 12 );
				oFont.setFamily( "Calibri" );
				vtResult = oFont;
				break;
			}

		/*case Qt::SizeHintRole:
			vtResult = QSize( -1, 40 );
			break;*/

			/*case eHasUnreadMessageRole:
			vtResult = bHasUnreadMessages;
			break;
		case eGetIDRole:
			vtResult = int( strData.nID );
			break;
		case ePhoneNumberRole:
			vtResult = GetPhoneNumber( strData.nID );
			break;*/


		default:
			break;
	}

	return vtResult;
}


//
//	Slots
//

void CContactHeaderModel::onUpdateData()
{
	beginResetModel();

	m_lstHeaders.clear();
	if (!m_pDataStore)
		return;

	m_lstHeaders = m_pDataStore->GetContactHeadersList();

	endResetModel();
}
//
//	Internal Slots
//
void CContactHeaderModel::onStoreAboutToBeCleared()
{
	beginResetModel();
	
	m_lstHeaders.clear();
	
	endResetModel();
}

void CContactHeaderModel::onContactAdded( pb::CContact const& oContact )
{
	int nInsertPos = m_lstHeaders.size();

	beginInsertRows( QModelIndex(), nInsertPos, nInsertPos );

	m_lstHeaders.insert( nInsertPos, oContact.Header() );

	endInsertRows();
}

void CContactHeaderModel::onContactAboutToBeRemoved( t_ContactID nContactID )
{
	for (int i = 0; i < m_lstHeaders.size(); ++i)
	{
		t_ContactID nCurrentId = m_lstHeaders[i].GetID();
		if (nCurrentId == nContactID)
		{
			beginRemoveRows( QModelIndex(), i, i );
			m_lstHeaders.removeAt( i );
			endRemoveRows();
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
} // namespace pb
////////////////////////////////////////////////////////////////////////////////
