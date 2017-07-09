////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_sendersview.h"
#include "gui_sendersmodel.h"
#include "se_core.h"
// Qt
#include <QPainter>
#include <QMenu>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSendersView
//
CSendersView::CSendersView( QWidget* pParent )
	: Base( pParent ),
	m_pRemoveSenderItemAction( nullptr )
{
	CSenderItemDelegate* pDelegate = new CSenderItemDelegate( this );
	setItemDelegate( pDelegate );
	setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );
	// configure context menu settings
	setContextMenuPolicy( Qt::CustomContextMenu );
	SE_CONNECT( this, customContextMenuRequested(const QPoint&),
				this,          onShowContextMenu(const QPoint&) );

	// Create remove sender item action
	QIcon oRemoveIcon( ":/columba/Resources/remove.png" );
	m_pRemoveSenderItemAction = new QAction( oRemoveIcon, tr( "Remove Conversation" ), this );
	SE_CONNECT( m_pRemoveSenderItemAction, triggered(),
				this, onRemoveItemClicked() );
		
}

// onShowContextMenu
void CSendersView::onShowContextMenu( QPoint const& oPos )
{
	QModelIndex oIndex = indexAt( oPos );
	if (!oIndex.isValid())
		return;
	SE_ASSERT( m_pRemoveSenderItemAction );
	m_pRemoveSenderItemAction->setData( QVariant( oIndex ) );

	QMenu oMenu;
	oMenu.addAction( m_pRemoveSenderItemAction );
	QPoint oGlobalPos = mapToGlobal( oPos );
	// Show context menu at handling position
	oMenu.exec( oGlobalPos );
}

// onRemoveItemClicked
void CSendersView::onRemoveItemClicked()
{
	QAction* pRemoveAction = qobject_cast<QAction*>(sender());
	SE_ASSERT( pRemoveAction );
	if (!pRemoveAction)
		return;

	QModelIndex oIndex = pRemoveAction->data().toModelIndex();
	if (!oIndex.isValid())
		return;
	CSendersModel const* pModel = dynamic_cast<CSendersModel const*>(oIndex.model());
	SE_ASSERT( pModel );
	if (pModel)
	{
		int nSenderID = pModel->data( oIndex, CSendersModel::eGetIDRole ).toInt();
		emit sigRemoveItem( oIndex, nSenderID );
	}
}

//
//	nested class CSenderItemDelegate
//
CSendersView::CSenderItemDelegate::CSenderItemDelegate( QObject* pParent )
	: Base( pParent ),
	  m_nNavigatorWidth( 5 )
{}

// paint
void CSendersView::CSenderItemDelegate::paint( QPainter* pPainter,
											   const QStyleOptionViewItem& oOption,
											   const QModelIndex& oIndex ) const
{
	if (oIndex.isValid())
	{
		pPainter->save();
		Base::paint( pPainter, oOption, oIndex );
		pPainter->restore();

		CSendersModel const* pModel = dynamic_cast<CSendersModel const*>(oIndex.model());
		if (pModel)
		{

			bool bIsUnread = pModel->data( oIndex,
										 CSendersModel::eHasUnreadMessageRole )
										 .toBool();

			if (bIsUnread)
			{
				int nX = oOption.rect.right() - m_nNavigatorWidth;
				int nY = oOption.rect.top() + 2;
				int nHeight = oOption.rect.height();

				pPainter->setRenderHint( QPainter::RenderHint::Antialiasing );
				QPen oPen = pPainter->pen();
				oPen.setColor( QColor( 6, 161, 42 ) );
				pPainter->setPen( oPen );

				QBrush oBrush = pPainter->brush();
				oBrush.setColor( QColor( 6, 161, 42 ) );
				oBrush.setStyle( Qt::SolidPattern );
				pPainter->setBrush( oBrush );
				pPainter->drawRoundedRect( QRect( nX, nY, 6, nHeight - 2 ), 0, 5 );
			}
		}
	}
	//Base::paint( pPainter, oOption, oIndex );
}

// sizeHint
QSize CSendersView::CSenderItemDelegate::sizeHint( const QStyleOptionViewItem& oOption,
												   const QModelIndex& oIndex ) const
{
	QSize oHint = Base::sizeHint( oOption, oIndex );
	oHint.setWidth( oHint.width() + m_nNavigatorWidth );
	return oHint;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
