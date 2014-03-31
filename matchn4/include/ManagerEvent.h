#pragma once

#include "struct.h"


namespace matchn {


class Event;


class ManagerEvent {
public:
    /**
    * ��������� ��� �������.
    *
    * @todo optimize �������� �� typedef size_t  uid_t.
    */
    typedef std::string  uid_t;
    typedef std::map< uid_t, std::shared_ptr< Event > >  store_t;



public:
    ManagerEvent();

    virtual ~ManagerEvent();


    inline store_t const&  store() const {
        return mStore;
    }

    inline store_t&  store() {
        return mStore;
    }


    template< class A >
    inline void push( const uid_t& ume,  std::shared_ptr< A >  e ) {
        mStore.insert( std::make_pair( ume, e ) );
        /*
        CONSOLE << "Added " << *e << " " <<
            typeid( *e ).name() << " for " << typeid( *e->element() ).name() << " " <<
            e->element()->logicCoord() << "." << std::endl;
        */
    }


    inline void remove( const uid_t& ume ) {
        auto ftr = mStore.find( ume );
        if (ftr != mStore.end()) {
            mStore.erase( ftr );
        }
    }


    /**
    * ������� ��� �������.
    */
    inline void clear() {
        mStore.clear();
    }


    /**
    * ������������ ����������� ����� push() �������.
    */
    void make();


    /**
    * ������� ������������ ������� (�������������� �����������).
    */
    void discardNotActuality();


    /**
    * @return ������� � �������� ManagerEvent::uid_t ���� � ���� �������.
    */
    inline bool has( const uid_t& ume ) const {
        return (mStore.find( ume ) != mStore.cend());
    }


    static uid_t hash( const nameEvent_t&, const uidElement_t& );


    /**
    * @return ������� �����, ��������� �� ������ �������, ��.
    */
    static int time();


    /**
    * @return ��������� �����, ��������� �� ������ �������.
    */
    static size_t random();


    /**
    * @return ������ � UID �������.
    */
    template< class T >
    static inline groupEvent_t group() {
        return T::staticGroup();
    }


    template< class T >
    static inline nameEvent_t name() {
        return T::staticName();
    }



private:
    store_t  mStore;
};


} // matchn








namespace std {

std::ostream& operator<<( std::ostream&,  const matchn::ManagerEvent& );

} // std
