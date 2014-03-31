#pragma once

#include "../../Incarnate.h"
#include "attention-container-event.h"
#include "drop-container-event.h"
#include "same-container-event.h"
#include "wait-container-event.h"


namespace matchn {


class World;


/**
* ��������� (��� ����� ������ �������).
*/
class Container :
    public Incarnate
{
protected:
    Container(
        std::shared_ptr< World >,
        const nameElement_t&,
        const typelib::coord2Int_t&  logicCoord
    );




public:
    static std::shared_ptr< Container >  valueOf(
        std::shared_ptr< World >,
        const nameElement_t&,
        const typelib::coord2Int_t&  logicCoord
    );




    virtual ~Container();




    /**
    * @virtual VIncarnate
    */
    virtual void draw( std::shared_ptr< World > ) const;




    /**
    * @virtual VIncarnate
    */
    virtual typelib::size2Int_t needVisualSize() const;




    /**
    * @return ��������� ���������.
    */
    static nameElement_t nextRandom();




    /**
    * @return ���������� ��������� �����������.
    *
    * # ������������ �� ����������� ����� 'media/container'.
    */
    static size_t countAvailable();




protected:
    /**
    * ���.
    */
    static size_t countAvailable_;
};


} // matchn
