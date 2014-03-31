#pragma once

#include "EventMod.h"
#include "Incarnate.h"
#include <boost/assign.hpp>


namespace matchn {


/**
* Событие элемента.
*/
class Event {
public:
    /**
    * Конфигурация события.
    */
    typedef int  timeMS_t;
    struct DataEvent {
        /**
        * Время начала события, мс.
        */
        timeMS_t  start;

        /**
        * Длительность события, мс.
        * # Меньше 0, если событие не имеет длительности.
        */
        timeMS_t  duration;

        /**
        * Время завершения события, мс.
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
    * @return Группа и UID события. Используется для контроля уникальности
    *         при работе в потоках.
    */
    virtual groupEvent_t group() const = 0;

    virtual nameEvent_t name() const = 0;

    uidEvent_t uid() const;


    inline sourceEvent_t const& source() const {
        return mSource;
    }


    /**
    * @return true, если Incarnate и/или его World разрушен.
    *
    * # Введён из-за static ManagerEvent, который разрушается последним,
    *   вызывая задействованные в событиях деструкторы и аварийный выход.
    */
    inline bool die() const {
        return !element() || !world();
    }


    /**
    * @return Ссылку на элемент или nullptr, если элемент уничтожен.
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
    * @return Время отработки события вышло.
    */
    inline bool timeOver() const {
        const auto ct = ManagerEvent::time();
        return (mData.duration > 0) && (ct > mData.stop);
    }


    /**
    * @return Событие актуально на данный момент времени.
    *         Проверяется признак 'mActuality'.
    */
    inline bool actuality() const {
        return mActuality;
    }


    /**
    * Делает событие неактуальным.
    */
    inline void dropActuality() {
        mActuality = false;
    }


    /**
    * @return Сколько времени в мс осталось до завершения этого события.
    *         Отрицательное число или 0 - событие завершилось.
    */
    inline int deltatime() const {
        return (mData.stop - ManagerEvent::time());
    }


    /**
    * @return Сколько в диапазоне  [ 0.0 (началось); 1.0 (завершено) )
    *         осталось до завершения этого события.
    */
    inline float nepperDeltatime() const {
        ASSERT( (mData.duration != 0)
            && "Нарушено соглашение: Длительность события не может быть == 0." );
        const float u =
            1.0f -
            static_cast< float >( mData.stop - ManagerEvent::time() ) /
            static_cast< float >( mData.duration );
        return (u > 1.0f) ? 1.0f : (u < 0.0f) ? 0.0f : u;
    }



    /**
    * # Для работы STL-коллекций.
    */
    inline bool operator<( const Event& e ) const {
        return (uid() < e.uid());
    }



protected:
    /**
    * Загружает данные из mRawData в mData.
    */
    void reload();


protected:
    const sourceEvent_t  mSource;

    std::weak_ptr< Incarnate >  mElement;

    
    /**
    * Конфигурация события.
    */
    protocolDataEvent_t  mRawData;

    /**
    * Распознанная из mRawData и дополненная конфигурация.
    * Используется для снижения нагрузки на CPU.
    */
    DataEvent  mData;

    /**
    * Признак: событие актуально.
    *
    * #i Была попытка привязаться ко времени: драматично. Надёжней:
    *    # каждое событие заботится о своей актуальности.
    */
    bool mActuality;


    /**
    * Время (для удобства работы).
    */
    static const boost::posix_time::milliseconds  HALF_SECOND;
    static const boost::posix_time::milliseconds  SECOND;
    static const boost::posix_time::milliseconds  TWO_SECOND;
};




/**
* Событие "Элемент падает".
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
* Событие "Игрок обратил внимание на элемент".
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
* Событие "Элементы того же вида, на которые обратил внимание игрок".
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
* Событие "Элемент ждёт".
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
        // # Отсутствие ожидания == бесконечному ожиданию.
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
