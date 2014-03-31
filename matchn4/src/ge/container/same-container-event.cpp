#include "../../../include/stdafx.h"
#include "../../../include/ge/container/Container.h"
#include "../../../include/ge/world/World.h"


namespace matchn {


SameContainer::SameContainer(
    const sourceEvent_t&          source,
    std::shared_ptr< Incarnate >  ie,
    const protocolDataEvent_t&    data
) : SameEvent( source, ie, data ) {
    //CONSOLE << "\n(Start SameContainer " << element()->logicCoord() << ")\n";
}




void
SameContainer::operator()() {
    //CONSOLE << "(SameContainer " << element()->logicCoord() << ")\t";

    // нужные действия проделал конструктор
}




SameContainer::~SameContainer() {

    if ( die() ) { return; }

    //CONSOLE << "\n(Stop SameContainer " << element()->logicCoord() << ")\n";

    element()->addEvent< WaitContainer >( "~SameContainer" );
}


} // matchn
