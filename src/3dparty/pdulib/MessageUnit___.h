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
#ifndef _PDULIB_MESSAGE_UNIT_
#define _PDULIB_MESSAGE_UNIT_


#include <loki/Typelist.h>
#include <loki/HierarchyGenerators.h>
#include <loki/SmartPtr.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <bitset>
#include <memory>
#include <map>


/** \brief Comprises all the classes of PduLib. */
namespace PduLib
{

class TPServiceCenterAddress;
class TPMtiAndFriendsSmsSubmit;
class TPMtiAndFriendsSmsSubmitReport;
class TPMtiAndFriendsSmsDeliverReport;
class TPMtiAndFriendsSmsDeliver;
class TPMtiAndFriendsSmsDeliverReport;
class TPMtiAndFriendsSmsStatusReport;
class TPMtiAndFriendsSmsCommand;
class TPMessageReference;
class TPOriginatorAddress;
class TPDestinationAddress;
class TPRecipientAddress;
class TPProtocolIdentifier;
class TPFailureCause;
class TPStatus;
class TPCommandType;
class TPCommandData;
class TPMessageNumber;
class TPParameterIndicator;
class TPDataCodingScheme;
class TPServiceCentreTimeStamp;
class TPDischargeTime;
class TPValidityPeriod;
class TPUserData;


template<class CharT, class Traits = std::char_traits<CharT> >
class ServiceCenterAddress
{
public:
    typedef std::basic_string<CharT, Traits> _MyString;

    ServiceCenterAddress()
    {
    	init();
    }

    ServiceCenterAddress(const std::string address, unsigned char addressType = 0x81)
    {
    	init(address, addressType);
    }

    explicit ServiceCenterAddress(std::istream& st)
    {
    	init();
        from_stream(st);
    }

    ServiceCenterAddress(const ServiceCenterAddress& right)
    {
    	init(right.address_, right.addressType_);
    }

    ServiceCenterAddress& operator=(const ServiceCenterAddress& right)
    {
        if (this != &right)
        	init(right.address_, right.addressType_);
        return *this;
    }

    bool operator==(const ServiceCenterAddress& right) const
    {
        return address_ == right.address_
            && addressType_ == right.addressType_;
    }

    void from_stream(std::istream& st)
    {
        std::basic_ostringstream<CharT, Traits> dst;
        unsigned char len = 0;
        unsigned char toa = 0x81; // international unknown

        for (unsigned char i = 0; i < len + 1; ++i)
        {
            unsigned char x = (unsigned char)st.get();
            if (!st)
            {
                st.setstate( std::ios::failbit );
                return;
            }

            if (0 == i)
                len = x;
            else if (1 == i)
            {
                toa = x;
                if (toa == 0x91)
                    dst << dst.widen('+');
            }
            else
            {
                unsigned int hi = x >> 4;
                unsigned int lo = x & 0x0F;

                if ((lo > 9) || (hi > 9 && i < len-1))
                {
                    st.setstate( std::ios::failbit );
                    return;
                }

                dst << lo;
                if (hi < 10)
                    dst << hi;
            }
        }

        addressType_ = toa;
        address_ = dst.str();
    }

    void to_stream(std::ostream& st) const
    {
        typedef std::ctype<CharT> _Ctype;
        const _Ctype &fac = std::use_facet<_Ctype>(std::locale::classic());

        std::stringstream dst;
        std::stringstream tmp;

        bool b = false;
        unsigned int x = 0;
        unsigned int y = 0;

        for (size_t i = 0; i < address_.length(); ++i)
        {
            if (i == 0 && address_[i] == fac.widen('+'))
                continue;

            tmp << (unsigned char)address_[i] << std::endl;
            if (!(tmp >> std::dec >> x))
            {
                st.setstate( std::ios::failbit );
                return;
            }

            if (b)
                dst << (unsigned char) ((x << 4) | y);
            else
                y = x;

            b = !b;
        }

        if (b)
            dst << (unsigned char) (0xF0 | y);

        if (0 < dst.str().length())
        {
            st << (unsigned char) (dst.str().length() + 1);
            st << addressType_;
            st << dst.str();
        }
        else
            st << (unsigned char) 0x00;
    }

    unsigned char addressType() const
    {
        return addressType_;
    }

    void addressType(unsigned char value)
    {
        addressType_ = value;
    }

    _MyString address() const
    {
        return address_;
    }

    void address(const _MyString& value)
    {
        typedef std::ctype<CharT> _Ctype;
        const _Ctype &fac = std::use_facet<_Ctype>(std::locale::classic());

        address_ = value;
        if (address_.size() && fac.narrow(address_[0], '\0') == '+')
            addressType_ = 0x91;
    }

private:
	void init(const _MyString& address = _MyString(), unsigned char addressType = 0x81)
	{
		address_ = address;
		addressType_ = addressType;
	}

    _MyString address_;
    unsigned char addressType_;
};


// Address

template<class CharT, class Traits = std::char_traits<CharT> >
class Address
{
public:
    typedef std::basic_string<CharT, Traits> _MyString;

    Address()
    {
    	init();
    }

    explicit Address(std::istream& st)
    {
        from_stream(st);
    }

    Address(const Address& right)
    {
    	init(right.address_, right.addressType_);
    }

    Address& operator=(const Address& right)
    {
        if (this != &right)
        	init(right.address_, right.addressType_);
        return *this;
    }

    bool operator==(const Address& right) const
    {
        return addressType_ == right.addressType_
            && address_ == right.address_;
    }

    void from_stream(std::istream& st)
    {
        std::basic_ostringstream<CharT, Traits> dst;
        unsigned char len = 0;
        unsigned char toa = 0;

        for (unsigned char i = 0; i < len + 1; ++i)
        {
            unsigned char x = (unsigned char)st.get();
            if (!st) {
                st.setstate( std::ios::failbit );
                return;
            }

            if (0 == i)
                len = x / 2 + x % 2 + 1;
            else if (1 == i)
            {
                toa = x;
                if (0x91 == toa && 1 < len)
                    dst << dst.widen('+');
            }
            else
            {
                unsigned int hi = x >> 4;
                unsigned int low = x & 0x0F;

                if ( (low > 9) || (hi > 9 && i < len-1) )
                {
                    st.setstate( std::ios::failbit );
                    return;
                }

                dst << low;
                if (hi < 10)
                    dst << hi;
            }
        }

        addressType_ = toa;
        address_ = dst.str();
    }

    void to_stream(std::ostream& st) const
    {
        typedef std::ctype<CharT> _Ctype;
        const _Ctype &fac = std::use_facet<_Ctype>(std::locale::classic());

        std::stringstream dst;
        std::stringstream tmp;

        bool sign = false;
        bool b = false;
        unsigned int x = 0;
        unsigned int y = 0;
        size_t i = 0;

        for (; i < address_.length(); ++i)
        {
            if (i == 0 && address_[i] == fac.widen('+'))
            {
                sign = true;
                continue;
            }

            tmp << (unsigned char)address_[i] << std::endl;
            if (!(tmp >> std::dec >> x))
            {
                st.setstate( std::ios::failbit );
                return;
            }

            if (b)
                dst << (unsigned char) ((x << 4) | y);
            else
                y = x;

            b = !b;
        }

        if (b)
            dst << (unsigned char) (0xF0 | y);

        st << (unsigned char)(i - (sign ? 1 : 0));
        st << addressType_;
        if (0 < dst.str().length())
            st << dst.str();
    }

    unsigned char addressType() const
    {
        return addressType_;
    }

    void addressType(unsigned char value)
    {
        addressType_ = value;
    }

    _MyString address() const
    {
        return address_;
    }

    void address(const _MyString& value)
    {
        typedef std::ctype<CharT> _Ctype;
        const _Ctype &fac = std::use_facet<_Ctype>(std::locale::classic());

        address_ = value;
        if (address_.size() && fac.narrow(address_[0], '\0') == '+')
            addressType_ = 0x91;
    }

private:
	void init(const _MyString& address = _MyString(), unsigned char addressType = 0x81)
	{
		address_ = address;
		addressType_ = addressType;
	}

	_MyString address_;
    unsigned char addressType_;
};


/** \brief Represents the local time.
    \sa MessageUnit<TPServiceCentreTimeStamp, Base>, MessageUnit<TPDischargeTime, Base>, MessageUnit<TPValidityPeriod, Base>::AbsoluteValidityPeriod
 */
class TimeStamp
{
public:
    TimeStamp()
    {
        init(0, 0, 0, 0, 0, 0, 0);
    }

    TimeStamp(unsigned char year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second, signed char timezone /* expressed in quarters of an hour */)
    {
        init(year, month, day, hour, minute, second, timezone);
    }

    explicit TimeStamp(std::istream& st)
    {
        from_stream(st);
    }

    TimeStamp(const TimeStamp& right)
    {
        init(right.year_, right.month_, right.day_, right.hour_, right.minute_, right.second_, right.timezone_);
    }

    TimeStamp& operator=(const TimeStamp& right)
    {
        if (this != &right)
            init(right.year_, right.month_, right.day_, right.hour_, right.minute_, right.second_, right.timezone_);
        return *this;
    }

    bool operator==(const TimeStamp& right) const
    {
        return year_ == right.year_
            && month_ == right.month_
            && day_ == right.day_
            && hour_ == right.hour_
            && minute_ == right.minute_
            && second_ == right.second_
            && timezone_ == right.timezone_;
    }

    void from_stream(std::istream& st)
    {
        unsigned char year = getUnsigned(st);
        unsigned char month = getUnsigned(st);
        unsigned char day = getUnsigned(st);
        unsigned char hour = getUnsigned(st);
        unsigned char minute = getUnsigned(st);
        unsigned char second = getUnsigned(st);
        signed char timezone = getSigned(st);

        if (!(st
            && 0 < month && month <= 12
            && 0 < day && day <= 31
            && hour <= 23
            && minute <= 59
            && second <= 59
            && -48 <= timezone && timezone <= 52))
        {
            st.setstate(std::ios::failbit);
        }

        year_ = year;
        month_ = month;
        day_ = day;
        hour_ = hour;
        minute_ = minute;
        second_ = second;
        timezone_ = timezone;
    }

    void to_stream(std::ostream& st) const
    {
        if (!(st
            && 0 < month_ && month_ <= 12
            && 0 < day_ && day_ <= 31
            && hour_ <= 23
            && minute_ <= 59
            && second_ <= 59
            && -48 <= timezone_ && timezone_ <= 52))
        {
            st.setstate(std::ios::failbit);
        }

        putUnsigned(st, year_);
        putUnsigned(st, month_);
        putUnsigned(st, day_);
        putUnsigned(st, hour_);
        putUnsigned(st, minute_);
        putUnsigned(st, second_);
        putSigned(st, timezone_);
    }

    unsigned char year() const { return year_; }
    void year(unsigned char value) { year_ = value; }

    unsigned char month() const { return month_; }
    void month(unsigned char value) { month_ = value; }

    unsigned char day() const { return day_; }
    void day(unsigned char value) { day_ = value; }

    unsigned char hour() const { return hour_; }
    void hour(unsigned char value) { hour_ = value; }

    unsigned char minute() const { return minute_; }
    void minute(unsigned char value) { minute_ = value; }

    unsigned char second() const { return second_; }
    void second(unsigned char value) { second_ = value; }

    signed char timezone() const { return timezone_; }
    void timezone(signed char value) { timezone_ = value; }

private:
    void init(unsigned char year, unsigned char month, unsigned char day, unsigned char hour, unsigned char minute, unsigned char second, signed char timezone)
    {
        year_ = year;
        month_ = month;
        day_ = day;
        hour_ = hour;
        minute_ = minute;
        second_ = second;
        timezone_ = timezone;
    }

    unsigned char getUnsigned(std::istream& st) const
    {
        unsigned char ch = (unsigned char)st.get();
        if (!st) {
            st.setstate(std::ios::failbit);
            return 0;
        }

        unsigned char hi = ch >> 4;
        unsigned char lo = ch & 0x0F;
        if (hi > 9 || lo > 9) {
            st.setstate(std::ios::failbit);
            return 0;
        }

        return 10 * lo + hi;
    }

    void putUnsigned(std::ostream& st, unsigned char x) const
    {
        if (x > 99) {
            st.setstate(std::ios::failbit);
            return;
        }

        st.put( ((x % 10) << 4) | (x / 10) );
    }

    signed char getSigned(std::istream& st) const
    {
        unsigned char ch = (unsigned char)st.get();
        if (!st) {
            st.setstate(std::ios::failbit);
            return 0;
        }

        unsigned char hi = ch >> 4;
        unsigned char lo = ch & 0x07;
        bool sign = 0 != (ch & 0x08);

        if (hi > 9) {
            st.setstate(std::ios::failbit);
            return 0;
        }

        signed char res = (signed char)(10 * lo + hi);
        return sign ? -res : res;
    }

