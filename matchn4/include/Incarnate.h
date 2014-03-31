#pragma once

#include "GE.h"
#include "LIncarnate.h"
#include "VIncarnate.h"
#include "ManagerEvent.h"


namespace matchn {


class Incarnate :
    public std::enable_shared_from_this< Incarnate >,
    public GE,
    public LIncarnate,
    public VIncarnate
{
public:
    /**
    * События, назначенные этому элементу.
    */
    typedef std::vector< std::shared_ptr< Event > >  incarnateEvent_t;




protected:
    Incarnate(
        std::shared_ptr< World >,
        const std::string&  name,
        const typelib::coord2Int_t&  logicCoord,
        int  visualOrder
    );




public:
    virtual ~Incarnate();




    /**
    * @return Имя воплощённого элемента.
    */
    inline std::string const& name() const {
        return mName;
    }




    inline ManagerEvent const* managerEvent() const {
        return mManagerEvent.get();
    }


    inline ManagerEvent* managerEvent() {
        return mManagerEvent.get();
    }




    /**
    * Создаёт и добавляет новое событие.
    */
    template< class A >
    inline void addEvent(
        const sourceEvent_t&  source,
        const protocolDataEvent_t&  data = protocolDataEvent_t()
    ) {
        // добавим *уникальное* событие в пул событий
        const ManagerEvent::uid_t ume =
            ManagerEvent::hash( A::staticName(), uid() );
        if ( !mManagerEvent->has( ume ) ) {
            auto& ie = this->shared_from_this();
            // # Событие само позаботится о целостности цепочки событий.
            // # Конструкторы события (см. родителей) выполнят нужные действия.
            std::shared_ptr< A >  e( new A( source, ie, data ) );
            mManagerEvent->push( ume, e );
        }
    }




    /**
    * Отменяет событие.
    */
    template< class A >
    inline void removeEvent() {
        const ManagerEvent::uid_t ume =
            ManagerEvent::hash( A::staticName(), uid() );
        mManagerEvent->remove( ume );
    }




    /**
    * @return Список актуальных событий, назначенных этому элементу.
    */
    incarnateEvent_t actualityEventElement() const;



    
private:
    /**
    * Имя воплощённого элемента.
    */
    const std::string  mName;

    static std::unique_ptr< ManagerEvent >  mManagerEvent;


    friend class World;
};


} // matchn








namespace std {

std::ostream& operator<<( std::ostream&,  const matchn::Incarnate& );

} // std
