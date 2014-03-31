#pragma once

#include "../../Event.h"


namespace matchn {


/**
* ����������� ���������.
*/
class WaitContainer :
    public WaitEvent
{
public:
    WaitContainer(
        const sourceEvent_t&,
        std::shared_ptr< Incarnate >,
        const protocolDataEvent_t&
    );

    virtual void operator()();

    ~WaitContainer();

    static inline groupEvent_t staticName() { return WaitEvent::staticGroup() + "-container"; }

    virtual inline nameEvent_t name() const { return staticName(); }


private:
    /**
    * ���������, ���� �� ��� ���� ����������� ��������� �����.
    */
    bool freePlaceAbove() const;
};


} // matchn