    void putSigned(std::ostream& st, signed char x) const
    {
        if (x < -79 || x > 79) {
            st.setstate(std::ios::failbit);
            return;
        }

        unsigned char u = (unsigned char)((x < 0) ? -x : x);
        unsigned char sign = (x < 0) ? 0x08 : 0x00;

        st.put( ((u % 10) << 4) | (u / 10 | sign) );
    }

    unsigned char year_;
    unsigned char month_;
    unsigned char day_;
    unsigned char hour_;
    unsigned char minute_;
    unsigned char second_;
    signed char timezone_; // expressed in quarters of an hour
};


// MessageUnit template declaration

template<class MessageType, class Base> class MessageUnit;


/** \brief Base class in MessageUnit class hierarchy. */
template<class CharT = char, class Traits = std::char_traits<CharT> >
class MessageUnitBase
{
public:
    typedef CharT char_type;
    typedef Traits traits_type;

    /** \brief All subclasses in the hierarchy use this exception class. */
    class failure
        : public std::runtime_error
    {
    public:
        explicit failure(const std::string &_Message)
            : runtime_error(_Message)
        {}
    };

    MessageUnitBase()
    {}

    MessageUnitBase(const MessageUnitBase& /*right*/)
    {}

    MessageUnitBase& operator=(const MessageUnitBase& /*right*/)
    {
        return *this;
    }

    virtual ~MessageUnitBase()
    {}

    bool operator==(const MessageUnitBase& /*right*/) const
    {
        return true;
    }

    void from_stream(std::istream& /*st*/)
    {}

    void to_stream(std::ostream& /*st*/) const
    {}

    /** It is overridden by the MessageUnit<TPParameterIndicator, Base>::protocolIdentifierPresent() method. Always returns true here. */
    bool protocolIdentifierPresent() const
    {
        return true;
    }

    /** It is overridden by the MessageUnit<TPParameterIndicator, Base>::dataCodingSchemePresent() method. Always returns true here. */
    bool dataCodingSchemePresent() const
    {
        return true;
    }

    /** It is overridden by the MessageUnit<TPParameterIndicator, Base>::userDataPresent() method. Always returns true here. */
    bool userDataPresent() const
    {
        return true;
    }
};


/** \brief TP-Service-Center-Address field. */
template<class Base>
class MessageUnit<TPServiceCenterAddress, Base> : public Base
{
public:
    typedef ServiceCenterAddress<typename Base::char_type, typename Base::traits_type> _ServiceCenterAddress;
    typedef std::basic_string<typename Base::char_type, typename Base::traits_type> _MyString;

    MessageUnit()
    {}

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
        , address_(right.address_)
    {}

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right) {
            Base::operator=(right);
            address_ = right.address_;
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && address_ == right.address_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);
        address_.from_stream(st);
        if (!st)
            throw typename Base::failure("TPServiceCenterAddress: input stream read failure");
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);
        address_.to_stream(st);
        if (!st)
            throw typename Base::failure("TPServiceCenterAddress: output stream write failure");
    }

    _MyString serviceCenterAddress() const
    {
        return address_.address();
    }

    void serviceCenterAddress(const _MyString& value)
    {
        address_.address(value);
    }

    unsigned char serviceCenterAddressType() const
    {
        return address_.addressType();
    }

    void serviceCenterAddressType(unsigned char value)
    {
        address_.addressType(value);
    }

private:
    _ServiceCenterAddress address_;
};


/** \brief TP-Message-Type-Indicator, TP-Reject-Duplicates, TP-Validity-Period-Format, TP-Reply-Path, TP-User-Data-Header-Indicator, TP-Status-Report-Request fields for SMS-SUBMIT. */
template<class Base>
class MessageUnit<TPMtiAndFriendsSmsSubmit, Base> : public Base
{
public:
    MessageUnit()
    {
    	init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
    	init(right.rejectDuplicates_, right.validityPeriodFormat_, right.statusReportRequest_, right.userDataHeaderIndicator_, right.replyPath_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.rejectDuplicates_, right.validityPeriodFormat_, right.statusReportRequest_, right.userDataHeaderIndicator_, right.replyPath_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right) &&
            rejectDuplicates_ == right.rejectDuplicates_ &&
            validityPeriodFormat_ == right.validityPeriodFormat_ &&
            statusReportRequest_ == right.statusReportRequest_ &&
            userDataHeaderIndicator_ == right.userDataHeaderIndicator_ &&
            replyPath_ == right.replyPath_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char val = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPMtiAndFriendsSmsSubmit: input stream read failure");

        std::bitset<8> bs(val);
        if (!bs[0] || bs[1])
            throw typename Base::failure("TPMtiAndFriendsSmsSubmit: bad MTI");
        rejectDuplicates_ = bs[2];
        validityPeriodFormat_ = (bs[4] ? 0x02 : 0) | (bs[3] ? 0x01 : 0);
        statusReportRequest_ = bs[5];
        userDataHeaderIndicator_ = bs[6];
        replyPath_ = bs[7];
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        std::bitset<8> bs;
            bs[0] = 1;
            bs[1] = 0;
            bs[2] = rejectDuplicates_;
            bs[3] = 0 != (validityPeriodFormat_ & 0x01);
            bs[4] = 0 != (validityPeriodFormat_ & 0x02);
            bs[5] = statusReportRequest_;
            bs[6] = userDataHeaderIndicator_;
            bs[7] = replyPath_;
        if (!(st << (unsigned char) bs.to_ulong()))
            throw typename Base::failure("TPMtiAndFriendsSmsSubmit: output stream write failure");
    }

    bool rejectDuplicates() const { return rejectDuplicates_; }
    void rejectDuplicates(bool value) { rejectDuplicates_ = value; }

    bool statusReportRequest() const { return statusReportRequest_; }
    void statusReportRequest(bool value) { statusReportRequest_ = value; }

    bool replyPath() const { return replyPath_; }
    void replyPath(bool value) { replyPath_ = value; }

protected:
    int validityPeriodFormat() const { return validityPeriodFormat_; }
    void validityPeriodFormat(int value) { validityPeriodFormat_ = value; }

    bool userDataHeaderIndicator() const { return userDataHeaderIndicator_; }
    void userDataHeaderIndicator(bool value) { userDataHeaderIndicator_ = value; }

private:
	void init(bool rejectDuplicates = false, int validityPeriodFormat = 0, bool statusReportRequest = false, bool userDataHeaderIndicator = false, bool replyPath = false)
	{
		rejectDuplicates_ = rejectDuplicates;
		validityPeriodFormat_ = validityPeriodFormat;
		statusReportRequest_ = statusReportRequest;
		userDataHeaderIndicator_ = userDataHeaderIndicator;
		replyPath_ = replyPath;
	}

    bool rejectDuplicates_;
    int validityPeriodFormat_;
    bool statusReportRequest_;
    bool userDataHeaderIndicator_;
    bool replyPath_;
};


/** \brief TP-Message-Type-Indicator, TP-User-Data-Header-Indication fields for SMS-SUBMIT-REPORT. */
template<class Base>
class MessageUnit<TPMtiAndFriendsSmsSubmitReport, Base> : public Base
{
public:
    MessageUnit()
    {
    	init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
    	init(right.userDataHeaderIndicator_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.userDataHeaderIndicator_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && userDataHeaderIndicator_ == right.userDataHeaderIndicator_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char val = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPMtiAndFriendsSmsSubmitReport: input stream read failure");

        std::bitset<8> bs(val);
        if (!bs[0] || bs[1])
            throw typename Base::failure("TPMtiAndFriendsSmsSubmitReport: bad MTI");

        userDataHeaderIndicator_ = bs[6];
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        std::bitset<8> bs;
        bs[0] = 1;
        bs[1] = 0;
        bs[6] = userDataHeaderIndicator_;

        if (!(st << (unsigned char) bs.to_ulong()))
            throw typename Base::failure("TPMtiAndFriendsSmsSubmitReport: output stream write failure");
    }

protected:
    bool userDataHeaderIndicator() const { return userDataHeaderIndicator_; }
    void userDataHeaderIndicator(bool value) { userDataHeaderIndicator_ = value; }

private:
	void init(bool userDataHeaderIndicator = false)
	{
		userDataHeaderIndicator_ = userDataHeaderIndicator;
	}

    bool userDataHeaderIndicator_;
};


/** \brief TP-Message-Type-Indicator, TP-More-Messages-to-Send, TP-Reply-Path, TP-User-Data-Header-Indication, TP-Status-Report-Indication fields for SMS-DELIVER. */
template<class Base>
class MessageUnit<TPMtiAndFriendsSmsDeliver, Base> : public Base
{
public:
    MessageUnit()
    {
    	init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
    	init(right.moreMessageToSend_, right.statusReportIndication_, right.userDataHeaderIndicator_, right.replyPath_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.moreMessageToSend_, right.statusReportIndication_, right.userDataHeaderIndicator_, right.replyPath_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right) &&
            moreMessageToSend_ == right.moreMessageToSend_ &&
            statusReportIndication_ == right.statusReportIndication_ &&
            userDataHeaderIndicator_ == right.userDataHeaderIndicator_ &&
            replyPath_ == right.replyPath_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char val = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPMtiAndFriendsSmsDeliver: input stream read failure");

        std::bitset<8> bs(val);
        if (bs[0] || bs[1])
            throw typename Base::failure("TPMtiAndFriendsSmsDeliver: bad MTI");

        moreMessageToSend_ = bs[2];
        statusReportIndication_ = bs[5];
        userDataHeaderIndicator_ = bs[6];
        replyPath_ = bs[7];
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        std::bitset<8> bs;
        bs[0] = 0;
        bs[1] = 0;
        bs[2] = moreMessageToSend_;
        bs[5] = statusReportIndication_;
        bs[6] = userDataHeaderIndicator_;
        bs[7] = replyPath_;
        if (!(st << (unsigned char) bs.to_ulong()))
            throw typename Base::failure("TPMtiAndFriendsSmsDeliver: output stream write failure");
    }

    bool moreMessageToSend() const { return moreMessageToSend_; }
    void moreMessageToSend(bool value) { moreMessageToSend_ = value; }

    bool statusReportIndication() const { return statusReportIndication_; }
    void statusReportIndication(bool value) { statusReportIndication_ = value; }

    bool replyPath() const { return replyPath_; }
    void replyPath(bool value) { replyPath_ = value; }

protected:
    bool userDataHeaderIndicator() const { return userDataHeaderIndicator_; }
    void userDataHeaderIndicator(bool value) { userDataHeaderIndicator_ = value; }

private:
	void init(bool moreMessageToSend = false, bool statusReportIndication = false, bool userDataHeaderIndicator = false, bool replyPath = false)
	{
	     moreMessageToSend_ = moreMessageToSend;
	     statusReportIndication_ = statusReportIndication;
	     userDataHeaderIndicator_ = userDataHeaderIndicator;
	     replyPath_ = replyPath;
	}

