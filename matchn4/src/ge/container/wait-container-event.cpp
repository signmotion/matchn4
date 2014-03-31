#include "../../../include/stdafx.h"
#include "../../../include/ge/container/Container.h"
#include "../../../include/ge/world/World.h"


namespace matchn {


WaitContainer::WaitContainer(
    const sourceEvent_t&          source,
    std::shared_ptr< Incarnate >  ie,
    const protocolDataEvent_t&    data
) : WaitEvent( source, ie, data ) {
    //CONSOLE << "\n(Start WaitContainer " << element()->logicCoord() << ")\n";
}




void
WaitContainer::operator()() {

    //CONSOLE << "(WaitContainer " << element()->logicCoord() << ")\t";

    // #? јктивное событие может служить признаком, какой спрайт рисовать.

    if ( freePlaceAbove() ) {
        // скажем контейнеру падать
        protocolDataEvent_t  data;
        data[ "duration" ] = 0.5f;
        // # Ќовое событие отменит актуальность дл€ всех событий этого
        //   элемента, включа€ выполн€емое сейчас.
        element()->addEvent< DropContainer >( "WaitContainer()", data );
    }
}




WaitContainer::~WaitContainer() {

    if ( die() ) { return; }

    //CONSOLE << "\n(Stop WaitContainer " << element()->logicCoord() << ")\n";
}




bool
WaitContainer::freePlaceAbove() const {

    const auto lc = element()->logicCoord();
    const auto w = world();
    if (lc.y == w->minCoord().y) {
        // нижнему контейнеру некуда падать
        return false;
    }

    // смотрим наличие контейнера ниже
    const auto shift = w->isc( 3 );
    const auto above = lc + shift;
    const auto is = w->incarnateSet();
    for (auto itr = is.cbegin(); itr != is.cend(); ++itr) {
        const auto& el = *itr;
        if (typeid( Container ) == typeid( *el )) {
            const auto& lc = el->logicCoord();
            if (lc == above) {
                return false;
            }
        }
    }

    return true;
}


} // matchn
