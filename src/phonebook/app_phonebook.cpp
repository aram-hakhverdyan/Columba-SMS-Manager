////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "app_phonebook.h"

// Should BE DELETED
#include "pb_contactsview.h"
#include "pb_contactheadermodel.h"
#include "pb_profilephotoviewer.h"
#include <QImageReader>
#include <QVBoxLayout>

#include "se_core_macros.h"
// GUI

// SE

// DB

// Qt
#include <chrono>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace app {
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CPhoneBook
//
CPhoneBook::CPhoneBook( QObject* pParent )
	: Base( pParent )
{
	// DB
	m_pDataStore.reset( new db::CPhoneBookDataStore() );
}

//! Destructor
CPhoneBook::~CPhoneBook()
{
}


void CPhoneBook::Initialize()
{
	// 
	m_pDataStore->Reload();

	QPixmap oIcon( "c:/icon.png" );

	pb::CContact oContact( "Mane", "Muradyan", oIcon );
	oContact.AddPhoneNumberEntry( pb::CPhoneNumber( "+37477445535" ) );
	oContact.AddPhoneNumberEntry( pb::CPhoneNumber( "+37493816563" ) );


	auto start_time = std::chrono::monotonic_clock::now();
	//
	//
	//{
	//	pb::t_IdToContactMap mapMap;
	//	for (int i = 0; i < 100000; ++i)
	//	{
	//		//QPixmap oIcon( "c:/icon.png" );
	//		pb::CContact pContact( /*pb::CContact(*/ "Mane", "Muradyan", oIcon /*)*/ );
	//		pContact.AddPhoneNumberEntry( pb::CPhoneNumber( "+37477445535" ) );
	//		pContact.AddPhoneNumberEntry( pb::CPhoneNumber( "+37493816563" ) );
	//		mapMap.insert( i, pContact );
	//	}
	//}

	//
	//
	auto end_time = std::chrono::monotonic_clock::now();
	auto time = end_time - start_time;
	auto time_in_milisecund = time / 10000;
	auto time_in_secund = time_in_milisecund / 1000;

	{
		QPixmap oIcon( "c:/icon.png" );
		pb::CContact pContact( /*pb::CContact(*/ "Mane", "Muradyan", oIcon /*)*/ );

		auto start_time = std::chrono::monotonic_clock::now();
		//
		auto end_time = std::chrono::monotonic_clock::now();
		auto time = end_time - start_time;
		auto time_in_milisecund = time / 10000;
		auto time_in_secund = time_in_milisecund / 1000;
	}


	SE_ASSERT( !oIcon.isNull() );
	int nSz = sizeof( pb::CContact* );
	m_pDataStore->AddContact( oContact );
}

// SaveSettings
void CPhoneBook::SaveSettings( QSettings* pSettings )
{
	// TODO

}

// LoadSettings
void CPhoneBook::LoadSettings( QSettings const* pSettings )
{
	// TODO
}


QString CPhoneBook::GetName() const
{
	return tr( "Phone Book" );
}

QString CPhoneBook::Topic() const
{
	return tr( "Manages contact profiles" );
}

QWidget* CPhoneBook::GetWidget()
{
	/*pb::CContactsView* pView = new pb::CContactsView();
	pb::CContactHeaderModel* pModel = new pb::CContactHeaderModel( this );
	pModel->SetDataStore( m_pDataStore.get() );
	pView->setModel( pModel );
	return pView;*/

	QWidget* pW = new QWidget();

	QHBoxLayout* pLay = new QHBoxLayout( pW );
	pW->resize( 500, 500 );

	QImageReader reader(  ":/phonebook/Resources/default_avatar2.png" );
	reader.setAutoTransform( true );
	const QImage image = reader.read();

	pb::CProfilePhotoViewer* pViewer = new pb::CProfilePhotoViewer( pW );
	pViewer->SetPhoto( QPixmap::fromImage( image ) );

	

	QFrame* pLine1 = new QFrame(pW);
	pLine1->setFrameShape(QFrame::HLine);
	pLine1->setFrameShadow(QFrame::Sunken);
	pLine1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	pLine1->setMinimumWidth(30);
	QVBoxLayout* pVLay1 = new QVBoxLayout();
	pVLay1->addSpacerItem(new QSpacerItem(10, 70, QSizePolicy::Ignored, QSizePolicy::Fixed));
	pVLay1->addWidget(pLine1);
	pVLay1->addSpacerItem(new QSpacerItem(10, 5, QSizePolicy::Ignored, QSizePolicy::Fixed));

	QFrame* pLine2 = new QFrame(pW);
	pLine2->setMinimumWidth(30);
	pLine2->setFrameShape(QFrame::HLine);
	pLine2->setFrameShadow(QFrame::Sunken);
	pLine2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	QVBoxLayout* pVLay2 = new QVBoxLayout();
	pVLay2->addSpacerItem(new QSpacerItem(10, 40, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));

	QLabel* pLabel = new QLabel("Aram Hakhverdyan");
	pLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QFont oFont = pLabel->font();
	oFont.setPointSize(40);
	pLabel->setFont(oFont);
	pVLay2->addWidget(pLabel);

	pVLay2->addWidget(pLine2);
	pVLay2->addSpacerItem(new QSpacerItem(10, 5, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));

	pLay->addLayout(pVLay1);
	pLay->addWidget(pViewer);
	pLay->addLayout(pVLay2);

	pLay->setAlignment(pLine2, Qt::AlignBottom);
	//return pViewer;

	//pLay->addWidget( pViewer );

	pViewer->move( 100, 100 );
	//pViewer->show();

	return pW;


	return nullptr;
}

QIcon CPhoneBook::GetIcon() const
{
	return QIcon();
}

QList<QActionGroup*>  CPhoneBook::GetActionList()
{
	// TODO
	/*QList<QActionGroup*> lstResult;
	lstResult.append( m_pDefaultActionGroup );
	return lstResult;*/
	return QList<QActionGroup*>();
}

t_IDBComponentSPtr CPhoneBook::GetDBComponent()
{
	return m_pDataStore;
}





//
//	Own Interface
//

//
//	SLOTS
//
// onNewMessage

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
} // namespace app
////////////////////////////////////////////////////////////////////////////////
