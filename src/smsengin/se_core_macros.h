#ifndef SE_CORE_MACROS_H
#define SE_CORE_MACROS_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
//
//	Qt Includes
//
#include <QtGlobal>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Useful Macros
//
#if defined SE_MACRO_HELPERS
#	undef SE_MACRO_HELPERS
#	undef SE_ASSERT
#	undef SE_VERIFY
#	undef SE_DBGMESSAGE
#	undef SE_DBGBREAK
#	undef SE_STUB
#	undef SE_CHK_IDX
#	undef SE_CHK_PTR
#	undef SE_CHK_NEW_PTR
#	undef SE_THROW_BAD
#	undef SE_THROW_FATAL
#	undef SE_THROW_INVALID_PTR
#	undef SE_THROW_OUT_OF_RANGE
#	undef SE_THROW_OUT_OF_MEMORY
#	undef SE_CONNECT
#	undef SE_CONDUCT
#endif //SE_MACRO_HELPERS
#define SE_MACRO_HELPERS
#if defined (QT_DEBUG)
#	define SE_DEBUG( _expression_ )			_expression_
#	define SE_ASSERT( _expression_ )		Q_ASSERT( _expression_ )
#	define SE_VERIFY( _expression_ )		Q_ASSERT( _expression_ )
#	define SE_DBGMESSAGE( _msg_ )			qDebug( _msg_ )
#	define SE_DBGBREAK( _msg_ )				SE_DBGMESSAGE( _msg_ ); SE_ASSERT(false)
#	define SE_STUB( _func_ )				SE_DBGBREAK( _func_ )
#	define SE_THROW_BAD				 		SE_DBGBREAK("Bad Exception:"); throw std::bad_exception()
#	define SE_THROW_FATAL					SE_DBGBREAK("Fatal Exception:"); std::runtime_error("Fatal Error:")
#	define SE_THROW_INVALID_PTR			    SE_DBGBREAK("Invalid Pointer Exception:"); throw std::invalid_argument("Invalid Pointer:")
#	define SE_THROW_OUT_OF_RANGE			SE_DBGBREAK("Out Of Range Exception:"); throw std::out_of_range("Index out of range:")
#	define SE_THROW_OUT_OF_MEMORY			SE_DBGBREAK("Out Of Memory Exception:"); throw std::bad_alloc()
#	define SE_CHECK_AND_RETURN( _cndtn_ ) if( !bool(_cndtn_) ) {SE_ASSERT( false ); return;}
#	define SE_CHECK_AND_RETURN_VAL( _cndtn_, _val_ ) if( !bool(_cndtn_) ) {SE_ASSERT( false ); return _val_;}
#	define SE_CHECK_IDX( _idx_, _max_ )	    SE_ASSERT( _idx_ >= 0 && _idx_ < _max_ )
#	define SE_ASSERT_IDX( _idx_, _max_ )	SE_ASSERT( _idx_ >= 0 && _idx_ < _max_ )
#	define SE_CHECK_IDX_AND_RETURN( _idx_, _max_ ) if( _idx_ < 0 || _idx_ >= _max_ ) {SE_ASSERT( false ); return;}
#	define SE_CHECK_IDX_AND_RETURN_VAL( _idx_, _max_, _val_ ) if( _idx_ < 0 || _idx_ >= _max_ ) {SE_ASSERT( false ); return _val_;}
#	define SE_CHECK_PTR( _ptr_ )			SE_ASSERT( _ptr_ != nullptr )
#	define SE_ASSERT_PTR( _ptr_ )			SE_ASSERT( _ptr_ != nullptr )
#	define SE_CHECK_NEW_PTR( _ptr_ )		SE_ASSERT( _ptr_ )
#	define SE_CHECK_PTR_AND_RETURN( _ptr_ ) if( _ptr_ == NULL ) {SE_ASSERT( false ); return;}
#	define SE_CHECK_PTR_AND_RETURN_VAL( _ptr_, _val_ ) if( _ptr_ == NULL ) {SE_ASSERT( false ); return _val_;}
#elif defined (QT_NO_DEBUG)
#	define SE_DEBUG( _expression_ )			qt_noop()
#	define SE_ASSERT( _expression_ )		qt_noop()
#	define SE_VERIFY( _expression_ )		((void)(_expression_))
#	define SE_DBGMESSAGE( _msg_ )			
#	define SE_DBGBREAK( _msg_ )			
#	define SE_STUB( _func_ )				
#	define SE_THROW_BAD						throw std::bad_exception()
#	define SE_THROW_FATAL					throw std::runtime_error("Fatal Error:")
#	define SE_THROW_INVALID_PTR				throw std::invalid_argument("Invalid Pointer:")
#	define SE_THROW_OUT_OF_RANGE			throw std::out_of_range("Index out of range:")
#	define SE_THROW_OUT_OF_MEMORY			throw std::bad_alloc()
#	define SE_CHECK_AND_RETURN( _cndtn_ ) if( !bool(_cndtn_) ) {return;}
#	define SE_CHECK_AND_RETURN_VAL( _cndtn_, _val_ ) if( !bool(_cndtn_) ) {return _val_;}
#	define SE_CHECK_IDX( _idx_, _max_ )	if( _idx_ < 0 || _idx_ >= _max_ ) SE_THROW_OUT_OF_RANGE
#	define SE_ASSERT_IDX( _idx_, _max_ )	qt_noop()
#	define SE_CHECK_IDX_AND_RETURN( _idx_, _max_ )	if( _idx_ < 0 || _idx_ >= _max_ ) {return;}
#	define SE_CHECK_IDX_AND_RETURN_VAL( _idx_, _max_, _val_ )	if( _idx_ < 0 || _idx_ >= _max_ ) {return _val_;}
#	define SE_CHECK_PTR( _ptr_ )			if( _ptr_ == nullptr ) SE_THROW_INVALID_PTR
#	define SE_ASSERT_PTR( _ptr_ )			qt_noop()
#	define SE_CHECK_NEW_PTR( _ptr_ )		if( _ptr_ == nullptr ) SE_THROW_OUT_OF_MEMORY
#	define SE_CHECK_PTR_AND_RETURN( _ptr_ ) if( _ptr_ == nullptr ) {return;}
#	define SE_CHECK_PTR_AND_RETURN_VAL( _ptr_, _val_ ) if( _ptr_ == nullptr ) {return _val_;}
#endif //_DEBUG