    bool moreMessageToSend_;
    bool statusReportIndication_;
    bool userDataHeaderIndicator_;
    bool replyPath_;
};


/** \brief TP-Message-Type-Indicator, TP-User-Data-Header-Indication fields for SMS-DELIVER-REPORT. */
template<class Base>
class MessageUnit<TPMtiAndFriendsSmsDeliverReport, Base> : public Base
{
public:
    MessageUnit()
    {
    	init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
    	init(right.userDataHeaderIndicator_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.userDataHeaderIndicator_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && userDataHeaderIndicator_ == right.userDataHeaderIndicator_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char val = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPMtiAndFriendsSmsDeliverReport: input stream read failure");

        std::bitset<8> bs(val);
        if (bs[0] || bs[1])
            throw typename Base::failure("TPMtiAndFriendsSmsDeliverReport: bad MTI");

        userDataHeaderIndicator_ = bs[6];
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        std::bitset<8> bs;
        bs[0] = 0;
        bs[1] = 0;
        bs[6] = userDataHeaderIndicator_;

        if (!(st << (unsigned char) bs.to_ulong()))
            throw typename Base::failure("TPMtiAndFriendsSmsSubmitReport: output stream write failure");
    }

protected:
    bool userDataHeaderIndicator() const { return userDataHeaderIndicator_; }
    void userDataHeaderIndicator(bool value) { userDataHeaderIndicator_ = value; }

private:
	void init(bool userDataHeaderIndicator = false)
	{
		userDataHeaderIndicator_ = userDataHeaderIndicator;
	}

    bool userDataHeaderIndicator_;
};


/** \brief TP-Message-Type-Indicator, TP-User-Data-Header-Indication, TP-More-Messages-to-Send, TP-Status-Report-Qualifier fields for SMS-STATUS-REPORT. */
template<class Base>
class MessageUnit<TPMtiAndFriendsSmsStatusReport, Base> : public Base
{
public:
    MessageUnit()
    {
    	init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
    	init(right.moreMessageToSend_, right.userDataHeaderIndicator_, right.statusReportQualifier_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.moreMessageToSend_, right.userDataHeaderIndicator_, right.statusReportQualifier_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && moreMessageToSend_ == right.moreMessageToSend_
            && userDataHeaderIndicator_ == right.userDataHeaderIndicator_
            && statusReportQualifier_ == right.statusReportQualifier_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char ch = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPMtiAndFriendsSmsStatusReport: input stream read failure");

        std::bitset<8> bs(ch);
        if (bs[0] || !bs[1])
            throw typename Base::failure("TPMtiAndFriendsSmsStatusReport: bad MTI");

        moreMessageToSend_ = bs[2];
        statusReportQualifier_ = bs[5];
        userDataHeaderIndicator_ = bs[6];
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        std::bitset<8> bs;
        bs[0] = 0;
        bs[1] = 1;
        bs[2] = moreMessageToSend_;
        bs[5] = statusReportQualifier_;
        bs[6] = userDataHeaderIndicator_;
        if (!(st << (unsigned char) bs.to_ulong()))
            throw typename Base::failure("TPMtiAndFriendsSmsDeliver: output stream write failure");
    }

    bool moreMessageToSend() const { return moreMessageToSend_; }
    void moreMessageToSend(bool value) { moreMessageToSend_ = value; }

    bool statusReportQualifier() const { return statusReportQualifier_; }
    void statusReportQualifier(bool value) { statusReportQualifier_ = value; }

protected:
    bool userDataHeaderIndicator() const { return userDataHeaderIndicator_; }
    void userDataHeaderIndicator(bool value) { userDataHeaderIndicator_ = value; }

private:
	void init(bool moreMessageToSend = false, bool userDataHeaderIndicator = false, bool statusReportQualifier = false)
	{
	    moreMessageToSend_ = moreMessageToSend;
	    userDataHeaderIndicator_ = userDataHeaderIndicator;
	    statusReportQualifier_ = statusReportQualifier;
	}

    bool moreMessageToSend_;
    bool userDataHeaderIndicator_;
    bool statusReportQualifier_;
};


/** \brief TP-Message-Type-Indicator,TP-User-Data-Header-Indication,TP-Status-Report-Request fields for SMS-COMMAND. */
template<class Base>
class MessageUnit<TPMtiAndFriendsSmsCommand, Base> : public Base
{
public:
    MessageUnit()
    {
    	init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
    	init(right.statusReportRequest_, right.userDataHeaderIndicator_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.statusReportRequest_, right.userDataHeaderIndicator_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right)
    {
        return Base::operator==(right)
            && statusReportRequest_ == right.statusReportRequest_
            && userDataHeaderIndicator_ == right.userDataHeaderIndicator_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char val = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPMtiAndFriendsSmsCommand: input stream read failure");

        std::bitset<8> bs(val);
        if (bs[0] || !bs[1])
            throw typename Base::failure("TPMtiAndFriendsSmsCommand: bad MTI");

        statusReportRequest_ = bs[5];
        userDataHeaderIndicator_ = bs[6];
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        std::bitset<8> bs;
        bs[0] = 0;
        bs[1] = 1;
        bs[5] = statusReportRequest_;
        bs[6] = userDataHeaderIndicator_;

        if (!(st << (unsigned char) bs.to_ulong()))
            throw typename Base::failure("TPMtiAndFriendsSmsCommand: output stream write failure");
    }

    bool statusReportRequest() const { return statusReportRequest_; }
    void statusReportRequest(bool value) { statusReportRequest_ = value; }

protected:
    bool userDataHeaderIndicator() const { return userDataHeaderIndicator_; }
    void userDataHeaderIndicator(bool value) { userDataHeaderIndicator_ = value; }

private:
	void init(bool statusReportRequest = false, bool userDataHeaderIndicator = false)
	{
	   statusReportRequest_ = statusReportRequest;
	   userDataHeaderIndicator_ = userDataHeaderIndicator;
	}

    bool statusReportRequest_;
    bool userDataHeaderIndicator_;
};


/** \brief TP-Message-Reference field, see 9.2.3.6 of GSM 03.40. */
template<class Base>
class MessageUnit<TPMessageReference, Base> : public Base
{
public:
    MessageUnit()
    {
    	init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
    	init(right.messageReference_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.messageReference_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && messageReference_ == right.messageReference_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char ch = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPMessageReference: input stream read failure");
        messageReference_ = ch;
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);
        if (!(st << messageReference_))
            throw typename Base::failure("TPMessageReference: output stream write failure");
    }

    unsigned char messageReference() const { return messageReference_; }
    void messageReference(unsigned char value) { messageReference_ = value; }

private:
	void init(unsigned char messageReference = 0)
	{
		messageReference_ = messageReference;
	}

    unsigned char messageReference_;
};


/** \brief TP-Destination-Address field, see 9.2.3.8 of GSM 03.40. */
template<class Base>
class MessageUnit<TPDestinationAddress, Base> : public Base
{
    typedef Address<typename Base::char_type,
        typename Base::traits_type> _Address;

public:
    typedef std::basic_string<typename Base::char_type,
        typename Base::traits_type> _MyString;

    MessageUnit()
    {}

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
        , address_(right.address_)
    {}

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right) {
            Base::operator=(right);
            address_ = right.address_;
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && address_ == right.address_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);
        address_.from_stream(st);
        if (!st)
            throw typename Base::failure("TPDestinationAddress: input stream read failure");
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);
        address_.to_stream(st);
        if (!st)
            throw typename Base::failure("TPDestinationAddress: output stream write failure");
    }

    _MyString destinationAddress() const { return address_.address(); }
    void destinationAddress(const _MyString& value) { address_.address(value); }

    unsigned char destinationAddressType() const { return address_.addressType(); }
    void destinationAddressType(unsigned char value) { address_.addressType(value); }

private:
    _Address address_;
};


/** \brief TP-Originating-Address field, see 9.2.3.7 of GSM 03.40. */
template<class Base>
class MessageUnit<TPOriginatorAddress, Base> : public Base
{
public:
    typedef std::basic_string<typename Base::char_type, typename Base::traits_type> _MyString;
    typedef Address<typename Base::char_type, typename Base::traits_type> _Address;

    MessageUnit()
    {}

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
        , address_(right.address_)
    {}

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right) {
            Base::operator=(right);
            address_ = right.address_;
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && address_==right.address_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);
        address_.from_stream(st);
        if (!st)
            throw typename Base::failure("TPDestinationAddress: input stream read failure");
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);
        address_.to_stream(st);
        if (!st)
            throw typename Base::failure("TPDestinationAddress: output stream write failure");
    }

    _MyString originatorAddress() const { return address_.address(); }
    void originatorAddress(const _MyString& value) { address_.address(value); }

    unsigned char originatorAddressType() const { return address_.addressType(); }
    void originatorAddressType(unsigned char value) { address_.addressType(value); }

private:
     _Address address_;
};


/** \brief TP-Recipient-Address field, see 9.2.3.14 of GSM 03.40. */
template<class Base>
class MessageUnit<TPRecipientAddress, Base> : public Base
{
public:
    typedef std::basic_string<typename Base::char_type, typename Base::traits_type> _MyString;
    typedef Address<typename Base::char_type, typename Base::traits_type> _Address;

    MessageUnit()
    {}

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
        , address_(right.address_)
    {}

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right) {
            Base::operator=(right);
            address_ = right.address_;
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && address_ == right.address_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);
        address_.from_stream(st);
        if (!st)
            throw typename Base::failure("TPRecipientAddress: input stream read failure");
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);
        address_.to_stream(st);
        if (!st)
            throw typename Base::failure("TPRecipientAddress: output stream write failure");
    }

    _MyString recipientAddress() const { return address_.address(); }
    void recipientAddress(const _MyString& value) { address_.address(value); }

    unsigned char recipientAddressType() const { return address_.addressType(); }
    void recipientAddressType(unsigned char value) { address_.addressType(value); }

private:
    _Address address_;
};


/** \brief TP-Parameter-Indicator field, see 9.2.3.27 of GSM 03.40. */
template<class Base>
class MessageUnit<TPParameterIndicator, Base> : public Base
{
public:
    MessageUnit()
    {
    	init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
    	init(right.protocolIdentifierPresent_, right.dataCodingSchemePresent_, right.userDataPresent_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.protocolIdentifierPresent_, right.dataCodingSchemePresent_, right.userDataPresent_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && protocolIdentifierPresent_ == right.protocolIdentifierPresent_
            && dataCodingSchemePresent_ == right.dataCodingSchemePresent_
            && userDataPresent_ == right.userDataPresent_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char ch = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPParameterIndicator: input stream read failure");

        std::bitset<8> bs(ch);
        protocolIdentifierPresent_ = bs[0];
        dataCodingSchemePresent_ = bs[1];
        userDataPresent_ = bs[2];
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        std::bitset<8> bs;
        bs[0] = protocolIdentifierPresent_;
        bs[1] = dataCodingSchemePresent_;
        bs[2] = userDataPresent_;

        st << (unsigned char) bs.to_ulong();
        if (!st)
            throw typename Base::failure("TPParameterIndicator: output stream write failure");
    }

    bool protocolIdentifierPresent() const { return protocolIdentifierPresent_; }
    void protocolIdentifierPresent(bool value) { protocolIdentifierPresent_ = value; }

    bool dataCodingSchemePresent() const { return dataCodingSchemePresent_; }
    void dataCodingSchemePresent(bool value) { dataCodingSchemePresent_ = value; }

    bool userDataPresent() const { return userDataPresent_; }
    void userDataPresent(bool value) { userDataPresent_ = value; }

private:
	void init(bool protocolIdentifierPresent = false, bool dataCodingSchemePresent = false, bool userDataPresent = false)
	{
		protocolIdentifierPresent_ = protocolIdentifierPresent;
		dataCodingSchemePresent_ = dataCodingSchemePresent;
		userDataPresent_ = userDataPresent;
	}

    bool protocolIdentifierPresent_;
    bool dataCodingSchemePresent_;
    bool userDataPresent_;
};


/** \brief TP-Protocol-Identifier field, see 9.2.3.9 of GSM 03.40. */
template<class Base>
class MessageUnit<TPProtocolIdentifier, Base> : public Base
{
public:
    MessageUnit()
    {
    	init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
    	init(right.protocolIdentifier_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.protocolIdentifier_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right) &&
            protocolIdentifier_ == right.protocolIdentifier_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        if (!Base::protocolIdentifierPresent())
            return;

        unsigned char ch = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPProtocolIdentifier: input stream read failure");
        protocolIdentifier_ = ch;
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        if (!Base::protocolIdentifierPresent())
            return;

        if (!(st << protocolIdentifier_))
            throw typename Base::failure("TPProtocolIdentifier: output stream write failure");
    }

    unsigned char protocolIdentifier() const { return protocolIdentifier_; }
    void protocolIdentifier(unsigned char value ) { protocolIdentifier_ = value; }

private:
	void init(unsigned char protocolIdentifier = 0)
	{
		 protocolIdentifier_ = protocolIdentifier;
	}

    unsigned char protocolIdentifier_;
};


/** \brief TP-Failure-Cause field, see 9.2.3.22 of GSM 03.40. */
template<class Base>
class MessageUnit<TPFailureCause, Base> : public Base
{
public:
    MessageUnit()
    {
    	init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
    	init(right.failureCause_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.failureCause_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right) &&
            failureCause_ == right.failureCause_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char ch = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPFailureCause: input stream read failure");
        failureCause_ = ch;
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);
        if (!(st << failureCause_))
            throw typename Base::failure("TPFailureCause: output stream write failure");
    }

public:
    unsigned char failureCause() const { return failureCause_; }
    void failureCause(unsigned char value) { failureCause_ = value; }

private:
	void init(unsigned char failureCause = 0)
	{
		failureCause_ = failureCause;
	}

    unsigned char failureCause_;
};


/** \brief TP-Status field, see 9.2.3.15 of GSM 03.40. */
template<class Base>
class MessageUnit<TPStatus, Base> : public Base
{
public:
    MessageUnit()
    {
    	init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
    	init(right.status_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.status_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right) &&
            status_ == right.status_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char ch = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPStatus: input stream read failure");
        status_ = ch;
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);
        if (!(st << status_))
            throw typename Base::failure("TPStatus: output stream write failure");
    }

    unsigned char status() const { return status_; }
    void status(unsigned char value) { status_ = value; }

