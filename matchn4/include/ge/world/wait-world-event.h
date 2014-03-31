#pragma once

#include "../../Event.h"


namespace matchn {


/**
* Здесь мир обрабатывает события.
*/
class WaitWorld :
    public WaitEvent
{
public:
    WaitWorld(
        const sourceEvent_t&,
        std::shared_ptr< Incarnate >,
        const protocolDataEvent_t&
    );

    virtual void operator()();

    ~WaitWorld();

    static inline groupEvent_t staticName() { return WaitEvent::staticGroup() + "-world"; }

    virtual inline nameEvent_t name() const { return staticName(); }
};


} // matchn
