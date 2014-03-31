#include "../../../include/stdafx.h"
#include "../../../include/ge/background/Background.h"
#include "../../../include/ge/container/Container.h"
#include "../../../include/ge/world/World.h"


namespace matchn {


World::World(
    size_t K, size_t N, size_t M
) :
    // дл€ World пор€док отрисовки не важен
    Incarnate( nullptr, ".",  typelib::coord2Int_t::ZERO(), 0 ),
    K( K ), N( N ), M( M ),
    mIncarnateSet(),
    mCurrentTime( 0 )
{
    ASSERT( (K >= 2 )
        && " оличество собираемых вместе элементов должно быть 2 или больше." );
    ASSERT( ( (N >= 3) && (M >= 3) )
        && "–азмер пол€ не может быть меньше чем 3 x 3." );
    ASSERT( ( ((N % 2) != 0) && ((M % 2) != 0) )
        && "–азмеры пол€ должны быть нечЄтными." );


    // *графический мир*
    // @source Example "SpritesRTS" from http://clanlib.org
    // @source Example "TileMap" from http://clanlib.org
    {
        // инициализируем ClanLib
	    CL_SetupCore setup_core;
	    CL_SetupDisplay setup_display;
	    #ifdef USE_SOFTWARE_RENDERER
		    CL_SetupSWRender setup_swrender;
	    #endif
	    #ifdef USE_OPENGL_1
		    CL_SetupGL1 setup_gl1;
	    #endif
	    #ifdef USE_OPENGL_2
		    CL_SetupGL setup_gl;
	    #endif
	    //CL_SetupSound setup_sound;

        std::stringstream  ss;
        ss << "Match" << K << "    " << N << " x " << M;
        CL_DisplayWindowDescription  desc( ss.str() );
	    desc.set_size( CL_Size( WINDOW_WIDTH, WINDOW_HEIGHT ),  false );
	    desc.set_allow_resize( false );
	    mVisualWorld =
            std::unique_ptr< CL_DisplayWindow >( new CL_DisplayWindow( desc ) );
    }


    // *мир звуков*
    // @todo mAudiuWorld = ...


    // подключаем слушателей событий от игрока
    {
        mSlotMouseDown =
            mVisualWorld->get_ic().get_mouse().sig_key_down()
            .connect( this, &World::onMouseDown );
        mSlotMouseMove =
            mVisualWorld->get_ic().get_mouse().sig_pointer_move()
            .connect( this, &World::onMouseMove );
	    mSlotKeyboardDown =
            mVisualWorld->get_ic().get_keyboard().sig_key_down()
            .connect( this, &World::onKeyboardDown );
        mSlotWindowClose =
            mVisualWorld->sig_window_close()
            .connect( this, &World::onWindowClose );
    }


    //mVisualWorld->hide_cursor();
}




std::shared_ptr< World >
World::valueOf( size_t K, size_t N, size_t M ) {
    return std::shared_ptr< World >( new World( K, N, M ) );
}




World::~World() {
}




void World::draw( std::shared_ptr< World >  world ) const {
    auto& is = incarnateSet();
    for (auto itr = is.begin(); itr != is.end(); ++itr) {
        ( *itr )->draw( world );
    }
}




void
World::incarnateBackground( const nameElement_t&  name ) {
    auto elu = Background::valueOf( shared(), name );
    auto els = std::shared_ptr< Background >( std::move( elu ) );
    auto elsi = std::static_pointer_cast< Incarnate >( els );
    mIncarnateSet.push_back( elsi );

    // # Ёлементы расположены в пор€дке отрисовки.
    // @todo optimize ƒобавл€ть сразу в нужное место.
    mIncarnateSet.sort( [] (
        const std::shared_ptr< Incarnate >&  a,
        const std::shared_ptr< Incarnate >&  b
    ) {
        return (a->visualOrder() < b->visualOrder());
    } );

    els->addEvent< WaitBackground >( "World::incarnateBackground()" );
}




void
World::incarnateContainer(
    const nameElement_t&         name,
    const typelib::coord2Int_t&  logicCoord
) {
    auto elu = Container::valueOf( shared(), name, logicCoord );
    auto els = std::shared_ptr< Container >( std::move( elu ) );
    auto elsi = std::static_pointer_cast< Incarnate >( els );
    mIncarnateSet.push_back( elsi );

    // # Ёлементы расположены в пор€дке отрисовки.
    // @todo optimize ƒобавл€ть сразу в нужное место.
    mIncarnateSet.sort( [] (
        const std::shared_ptr< Incarnate >&  a,
        const std::shared_ptr< Incarnate >&  b
    ) {
        return (a->visualOrder() < b->visualOrder());
    } );

    els->addEvent< WaitContainer >( "World::incarnateContainer()" );
}




World::command_t
World::go() {

    // создаЄм событие дл€ обработки событий мира
    addEvent< WaitWorld >( "World::go()" );


    // воплощаем об€зательные элементы
    incarnateBackground( "fable" );


    // показываем
    auto gc = mVisualWorld->get_gc();
    auto ic = mVisualWorld->get_ic();
    CL_Font  font( gc, "Tahoma", 16 );
    CL_BlendMode  blendTransparent;
    int lastTime = CL_System::get_time();
    for (auto keyboard = ic.get_keyboard();
         !keyboard.get_keycode( CL_KEY_ESCAPE );
    ) {
        // @source http://clanlib.org/wiki/MainDocs:Timing
		mCurrentTime = CL_System::get_time();
		int deltaTime = mCurrentTime - lastTime;
        // ждЄм: окно приложени€ могут перетаскивать
		if (deltaTime > 1000) { deltaTime = 1000; }
        // 'deltaTimeMs' можно использовать дл€ корректного тайминга
		//const float deltaTimeMs = static_cast< float >( deltaTime );
        lastTime = mCurrentTime;


#ifdef _DEBUG
        // дл€ FPS
        // @source http://clanlib.org/docs/clanlib-0.8.0/Overview/tipsandtricks.html
        const size_t fps = World::calcFPS( deltaTime );
#endif


        gc.clear( CL_Colorf::aquamarine );

        //gc.set_map_mode( cl_map_2d_upper_left );
        //gc.set_blend_mode( blendTransparent );


        managerEvent()->make();
        // @test
        //std::cout << std::endl << *managerEvent() << std::endl;

        /* @test
        {
            auto& is = incarnateSet();
            for (auto itr = is.begin(); itr != is.end(); ++itr) {
                const auto& el = **itr;
                const auto an = typeid( Container ).name();
                const auto bn = typeid( el ).name();
                if (typeid( Container ) != typeid( el )) {
                    continue;
                }
                const auto aee = el.actualityEventElement();
                typelib::printPtr( std::cout, aee );
                CONSOLE << " from World::go()" << std::endl;
                if ( (aee.size() != 1) && (aee.size() != 2) ) {
                    CONSOLE << std::endl << *managerEvent() << std::endl;
                    addEvent< DropContainer >( "." );
                    CONSOLE << std::endl << *managerEvent() << std::endl;
                }
            }
        }
        */


        draw( this->shared() );


#ifdef _DEBUG
        // покажем информацию дл€ отладки
        {
            std::ostringstream ss;
            ss << fps;
            //const auto s = CL_StringHelp::int_to_text( fps );
            font.draw_text( gc, 10, 20, ss.str(), CL_Colorf::lightcoral );
        }
#endif

        mVisualWorld->flip( 1 );
        CL_KeepAlive::process();

		mCurrentTime = CL_System::get_time();
        // 10 ms (100 hz)
		static const int mainLoopRate = 10;
		const int timeToSleep = mainLoopRate - (mCurrentTime - lastTime);
		if (timeToSleep > 0) {
            CL_System::sleep( timeToSleep );
        }


        // опрашиваем клавиатуру
        if ( keyboard.get_keycode( CL_KEY_LEFT ) ) {
            // уменьшаем длину цепочки
            return command_t( DECREASE_CHAIN, nullptr );

        } else if ( keyboard.get_keycode( CL_KEY_RIGHT ) ) {
            // увеличиваем длину цепочки
            return command_t( INCREASE_CHAIN, nullptr );

        } if ( keyboard.get_keycode( CL_KEY_DOWN ) ) {
            // уменьшаем размер территории
            return command_t( DECREASE_TERRITORY, nullptr );

        } if ( keyboard.get_keycode( CL_KEY_UP ) ) {
            // увеличиваем размер территории
            return command_t( INCREASE_TERRITORY, nullptr );
        }

    } // for (auto keyboard = ic.get_keyboard(); ...


    return command_t( QUIT, nullptr );
}




typelib::coord2Int_t
World::visualByLogicCoordContainer(
    const typelib::coord2Int_t&  lc
) const {
    const auto vss =
        typelib::size2_t( WINDOW_WIDTH, WINDOW_HEIGHT ) /
        typelib::size2_t( N, M ) * 0.9;
    const auto vnlc =
        static_cast< typelib::coord2_t >( lc ) *
        static_cast< typelib::coord2_t >( vss );
    const auto c = typelib::coord2Int_t(
        vnlc.x + CENTER_WINDOW_WIDTH,
       -vnlc.y + CENTER_WINDOW_HEIGHT
    );
    return c;
}




typelib::coord2Int_t
World::logicByVisualCoordContainer(
    const typelib::coord2Int_t&  vc
) const {
    const auto lnvc = typelib::coord2_t(
        vc.x - CENTER_WINDOW_WIDTH,
      -(vc.y - CENTER_WINDOW_HEIGHT)
    );
    const auto vss =
        typelib::size2_t( WINDOW_WIDTH, WINDOW_HEIGHT ) /
        typelib::size2_t( N, M ) * 0.9;
    const auto c = lnvc / static_cast< typelib::coord2_t >( vss );
    return typelib::coord2Int_t(
        boost::math::round( c.x ),
        boost::math::round( c.y )
    );
}




template< class T >
std::shared_ptr< Incarnate >
World::element( const typelib::coord2Int_t&  logicCoord ) {

    for (auto itr = mIncarnateSet.begin(); itr != mIncarnateSet.end(); ++itr) {
        if (typeid( T ) == typeid( **itr )) {
            const auto& lc = ( *itr )->logicCoord();
            if (lc == logicCoord) {
                return *itr;
            }
        }
    }

    return nullptr;
}




#if 0
World::incarnateSet_t::iterator
World::element( const typelib::coord2Int_t&  logicCoord ) {
    auto itr = mIncarnateSet.begin();
    for ( ; itr != mIncarnateSet.end(); ++itr) {
        const auto& lc = ( *itr )->logicCoord();
        if ( /* (typeid( T ) == typeid( **itr )) - сейчас без разницы. @todo
          && */ (lc == logicCoord)
        ) {
            break;
        }
    }
    return itr;
}
#endif




void
World::onMouseDown( const CL_InputEvent& ie, const CL_InputState& is ) {
}




void
World::onMouseMove( const CL_InputEvent& ie, const CL_InputState& is ) {

    boost::mutex::scoped_lock  lock( mtxMouseAction );

    const auto attentionPoint =
        typelib::coord2Int_t( ie.mouse_pos.x, ie.mouse_pos.y );
    const auto lc = logicByVisualCoordContainer( attentionPoint );
    auto& it = element< Container >( lc );
    if ( it ) {
        // # ѕоведение реализовано в классе событи€.
        //it->removeEvent< OnAttentionContainer >();
        it->addEvent< OnAttentionContainer >( "World::onMouseMove()" );
    }
}




void
World::onKeyboardDown( const CL_InputEvent& ie, const CL_InputState& is ) {
}




void
World::onWindowClose() {
}




size_t
World::calcFPS( size_t frameTime ) {
	static size_t fpsResult  = 0;
	static size_t fpsCounter = 0;
	static size_t totalTime  = 0;
	
	totalTime += frameTime;
	if (totalTime >= 1000) {
		fpsResult = fpsCounter + 1;
		fpsCounter = totalTime = 0;
	}
	++fpsCounter;

	return fpsResult;
}


} // matchn
