////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_settings_dlg.h"
#include "se_core.h"
// Qt
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSendersModel
//
CSettingsDialog::CSettingsDialog( QWidget* pParent )
	: Base( pParent ),
	m_pStatusView( nullptr )
{
	ui.setupUi( this );
	setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint );
	// Make Connections
	//SE_CONNECT( ui.btnOk, clicked(), this, onAccept() );
	// Update
	SE_CONDUCT( ui.btnUpdate, clicked(), this, sigUpdate() );
	// Connect
	SE_CONNECT( ui.btnConnect, clicked(), this, onConnect() );
	// Port changed
	SE_CONNECT( ui.cbxPortItems, currentIndexChanged(int), this, onCurrentPortChanged(int) );

	// Setup Status View
	m_pStatusView = new CStatusView();
	ui.verticalLayout->addWidget( m_pStatusView );
	//m_pStatusView->hide();
}

// SetPortItems
void CSettingsDialog::SetPortItems( QStringList const& lstItems )
{
	ui.cbxPortItems->clear();
	ui.cbxPortItems->addItems( lstItems );
	for (int i = 0; i < ui.cbxPortItems->count(); ++i)
		ui.cbxPortItems->setItemData( i, QVariant( int( EConnectionStatus::Undefined ) ) );

}

// GetPortItemAt
QString CSettingsDialog::GetPortItemAt( int nIdx )
{
	return ui.cbxPortItems->itemText( nIdx );
}

EConnectionStatus CSettingsDialog::GetConnectionStatusAt( int nIdx ) const
{
	return static_cast<EConnectionStatus>(ui.cbxPortItems->itemData( nIdx ).toInt());
}

// SetConnectionStatusAt
void CSettingsDialog::SetConnectionStatusAt( int nIdx, EConnectionStatus eStatus )
{
	//SE_ASSERT( nIdx >= 0 && nIdx < ui.cbxPortItems->count() );
	if (!(nIdx >= 0 && nIdx < ui.cbxPortItems->count()))
		return;
	ui.cbxPortItems->setItemData( nIdx, QVariant( int( eStatus ) ) );
}


int CSettingsDialog::GetCurrentPortIndex() const
{
	return ui.cbxPortItems->currentIndex();
}

void CSettingsDialog::Reset()
{
	ui.cbxPortItems->clear();
	m_pStatusView->SetFailed();
	m_pStatusView->hide();
}

int CSettingsDialog::GetPortItemCount() const
{
	return ui.cbxPortItems->count();
}

// onAccepted [SLOT]
//void CSettingsDialog::onAccept()
//{
//	int nCurrentIndex = ui.cbxPortItems->currentIndex();
//	emit sigAccepted( nCurrentIndex );
//	accept();
//}

// onTestItem
void CSettingsDialog::onConnect()
{
	int nCurrentIndex = ui.cbxPortItems->currentIndex();
	if (nCurrentIndex < 0)
	{
		m_pStatusView->SetFailed( tr("No ports available") );
		m_pStatusView->show();
		return;
	}

	if (GetConnectionStatusAt( nCurrentIndex ) == EConnectionStatus::Connected)
	{
		//
		//	Disconnect
		//
		m_pStatusView->SetWaiting( tr( "Disconnecting..." ) );
		m_pStatusView->show();
		ui.btnConnect->setDisabled( true );

		bool bReady = false;
		QString sMessage;
		// Try connect
		emit sigTryDisconnect( nCurrentIndex, bReady, &sMessage );

		ui.btnConnect->setEnabled( true );
		if (bReady)
		{
			QString sText = tr( "Disconnected" );
			if (!sMessage.isEmpty())
				sText += tr( ": %1" ).arg( sMessage );
			m_pStatusView->SetDisconneced( sText );
			// Mark as tested
			ui.cbxPortItems->setItemData( nCurrentIndex, QVariant( int( EConnectionStatus::Disconnected ) ) );
			ui.btnConnect->setText( tr( "Connect" ) );
		}
		else
		{
			QString sText = tr( "Failed to disconnect" );
			if (!sMessage.isEmpty())
				sText += tr( ": %1" ).arg( sMessage );
			m_pStatusView->SetFailed( sText );
			ui.cbxPortItems->setItemData( nCurrentIndex, QVariant( int( EConnectionStatus::FailedToConnect ) ) );
		}
	}
	else // Connect
	{

		m_pStatusView->SetWaiting( tr( "Connecting..." ) );
		m_pStatusView->show();
		ui.btnConnect->setDisabled( true );

		bool bReady = false;
		QString sMessage;
		// Try connect
		emit sigTryConnect( nCurrentIndex, bReady, &sMessage );

		ui.btnConnect->setEnabled( true );
		if (bReady)
		{
			QString sText = tr( "Connected" );
			if (!sMessage.isEmpty())
				sText += tr( ": %1" ).arg( sMessage );
			m_pStatusView->SetReady( sText );
			// Mark as tested
			ui.cbxPortItems->setItemData( nCurrentIndex, QVariant( int( EConnectionStatus::Connected ) ) );
			ui.btnConnect->setText( tr( "Disconnect" ) );
		}
		else
		{
			QString sText = tr( "Failed to connect" );
			if (!sMessage.isEmpty())
				sText += tr( ": %1" ).arg( sMessage );
			m_pStatusView->SetFailed( sText );
			ui.cbxPortItems->setItemData( nCurrentIndex, QVariant( int( EConnectionStatus::FailedToConnect ) ) );
		}
	}
}

// onCurrentPortChanged
void CSettingsDialog::onCurrentPortChanged( int nIndex )
{
	if (nIndex < 0)
		m_pStatusView->SetUndefined();
	EConnectionStatus eStatus = EConnectionStatus(ui.cbxPortItems->itemData( nIndex ).toInt());
	switch (eStatus)
	{
		case EConnectionStatus::Undefined:
			m_pStatusView->SetUndefined();
			ui.btnConnect->setText( tr( "Connect" ) );
			break;
		case EConnectionStatus::FailedToConnect:
			m_pStatusView->SetFailed( tr( "Failed to conect" ) );
			ui.btnConnect->setText( tr( "Connect" ) );
			break;
		case EConnectionStatus::Disconnected:
			m_pStatusView->SetDisconneced( tr( "Disconnected" ) );
			ui.btnConnect->setText( tr( "Connect" ) );
			break;
		case EConnectionStatus::Connected:
			m_pStatusView->SetReady( tr( "Connected" ) );
			ui.btnConnect->setText( tr( "Disconnect" ) );
			break;
		case EConnectionStatus::Connecting:
			ui.btnConnect->setText( tr( "Connect" ) );
			m_pStatusView->SetWaiting( tr( "Connecting..." ) );
			break;
		case EConnectionStatus::Disconnecting:
			ui.btnConnect->setText( tr( "Connect" ) );
			m_pStatusView->SetWaiting( tr( "Disconnecting..." ) );
			break;
		default:
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
