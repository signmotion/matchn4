#include "../../../include/stdafx.h"
#include "../../../include/ge/background/Background.h"


namespace matchn {


WaitBackground::WaitBackground(
    const sourceEvent_t&          source,
    std::shared_ptr< Incarnate >  ie,
    const protocolDataEvent_t&    data
) : WaitEvent( source, ie, data ) {
    //CONSOLE << "\n(Start WaitBackground)\n";
}




void
WaitBackground::operator()() {

    // # О целостности структуры заботится ManagerEvent.

    //CONSOLE << "(WaitBackground)\t";
}



WaitBackground::~WaitBackground() {

    if ( die() ) { return; }

    //CONSOLE << "\n(Stop WaitBackground)\n";
}


} // matchn
