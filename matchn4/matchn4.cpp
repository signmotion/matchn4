#include "include/stdafx.h"
#include "include/configure.h"
#include "include/ge/world/World.h"




void run();




/**
* ������������ ����� ������������ ������� ����-������ MatchN3.
*
* -> ��. MatchN5.
*/
int
main( int argc, char** argv ) {

    using namespace matchn;

            
    setlocale( LC_ALL, "Russian" );
    setlocale( LC_NUMERIC, "C" );


    // ������ ���
    int code = 0;
    try {
        run();

	} catch ( const CL_Exception& ex ) {
		CL_ConsoleWindow  console( "Console", 100, 200 );
		CL_Console::write_line(
            "(!) Exception caught: " + ex.get_message_and_stack_trace()
        );
		console.display_close_message();
		code = -1;

    } catch ( const std::exception& ex ) {
        std::cerr << std::endl << ex.what() << std::endl;
        code = -2;

    } catch ( const std::string& ex ) {
        std::cerr << std::endl << ex << std::endl;
        code = -100;

    } catch ( const char* const ex ) {
        std::cerr << std::endl << *ex << std::endl;
        code = -200;

    } catch ( ... ) {
        std::cerr << std::endl << "(!) Unknown exception." << std::endl;
        code = -300;
    }

    std::cout << "\n^\n";
    //std::cin.ignore();

    return code;

}; // main()








inline void
run() {

    using namespace matchn;

    std::shared_ptr< World >  w;
    for (auto command = World::command_t();
         command.first != World::QUIT;
    ) {
        // ������������ ��������� ������� ����
        size_t K = w ? w->K : 3;
        size_t S = w ? w->N : 5;
        switch ( command.first ) {
            case World::DECREASE_CHAIN :
                K = w->K - 1;
                K = (K < 2) ? 2 : K;
                break;

            case World::DECREASE_TERRITORY :
                S = w->N - 2;
                S = (S < 3) ? 3 : S;
                break;

            case World::INCREASE_CHAIN :
                K = w->K + 1;
                K = (K > 10) ? 10 : K;
                break;

            case World::INCREASE_TERRITORY :
                S = w->N + 2;
                S = (S > 15) ? 15 : S;
                break;
        }

        w = World::valueOf( K, S, S );
        command = w->go();

        if (command.first == World::QUIT) {
            // ��������� ������ � �����
            break;
        }

    } // for (auto command = ...

}
