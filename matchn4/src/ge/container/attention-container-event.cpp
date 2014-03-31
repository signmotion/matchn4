#include "../../../include/stdafx.h"
#include "../../../include/ge/container/Container.h"
#include "../../../include/ge/world/World.h"


namespace matchn {


OnAttentionContainer::OnAttentionContainer(
    const sourceEvent_t&          source,
    std::shared_ptr< Incarnate >  ie,
    const protocolDataEvent_t&    d
) : OnAttentionEvent( source, ie, d ) {
    //CONSOLE << "\n(Start OnAttentionContainer " << element()->logicCoord() << ")\n";

    // # Покажем все контейнеры того же типа, на который наведена мышь.

    // выделяем одинаковые контейнеры, гасим остальные
    const auto& is = world()->incarnateSet();
    for (auto itr = is.cbegin(); itr != is.cend(); ++itr) {
        const auto& el = *itr;
        if (typeid( Container ) != typeid( *el )) {
            continue;
        }
        if (el->name() == element()->name()) {
            protocolDataEvent_t data;
            data[ "duration" ] = 2;
            el->addEvent< SameContainer >( "OnAttentionContainer", data );
        } else {
            //el->addEvent< WaitContainer >( "OnAttentionContainer" );
        }
    }
}




void
OnAttentionContainer::operator()() {

    //CONSOLE << "(OnAttentionContainer " << element()->logicCoord() << ")\t";
}




OnAttentionContainer::~OnAttentionContainer() {

    if ( die() ) { return; }

    // дадим этому контейнеру то же событие, что он дал себе подобным
    element()->addEvent< SameContainer >( "~OnAttentionContainer" );

    //CONSOLE << "\n(Stop OnAttentionContainer " << element()->logicCoord() << ")\n";
}


} // matchn
