////////////////////////////////////////////////////////////////////////////////
//
// The PduLib Library
// Copyright (c) 2009 Litwin Sergey
// The copyright on this file is protected under the terms of the MIT license.
//
// Permission to use, copy, modify, distribute and sell this software for any
// purpose is hereby granted without fee, provided that the above copyright
// notice appear in all copies and that both that copyright notice and this
// permission notice appear in supporting documentation.
//
// The author makes no representations about the suitability of this software
// for any purpose. It is provided "as is" without express or implied warranty.
//
//////////////////////////////////////////////////////////////////////////////// 
#ifndef _PDULIB_MESSAGE_
#define _PDULIB_MESSAGE_


#include "MessageUnit.h"

namespace PduLib
{

/** \defgroup PDUTypeListsGroup PDU Type Lists */

/** \brief SMS-SUBMIT type list 
    \ingroup PDUTypeListsGroup
 */
typedef Loki::TL::MakeTypelist
<
    TPServiceCenterAddress,
    TPMtiAndFriendsSmsSubmit,
    TPMessageReference,
    TPDestinationAddress,
    TPProtocolIdentifier,
    TPDataCodingScheme,
    TPValidityPeriod,
    TPUserData
>::Result SmsSubmit;


/** \brief SMS-SUBMIT-REPORT for RP-ACK type list 
    \ingroup PDUTypeListsGroup
 */
typedef Loki::TL::MakeTypelist
<
    TPMtiAndFriendsSmsSubmitReport,
    TPParameterIndicator,
    TPServiceCentreTimeStamp,
    TPProtocolIdentifier,
    TPDataCodingScheme,
    TPUserData
>::Result SmsSubmitReportAck;


/** \brief SMS-SUBMIT-REPORT for RP-ERROR type list 
    \ingroup PDUTypeListsGroup
 */
typedef Loki::TL::MakeTypelist
<
    TPMtiAndFriendsSmsSubmitReport,
    TPFailureCause,
    TPParameterIndicator,
    TPServiceCentreTimeStamp,
    TPProtocolIdentifier,
    TPDataCodingScheme,
    TPUserData
>::Result SmsSubmitReportError;


/** \brief SMS-DELIVER type list 
    \ingroup PDUTypeListsGroup
 */
typedef Loki::TL::MakeTypelist
<
    TPServiceCenterAddress,
    TPMtiAndFriendsSmsDeliver,
    TPOriginatorAddress,
    TPProtocolIdentifier,
    TPDataCodingScheme,
    TPServiceCentreTimeStamp,
    TPUserData
>::Result SmsDeliver;


/** \brief SMS-DELIVER-REPORT for RP-ACK type list
    \ingroup PDUTypeListsGroup
 */
typedef Loki::TL::MakeTypelist
<
    TPMtiAndFriendsSmsDeliverReport,
    TPParameterIndicator,
    TPProtocolIdentifier,
    TPDataCodingScheme,
    TPUserData
>::Result SmsDeliverReportAck;


/** \brief SMS-DELIVER-REPORT for RP-ERROR type list 
    \ingroup PDUTypeListsGroup
 */
typedef Loki::TL::MakeTypelist
<
    TPMtiAndFriendsSmsDeliverReport,
    TPFailureCause,
    TPParameterIndicator,
    TPProtocolIdentifier,
    TPDataCodingScheme,
    TPUserData
>::Result SmsDeliverReportError;


/** \brief SMS-STATUS-REPORT type list 
    \ingroup PDUTypeListsGroup
 */
typedef Loki::TL::MakeTypelist
<
    TPServiceCenterAddress,
    TPMtiAndFriendsSmsStatusReport,
    TPMessageReference,
    TPRecipientAddress,
    TPServiceCentreTimeStamp,
    TPDischargeTime,
    TPStatus,
    TPParameterIndicator,
    TPProtocolIdentifier,
    TPDataCodingScheme,
    TPUserData
>::Result SmsStatusReport;


/** \brief SMS-COMMAND type list 
    \ingroup PDUTypeListsGroup
 */
typedef Loki::TL::MakeTypelist
<
    TPServiceCenterAddress,
    TPMtiAndFriendsSmsCommand,
    TPMessageReference,
    TPProtocolIdentifier,
    TPCommandType,
    TPMessageNumber,
    TPDestinationAddress,
    TPCommandData
>::Result SmsCommand;


/** \brief Generates the class hierarchy for a given PDU type list 
    \ingroup PDUTypeListsGroup
 */
template<class MessageType>
class Message
    : public Loki::GenLinearHierarchy
    <
        typename Loki::TL::Reverse<MessageType>::Result,
        PduLib::MessageUnit, PduLib::MessageUnitBase<char, std::char_traits<char> >
    >
{
    typedef Loki::GenLinearHierarchy<
        typename Loki::TL::Reverse<MessageType>::Result,
        PduLib::MessageUnit, PduLib::MessageUnitBase<char, std::char_traits<char> >
    > _Base;

public:
    Message()
    {}

    explicit Message(std::istream& st)
    {
		_Base::from_stream(st);
    }

    Message(const Message& message)
        : _Base(message)
    {}
};

} // namespace PduLib

#endif // _PDULIB_MESSAGE_
