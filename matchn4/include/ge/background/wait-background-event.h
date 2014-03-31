#pragma once

#include "../../Event.h"


namespace matchn {


/**
* Неподвижный фон.
*/
class WaitBackground :
    public WaitEvent
{
public:
    WaitBackground(
        const sourceEvent_t&,
        std::shared_ptr< Incarnate >,
        const protocolDataEvent_t&
    );

    virtual void operator()();

    ~WaitBackground();

    static inline groupEvent_t staticName() { return WaitEvent::staticGroup() + "-background"; }

    virtual inline nameEvent_t name() const { return staticName(); }
};


} // matchn