private:
	void init(unsigned char status = 0)
	{
		 status_ = status;
	}

    unsigned char status_;
};


/** \brief TP-Message-Number field, see 9.2.3.18 of GSM 03.40. */
template<class Base>
class MessageUnit<TPMessageNumber, Base> : public Base
{
public:
    MessageUnit()
    {
        init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
        init(right.messageNumber_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.messageNumber_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && messageNumber_ == right.messageNumber_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char ch = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPMessageNumber: input stream read failure");
        messageNumber_ = ch;
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        if (!(st << messageNumber_))
            throw typename Base::failure("TPMessageNumber: output stream write failure");
    }

public:
    unsigned char messageNumber() const { return messageNumber_; }
    void messageNumber(unsigned char value) { messageNumber_ = value; }

private:
    void init(unsigned char messageNumber = 0)
    {
        messageNumber_ = messageNumber;
    }

    unsigned char messageNumber_;
};


/** \brief TP-Command-Type field, see 9.2.3.19 of GSM 03.40. */
template<class Base>
class MessageUnit<TPCommandType, Base> : public Base
{
public:
    MessageUnit()
    {
    	init();
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
    	init(right.commandType_);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right.commandType_);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right) &&
            commandType_ == right.commandType_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char ch = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPCommandType: input stream read failure");
        commandType_ = ch;
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);
        if (!(st << commandType_))
            throw typename Base::failure("TPCommandType: output stream write failure");
    }

public:
    unsigned char commandType() { return commandType_; }
    void commandType(unsigned char value) { commandType_ = value; }

private:
	void init(unsigned char commandType = 0)
	{
		 commandType_ = commandType;
	}

    unsigned char commandType_;
};


/** \brief TP-Command-Data, TP-Command-Data-Length fields, see 9.2.3.20, 9.2.3.21 of GSM 03.40. */
template <class Base>
class MessageUnit<TPCommandData, Base> : public Base
{
public:
    typedef std::vector<unsigned char> _BinaryData;

    MessageUnit()
    {}

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
        , commandData_(right.commandData_)
    {}

    MessageUnit& operator=(const MessageUnit& right)
    {
        Base::operator=(right);
        commandData_ = right.commandData_;
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && commandData_ == right.commandData_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        unsigned char octetLength = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPCommandData: input stream read failure");

        if (!octetLength)
            commandData_.clear();
        else
        {
            _BinaryData data(octetLength);
            st.read((char*)&data[0], octetLength);
            if (!st)
                throw typename Base::failure("TPCommandData: input stream read failure");
            commandData_.swap(data);
        }
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        if (!(st << (unsigned char)commandData_.size()))
            throw typename Base::failure("TPCommandData: output stream write failure");

        if (commandData_.size())
        {
            st.write((char*)&commandData_[0], commandData_.size());
            if (!st)
                throw typename Base::failure("TPCommandData: output stream write failure");
        }
    }

    _BinaryData& commandData() const { return commandData_; }
    void commandData(const _BinaryData& value) { commandData_ = value; }

private:
    _BinaryData commandData_;
};


/** \brief TP-Data-Coding-Scheme field, see GSM 03.38. */
template<class Base>
class MessageUnit<TPDataCodingScheme, Base> : public Base
{
public:
    /** \brief Enumerates %Message Classes. */
    enum MessageClass {
        NoMessageClass = 0,
        MessageClass0,
        MessageClass1_MESpecific,
        MessageClass2_SIMSpecific,
        MessageClass3_TESpecific
    };

    /** \brief Enumerates Alphabets. */
    enum Alphabet {
        DefaultAlphabet = 0,
        EightBitDataAlphabet,
        UCS2Alphabet,
        ReservedAlphabet
    };

    /** \brief Enumerates Indication Types. */
    enum IndicationType {
        VoicemailIndicationType = 0,
        FaxIndicationType,
        EMailIndicationType,
        OtherIndicationType
    };

    /** \brief Each subclass of this abstract class represents corresponding Coding Group, see GSM 03.38. */
    class DataCodingSchemeBase
    {
    public:
        virtual ~DataCodingSchemeBase() {}
        virtual std::auto_ptr<DataCodingSchemeBase> clone() const = 0;
        virtual bool equal(const DataCodingSchemeBase &right) const = 0;
        virtual void from_stream(std::istream& st) = 0;
        virtual void to_stream(std::ostream& st) const = 0;
        virtual void from_char(unsigned char ch) = 0;
        virtual unsigned char to_char() const = 0;
    };

    /** \brief Represents General Data Coding Indication Group. */
    class GeneralDataCodingScheme : public DataCodingSchemeBase
    {
    public:
        GeneralDataCodingScheme()
        {
        	init();
        }

        GeneralDataCodingScheme(bool compressed, MessageClass messageClass, Alphabet alphabet)
        {
        	init(compressed, messageClass, alphabet);
        }

        explicit GeneralDataCodingScheme(unsigned char ch)
        {
            from_char(ch);
        }

        explicit GeneralDataCodingScheme(std::istream& st)
        {
            from_stream(st);
        }

        GeneralDataCodingScheme(const GeneralDataCodingScheme& dcs)
        {
        	init(dcs.compressed_, dcs.messageClass_, dcs.alphabet_);
        }

        GeneralDataCodingScheme& operator=(const GeneralDataCodingScheme& dcs)
        {
            if (this != &dcs)
            	init(dcs.compressed, dcs.messageClass, dcs.alphabet);
            return *this;
        }

        bool operator==(const GeneralDataCodingScheme& right) const
        {
            return compressed_ == right.compressed_ &&
                messageClass_ == right.messageClass_ &&
                alphabet_ == right.alphabet_;
        }

        virtual void from_stream(std::istream& st)
        {
            unsigned char ch = (unsigned char)st.get();
            if (!st)
                throw typename Base::failure("GeneralDataCodingScheme: input stream read failure");

            from_char(ch);
        }

        virtual void to_stream(std::ostream& st) const
        {
            if (!(st << to_char()))
                throw typename Base::failure("GeneralDataCodingScheme: output stream write failure");
        }

        virtual void from_char(unsigned char ch)
        {
            std::bitset<8> bs(ch);
            if (bs[6] || bs[7])
                throw typename Base::failure("GeneralDataCodingScheme: invalid coding scheme");

            if (!bs[2] && !bs[3])
                alphabet_ = DefaultAlphabet;
            else if (bs[2] && !bs[3])
                alphabet_ = EightBitDataAlphabet;
            else if (!bs[2] && bs[3])
                alphabet_ = UCS2Alphabet;
            else if (bs[2] && bs[3])
                alphabet_ = ReservedAlphabet;

            if (bs[4]) {
                if (!bs[0] && !bs[1])
                    messageClass_ = MessageClass0;
                else if (bs[0] && !bs[1])
                    messageClass_ = MessageClass1_MESpecific;
                else if (!bs[0] && bs[1])
                    messageClass_ = MessageClass2_SIMSpecific;
                else if (bs[0] && bs[1])
                    messageClass_ = MessageClass3_TESpecific;
            }
            else {
                messageClass_ = NoMessageClass;
            }

            compressed_ = bs[5];
        }

        virtual unsigned char to_char() const
        {
            std::bitset<8> bs;
            bs[6] = false;
            bs[7] = false;

            if (DefaultAlphabet == alphabet_) {
                bs[2] = false; bs[3] = false;
            }
            else if (EightBitDataAlphabet == alphabet_) {
                bs[2] = true; bs[3] = false;
            }
            else if (UCS2Alphabet == alphabet_) {
                bs[2] = false; bs[3] = true;
            }
            else if (ReservedAlphabet == alphabet_) {
                bs[2] = true; bs[3] = true;
            }

            if (NoMessageClass != messageClass_)
                bs[4] = true;

            if (MessageClass0 == messageClass_) {
                bs[0] = false; bs[1] = false;
            }
            else if (MessageClass1_MESpecific == messageClass_) {
                bs[0] = true; bs[1] = false;
            }
            else if (MessageClass2_SIMSpecific == messageClass_) {
                bs[0] = false; bs[1] = true;
            }
            else if (MessageClass3_TESpecific == messageClass_) {
                bs[0] = true; bs[1] = true;
            }
            bs[5] = compressed_;

            return (unsigned char)bs.to_ulong();
        }

        virtual std::auto_ptr<DataCodingSchemeBase> clone() const
        {
            return std::auto_ptr<DataCodingSchemeBase>(new GeneralDataCodingScheme(*this));
        }

        virtual bool equal(const DataCodingSchemeBase &dcs) const
        {
            const GeneralDataCodingScheme* p = dynamic_cast<const GeneralDataCodingScheme*>(&dcs);
            return p && *this == *p;
        }

        bool compressed() const { return compressed_; }
        void compressed(bool value) { compressed_ = value; }

        MessageClass messageClass() const { return messageClass_; }
        void messageClass(MessageClass value) { messageClass_ = value; }

        Alphabet alphabet() const { return alphabet_; }
        void alphabet(Alphabet value) { alphabet_ = value; }

    private:
    	void init(bool compressed = false, MessageClass messageClass = NoMessageClass, Alphabet alphabet = DefaultAlphabet)
    	{
			compressed_ = compressed;
			messageClass_ = messageClass;
			alphabet_ = alphabet;
    	}

        bool compressed_;
        MessageClass messageClass_;
        Alphabet alphabet_;
    };

    /** \brief Represents %Message Waiting Indication Group. */
    class MessageWaitingDataCodingScheme : public DataCodingSchemeBase
    {
    public:
        MessageWaitingDataCodingScheme()
        {
        	init();
        }

        MessageWaitingDataCodingScheme(bool discardContents, bool indicationActive, bool UCS2Alphabet, IndicationType indicationType)
        {
        	init(discardContents, indicationActive, UCS2Alphabet, indicationType);
        }

        explicit MessageWaitingDataCodingScheme(unsigned char ch)
        {
            from_char(ch);
        }

        explicit MessageWaitingDataCodingScheme(std::istream& st)
        {
            from_stream(st);
        }

        MessageWaitingDataCodingScheme(const MessageWaitingDataCodingScheme& dcs)
        {
        	init(dcs.discardContents_, dcs.indicationActive_, dcs.UCS2Alphabet_, dcs.indicationType_);
        }

        MessageWaitingDataCodingScheme& operator=(const MessageWaitingDataCodingScheme& dcs)
        {
            if (this != &dcs)
            	init(dcs.discardContents_, dcs.indicationActive_, dcs.UCS2Alphabet_, dcs.indicationType_);
            return *this;
        }

        bool operator==(const MessageWaitingDataCodingScheme& right) const
        {
            return discardContents_ == right.discardContents_
                && indicationActive_ == right.indicationActive_
                && UCS2Alphabet_ == right.UCS2Alphabet_
                && indicationType_ == right.indicationType_;
        }

        virtual void from_stream(std::istream& st)
        {
            unsigned char ch = (unsigned char)st.get();
            if (!st)
                throw typename Base::failure("MessageWaitingDataCodingScheme: input stream read failure");

            from_char(ch);
        }

        virtual void to_stream(std::ostream& st) const
        {
            if (!(st << (unsigned char) to_char()))
                throw typename Base::failure("MessageWaitingDataCodingScheme: output stream write failure");
        }

        virtual void from_char(unsigned char ch)
        {
            std::bitset<8> bs(ch);
            if (!bs[4] && !bs[5] && bs[6] && bs[7])
            {
                discardContents_ = true;
            }
            else if (bs[4] && !bs[5] && bs[6] && bs[7])
            {
                discardContents_ = false;
                UCS2Alphabet_ = false;
            }
            else if (!bs[4] && bs[5] && bs[6] && bs[7])
            {
                discardContents_ = false;
                UCS2Alphabet_ = true;
            }
            else
                throw typename Base::failure("MessageWaitingDataCodingScheme: invalid coding scheme");

            indicationActive_ = bs[3];

            if (!bs[0] && !bs[1])
                indicationType_ = VoicemailIndicationType;
            else if (bs[0] && !bs[1])
                indicationType_ = FaxIndicationType;
            else if (!bs[0] && bs[1])
                indicationType_ = EMailIndicationType;
            else if (bs[0] && bs[1])
                indicationType_ = OtherIndicationType;
        }

        virtual unsigned char to_char() const
        {
            std::bitset<8> bs;
            bs[6] = true; bs[7] = true;
            if (discardContents_) {
                bs[4] = false; bs[5] = false;
            }
            else {
                if (UCS2Alphabet_) {
                    bs[4] = false; bs[5] = true;
                }
                else {
                    bs[4] = true; bs[5] = false;
                }
            }

            bs[3] = indicationActive_;

            if (VoicemailIndicationType == indicationType_) {
                bs[0] = false; bs[1] = false;
            }
            else if (FaxIndicationType == indicationType_) {
                bs[0] = true; bs[1] = false;
            }
            else if    (EMailIndicationType == indicationType_) {
                bs[0] = false; bs[1] = true;
            }
            else if (OtherIndicationType == indicationType_) {
                bs[0] = true; bs[0] = true;
            }

            return (unsigned char)bs.to_ulong();
        }

