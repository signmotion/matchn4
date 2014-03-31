#pragma once

#include "Incarnate.h"


namespace matchn {


/**
* Модификаторы события.
* Меняют события для переданного элемента.
*
* # Изменения в элемент вносятся при создании модификатора.
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
* Удаляет события с указанным именем (делает с. неактуальными) у элемента.
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
