#pragma once

#include "../../Incarnate.h"
#include "wait-background-event.h"


namespace matchn {


class World;


/**
* ��� ����.
*/
class Background :
    public Incarnate
{
protected:
    Background(
        std::shared_ptr< World >,
        const nameElement_t&
    );




public:
    static std::shared_ptr< Background >  valueOf(
        std::shared_ptr< World >,
        const nameElement_t&
    );




    virtual ~Background();




    /**
    * @virtual VIncarnate
    */
    virtual void draw( std::shared_ptr< World > ) const;




    /**
    * @virtual VIncarnate
    */
    virtual inline typelib::size2Int_t needVisualSize() const {
        // ��� ������ ��������� �� ������� ������� (�� ����)
        return typelib::size2Int_t( WINDOW_WIDTH, WINDOW_HEIGHT );
    }
};


} // matchn
