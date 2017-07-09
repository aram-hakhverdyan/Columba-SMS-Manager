#ifndef GUI_MESSAGE_EDITOR_H
#define GUI_MESSAGE_EDITOR_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//

// Qt
#include <QPlainTextEdit>
#include <QPushButton>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace gui { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CMessageEditor
//
class CMessageEditor : public QWidget
{
	Q_OBJECT
	typedef QWidget Base;

public:
	CMessageEditor( QWidget* pParent = nullptr );
	~CMessageEditor() = default;

public:
	//
	//	Own Interface
	//
	inline void SetText( QString const& sText );
	inline QString GetText() const;
	inline void SetReadOnly( bool bReadOnly = true );
	inline bool IsReadOnly() const;

public slots:
	void onClear();

private:
	//
	//	Content
	//
	QPlainTextEdit* m_pEditor;
};
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	class CSendButton
//
class CSendButton : public QPushButton
{
	Q_OBJECT
		typedef QPushButton Base;

public:
	CSendButton( QWidget* pParent = nullptr );
	~CSendButton() = default;

public:
	// Overidden
	//QSize sizeHint() const override;
};
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//	Inline Implementations
//
////////////////////////////////////////////////////////////////////////////////

inline void CMessageEditor::SetText( QString const& sText )
{
	Q_ASSERT( m_pEditor );
	m_pEditor->setPlainText( sText );
}

inline QString CMessageEditor::GetText() const
{
	Q_ASSERT( m_pEditor );
	return m_pEditor->toPlainText();
}

inline void CMessageEditor::SetReadOnly( bool bReadOnly )
{
	Q_ASSERT( m_pEditor );
	m_pEditor->setReadOnly( bReadOnly );
}

inline bool CMessageEditor::IsReadOnly() const
{
	Q_ASSERT( m_pEditor );
	return m_pEditor->isReadOnly();
}


////////////////////////////////////////////////////////////////////////////////
} // namespace gui
////////////////////////////////////////////////////////////////////////////////

#endif // GUI_MESSAGE_EDITOR_H
