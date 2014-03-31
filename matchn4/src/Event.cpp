#include "../include/stdafx.h"
#include "../include/Event.h"


namespace matchn {


const boost::posix_time::milliseconds  Event::HALF_SECOND( 500 );
const boost::posix_time::milliseconds  Event::SECOND( 1000 );
const boost::posix_time::milliseconds  Event::TWO_SECOND( 2000 );




Event::Event(
    const sourceEvent_t&           source,
    std::shared_ptr< Incarnate >   element,
    const typelib::json::Variant&  rawData
) :
    mSource( source ),
    mElement( element ),
    mRawData( rawData ),
    mActuality( true )
{
    ASSERT( !source.empty() );
    ASSERT( element );

    // распознаём данные
    reload();
}




Event::~Event() {
}




uidEvent_t Event::uid() const {
    std::ostringstream  ss;
    ss << name() << "-" << element()->uid();
    return ss.str();
}




void Event::reload() {

    const auto start = ManagerEvent::time();
    const auto duration = data< float >( "duration", -1.0f ) * 1000.0f;
    ASSERT( ( (duration > 0) || typelib::equal( duration, -1000.0f) )
        && "Длительность события должна быть больше 0 или == -1"
           " для бесконечного события.");
    const auto stop = start + duration;

    const auto overlap = data< bool >( "overlap", false );

    const DataEvent t = {
        static_cast< timeMS_t >( boost::math::round( start ) ),
        static_cast< timeMS_t >( boost::math::round( duration ) ),
        static_cast< timeMS_t >( boost::math::round( stop ) )
    };
    mData = t;
}


} // matchn








namespace std {

std::ostream& operator<<( std::ostream& out,  const matchn::Event& e ) {
    out << e.source() << " >> " <<
        (e.actuality() ? "+" : "-") << " " <<
        e.uid() << "  " << e.element()->logicCoord();
    return out;
}

} // std
