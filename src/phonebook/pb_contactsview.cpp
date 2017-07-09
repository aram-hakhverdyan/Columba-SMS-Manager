////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "pb_contactsview.h"
//#include "pb_contactsmodel.h"
#include "se_core_macros.h"
// Qt
#include <QPainter>
#include <QMenu>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace pb {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSendersView
//
CContactsView::CContactsView( QWidget* pParent )
	: Base( pParent )
{
	/*CContactItemDelegate* pDelegate = new CContactItemDelegate( this );
	setItemDelegate( pDelegate );*/
	setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );
	// configure context menu settings
	setContextMenuPolicy( Qt::CustomContextMenu );
	SE_CONNECT( this, customContextMenuRequested( const QPoint& ),
				this, onShowContextMenu( const QPoint& ) );
}

// onShowContextMenu
void CContactsView::onShowContextMenu( QPoint const& oPos )
{
	QModelIndex oIndex = indexAt( oPos );
	if (!oIndex.isValid())
		return;

	// TODO

	QMenu oMenu;
	//oMenu.addAction( m_pRemoveSenderItemAction );
	QPoint oGlobalPos = mapToGlobal( oPos );
	// Show context menu at handling position
	oMenu.exec( oGlobalPos );
}


//
//	nested class CSenderItemDelegate
//
CContactsView::CContactItemDelegate::CContactItemDelegate( QObject* pParent )
	: Base( pParent )
{}

// paint
void CContactsView::CContactItemDelegate::paint( QPainter* pPainter,
											     const QStyleOptionViewItem& oOption,
											     const QModelIndex& oIndex ) const
{
	if (oIndex.isValid())
	{
		pPainter->save();
		Base::paint( pPainter, oOption, oIndex );
		pPainter->restore();

		/*CSendersModel const* pModel = dynamic_cast<CSendersModel const*>(oIndex.model());
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
		}*/
	}
	//Base::paint( pPainter, oOption, oIndex );
}

// sizeHint
QSize CContactsView::CContactItemDelegate::sizeHint( const QStyleOptionViewItem& oOption,
												     const QModelIndex& oIndex ) const
{
	QSize oHint = Base::sizeHint( oOption, oIndex );
	return oHint;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
