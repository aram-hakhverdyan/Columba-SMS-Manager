////////////////////////////////////////////////////////////////////////////////
//
//               ***              ****     ****         **         **                           
//              ** **             ** **   ** **          **       **                                     
//             **   **            **  ** **  **           **     **                            	 
//            ** *** **           **   ***   **            **   **                           
//           **       **          **         **             ** **                          
//          **         **         **         **              ***        
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SE_CORE_H
#define SE_CORE_H

////////////////////////////////////////////////////////////////////////////////
//
//	Includes
//
#include "smsengine_global.h"
#include "se_core_macros.h"
#include "se_logger.h"
//
//	Qt Includes
//
#include <QString>
#include <QCoreApplication>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Configure Default Logger
//
static se::CLoger gs_oLogger( "ColumbaTemp" );

//#ifdef SE_LOGGING_OFF
//#	undef SE_LOGGING_OFF
//#endif
//#define SE_LOGGING_ON
#define SE_LOGGING_OFF

#if defined (SE_LOGGING_ON)
#	define SE_LOG( _msg_ )					gs_oLogger.Log(_msg_)
#	define LOG(_msg_)						SE_LOG( _msg_ )	
#	define SE_START_HEADER(_msg_)					gs_oLogger.StartHeader(_msg_)
#	define LOG_HEADER(_msg_)					    SE_START_HEADER(_msg_)
#elif defined (SE_LOGGING_OFF)
#	define SE_LOG( _msg_ )					qt_noop()
#	define LOG(_msg_)						SE_LOG( _msg_ )	
#	define SE_START_HEADER(_msg_)					qt_noop()
#	define LOG_HEADER(_msg_)					    SE_START_HEADER(_msg_)
#endif 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Type Redefinitions
//
typedef QLatin1String       t_sz;
typedef const QLatin1String t_csz;
////////////////////////////////////////////////////////////////////////////////

#define qtr(_text_) QCoreApplication::translate( "se_core", _text_ )


////////////////////////////////////////////////////////////////////////////////
namespace se { // SMS Engin 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//	Global Enumerators
//
	
// 
//	SMS Message Origin
//
enum class EMessageOrigin
{
	eUndefined = 0,
	eNewMessage,
	eReceivedMessage
};

//
//	SMS Mode
//
enum class ESmsMode
{
	ePDUMode  = 0,
	eTextMode = 1,
	eUnknown
};


////////////////////////////////////////////////////////////////////////////////
//
//	AT Enumerations
//
////////////////////////////////////////////////////////////////////////////////

//
//	AT Command Role 
//
enum class ECommandRole
{
	ExecuteCommand = 0,
	eCheckState,         // ?
	eCheckPossibleValues // =?
};

//
//	AT Command Status 
//
enum class ECommandStatus
{
	Undefined = 0,
	Succeeded,
	Failed,
	Executing
};

//
//	AT Parsing Status
//
enum class EParsingStatus
{
	Unrecognised = 0,
	Processed,
	NeedMoreData,
	Failed
};

//
//	AT Responce Major Type
//
enum class EATResponceMajorType : quint8
{	
	Unknown = 0,
	ATEvent,
	ATCommandResponce,
};

inline QString ToString( EATResponceMajorType eMajorType )
{
	switch (eMajorType)
	{
		case EATResponceMajorType::Unknown:
			return QString( "Unknown" );
		case EATResponceMajorType::ATEvent:
			return QString( "AT Event" );
		case EATResponceMajorType::ATCommandResponce:
			return QString( "Command Responce" );
		default:
			return QString();
	}
}

//
//	AT Packet Type
//! enumeration of AT respone packet types
enum class EATResponceType : quint16
{
	Unknown         = quint8( EATResponceMajorType::Unknown ),
	//
	//	Events
	// 
	InvalidEvent = quint8( EATResponceMajorType::ATEvent ),
	RSSI = quint8( EATResponceMajorType::ATEvent )       | 0x10, // RSSI value changed
	NewMessage = quint8( EATResponceMajorType::ATEvent ) | 0x20,
	

	
	//
	//	Command Responces
	//
	InvalidCommandResponce = quint8( EATResponceMajorType::ATCommandResponce ),
	DefaultOK			   = quint8( EATResponceMajorType::ATCommandResponce ) | 0x10,	// OK
	DefaultError		   = quint8( EATResponceMajorType::ATCommandResponce ) | 0x20,	// ERROR
	EmptyResponce		   = quint8( EATResponceMajorType::ATCommandResponce ) | 0x30, 
	CMD_CMGF			   = quint8( EATResponceMajorType::ATCommandResponce ) | 0x40,
	CMD_CMGS			   = quint8( EATResponceMajorType::ATCommandResponce ) | 0x50,
	CMD_CSCA			   = quint8( EATResponceMajorType::ATCommandResponce ) | 0x60,
	CMD_CMSError		   = quint8( EATResponceMajorType::ATCommandResponce ) | 0x70
};

//
//	AT Task Status 
//
enum class ETaskStatus
{
	Undefined = int( ECommandStatus::Undefined ),
	Succeeded = int( ECommandStatus::Succeeded ),
	Failed    = int( ECommandStatus::Failed )
};

////////////////////////////////////////////////////////////////////////////////








////////////////////////////////////////////////////////////////////////////////
} // namespace se
////////////////////////////////////////////////////////////////////////////////

#endif // SE_CORE_H
