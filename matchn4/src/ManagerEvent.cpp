#include "../include/stdafx.h"
#include "../include/ManagerEvent.h"
#include "../include/Event.h"
#include "../include/ge/background/wait-background-event.h"
#include "../include/ge/container/wait-container-event.h"


namespace matchn {


ManagerEvent::ManagerEvent() {
}




ManagerEvent::~ManagerEvent() {
}




void
ManagerEvent::make() {

    // снимаем зависшие события
    // @todo opimize fine Делать только в конце метода.
    discardNotActuality();

    for (auto itr = mStore.begin(); itr != mStore.end(); ) {
        std::shared_ptr< Event >&  e = itr->second;
        // #! Событие может ни разу не выполнить свой operator()().
        if ( e->actuality() && !e->timeOver() ) {
            // событие ещё актуально
            ( *e )();
            ++itr;
        } else {
            // снимаем событие
            // # Деструктор события выполнит нужные действия.
            itr = mStore.erase( itr );
        }
    }

    // отрабатываем команды деструктора
    discardNotActuality();
}




void
ManagerEvent::discardNotActuality() {

    // деструктор может отменить др. события
    for (size_t q = mStore.size(); ; ) {
        for (auto itr = mStore.begin(); itr != mStore.end(); ) {
            const std::shared_ptr< Event >&  e = itr->second;
            e->actuality()
                ? ++itr
                : itr = mStore.erase( itr );
        }
        if (mStore.size() == q) {
            // все деструкторы отработаны, все события актуальны
            break;
        }
        q = mStore.size();

    } // for (size_t q = mStore.size(); ...
}




ManagerEvent::uid_t
ManagerEvent::hash( const nameEvent_t& ne,  const uidElement_t& ue ) {
    std::ostringstream  ss;
    ss << ne << "-" << ue;
    return ss.str();
}




int
ManagerEvent::time() {

    static boost::mutex  mtxThis;
    boost::mutex::scoped_lock  lock( mtxThis );

    /* - Заменено на таймер от ClanLib. См. ниже.
    using namespace boost::gregorian;
    using namespace boost::local_time;
    using namespace boost::posix_time;

    static const ptime  epoch( date( 1970, 1, 1 ) );
    const ptime  now = microsec_clock::local_time();
    const time_duration  diff = now - epoch;

    return static_cast< long >( diff.total_milliseconds() );
    */

    return static_cast< int >( CL_System::get_time() );
}




size_t
ManagerEvent::random() {

    static boost::mutex  mtxThis;
    boost::mutex::scoped_lock  lock( mtxThis );

    static boost::mt19937  gen( 12345 );
    static boost::uniform_int<>  uni( 0, INT_MAX );
    static boost::variate_generator< boost::mt19937&, boost::uniform_int<> >
        rg( gen, uni );

    return rg();
}


} // matchn








namespace std {

std::ostream& operator<<( std::ostream& out,  const matchn::ManagerEvent& me ) {
    typelib::printPairPtr( out, me.store(), "", "", std::make_pair( "", "" ), "\n" );
    return out;
}

} // std
