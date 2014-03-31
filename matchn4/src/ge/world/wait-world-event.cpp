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

    // # ����� �� ����� ������������ this->world().
    auto w = std::static_pointer_cast< World >( mElement.lock() );

    // ��������� ������� ����������� � ������� ������� �
    // ������ �� ����� ������ ����� ����� ���������
    const auto is = w->incarnateSet();
    typelib::coord2Int_t  cc( 0, w->maxCoord().y );
    for (cc.x = w->minCoord().x; cc.x <= w->maxCoord().x; ++cc.x) {
        // @test
        //if (cc.x != w->minCoord().x) { continue; }

        const std::shared_ptr< Incarnate > container =
            w->element< Container >( cc );
        if ( container ) {
            // ����� �� ���� ���������� ������
            continue;
        }

        // @test ��������� 1 ������� � ����
        //if (w->incarnateSet().size() >= 2) { break; }

        // ����� �� ���� ���������� �������
        const nameElement_t name = Container::nextRandom();
        w->incarnateContainer( name, cc );

        // ��������� 1 ������� �� ���
        //break;

    } // for (cc.x = ...

}




WaitWorld::~WaitWorld() {

    if ( die() ) { return; }

    //CONSOLE << "\n(Stop WaitWorld)\n";
}


} // matchn
