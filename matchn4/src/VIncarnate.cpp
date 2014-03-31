#include "../include/stdafx.h"
#include "../include/VIncarnate.h"
#include "../include/ge/world/World.h"
#include "../include/ManagerSprite.h"


namespace matchn {


std::unique_ptr< ManagerSprite >  VIncarnate::mManagerSprite(
    new ManagerSprite()
);




VIncarnate::VIncarnate(
    const typelib::coord2Int_t&  vc,
    int  order
) :
    mCoord( vc ),
    mRotation( 0 ),
    mAlpha( 1.0f ),
    mOrder( order )
{
}




VIncarnate::~VIncarnate() {
}




void
VIncarnate::drawSprite(
    CL_GraphicContext&          gc,
    const typelib::size2Int_t&  needVisualSize,
    const kindElement_t&        kind,
    const nameElement_t&        name,
    const groupEvent_t&         ge
) const {
    auto s = mManagerSprite->sprite( gc, needVisualSize, kind, name, ge );
    DASSERT( s
        && "Спрайт для заданного события не найден." );

    s->set_angle( CL_Angle( visualRotation< float >(), cl_degrees ) );
    s->set_alpha( visualAlpha() );
    s->draw( gc, visualCoord().x, visualCoord().y );
    //CONSOLE << visualCoord() << std::endl;
    s->update();
}


} // matchn