        virtual std::auto_ptr<DataCodingSchemeBase> clone() const
        {
            return std::auto_ptr<DataCodingSchemeBase>(new MessageWaitingDataCodingScheme(*this));
        }

        virtual bool equal(const DataCodingSchemeBase &dcs) const
        {
            const MessageWaitingDataCodingScheme* p = dynamic_cast<const MessageWaitingDataCodingScheme*>(&dcs);
            return p && *this == *p;
        }

        IndicationType indicationType() const { return indicationType_; }
        void indicationType(IndicationType value) { indicationType_ = value; }

        bool discardContents() const { return discardContents_; }
        void discardContents(bool value) { discardContents_ = value; }

        bool indicationActive() const { return indicationActive_; }
        void indicationActive(bool value) { indicationActive_ = value; }

        bool UCS2Alphabet() const { return UCS2Alphabet_; }
        void UCS2Alphabet(bool value) { UCS2Alphabet_ = value; }

    private:
    	void init(bool discardContents = false, bool indicationActive = false, bool UCS2Alphabet = false, IndicationType indicationType = OtherIndicationType)
    	{
			discardContents_ = discardContents;
			indicationActive_ = indicationActive;
			UCS2Alphabet_ = UCS2Alphabet;
			indicationType_ = indicationType;
    	}

        bool discardContents_;
        bool indicationActive_;
        bool UCS2Alphabet_;
        IndicationType indicationType_;
    };

    /** \brief Represents Data Coding/Message Class Indication Group. */
    class MessageClassDataCodingScheme : public DataCodingSchemeBase
    {
    public:
        MessageClassDataCodingScheme()
        {
        	init();
        }

        MessageClassDataCodingScheme(MessageClass messageClass, bool eightBitData)
        {
        	init(messageClass, eightBitData);
        }

        explicit MessageClassDataCodingScheme(unsigned char ch)
        {
            from_char(ch);
        }

        explicit MessageClassDataCodingScheme(std::istream& st)
        {
            from_stream(st);
        }

        MessageClassDataCodingScheme(const MessageClassDataCodingScheme& dcs)
        {
        	init(dcs.messageClass_, dcs.eightBitData_);
        }

        MessageClassDataCodingScheme& operator=(const MessageClassDataCodingScheme& dcs)
        {
            if (this != &dcs)
            	init(dcs.messageClass, dcs.eightBitData);
            return *this;
        }

        bool operator==(const MessageClassDataCodingScheme& right) const
        {
            return messageClass_ == right.messageClass_ &&
                eightBitData_ == right.eightBitData_;
        }

        virtual void from_stream(std::istream& st)
        {
            unsigned char ch = (unsigned char)st.get();
            if (!st)
                throw typename Base::failure("MessageClassDataCodingScheme: input stream read failure");

            from_char(ch);
        }

        virtual void to_stream(std::ostream& st) const
        {
            if (!(st << to_char()))
                throw typename Base::failure("MessageClassDataCodingScheme: output stream write failure");
        }

        virtual void from_char(unsigned char ch)
        {
            std::bitset<8> bs(ch);
            if (!bs[4] || !bs[5] || !bs[6] || !bs[7])
                throw typename Base::failure("MessageClassDataCodingScheme: invalid coding scheme");

            if (bs[2])
                eightBitData_ = true;
            else
                eightBitData_ = false;

            if (!bs[0] && !bs[1])
                messageClass_ = MessageClass0;
            else if (bs[0] && !bs[1])
                messageClass_ = MessageClass1_MESpecific;
            else if (!bs[0] && bs[1])
                messageClass_ = MessageClass2_SIMSpecific;
            else if (bs[0] && bs[1])
                messageClass_ = MessageClass3_TESpecific;
        }

        virtual unsigned char to_char() const
        {
            std::bitset<8> bs;
            bs[4] = true; bs[5] = true;
            bs[6] = true; bs[7] = true;

            if(eightBitData_)
                bs[2] = true;

            if (MessageClass0 == messageClass_) {
                bs[0] = false; bs[1] = false;
            }
            else if (MessageClass1_MESpecific == messageClass_) {
                bs[0] = true; bs[1] = false;
            }
            else if (MessageClass2_SIMSpecific == messageClass_) {
                bs[0] = false; bs[1] = true;
            }
            else if (MessageClass3_TESpecific == messageClass_) {
                bs[0] = true; bs[1] = true;
            }

            return (unsigned char)bs.to_ulong();
        }

        virtual std::auto_ptr<DataCodingSchemeBase> clone() const
        {
            return std::auto_ptr<DataCodingSchemeBase>(new MessageClassDataCodingScheme(*this));
        }

        virtual bool equal(const DataCodingSchemeBase &dcs) const
        {
            const MessageClassDataCodingScheme* p = dynamic_cast<const MessageClassDataCodingScheme*>(&dcs);
            return p && *this == *p;
        }

        MessageClass messageClass() const { return messageClass_; }
        void messageClass(MessageClass value) { messageClass_ = value; }

        bool eightBitData() const { return eightBitData_; }
        void eightBitData(bool value) { eightBitData_ = value; }

    private:
    	void init(MessageClass messageClass = MessageClass0, bool eightBitData = false)
    	{
			messageClass_ = messageClass;
			eightBitData_ = eightBitData;
    	}

        MessageClass messageClass_;
        bool eightBitData_;
    };

    MessageUnit()
        : dataCodingScheme_(new GeneralDataCodingScheme())
    {}

    explicit MessageUnit(std::istream& st)
        : dataCodingScheme_(new GeneralDataCodingScheme())
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
        , dataCodingScheme_(right.dataCodingScheme_->clone())
    {}

    MessageUnit& operator=(const MessageUnit& right)
    {
        Base::operator=(right);
        dataCodingScheme_ = right.dataCodingScheme_->clone();
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right) &&
            dataCodingScheme_->equal(*right.dataCodingScheme_);
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        if (!Base::dataCodingSchemePresent())
            return;

        unsigned char ch = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPDataCodingScheme: input stream read failure");

        try
        {
            dataCodingScheme_.reset(new GeneralDataCodingScheme(ch));
        }
        catch (typename Base::failure&)
        {
            try
            {
                dataCodingScheme_.reset(new MessageWaitingDataCodingScheme(ch));
            }
            catch (typename Base::failure&)
            {
                dataCodingScheme_.reset(new MessageClassDataCodingScheme(ch));
            }
        }
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        if (!Base::dataCodingSchemePresent())
            return;

        dataCodingScheme_->to_stream(st);
    }

    /** Returns true if the stored Data Coding Scheme has specified type; false if it does not. */
    template<class _DataCodingScheme>
    bool has_dataCodingScheme() const
    {
        _DataCodingScheme* p = dynamic_cast<_DataCodingScheme*>(dataCodingScheme_.get());
        return (NULL != p);
    }

    /** Returns a reference to the stored Data Coding Scheme of specified type. Throws MessageUnitBase::failure exception if the stored Data Coding Scheme has a different type than specified. */
    template<class _DataCodingScheme>
    const _DataCodingScheme& dataCodingScheme() const
    {
        _DataCodingScheme* p = dynamic_cast<_DataCodingScheme*>(dataCodingScheme_.get());
        if (!p)
            throw typename Base::failure("TPDataCodingScheme: bad cast");
        return *p;
    }

    /** Sets the Data Coding Scheme of specified type. */
    template<class _DataCodingScheme>
    void dataCodingScheme(const _DataCodingScheme& value)
    {
        dataCodingScheme_.reset(new _DataCodingScheme(value));
    }

    /** Returns the #Alphabet which the stored Data Coding Scheme has */
    Alphabet alphabet() const
    {
        if (has_dataCodingScheme<GeneralDataCodingScheme>())
            return dataCodingScheme<GeneralDataCodingScheme>().alphabet();

        if (has_dataCodingScheme<MessageWaitingDataCodingScheme>()
            && dataCodingScheme<MessageWaitingDataCodingScheme>().UCS2Alphabet())
            return UCS2Alphabet;

        if (has_dataCodingScheme<MessageClassDataCodingScheme>()
            && dataCodingScheme<MessageClassDataCodingScheme>().eightBitData())
            return EightBitDataAlphabet;

        return DefaultAlphabet;
    }

private:
     std::auto_ptr<DataCodingSchemeBase> dataCodingScheme_;
};


/** \brief TP-Service-Centre-Time-Stamp field, see 9.2.3.11 of GSM 03.40. */
template <class Base>
class MessageUnit<TPServiceCentreTimeStamp, Base> : public Base
{
public:
    MessageUnit()
    {}

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
        , timeStamp_(right.timeStamp_)
    {}

    MessageUnit& operator=(const MessageUnit& right)
    {
        Base::operator=(right);
        timeStamp_ = right.timeStamp_;
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && timeStamp_ == right.timeStamp_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        timeStamp_.from_stream(st);
        if (!st)
            throw typename Base::failure("TPServiceCentreTimeStamp: input stream read failure");
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        timeStamp_.to_stream(st);
        if (!st)
            throw typename Base::failure("TPServiceCentreTimeStamp: output stream write failure");
    }

    TimeStamp serviceCentreTimeStamp() const { return timeStamp_; }
    void serviceCentreTimeStamp(const TimeStamp& value) { timeStamp_ = value; }

private:
    TimeStamp timeStamp_;
};


/** \brief TP-Discharge-Time field, see 9.2.3.13 of GSM 03.40. */
template <class Base>
class MessageUnit<TPDischargeTime, Base> : public Base
{
public:
    MessageUnit()
    {}

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
        , timeStamp_(right.timeStamp_)
    {}

    MessageUnit& operator=(const MessageUnit& right)
    {
        Base::operator=(right);
        timeStamp_ = right.timeStamp_;
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && timeStamp_ == right.timeStamp_;
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        timeStamp_.from_stream(st);
        if (!st)
            throw typename Base::failure("TPDischargeTime: input stream read failure");
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        timeStamp_.to_stream(st);
        if (!st)
            throw typename Base::failure("TPDischargeTime: output stream write failure");
    }

    TimeStamp dischargeTime() const { return timeStamp_; }
    void dischargeTime(const TimeStamp& value) { timeStamp_ = value; }

private:
    TimeStamp timeStamp_;
};


/** \brief TP-Validity-Period field, see 9.2.3.12 of GSM 03.40. */
template <class Base>
class MessageUnit<TPValidityPeriod, Base> : public Base
{
public:
    /** \brief Enumerates Validity Period Formats, see 9.2.3.3 of GSM 03.40 */
    enum ValidityPeriodFormat
    {
        NoValidityPeriodFormat = 0,
        EnhancedValidityPeriodFormat = 1,
        RelativeValidityPeriodFormat = 2,
        AbsoluteValidityPeriodFormat = 3
    };

    /** \brief Each subclass of this abstract class represents corresponding Validity Period Format, see GSM 03.40.
        \todo TP-Validity-Period Enhanced Format is not implemented yet.
     */
    class ValidityPeriodBase
    {
    public:
        typedef typename Base::failure failure;

        virtual ~ValidityPeriodBase() {}
        virtual void from_stream(std::istream& st) = 0;
        virtual void to_stream(std::ostream& st) const = 0;
        virtual std::auto_ptr<ValidityPeriodBase> clone() const = 0;
        virtual bool equal(const ValidityPeriodBase& vp) const = 0;
    };

    /** \brief Means that TP-Validity-Period field is not present. */
    class NoValidityPeriod : public ValidityPeriodBase
    {
    public:
        enum { validityPeriodFormat = NoValidityPeriodFormat };

        NoValidityPeriod()
        {}

        explicit NoValidityPeriod(std::istream& st)
        {
            from_stream(st);
        }

        NoValidityPeriod(const NoValidityPeriod& /*vp*/)
        {}

        NoValidityPeriod& operator=(const NoValidityPeriod& /*vp*/)
        {
            return *this;
        }

        bool operator==(const NoValidityPeriod& /*vp*/) const
        {
            return true;
        }

        virtual void from_stream(std::istream& /*st*/)
        {}

        virtual void to_stream(std::ostream& /*st*/) const
        {}

        virtual std::auto_ptr<ValidityPeriodBase> clone() const
        {
            return std::auto_ptr<ValidityPeriodBase>(new NoValidityPeriod(*this));
        }

        virtual bool equal(const ValidityPeriodBase& vp) const
        {
            return NULL != dynamic_cast<const NoValidityPeriod*>(&vp);
        }
    };