// Connect/Disconnect sender's signal and reciever's slot.
#define SE_CONNECT( _sender_, _signal_, _receiver_, _slot_ )			\
	SE_VERIFY( QObject::connect( _sender_, SIGNAL( _signal_ ), _receiver_, SLOT( _slot_ ) ) )
#define SE_CONNECT_EX( _sender_, _signal_, _receiver_, _slot_, _type_ )\
	SE_VERIFY( QObject::connect( _sender_, SIGNAL( _signal_ ), _receiver_, SLOT( _slot_ ), _type_ ) )
#define SE_CONDUCT( _sender_, _signal_, _receiver_, _sgnl_)			\
	SE_VERIFY( QObject::connect( _sender_, SIGNAL( _signal_ ), _receiver_, SIGNAL( _sgnl_ ) ) )
#define SE_CONDUCT_EX( _sender_, _signal_, _receiver_, _sgnl_, _type_ )\
	SE_VERIFY( QObject::connect( _sender_, SIGNAL( _signal_ ), _receiver_, SIGNAL( _sgnl_ ), _type_ ) )

#define SE_DISCONNECT( _sender_, _signal_, _receiver_, _slot_)			\
	(QObject::disconnect( _sender_, SIGNAL( _signal_ ), _receiver_, SLOT( _slot_ ) ))
#define SE_DISCONDUCT( _sender_, _signal_, _receiver_, _sgnl_)			\
	SE_VERIFY( QObject::disconnect( _sender_, SIGNAL( _signal_ ), _receiver_, SIGNAL( _sgnl_ ) ) )
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Useful Defines
//
#define SE_DEFINES
////////////////////////////////////////////////////////////////////////////////

#endif // SE_CORE_MACROS_H
