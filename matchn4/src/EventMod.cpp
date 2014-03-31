#include "../include/stdafx.h"
#include "../include/EventMod.h"
#include "../include/Event.h"


namespace matchn {


CancelEventMod::CancelEventMod(
    std::shared_ptr< Incarnate >       ie,
    const std::vector< groupEvent_t >&  vge
) {
    //CONSOLE << "\n(Do CancelEventMod)\n";

    ASSERT( ie );
    ASSERT( !vge.empty()
        && "Список событий для отмены должен быть задан." );

    // прекратим (пометим как неактуальные) события для этого элемента
    auto& store = ie->managerEvent()->store();
    for (auto itr = store.begin(); itr != store.end(); ++itr) {
        const std::shared_ptr< Event >&  e = itr->second;
        const std::shared_ptr< Incarnate >&  el = e->element();
        if (el->uid() == ie->uid()) {
            const nameEvent_t& ge = e->group();
            for (auto vtr = vge.cbegin(); vtr != vge.cend(); ++vtr) {
                if (*vtr == ge) {
                    itr->second->dropActuality();
                }
            }
        }
    } // for (auto itr = store.begin(); ...
}


} // matchn