    /** \brief Represents TP-Validity-Period in relative format. */
    class RelativeValidityPeriod : public ValidityPeriodBase
    {
    public:
        enum { validityPeriodFormat = RelativeValidityPeriodFormat };

        RelativeValidityPeriod()
            : relativeValidityPeriod_(0)
        {}

        explicit RelativeValidityPeriod(unsigned char value)
            : relativeValidityPeriod_(value)
        {}

        explicit RelativeValidityPeriod(std::istream& st)
        {
            from_stream(st);
        }

        RelativeValidityPeriod(const RelativeValidityPeriod& vp)
            : relativeValidityPeriod_(vp.relativeValidityPeriod_)
        {}

        RelativeValidityPeriod& operator=(const RelativeValidityPeriod& vp)
        {
            if (this != &vp)
                relativeValidityPeriod_ = vp;
            return *this;
        }

        bool operator==(const RelativeValidityPeriod& vp) const
        {
            return relativeValidityPeriod_ == vp.relativeValidityPeriod_;
        }

        virtual void from_stream(std::istream& st)
        {
            unsigned char ch = (unsigned char)st.get();
            if (!st)
                throw typename Base::failure("RelativeValidityPeriod: input stream read failure");
            relativeValidityPeriod_ = ch;
        }

        virtual void to_stream(std::ostream& st) const
        {
            st << relativeValidityPeriod_;
            if (!st)
                throw typename Base::failure("RelativeValidityPeriod: output stream write failure");
        }

        virtual std::auto_ptr<ValidityPeriodBase> clone() const
        {
            return std::auto_ptr<ValidityPeriodBase>(new RelativeValidityPeriod(*this));
        }

        virtual bool equal(const ValidityPeriodBase& vp) const
        {
            const RelativeValidityPeriod* p = dynamic_cast<const RelativeValidityPeriod*>(&vp);
            return p &&
                *this == *p;
        }

        unsigned char relativeValidityPeriod() const { return relativeValidityPeriod_; }
        void relativeValidityPeriod(unsigned char value) { relativeValidityPeriod_ = value; }

    private:
        unsigned char relativeValidityPeriod_;
    };

    /** \brief Represents TP-Validity-Period field in absolute format. */
    class AbsoluteValidityPeriod : public ValidityPeriodBase
    {
    public:
        enum { validityPeriodFormat = AbsoluteValidityPeriodFormat };

        AbsoluteValidityPeriod()
        {}

        explicit AbsoluteValidityPeriod(const TimeStamp& ts)
            : timeStamp_(ts)
        {}

        explicit AbsoluteValidityPeriod(std::istream& st)
        {
            from_stream(st);
        }

        AbsoluteValidityPeriod(const AbsoluteValidityPeriod& right)
            : timeStamp_(right.timeStamp_)
        {}

        AbsoluteValidityPeriod& operator=(const AbsoluteValidityPeriod& right)
        {
            Base::operator=(right);
            timeStamp_ = right.timeStamp_;
            return *this;
        }

        bool operator==(const AbsoluteValidityPeriod& right) const
        {
            return timeStamp_ == right.timeStamp_;
        }

        virtual void from_stream(std::istream& st)
        {
            timeStamp_.from_stream(st);
            if (!st)
                throw typename Base::failure("AbsoluteValidityPeriod: input stream read failure");
        }

        virtual void to_stream(std::ostream& st) const
        {
            timeStamp_.to_stream(st);
            if (!st)
                throw typename Base::failure("AbsoluteValidityPeriod: output stream write failure");
        }

        virtual std::auto_ptr<ValidityPeriodBase> clone() const
        {
            return std::auto_ptr<ValidityPeriodBase>(new AbsoluteValidityPeriod(*this));
        }

        virtual bool equal(const ValidityPeriodBase& vp) const
        {
            const AbsoluteValidityPeriod* p = dynamic_cast<const AbsoluteValidityPeriod*>(&vp);
            return p &&
                *this == *p;
        }

        TimeStamp absoluteValidityPeriod() const { return timeStamp_; }
        void absoluteValidityPeriod(const TimeStamp& value) { timeStamp_ = value; }

    private:
        TimeStamp timeStamp_;
    };

    typedef typename Loki::TL::MakeTypelist
    <
        NoValidityPeriod,
        RelativeValidityPeriod,
        AbsoluteValidityPeriod
    >::Result _VPTypelist;

    MessageUnit()
        : validityPeriod_(new NoValidityPeriod())
    {}

    explicit MessageUnit(std::istream& st)
        : validityPeriod_(new NoValidityPeriod())
    {
        from_stream(st);
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
        , validityPeriod_(right.validityPeriod_->clone())
    {}

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            validityPeriod_ = right.validityPeriod_->clone();
        }

        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
          return Base::operator==(right) &&
            validityPeriod_->equal(*right.validityPeriod_);
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);
        createValidityPeriod(st, _VPTypelist());
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);
        validityPeriod_->to_stream(st);
    }

    /** Returns true if the stored Validity Period has specified type; false if it does not. */
    template<class _ValidityPeriod>
    bool has_validityPeriod() const
    {
        return (NULL != dynamic_cast<_ValidityPeriod*>(validityPeriod_.get()));
    }

    /** Returns a reference to the stored Validity Period of specified type.
        Throws MessageUnitBase::failure exception if the stored Validity Period has a different type than specified. 
     */
    template<class _ValidityPeriod>
    const _ValidityPeriod& validityPeriod() const
    {
        _ValidityPeriod* p = dynamic_cast<_ValidityPeriod*>(validityPeriod_.get());
        if (!p)
            throw typename Base::failure("TPValidityPeriod: bad cast");
        return *p;
    }

    /** Sets the Validity Period of specified type. */
    template<class _ValidityPeriod>
    void validityPeriod(const _ValidityPeriod& value)
    {
        Base::validityPeriodFormat(_ValidityPeriod::validityPeriodFormat);
        validityPeriod_.reset(new _ValidityPeriod(value));
    }

private:
    template<class Head, class Tail>
    void createValidityPeriod(std::istream& st, Loki::Typelist<Head, Tail>)
    {
        if (Head::validityPeriodFormat == Base::validityPeriodFormat())
            validityPeriod_.reset(new Head(st));
        else
            createValidityPeriod(st, Tail());
    }

    void createValidityPeriod(std::istream& /*st*/, Loki::NullType)
    {
        throw typename Base::failure("TPValidityPeriod: not supported Validity Period");
    }

    std::auto_ptr<ValidityPeriodBase> validityPeriod_;
};


/** \brief TP-User-Data-Length, TP-User-Data fields, see 9.2.3.16, 9.2.3.24 of GSM 03.40. */
template <class Base>
class MessageUnit<TPUserData, Base> : public Base
{
public:
    /** \brief Information Element identifiers, see 9.2.3.24 of GSM 03.40. */
    enum InformationElementIdentifier
    {
        Concatenated8bitReferenceShortMessage = 0x00,
        Concatenated16bitReferenceShortMessage = 0x08
    };

    /** \brief Each subclass of this abstract class represents corresponding Info Element, see 9.2.3.24 of GSM 03.40. */
    class InfoElementBase
    {
    public:
        virtual ~InfoElementBase() {}
        virtual unsigned int octetLength() const = 0;
        virtual Loki::SmartPtr<InfoElementBase> clone() const = 0;
        virtual bool equal(const InfoElementBase& right) const = 0;
        virtual void from_stream(std::istream& st, unsigned int& userDataHeaderOctetLength) = 0;
        virtual void to_stream(std::ostream& st) const = 0;
    };

    /** \brief Concatenated short messages, 8-bit reference number info element, see 9.2.3.24.1 of GSM 03.40 */
    class ConcatenatedMessage8bitInfoElement : public InfoElementBase
    {
    public:
        enum {
            infoElementID = Concatenated8bitReferenceShortMessage,
        };

        ConcatenatedMessage8bitInfoElement()
        {
        	init();
        }

        ConcatenatedMessage8bitInfoElement(unsigned char referenceNumber, unsigned char maxNumber, unsigned char sequenceNumber)
        {
        	init(referenceNumber, maxNumber, sequenceNumber);
        }

        ConcatenatedMessage8bitInfoElement(std::istream& st, unsigned int& userDataHeaderOctetLength)
        {
            from_stream(st, userDataHeaderOctetLength);
        }

        ConcatenatedMessage8bitInfoElement(const ConcatenatedMessage8bitInfoElement& right)
        {
        	init(right.referenceNumber_, right.maxNumber_, right.sequenceNumber_);
        }

        ConcatenatedMessage8bitInfoElement& operator=(const ConcatenatedMessage8bitInfoElement& right)
        {
        	init(right.referenceNumber_, right.maxNumber_, right.sequenceNumber_);
            return *this;
        }

        bool operator==(const ConcatenatedMessage8bitInfoElement& right) const
        {
            return referenceNumber_ == right.referenceNumber_
                && maxNumber_ == right.maxNumber_
                && sequenceNumber_ == right.sequenceNumber_;
        }

        virtual void from_stream(std::istream& st, unsigned int& userDataHeaderOctetLength)
        {
            unsigned char infoElementOctetLength = (unsigned char)st.get();
            if (!st)
                throw typename Base::failure("ConcatenatedMessage8bitInfoElement: input stream read failure");
            --userDataHeaderOctetLength;

            if (3 != infoElementOctetLength)
                throw typename Base::failure("ConcatenatedMessage8bitInfoElement: invalid Information Element length");

            unsigned char ch[3];
            for (int i = 0; i < 3; ++i)
            {
                ch[i] = (unsigned char)st.get();
                if (!st)
                    throw typename Base::failure("ConcatenatedMessage8bitInfoElement: input stream read failure");
                --userDataHeaderOctetLength;
            }

            referenceNumber_ = ch[0];
            maxNumber_ = ch[1];
            sequenceNumber_ = ch[2];
        }

        virtual void to_stream(std::ostream& st) const
        {
            st << (unsigned char) infoElementID;
            st << (unsigned char) 3;
            st << referenceNumber_;
            st << maxNumber_;
            st << sequenceNumber_;

            if (!st)
                throw typename Base::failure("ConcatenatedMessage8bitInfoElement: output stream write failure");
        }

        virtual unsigned int octetLength() const
        {
            return 5;
        }

        virtual Loki::SmartPtr<InfoElementBase> clone() const
        {
            return Loki::SmartPtr<InfoElementBase>(new ConcatenatedMessage8bitInfoElement(*this));
        }

        virtual bool equal(const InfoElementBase& right) const
        {
            const ConcatenatedMessage8bitInfoElement* p = dynamic_cast<const ConcatenatedMessage8bitInfoElement*>(&right);
            return p
                && *this == *p;
        }

        void referenceNumber(unsigned char value) const { referenceNumber_ = value; }
        unsigned char referenceNumber() { return referenceNumber_; }

        void maxNumber(unsigned char value) const { maxNumber_ = value; }
        unsigned char maxNumber() { return maxNumber_; }

        void sequenceNumber(unsigned char value) const { sequenceNumber_ = value; }
        unsigned char sequenceNumber() { return sequenceNumber_; }

    private:
    	void init(unsigned char referenceNumber = 0, unsigned char maxNumber = 0, unsigned char sequenceNumber = 0)
    	{
			referenceNumber_= referenceNumber;
			maxNumber_ = maxNumber;
			sequenceNumber_ = sequenceNumber;
    	}

        unsigned char referenceNumber_;
        unsigned char maxNumber_;
        unsigned char sequenceNumber_;
    };

    /** \brief Concatenated short messages, 16-bit reference number info element, see 9.2.3.24.8 of GSM 03.40 */
    class ConcatenatedMessage16bitInfoElement : public InfoElementBase
    {
     public:
        enum {
            infoElementID = Concatenated16bitReferenceShortMessage
        };

        ConcatenatedMessage16bitInfoElement()
        {
        	init();
        }

        ConcatenatedMessage16bitInfoElement(unsigned short referenceNumber, unsigned char maxNumber, unsigned char sequenceNumber)
        {
        	init(referenceNumber, maxNumber, sequenceNumber);
        }

        explicit ConcatenatedMessage16bitInfoElement(std::istream& st, unsigned int& userDataHeaderOctetLength)
        {
            from_stream(st, userDataHeaderOctetLength);
        }

        ConcatenatedMessage16bitInfoElement(const ConcatenatedMessage16bitInfoElement& right)
        {
        	init(right.referenceNumber_, right.maxNumber_, right.sequenceNumber_);
        }

        ConcatenatedMessage16bitInfoElement& operator=(const ConcatenatedMessage16bitInfoElement& right)
        {
        	init(right.referenceNumber, right.maxNumber, right.sequenceNumber);
            return *this;
        }

