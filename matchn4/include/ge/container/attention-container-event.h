#pragma once

#include "../../Event.h"


namespace matchn {


/**
* Игрок обратил внимание на контейнер.
*/
class OnAttentionContainer :
    public OnAttentionEvent
{
public:
    OnAttentionContainer(
        const sourceEvent_t&,
        std::shared_ptr< Incarnate >,
        const protocolDataEvent_t&
    );

    virtual void operator()();

    ~OnAttentionContainer();

    static inline groupEvent_t staticName() { return OnAttentionEvent::staticGroup() + "-container"; }

    virtual inline nameEvent_t name() const { return staticName(); }
};


} // matchn
