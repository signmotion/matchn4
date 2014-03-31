#include "../include/stdafx.h"
#include "../include/Incarnate.h"
#include "../include/ge/world/World.h"


namespace matchn {


std::unique_ptr< ManagerEvent >  Incarnate::mManagerEvent( new ManagerEvent() );




Incarnate::Incarnate(
    std::shared_ptr< World >     w,
    const std::string&           name,
    const typelib::coord2Int_t&  logicCoord,
    int  order
) :
    GE( w ),
    mName( name ),
    LIncarnate( logicCoord ),
    // для World w не передаётся
    VIncarnate(
        w
          ? w->visualByLogicCoordContainer( logicCoord )
          : typelib::coord2Int_t::ZERO(),
        order
    )
{
    ASSERT( !mName.empty()
        && "Название элемента должно быть указано." );
}




Incarnate::~Incarnate() {
}




Incarnate::incarnateEvent_t Incarnate::actualityEventElement() const {

    incarnateEvent_t r;

    const auto se = mManagerEvent->store();
    for (auto itr = se.cbegin(); itr != se.cend(); ++itr) {
        const Event& e = *itr->second;
        const uidElement_t& u = itr->second->element()->uid();
        if ( e.actuality() && (u == uid()) ) {
            r.push_back( itr->second );
        }
    }

    return r;
}


} // matchn








namespace std {

std::ostream& operator<<( std::ostream& out,  const matchn::Incarnate& ie ) {
    out << ie.uid() << " " << ie.logicCoord();
    return out;
}

} // std
