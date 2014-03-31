#pragma once

#include "configure.h"
#include "struct.h"


namespace matchn {


/**
* ��������� ��������� ����.
*/
class ManagerSprite {
public:
    /**
    * ��������� ��������� �������.
    * ������� ������������� �� ��������.
    * ������ ������� ����� ����� ���� ������ (����� ��� ������������).
    */
    typedef std::string  pathSprite_t;
    typedef std::unordered_map<
        pathSprite_t,
        std::unique_ptr< CL_Sprite >
    > knownSprite_t;


    /**
    * ��������� ��������� ������ ��������.
    */
    typedef std::string  pathFrame_t;
    typedef boost::unordered_map<
        pathFrame_t,
        std::unique_ptr< CL_PixelBuffer >
    >  knownFrame_t;




    ManagerSprite();




public:
    virtual ~ManagerSprite();




    /**
    * @return ������ ��� �������� � ������ �������.
    *         nullptr, ���� ������ ��������� �� �������.
    *
    * # ������ �������� ������������� �� ������ ������� - ��. ����������
    *   �������� � ManagerEvent::uid< T >().
    * # ������ �������� ������ ���� ������ �������.
    *
    * @see pathSprite()
    */
    CL_Sprite* sprite(
        CL_GraphicContext&,
        const typelib::size2Int_t&  needVisualSize,
        const kindElement_t&,
        const nameElement_t&,
        const groupEvent_t&
    );




    /**
    * @return ����������� �� ���������� ���� ����� � ������� ClanLib.
    *         nullptr, ���� ����� ��������� �� �������.
    */
    CL_PixelBuffer* frame( const std::string& path );




    /**
    * @return ���� � ������� �������� � ������ �������.
    */
    static inline pathSprite_t pathSprite(
        const kindElement_t&  kind,
        const nameElement_t&  name,
        const groupEvent_t&   ge
    ) {
        const std::string path = PATH_MEDIA + "/" + kind + "/" + name + "/" + ge;
        return path;
    }




private:
    /**
    * �������� ������ �� ������� �� ���������� ����.
    */
    CL_Sprite* loadSprite(
        CL_GraphicContext&,
        const typelib::size2Int_t&  needVisualSize,
        const pathSprite_t&
    );




private:
    knownSprite_t  mKnownSprite;
    knownFrame_t   mKnownFrame;
};


} // matchn
