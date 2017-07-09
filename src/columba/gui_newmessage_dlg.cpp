////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "gui_newmessage_dlg.h"
#include "se_phonenumber.h"
#include "se_core.h"
// Qt
#include <QVBoxLayout>
#include <QLabel>
#include <QRegExpValidator>
//#include <QFormLayout>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CNewMessageDialog
//
CNewMessageDialog::CNewMessageDialog( QWidget* pParent )
	: Base( pParent ),
	m_pMessageEditor(nullptr),
	m_pReceiverEditor(nullptr),
	m_pSendButton( nullptr )
{
	// Remove ContextHelpButton
	//setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );
	setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint );
	// Setup Message Editing Section
	setWindowTitle( tr( "New Message" ) );
	m_pMessageEditor = new CMessageEditor();
	// Receiver Editor
	m_pReceiverEditor = new QLineEdit();
	//m_pReceiverEditor->setFrame( false );
	m_pReceiverEditor->setFont( m_pMessageEditor->font() );
	// Setup validator
	QRegExpValidator* pValidator = new QRegExpValidator( this );
	QRegExp oPhoneNumberRegExp = se::CPhoneNumber::GetValidationRegExp();
	SE_ASSERT( oPhoneNumberRegExp.isValid() );
	if (oPhoneNumberRegExp.isValid())
		pValidator->setRegExp( oPhoneNumberRegExp );
	m_pReceiverEditor->setValidator( pValidator );
	// Setup Send Button
	m_pSendButton = new CSendButton();
	SE_CONNECT( m_pSendButton, clicked(), this, onSendClicked() );
	/*pHlay->addWidget( pSendButton );
	pHlay->setAlignment( pSendButton, Qt::AlignBottom );*/
	// Add to form layout
	/*QFormLayout* pFormLay = new QFormLayout();*/
	m_plblReceiver = new QLabel( tr( "Receiver" ) );
	/*pFormLay->setWidget( 0, QFormLayout::ItemRole::LabelRole, plblReceiver );
	pFormLay->setWidget( 0, QFormLayout::ItemRole::FieldRole, m_pReceiverEditor );*/
	
	m_plblMessage = new QLabel( tr( "Message" ) );
	//pFormLay->setWidget( 1, QFormLayout::ItemRole::LabelRole, plblMessage );
	//pFormLay->setWidget( 1, QFormLayout::ItemRole::FieldRole, pEditerSection );
	//pFormLay->setSpacing( 10 );

	QVBoxLayout* pReceiverLayout = new QVBoxLayout();
	pReceiverLayout->addWidget( m_plblReceiver );
	pReceiverLayout->addWidget( m_pReceiverEditor );

	// Add Button
	QHBoxLayout* pReceiverAndButtonLay = new QHBoxLayout();
	pReceiverAndButtonLay->addLayout( pReceiverLayout );
	pReceiverAndButtonLay->addWidget( m_pSendButton );

	// Add Message Editor
	QVBoxLayout* pVLay = new QVBoxLayout( this );
	pVLay->addLayout( pReceiverAndButtonLay );
	pVLay->addWidget( m_plblMessage );
	pVLay->addWidget( m_pMessageEditor );

	resize( 480, 320 );
}

// UpdateRequiredFieldState
void CNewMessageDialog::UpdateRequiredFieldState( QLabel* pLabel, bool bCorrect )
{
	SE_ASSERT( pLabel );
	if (bCorrect)
		pLabel->setStyleSheet( "color: black" );
	else
		pLabel->setStyleSheet( "color: red" );
}

// onSendClicked
void CNewMessageDialog::onSendClicked()
{
	SE_ASSERT( m_pReceiverEditor );
	SE_ASSERT( m_pMessageEditor );

	QString sReceiverInfo = m_pReceiverEditor->text();
	bool bReceiverInfoOk = !sReceiverInfo.isEmpty();
	UpdateRequiredFieldState( m_plblReceiver, bReceiverInfoOk );

	QString sMessage = m_pMessageEditor->GetText();
	bool bMessageOk = !sMessage.isEmpty();
	UpdateRequiredFieldState( m_plblMessage, bMessageOk );

	if (!bMessageOk || !bReceiverInfoOk)
		return;
	
	emit sigSend( sReceiverInfo, sMessage );
}

// Clear
void CNewMessageDialog::Clear()
{
	SE_ASSERT( m_pReceiverEditor );
	SE_ASSERT( m_pMessageEditor );
	m_pReceiverEditor->clear();
	m_pMessageEditor->onClear();
	m_pSendButton->setEnabled( true );

	UpdateRequiredFieldState( m_plblReceiver, true );
	UpdateRequiredFieldState( m_plblMessage, true );
}


// onFreeze
void CNewMessageDialog::onFreeze()
{
	SE_ASSERT( m_pReceiverEditor );
	SE_ASSERT( m_pMessageEditor );
	SE_ASSERT( m_pSendButton );

	m_pReceiverEditor->setReadOnly( true );
	m_pMessageEditor->SetReadOnly();
	m_pSendButton->setDisabled( true );
}

// onUnfreeze
void CNewMessageDialog::onUnfreeze()
{
	SE_ASSERT( m_pReceiverEditor );
	SE_ASSERT( m_pMessageEditor );
	SE_ASSERT( m_pSendButton );

	m_pReceiverEditor->setReadOnly( false );
	m_pMessageEditor->SetReadOnly( false );
	m_pSendButton->setEnabled( true );
}

// IsFrozen
bool CNewMessageDialog::IsFrozen() const
{
	SE_ASSERT( m_pMessageEditor );
	return m_pMessageEditor->IsReadOnly();
}


////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////
