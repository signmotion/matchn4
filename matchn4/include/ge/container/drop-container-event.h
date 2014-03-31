#pragma once

#include "../../Event.h"


namespace matchn {


/**
* Контейнер падает.
*
* @todo fine? Выделить класс Action[Element] + множественное наследование.
*/
class DropContainer :
    public DropEvent
{
public:
    DropContainer(
        const sourceEvent_t&,
        std::shared_ptr< Incarnate >,
        const protocolDataEvent_t&
    );

    virtual void operator()();

    ~DropContainer();

    static inline groupEvent_t staticName() { return DropEvent::staticGroup() + "-container"; }

    virtual inline nameEvent_t name() const { return staticName(); }
};


} // matchn