        bool operator==(const ConcatenatedMessage16bitInfoElement& right) const
        {
            return referenceNumber_ == right.referenceNumber_ &&
                maxNumber_ == right.maxNumber_ &&
                sequenceNumber_ == right.sequenceNumber_;
        }

        virtual void from_stream(std::istream& st, unsigned int& userDataHeaderOctetLength)
        {
            unsigned char infoElementOctetLength = (unsigned char)st.get();
            if (!st)
                throw typename Base::failure("ConcatenatedMessage16bitInfoElement: input stream read failure");
            userDataHeaderOctetLength -= 1;

            if (4 != infoElementOctetLength)
                throw typename Base::failure("ConcatenatedMessage16bitInfoElement: invalid Information Element length");

            unsigned char ch[4];
            for (int i = 0; i < 4; ++i)
            {
                ch[i] = (unsigned char)st.get();
                if (!st)
                    throw typename Base::failure("ConcatenatedMessage16bitInfoElement: input stream read failure");
                --userDataHeaderOctetLength;
            }

            referenceNumber_ = (unsigned short)ch[0] & ((unsigned short)ch[1] << 8);
            maxNumber_ = ch[2];
            sequenceNumber_ = ch[3];
        }

        virtual void to_stream(std::ostream& st) const
        {
            st << (unsigned char) infoElementID;
            st << (unsigned char) 4;
            st << (unsigned char)(referenceNumber_ >> 8);
            st << (unsigned char)(referenceNumber_ & 0xFF);
            st << maxNumber_;
            st << sequenceNumber_;

            if (!st)
                throw typename Base::failure("ConcatenatedMessage16bitInfoElement: output stream write failure");
        }

        virtual unsigned int octetLength() const
        {
            return 6;
        }

        virtual Loki::SmartPtr<InfoElementBase> clone() const
        {
            return Loki::SmartPtr<InfoElementBase>(new ConcatenatedMessage16bitInfoElement(*this));
        }

        virtual bool equal(const InfoElementBase& right) const
        {
            const ConcatenatedMessage16bitInfoElement* p = dynamic_cast<const ConcatenatedMessage16bitInfoElement*>(&right);
            return p
                && *this == *p;
        }

        void referenceNumber(unsigned short value) const { referenceNumber_ = value; }
        unsigned short referenceNumber() { return referenceNumber_; }

        void maxNumber(unsigned char value) const { maxNumber_ = value; }
        unsigned char maxNumber() { return maxNumber_; }

        void sequenceNumber(unsigned char value) const { sequenceNumber_ = value; }
        unsigned char sequenceNumber() { return sequenceNumber_; }

    private:
    	void init(unsigned short referenceNumber = 0, unsigned char maxNumber = 0, unsigned char sequenceNumber = 0)
    	{
			referenceNumber_ = referenceNumber;
			maxNumber_ = maxNumber;
			sequenceNumber_ = sequenceNumber;
    	}

        unsigned short referenceNumber_;
        unsigned char maxNumber_;
        unsigned char sequenceNumber_;
    };

    /** \brief Each subclass of this abstract class corresponds to the appropriate MessageUnit<TPDataCodingScheme, Base>::Alphabet. */
    class UserDataBase
    {
    public:
        virtual ~UserDataBase() {}
        virtual unsigned int octetLength() const = 0;
        virtual std::auto_ptr<UserDataBase> clone() const = 0;
        virtual bool equal(const UserDataBase& ud) const = 0;
        virtual void from_stream(std::istream& st, unsigned int& userDataOctetLenght, unsigned int userDataHeaderOctetLength) = 0;
        virtual void to_stream(std::ostream& st, unsigned int userDataHeaderOctetLength) const = 0;
        virtual typename Base::Alphabet alphabet() const = 0;
    };

    /** \brief Corresponds to the MessageUnit<TPDataCodingScheme, Base>::DefaultAlphabet. */
    class DefaultUserData : public UserDataBase
    {
    public:
        typedef std::basic_string<typename Base::char_type, typename Base::traits_type> _MyString;

        enum {
            enum_alphabet = Base::DefaultAlphabet
        };

        DefaultUserData()
        {}

        explicit DefaultUserData(const _MyString& data)
            : userData_(data)
        {}

        DefaultUserData(std::istream& st, unsigned int& userDataOctetLenght, unsigned int userDataHeaderOctetLength)
        {
            from_stream(st, userDataOctetLenght, userDataHeaderOctetLength);
        }

        DefaultUserData(const DefaultUserData& right)
            : userData_(right.userData_)
        {}

        DefaultUserData& operator=(const DefaultUserData& right)
        {
            userData_ = right.userData_;
            return *this;
        }

        bool operator==(const DefaultUserData& right) const
        {
            return userData_ == right.userData_;
        }

        virtual void from_stream(std::istream& st, unsigned int& userDataOctetLenght, unsigned int userDataHeaderOctetLength)
        {
            std::basic_ostringstream<typename Base::char_type, typename Base::traits_type> ss;
            unsigned int n = 0;
            unsigned char cx = 0;

            if (userDataOctetLenght
                && userDataHeaderOctetLength
                && (userDataHeaderOctetLength + 1) % 7)
            {
                unsigned char x = (unsigned char)st.get();
                if (!st)
                    throw typename Base::failure("Default User Data: input stream read failure");
                --userDataOctetLenght;

                n = (userDataHeaderOctetLength + 1) % 7 + 1;
                cx = x >> (8 - n);
                if (n == 7)
                {
                    ss << ss.widen(cx);
                    n = 0;
                    cx = 0;
                }
            }

            while (userDataOctetLenght)
            {
                unsigned char x = (unsigned char)st.get();
                if (!st)
                    throw typename Base::failure("Default User Data: input stream read failure");
                --userDataOctetLenght;

                unsigned char ch = x;
                ch <<= n + 1;
                ch >>= 1;
                ch |= cx;
                ss << ss.widen(ch);

                ++n;
                cx = x >> (8 - n);

                if (n == 7)
                {
                    ss << ss.widen(cx);
                    n = 0;
                    cx = 0;
                }
            }
            userData_ = ss.str();
        }

        virtual void to_stream(std::ostream& st, unsigned int userDataHeaderOctetLength) const
        {
            typedef std::ctype<typename Base::char_type> _Ctype;
            const _Ctype &fac = std::use_facet<_Ctype>(std::locale::classic());

            unsigned char cx = 0;
            int n = userDataHeaderOctetLength ? ((userDataHeaderOctetLength + 2) % 8) : 0;

            for (size_t i = 0; i < userData_.length(); ++i)
            {
                unsigned char x = fac.narrow(userData_[i], '\0');
                if (n)
                {
                    unsigned char ch = x << (8 - n);
                    ch |= cx;
                    if (!(st << ch))
                        throw typename Base::failure("Default User Data: output stream write failure");
                }

                cx = x << 1;
                cx >>= (n + 1);

                if (++n > 7)
                    n = 0;
            }
            if (n /*Added by Aram to avoid from uncorrect '@' symbol*/ /*&& cx != '\0'*/)
            {
                if (!(st << cx))
                    throw typename Base::failure("Default User Data: output stream write failure");
            }
        }

        virtual unsigned int octetLength() const
        {
            return userData_.length() * 7 / 8
                + ((userData_.length() % 8) ? 1 : 0);
        }

        virtual std::auto_ptr<UserDataBase> clone() const
        {
            return std::auto_ptr<UserDataBase>(new DefaultUserData(*this));
        }

        virtual bool equal(const UserDataBase& ud) const
        {
            const DefaultUserData* p = dynamic_cast<const DefaultUserData*>(&ud);
            return p
                && *this == *p;
        }

        virtual typename Base::Alphabet alphabet() const
        {
            return Base::DefaultAlphabet;
        }

        const _MyString& userData() const { return userData_; }
        void userData(const _MyString& value) { userData_ = value; }


    private:
         _MyString userData_;
    };

    /** \brief Corresponds to the MessageUnit<TPDataCodingScheme, Base>::UCS2Alphabet. */
    class UCS2UserData : public UserDataBase
    {
    public:
        //typedef std::basic_string<typename Base::char_type, typename Base::traits_type> _MyString;
        typedef std::basic_string<wchar_t, std::char_traits<wchar_t> > _MyString;

        enum {
            enum_alphabet = Base::UCS2Alphabet
        };

        UCS2UserData()
        {}

        UCS2UserData(const _MyString& data)
            : userData_(data)
        {}

        UCS2UserData(std::istream& st, unsigned int& userDataOctetLenght, unsigned int userDataHeaderOctetLength)
        {
            from_stream(st, userDataOctetLenght, userDataHeaderOctetLength);
        }

        UCS2UserData(const UCS2UserData& right)
            : userData_(right.userData_)
        {}

        UCS2UserData& operator=(const UCS2UserData& right)
        {
            userData_ = right.userData_;
            return *this;
        }

        bool operator==(const UCS2UserData& right) const
        {
            return userData_ == right.userData_;
        }

        virtual void from_stream(std::istream& st, unsigned int& userDataOctetLenght, unsigned int /*userDataHeaderOctetLength*/)
        {
            //std::basic_ostringstream<typename Base::char_type, typename Base::traits_type> ss;
            std::basic_ostringstream<wchar_t, std::char_traits<wchar_t> > ss;
            ss.imbue(st.getloc());

            char hi = 0;
            bool isHi = true;

            while (userDataOctetLenght)
            {
                char ch = (char)st.get();
                if (!st)
                    throw typename Base::failure("UCS2 User Data: input stream read failure");
                --userDataOctetLenght;

                if (isHi)
                {
                    hi = ch;
                }
                else
                {
                    wchar_t wc = ((wchar_t)hi << 8) | (wchar_t)ch;
                    wide_char_to_stream(ss, wc);
                }

                isHi = !isHi;
            }

            if (!isHi)
                throw typename Base::failure("UCS2 User Data: bad user data length");

            userData_ = ss.str();
        }

        virtual void to_stream(std::ostream& st, unsigned int /*userDataHeaderOctetLength*/) const
        {
            //std::basic_istringstream<typename Base::char_type, typename Base::traits_type> ss(userData_);
            std::basic_istringstream<wchar_t, std::char_traits<wchar_t> > ss(userData_);
            ss.imbue(st.getloc());

            wchar_t wc = wide_char_from_stream(ss);
            while (ss)
            {
                st << (char)(wc >> 8);
                st << (char)(wc & 0xFF);
                if (!st)
                    throw typename Base::failure("UCS2 User Data: output stream write failure");

                wc = wide_char_from_stream(ss);
            }
        }

        virtual unsigned int octetLength() const
        {
            return userData_.length() * 2;
        }

        virtual std::auto_ptr<UserDataBase> clone() const
        {
            return std::auto_ptr<UserDataBase>(new UCS2UserData(*this));
        }

        virtual bool equal(const UserDataBase& ud) const
        {
            const UCS2UserData* p = dynamic_cast<const UCS2UserData*>(&ud);
            return p
                && *this == *p;
        }

        virtual typename Base::Alphabet alphabet() const
        {
            return Base::UCS2Alphabet;
        }

        const _MyString& userData() const { return userData_; }
        void userData(const _MyString& value) { userData_ = value; }

    private:
        //template<class _Stream>
        //void wide_char_to_stream(_Stream& st, wchar_t wc) const
        //{
        //    typedef std::ctype<wchar_t> _Fac;
        //    const _Fac& fac = std::use_facet<_Fac>(st.getloc());
        //    st << st.widen(fac.narrow(wc, '\0'));
        //}

        void wide_char_to_stream(std::basic_ostream<wchar_t>& st, wchar_t wc) const
        {
            st << wc;
        }

        //template<class StreamT>
        //wchar_t wide_char_from_stream(StreamT& st) const
        //{
        //    typedef typename StreamT::traits_type Traits;
        //    typedef std::ctype<wchar_t> _Fac;
        //    const _Fac& fac = std::use_facet<_Fac>(st.getloc());
        //    return fac.widen(st.narrow(Traits::to_char_type(st.get()), '\0'));
        //}

        wchar_t wide_char_from_stream(std::basic_istream<wchar_t>& st) const
        {
            return (wchar_t)st.get();
        }

        _MyString userData_;
    };

    /** \brief Corresponds to the MessageUnit<TPDataCodingScheme, Base>::EightBitDataAlphabet. */
    class EightBitUserData : public UserDataBase
    {
    public:
        typedef std::vector<unsigned char> _BinaryData;

        enum {
            enum_alphabet = Base::EightBitDataAlphabet
        };

        EightBitUserData()
        {}

        EightBitUserData(const std::vector<unsigned char>& data)
            : userData_(data)
        {}

