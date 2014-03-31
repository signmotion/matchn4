#pragma once

#include "EventMod.h"
#include "Incarnate.h"
#include <boost/assign.hpp>


namespace matchn {


/**
* ������� ��������.
*/
class Event {
public:
    /**
    * ������������ �������.
    */
    typedef int  timeMS_t;
    struct DataEvent {
        /**
        * ����� ������ �������, ��.
        */
        timeMS_t  start;

        /**
        * ������������ �������, ��.
        * # ������ 0, ���� ������� �� ����� ������������.
        */
        timeMS_t  duration;

        /**
        * ����� ���������� �������, ��.
        */
        timeMS_t  stop;
    };



public:
    Event(
        const sourceEvent_t&,
        std::shared_ptr< Incarnate >,
        const protocolDataEvent_t&
    );

    virtual void operator()() = 0;

    virtual ~Event();
    

    /**
    * @return ������ � UID �������. ������������ ��� �������� ������������
    *         ��� ������ � �������.
    */
    virtual groupEvent_t group() const = 0;

    virtual nameEvent_t name() const = 0;

    uidEvent_t uid() const;


    inline sourceEvent_t const& source() const {
        return mSource;
    }


    /**
    * @return true, ���� Incarnate �/��� ��� World ��������.
    *
    * # ����� ��-�� static ManagerEvent, ������� ����������� ���������,
    *   ������� ��������������� � �������� ����������� � ��������� �����.
    */
    inline bool die() const {
        return !element() || !world();
    }


    /**
    * @return ������ �� ������� ��� nullptr, ���� ������� ���������.
    */
    inline const std::shared_ptr< Incarnate >  element() const {
        return mElement.lock();
    }

    inline std::shared_ptr< Incarnate >  element() {
        return mElement.lock();
    }


    template< typename T >
    inline T data( const std::string& name, const T& def = T() ) const {
        return mRawData.at< T >( name, def );
    };


    template< typename T >
    inline void setData( const std::string& name,  const T& value ) {
        mRawData[ name.c_str() ] = value;
        reload();
    };


    inline DataEvent const&  data() const {
        return mData;
    }


    inline const std::shared_ptr< World >  world() const {
        return element()->world();
    }

    inline std::shared_ptr< World >  world() {
        return element()->world();
    }


    /**
    * @return ����� ��������� ������� �����.
    */
    inline bool timeOver() const {
        const auto ct = ManagerEvent::time();
        return (mData.duration > 0) && (ct > mData.stop);
    }


    /**
    * @return ������� ��������� �� ������ ������ �������.
    *         ����������� ������� 'mActuality'.
    */
    inline bool actuality() const {
        return mActuality;
    }


    /**
    * ������ ������� ������������.
    */
    inline void dropActuality() {
        mActuality = false;
    }


    /**
    * @return ������� ������� � �� �������� �� ���������� ����� �������.
    *         ������������� ����� ��� 0 - ������� �����������.
    */
    inline int deltatime() const {
        return (mData.stop - ManagerEvent::time());
    }


    /**
    * @return ������� � ���������  [ 0.0 (��������); 1.0 (���������) )
    *         �������� �� ���������� ����� �������.
    */
    inline float nepperDeltatime() const {
        ASSERT( (mData.duration != 0)
            && "�������� ����������: ������������ ������� �� ����� ���� == 0." );
        const float u =
            1.0f -
            static_cast< float >( mData.stop - ManagerEvent::time() ) /
            static_cast< float >( mData.duration );
        return (u > 1.0f) ? 1.0f : (u < 0.0f) ? 0.0f : u;
    }



    /**
    * # ��� ������ STL-���������.
    */
    inline bool operator<( const Event& e ) const {
        return (uid() < e.uid());
    }



protected:
    /**
    * ��������� ������ �� mRawData � mData.
    */
    void reload();


protected:
    const sourceEvent_t  mSource;

    std::weak_ptr< Incarnate >  mElement;

    
    /**
    * ������������ �������.
    */
    protocolDataEvent_t  mRawData;

    /**
    * ������������ �� mRawData � ����������� ������������.
    * ������������ ��� �������� �������� �� CPU.
    */
    DataEvent  mData;

    /**
    * �������: ������� ���������.
    *
    * #i ���� ������� ����������� �� �������: ����������. �������:
    *    # ������ ������� ��������� � ����� ������������.
    */
    bool mActuality;


    /**
    * ����� (��� �������� ������).
    */
    static const boost::posix_time::milliseconds  HALF_SECOND;
    static const boost::posix_time::milliseconds  SECOND;
    static const boost::posix_time::milliseconds  TWO_SECOND;
};




/**
* ������� "������� ������".
*/
class DropEvent :
    public Event,
    private CancelEventMod
{
protected:
    inline DropEvent(
        const sourceEvent_t&          source,
        std::shared_ptr< Incarnate >  ie,
        const protocolDataEvent_t&    d
    ) :
        Event( source, ie, d ),
        CancelEventMod( ie,  boost::assign::list_of
            ( "on-attention" )
            ( "wait" )
        )
    {
    }


    virtual inline ~DropEvent() {
    }


public:
    static inline groupEvent_t staticGroup() { return "drop"; }

    virtual inline groupEvent_t group() const { return staticGroup(); }
};




/**
* ������� "����� ������� �������� �� �������".
*/
class OnAttentionEvent :
    public Event,
    private CancelEventMod
{
protected:
    inline OnAttentionEvent(
        const sourceEvent_t&          source,
        std::shared_ptr< Incarnate >  ie,
        const protocolDataEvent_t&    d
    ) :
        Event( source, ie, d ),
        CancelEventMod( ie,  boost::assign::list_of
            //( "on-attention" )
            ( "same" )
            ( "wait" )
        )
    {
    }

    virtual inline ~OnAttentionEvent() {
    }


public:
    static inline groupEvent_t staticGroup() { return "on-attention"; }

    virtual inline groupEvent_t group() const { return staticGroup(); }
};




/**
* ������� "�������� ���� �� ����, �� ������� ������� �������� �����".
*/
class SameEvent :
    public Event,
    private CancelEventMod
{
protected:
    inline SameEvent(
        const sourceEvent_t&          source,
        std::shared_ptr< Incarnate >  ie,
        const protocolDataEvent_t&    d
    ) :
        Event( source, ie, d ),
        CancelEventMod( ie,  boost::assign::list_of
            ( "on-attention" )
            ( "same" )
        )
    {
    }


    virtual inline ~SameEvent() {
    }


public:
    static inline groupEvent_t staticGroup() { return "same"; }

    virtual inline groupEvent_t group() const { return staticGroup(); }
};




/**
* ������� "������� ���".
*/
class WaitEvent :
    public Event,
    private CancelEventMod
{
protected:
    inline WaitEvent(
        const sourceEvent_t&          source,
        std::shared_ptr< Incarnate >  ie,
        const protocolDataEvent_t&    d
    ) :
        Event( source, ie, d ),
        CancelEventMod( ie,  boost::assign::list_of
            ( "drop" )
            ( "on-attention" )
            ( "same" )
        )
    {
        // # ���������� �������� == ������������ ��������.
        const auto duration = data< float >( "duration", -1 );
        setData( "duration", duration );
    }


    virtual inline ~WaitEvent() {
    }


public:
    static inline groupEvent_t staticGroup() { return "wait"; }

    virtual inline groupEvent_t group() const { return staticGroup(); }
};


} // matchn








namespace std {

std::ostream& operator<<( std::ostream&,  const matchn::Event& );

} // std
