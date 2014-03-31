#include "../../../include/stdafx.h"
#include "../../../include/ge/world/wait-world-event.h"
#include "../../../include/ge/world/World.h"
#include "../../../include/ge/container/Container.h"


namespace matchn {


WaitWorld::WaitWorld(
    const sourceEvent_t&          source,
    std::shared_ptr< Incarnate >  ie,
    const protocolDataEvent_t&    data
) : WaitEvent( source, ie, data ) {
    //CONSOLE << "\n(Start WaitWorld)\n";
}




void
WaitWorld::operator()() {

    //CONSOLE << "(WaitWorld)\t";

    // # «десь не можем использовать this->world().
    auto w = std::static_pointer_cast< World >( mElement.lock() );

    // провер€ем наличие контейнеров в верхних €чейках и
    // создаЄм на месте пустых €чеек новые конейнеры
    const auto is = w->incarnateSet();
    typelib::coord2Int_t  cc( 0, w->maxCoord().y );
    for (cc.x = w->minCoord().x; cc.x <= w->maxCoord().x; ++cc.x) {
        // @test
        //if (cc.x != w->minCoord().x) { continue; }

        const std::shared_ptr< Incarnate > container =
            w->element< Container >( cc );
        if ( container ) {
            // место по этой координате зан€то
            continue;
        }

        // @test воплощаем 1 элемент в мире
        //if (w->incarnateSet().size() >= 2) { break; }

        // место по этой координате пустует
        const nameElement_t name = Container::nextRandom();
        w->incarnateContainer( name, cc );

        // воплощаем 1 элемент за раз
        //break;

    } // for (cc.x = ...

}




WaitWorld::~WaitWorld() {

    if ( die() ) { return; }

    //CONSOLE << "\n(Stop WaitWorld)\n";
}


} // matchn
