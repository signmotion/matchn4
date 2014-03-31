#pragma once

#include "../../Event.h"


namespace matchn {


/**
* Контейнеры того же типа, на которые обратил внимание игрок.
*/
class SameContainer :
    public SameEvent
{
public:
    SameContainer(
        const sourceEvent_t&,
        std::shared_ptr< Incarnate >,
        const protocolDataEvent_t&
    );

    virtual void operator()();

    ~SameContainer();

    static inline groupEvent_t staticName() { return SameEvent::staticGroup() + "-container"; }

    virtual inline nameEvent_t name() const { return staticName(); }
};


} // matchn