        EightBitUserData(std::istream& st, unsigned int& userDataOctetLenght,
            unsigned int userDataHeaderOctetLength)
        {
            from_stream(st, userDataOctetLenght, userDataHeaderOctetLength);
        }

        EightBitUserData(const EightBitUserData& right)
            : userData_(right.userData_)
        {}

        EightBitUserData& operator=(const EightBitUserData& right)
        {
            userData_ = right.userData_;
            return *this;
        }

        bool operator==(const EightBitUserData& right) const
        {
            return (userData_ == right.userData_);
        }

        virtual void from_stream(std::istream& st, unsigned int& userDataOctetLenght, unsigned int /*userDataHeaderOctetLength*/)
        {
            if (!userDataOctetLenght)
                userData_.clear();
            else
            {
                _BinaryData data(userDataOctetLenght);

                st.read((char*)&data[0], userDataOctetLenght);
                if (!st)
                    throw typename Base::failure("Eight Bit User Data: input stream read failure");
                userData_.swap(data);
            }

            userDataOctetLenght = 0;
        }

        virtual void to_stream(std::ostream& st, unsigned int /*userDataHeaderOctetLength*/) const
        {
            if (userData_.size())
            {
                st.write((char*)&userData_[0], userData_.size());
                if (!st)
                    throw typename Base::failure("Eight Bit User Data: output stream write failure");
            }
        }

        virtual unsigned int octetLength() const
        {
            return userData_.size();
        }

        virtual std::auto_ptr<UserDataBase> clone() const
        {
            return std::auto_ptr<UserDataBase>(new EightBitUserData(*this));
        }

        virtual bool equal(const UserDataBase& ud) const
        {
            const EightBitUserData* p = dynamic_cast<const EightBitUserData*>(&ud);
            return p
                && *this == *p;
        }

        virtual typename Base::Alphabet alphabet() const
        {
            return Base::EightBitDataAlphabet;
        }

        const _BinaryData& userData() const { return userData_; }
        void userData(const _BinaryData& value) { userData_ = value; }

    private:
        _BinaryData userData_;
    };

    typedef std::map<int, Loki::SmartPtr<InfoElementBase> > _InfoElementsMap;

    typedef typename Loki::TL::MakeTypelist
    <
        ConcatenatedMessage8bitInfoElement,
        ConcatenatedMessage16bitInfoElement
    >::Result _InfoElementTypelist;

    typedef typename Loki::TL::MakeTypelist
    <
        DefaultUserData,
        UCS2UserData,
        EightBitUserData
    >::Result _UserDataTypelist;

    MessageUnit()
    {
        init();
    }

    MessageUnit(const MessageUnit& right)
        : Base(right)
    {
        init(right);
    }

    explicit MessageUnit(std::istream& st)
    {
        from_stream(st);
    }

    MessageUnit& operator=(const MessageUnit& right)
    {
        if (this != &right)
        {
            Base::operator=(right);
            init(right);
        }
        return *this;
    }

    bool operator==(const MessageUnit& right) const
    {
        return Base::operator==(right)
            && userData_->equal(*right.userData_)
            && infoElements_.size() == right.infoElements_.size()
            && std::equal(infoElements_.begin(),
                infoElements_.end(),
                right.infoElements_.begin(),
                InfoElementsEqualCompare());
    }

    void from_stream(std::istream& st)
    {
        Base::from_stream(st);

        if (!Base::userDataPresent())
            return;

        unsigned char ch = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPUserData: input stream read failure");

        unsigned int userDataOctetLength = (unsigned int) ch;
        if (Base::DefaultAlphabet == Base::alphabet())
            userDataOctetLength = userDataOctetLength * 7 / 8 + ((userDataOctetLength % 8) ? 1 : 0);

        if (140 < userDataOctetLength)
            throw typename Base::failure("TPUserData: User Data length exceeds 140 octets");

        readUserDataHeader(st, userDataOctetLength);
        readUserData(st, userDataOctetLength, _UserDataTypelist());
    }

    void to_stream(std::ostream& st) const
    {
        Base::to_stream(st);

        if (!Base::userDataPresent())
            return;

        unsigned int userDataLength = calcUserDataOctetLength();
        if (140 < userDataLength)
            throw typename Base::failure("TPUserData: User Data length exceeds 140 octets");

        if (Base::DefaultAlphabet == Base::alphabet())
            userDataLength = userDataLength * 8 / 7;

        if (!(st << (unsigned char)userDataLength))
            throw typename Base::failure("TPUserData: output stream write failure");

        writeUserDataHeader(st);
        writeUserData(st);
    }

    /** Returns true if the User Data Header contains an Info Element of specified type; false if it does not. */
    template <class _InfoElement>
    bool has_infoElement() const
    {
        return infoElements_.end() != infoElements_.find(_InfoElement::infoElementID);
    }

    /** Returns a reference to an Info Element of specified type stored in the User Data Header.
        Throws MessageUnitBase::failure exception if the User Data Header contains no such an Info Element.
     */
    template <class _InfoElement>
    _InfoElement& infoElement() const
    {
        typename _InfoElementsMap::iterator it = infoElements_.find(_InfoElement::infoElementID);
        if (infoElements_.end() == it)
            throw typename Base::failure("TPUserData: Information Element not found");
        return *it;
    }

    /** Sets an Info Element of specified type to the User Data Header. */
    template <class _InfoElement>
    void infoElement(const _InfoElement& value)
    {
        if (infoElements_.end() == infoElements_.find((int)_InfoElement::infoElementID)
            && 140 < 1 + value.octetLength() + calcUserDataHeaderOctetLength() + userData_->octetLength())
        {
            throw typename Base::failure("TPUserData: User Data length exceeds 140 octets");
        }

        infoElements_.insert(std::make_pair((int)_InfoElement::infoElementID, new _InfoElement(value)));
        Base::userDataHeaderIndicator(0 != infoElements_.size());
    }

    /** Removes an Info Element of specified type from the User Data Header. */
    template <class _InfoElement>
    void removeInfoElement()
    {
        infoElements_.erase(_InfoElement::infoElementID);
        Base::userDataHeaderIndicator(0 != infoElements_.size());
    }

    /** Simply calls MessageUnit<TPDataCodingScheme, Base>::dataCodingScheme() method. */
    template<class _DataCodingScheme>
    const _DataCodingScheme& dataCodingScheme() const
    {
        return Base::template dataCodingScheme<_DataCodingScheme>();
    }

    /** Calls MessageUnit<TPDataCodingScheme, Base>::dataCodingScheme(const _DataCodingScheme& value) method and resets the stored User Data if its alphabet is different from one used in specified Data Coding Scheme. */
    template<class _DataCodingScheme>
    void dataCodingScheme(const _DataCodingScheme& value)
    {
        Base::template dataCodingScheme(value);
        if (Base::alphabet() != userData_->alphabet())
            createUserData(_UserDataTypelist());
    }

    /** Returns a reference to the stored User Data of specified type.
        Throws MessageUnitBase::failure exception if the stored User Data has a different type than specified.
     */
    template<class _UserData>
    const _UserData& userData() const
    {
        const _UserData* p = dynamic_cast<_UserData*>(userData_.get());
        if (!p)
            throw typename Base::failure("TPUserData: bad cast exception");
        return *p;
    }

    /** Sets a User Data of specified type.\n
        Throws MessageUnitBase::failure exception if the User Data has a different alphabet than already selected Data Coding Scheme.\n
        Throws MessageUnitBase::failure exception if total User Data length including User Data Header exceeds 140 octets.
     */
    template<class _UserData>
    void userData(const _UserData& value)
    {
        if (_UserData::enum_alphabet != (int)Base::alphabet())
            throw typename Base::failure("TPUserData: User Data has different alphabet than already selected Data Coding Scheme");

        if (140 < (0 < calcUserDataHeaderOctetLength() ? 1 : 0) + calcUserDataHeaderOctetLength() + value.octetLength())
            throw typename Base::failure("TPUserData: User Data length exceeds 140 octets");

        userData_.reset(new _UserData(value));
    }

    /** Calculates total length of User Data Header. */
    unsigned int calcUserDataHeaderOctetLength() const
    {
        unsigned int octetLength = 0;
        typename _InfoElementsMap::const_iterator it = infoElements_.begin();
        for (; it != infoElements_.end(); ++it)
        {
            octetLength += (*it).second->octetLength();
        }
        return octetLength;
    }

    /** Calculates total User Data length including User Data Header. */
    unsigned int calcUserDataOctetLength() const
    {
        return (0 < calcUserDataHeaderOctetLength() ? 1 : 0)
            + calcUserDataHeaderOctetLength()
            + userData_->octetLength();
    }

private:
    class InfoElementsEqualCompare
    {
    public:
        typedef std::pair<unsigned int, Loki::SmartPtr<InfoElementBase> > _Elem;
        bool operator() (const _Elem& left, const _Elem& right)
        {
            return left.first == right.first
                && left.second->equal(*right.second);
        }
    };

    void init()
    {
        infoElements_.clear();
        createUserData(_UserDataTypelist());
    }

    void init(const MessageUnit& right)
    {
        infoElements_.clear();
        typename _InfoElementsMap::const_iterator it = right.infoElements_.begin();
        for (; it != right.infoElements_.end(); ++it)
        {
            infoElements_.insert(std::make_pair(it->first, it->second->clone()));
        }

        userData_ = right.userData_->clone();
    }

    void readUserDataHeader(std::istream& st, unsigned int& userDataOctetLength)
    {
        if (!userDataOctetLength || !Base::userDataHeaderIndicator())
            return;

        unsigned char ch = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPUserData: input stream read failure");
        --userDataOctetLength;

        unsigned int userDataHeaderOctetLength = (unsigned int) ch;

        if (userDataHeaderOctetLength > userDataOctetLength)
            throw typename Base::failure("TPUserData: bad User Data Header length");
        userDataOctetLength -= userDataHeaderOctetLength;

        while (userDataHeaderOctetLength)
        {
            readInfoElement(st, userDataHeaderOctetLength);
        }
    }

    void readInfoElement(std::istream& st, unsigned int& userDataHeaderLength)
    {
        unsigned char infoElementID = (unsigned char)st.get();
        if (!st)
            throw typename Base::failure("TPUserData: input stream read failure");
        --userDataHeaderLength;

        createInfoElement(st, infoElementID, userDataHeaderLength, _InfoElementTypelist());
    }

    template <class Head, class Tail>
    void createInfoElement(std::istream& st, unsigned int infoElementID, unsigned int& userDataHeaderLength, Loki::Typelist<Head, Tail>)
    {
        if (Head::infoElementID == infoElementID)
        {
            infoElements_.insert(std::make_pair(infoElementID, new Head(st, userDataHeaderLength)));
            return;
        }
        else
            return createInfoElement(st, infoElementID, userDataHeaderLength, Tail());
    }

    void createInfoElement(std::istream& /*st*/, unsigned int /*infoElementID*/, unsigned int& /*userDataHeaderLength*/, Loki::NullType)
    {
        throw typename Base::failure("TPUserData: unsupported User Data Header Information Element");
    }

    template <class Head, class Tail>
    void createUserData(Loki::Typelist<Head, Tail>)
    {
        if (Head::enum_alphabet == (int)Base::alphabet())
            userData_.reset(new Head());
        else
            createUserData(Tail());
    }

    void createUserData(Loki::NullType)
    {
        throw typename Base::failure("TPUserData: Unsupported alphabet");
    }

    template <class Head, class Tail>
    void readUserData(std::istream& st, unsigned int& userDataOctetLenght, Loki::Typelist<Head, Tail>)
    {
        if (Head::enum_alphabet == (int)Base::alphabet())
        {
            userData_.reset(new Head(st, userDataOctetLenght, calcUserDataHeaderOctetLength()));
        }
        else
            readUserData(st, userDataOctetLenght, Tail());
    }

    void readUserData(std::istream& /*st*/, unsigned int& /*userDataOctetLenght*/, Loki::NullType)
    {
        throw typename Base::failure("TPUserData: Unsupported alphabet");
    }

    void writeUserDataHeader(std::ostream& st) const
    {
        if (0 == calcUserDataHeaderOctetLength())
            return;

        st << (unsigned char)calcUserDataHeaderOctetLength();
        if (!st)
            throw typename Base::failure("TPUserData: input stream read failure");

        typename _InfoElementsMap::const_iterator it = infoElements_.begin();
        for (; it != infoElements_.end(); ++it)
        {
            (*it).second->to_stream(st);
        }
    }

    void writeUserData(std::ostream& st) const
    {
        userData_->to_stream(st, calcUserDataHeaderOctetLength());
    }

private:
    _InfoElementsMap infoElements_;
    std::auto_ptr<UserDataBase> userData_;
};

} // namespace PduLib


#endif // _PDULIB_MESSAGE_UNIT_
