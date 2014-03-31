#include "../../../include/stdafx.h"
#include "../../../include/ge/container/Container.h"
#include "../../../include/ge/world/World.h"
#include "../../../include/ManagerEvent.h"


namespace matchn {


size_t Container::countAvailable_ = 0;




Container::Container(
    std::shared_ptr< World >     w,
    const nameElement_t&         name,
    const typelib::coord2Int_t&  logicCoord
) :
    // ���������� �������� ����� ����
    Incarnate( w, name, logicCoord, 1 )
{
}




Container::~Container() {
}




std::shared_ptr< Container >
Container::valueOf(
    std::shared_ptr< World >     world,
    const nameElement_t&         name,
    const typelib::coord2Int_t&  logicCoord
) {
    return std::unique_ptr< Container >( new Container( world, name, logicCoord ) );
}




void Container::draw( std::shared_ptr< World >  w ) const {

    // ��� ���������� ���������� ����� �������, �������� �� ���
    const auto aee = actualityEventElement();
    /* - @todo fine �������������� ����������� �����.
    if ( (aee.size() != 1) && (aee.size() != 2) ) {
        typelib::printPtr( std::cout, aee );
        CONSOLE << std::endl << *this << std::endl;
        CONSOLE << std::endl << *managerEvent() << std::endl;
    }
    DASSERT( (aee.size() == 1)
        && "��� ���������� ������ ������������ 1 � ������������ �������."
           " �������� ����������� ����." );
    */
    const auto& e = aee.front();
    const groupEvent_t ge = e->group();
    drawSprite( w->visual()->get_gc(), needVisualSize(), "container", name(), ge );
}




typelib::size2Int_t
Container::needVisualSize() const {
    // ��������� ������ ���������� � ����� ���� N x M
    const auto vs = 
        typelib::size2_t( WINDOW_WIDTH, WINDOW_HEIGHT ) /
        static_cast< typelib::size2_t >( ELEMENT_SPRITE_SIZE ) /
        typelib::size2_t( world()->N, world()->M ) * 0.9;

    return static_cast< typelib::size2_t >( ELEMENT_SPRITE_SIZE ) * vs;
}




nameElement_t
Container::nextRandom() {

    static boost::mt19937  gen;
    // # ��������� ������ ��� �������� �������: ��� ���� ���������, �������
    //   ������ ������. �������: -2.
    const size_t ne = countAvailable();
    ASSERT( (ne >= 2)
        && "� ����� 'media/element' ������ ���� ������ ������ ��������." );
    static boost::uniform_int<>   uni( 'a', 'a' + ne - 2 );
    static boost::variate_generator< boost::mt19937&, boost::uniform_int<> >
        random( gen, uni );

    const auto ch = static_cast< char >( random() );
    return std::string( 1, ch );
}




size_t
Container::countAvailable() {
    if (countAvailable_ == 0) {
        std::set< std::string >  folder;
        typelib::file::listFolder( &folder, PATH_MEDIA + "/container" );
        countAvailable_ = folder.size();
        ASSERT( (countAvailable_ > 0)
            && "� ����� 'media/container' �� ���������� �� ������ ��������." );
    }

    return countAvailable_;
}


} // matchn
