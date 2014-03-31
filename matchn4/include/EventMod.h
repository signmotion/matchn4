#pragma once

#include "Incarnate.h"


namespace matchn {


/**
* ������������ �������.
* ������ ������� ��� ����������� ��������.
*
* # ��������� � ������� �������� ��� �������� ������������.
*
* @see Event
*/
class EventMod {
public:
    inline EventMod() {
    }

    virtual inline ~EventMod() {
    }
};




/**
* ������� ������� � ��������� ������ (������ �. �������������) � ��������.
*/
class CancelEventMod :
    public EventMod
{
protected:
    CancelEventMod(
        std::shared_ptr< Incarnate >,
        const std::vector< groupEvent_t >&
    );

    virtual inline ~CancelEventMod() {
    }
};


} // matchn
