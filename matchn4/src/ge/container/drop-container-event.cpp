#include "../../../include/stdafx.h"
#include "../../../include/ge/container/Container.h"
#include "../../../include/ge/world/World.h"


namespace matchn {


DropContainer::DropContainer(
    const sourceEvent_t&          source,
    std::shared_ptr< Incarnate >  ie,
    const protocolDataEvent_t&    data
) : DropEvent( source, ie, data ) {
    //CONSOLE << "\n(Start DropContainer " << element()->logicCoord() << ")\n";
}




void
DropContainer::operator()() {

    //CONSOLE << "(DropContainer " << element()->logicCoord() << ")\t";

    // меняем координаты спрайта
    const auto nepper = nepperDeltatime();
    const auto height = static_cast< float >( element()->needVisualSize().y );
    const auto shift = nepper * height;
    const auto lc = element()->logicCoord();
    const auto vc = world()->visualByLogicCoordContainer( lc );
    element()->visualCoord( typelib::coord2Int_t( vc.x,  vc.y + shift ) );
}




DropContainer::~DropContainer() {

    if ( die() ) { return; }

    // меняем логическую координату, обновляем визуальную и заботимся,
    // чтобы цепочка событий сохранилась
    const auto& lc = element()->logicCoord();
    if (lc.y > world()->minCoord().y) {
        element()->logicCoord( typelib::coord2Int_t( lc.x,  lc.y - 1 ) );
        const auto vc = world()->visualByLogicCoordContainer( lc );
        element()->visualCoord( vc );
    }

    element()->addEvent< WaitContainer >( "~DropContainer" );

    //CONSOLE << "\n(Stop DropContainer " << element()->logicCoord() << ")\n";
}


} // matchn
