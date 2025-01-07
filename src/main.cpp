
#ifdef __unix__
#include <raylib.h>
#elif defined(_WIN32) || defined(WIN32)
#include "raylib.h"
#endif

#include <cstddef>
#include <cstdint>

#include <iostream>
#include <random>
#include <utility>

/*
* NOTES
* Write drawing bits
* Can't think of anything else
*/


int main( int argc, char** argv, char** envv ) {

    const std::size_t size_x = 50;
    const std::size_t size_y = 50;

    const std::size_t scale = 4;


    std::random_device os_seed;
    std::mt19937_64 randengine( os_seed() );

    std::uniform_int_distribution< int > randRangeX( 0, size_x - 1 );
    std::uniform_int_distribution< int > randRangeY( 0, size_y - 1 );

    int finishedCells = 0;

    /*
    * coordinate plane is graphics standard, ( +y is down, origin is top left), not cartesian standard ( +y is up, origin is center)
    */
    std::vector< std::vector< uint_least8_t > > maze( size_x, std::vector< uint_least8_t >( ( uint_least8_t ) size_y ) ); // 0 means not part of maze, 1 means part of maze but not path, 2 means part of maze path. outer vec is the x coord inner vec is y coord
    // maze.resize( size_x );
    // for ( auto row : maze ) {
    //     row.resize( size_y );
    // }

    int init_x = randRangeX( randengine );
    int init_y = randRangeY( randengine );

    maze[ init_x ][ init_y ] = 2;
    if ( init_x != 0 ) {
        maze[ init_x - 1 ][ init_y ] = 1;
    }
    if ( init_x != 0 && init_y != 0 ) {
        maze[ init_x - 1 ][ init_y - 1 ] = 1;
    }
    if ( init_y != 0 ) {
        maze[ init_x ][ init_y - 1 ] = 1;
    }
    if ( init_x < size_x && init_y != 0 ) {
        maze[ init_x + 1 ][ init_y - 1 ] = 1;
    }
    if ( init_x < size_x ) {
        maze[ init_x + 1 ][ init_y ] = 1;
    }
    if ( init_x < size_x && init_y < size_y ) {
        maze[ init_x + 1 ][ init_y + 1 ] = 1;
    }
    if ( init_y < size_y ) {
        maze[ init_x ][ init_y + 1 ] = 1;
    }
    if ( init_x != 0 && init_y < size_y ) {
        maze [ init_x - 1 ][ init_y + 1 ] = 1;
    }

    InitWindow( size_x * scale, size_y * scale, "Maze Generator" );


    /*
    * Actual algorithm begins here
    */

    while ( finishedCells != size_x * size_y ) {
        std::vector< std::pair< int, int > > loop;
        loop.push_back( std::pair( randRangeX( randengine ), randRangeY( randengine ) ) );
        if ( maze[ loop[ 0 ].first ][ loop[ 0 ].second ] == 2 ) {
            continue;
        }
        // add the "random walk" part and the "loop erased" part of the loop erased random walk
        bool finished = false;
        std::uniform_int_distribution< int > randRangeDirection( 0, 3 );
        while ( !finished ) {
            std::pair< int, int > newPos( loop[ loop.size() - 1 ] );
            
            BeginDrawing();
            ClearBackground( BLACK );
            std::cerr << "Drawing frame\n";
            for ( int i = 0; i < size_x; i++ ) {
                for ( int j = 0; j < size_y; j++ ) {
                    DrawRectangle( i * scale, j * scale, scale, scale, maze[ i ][ j ] == 2 ? WHITE : BLACK);
                }
            }
            for ( int i = 0; i < loop.size(); i++ ) {
                DrawRectangle( loop[ i ].first * scale, loop[ i ].second * scale, scale, scale, PINK );
            }
            std::cout << "Drawn frame\n";
            EndDrawing();


            // check if maze pos is equal to 1 (wall)

            if ( maze[ newPos.first ][ newPos.second ] == 1 ) {
                std::cerr << "Hit wall process start\n";
                for ( int i = 0; i < loop.size(); i++ ) {
                    std::cerr << "Hit wall process loop\n";
                    maze[ loop[ i ].first ][ loop[ i ].second ] = 2;

                    /*
                    * set all positions around the path to wall
                    */

                    if ( loop[ i ].first != 0 ) {
                        if ( maze[ loop[ i ].first - 1 ][ loop[ i ].second ] != 2) {
                            maze[ loop[ i ].first - 1 ][ loop[ i ].second ] = 1;
                        }
                    }
                    if ( loop[ i ].first != 0 && loop[ i ].second != 0 ) {
                        if ( maze[ loop[ i ].first - 1 ][ loop[ i ].second - 1 ] != 2 ) {
                            maze[ loop[ i ].first - 1 ][ loop[ i ].second - 1 ] = 1;
                        }
                    }
                    if ( loop[ i ].second != 0 ) {
                        if ( maze[ loop[ i ].first ][ loop[ i ].second - 1 ] != 2 ) {
                            maze[ loop[ i ].first ][ loop[ i ].second - 1 ] = 1;
                        }
                    }
                    if ( loop[ i ].first < size_x && loop[ i ].second != 0 ) {
                        if ( maze[ loop[ i ].first + 1 ][ loop[ i ].second - 1 ] != 2 ) {
                            maze[ loop[ i ].first + 1 ][ loop[ i ].second - 1 ] = 1;
                        }
                    }
                    if ( loop[ i ].first < size_x ) {
                        if ( maze[ loop[ i ].first + 1 ][ loop[ i ].second ] != 2 ) {
                            maze[ loop[ i ].first + 1 ][ loop[ i ].second ] = 1;
                        }
                    }
                    if ( loop[ i ].first < size_x && loop[ i ].second < size_y ) {
                        if ( maze[ loop[ i ].first + 1 ][ loop[ i ].second + 1 ] != 2 ) {
                            maze[ loop[ i ].first + 1 ][ loop[ i ].second + 1 ] = 1;
                        }
                    }
                    if ( loop[ i ].second < size_y ) {
                        if ( maze[ loop[ i ].first ][ loop[ i ].second + 1 ] != 2 ) {
                            maze[ loop[ i ].first ][ loop[ i ].second + 1 ] = 1;
                        }
                    }
                    if ( loop[ i ].first != 0 && loop[ i ].second < size_y ) {
                        if ( maze [ loop[ i ].first - 1 ][ loop[ i ].second + 1 ] != 2 ) {
                            maze[ loop[ i ].first - 1 ][ loop[ i ].second + 1 ] = 1;
                        }
                    }
                } // iter over `loop` and set maze values
                std::cerr << "Hit wall process done\n";
                finished = true;

            }

            /*
            * Random walk part
            */
            while ( true ) { // manual break
                std::cerr << "Bounds checking\n";
                int direction = randRangeDirection( randengine ); // 0-3, 0 is +y, 1 is +x, 2 is -y, 3 is -x (ccw)
                switch ( direction ) {
                case 0:
                    newPos.second++;
                    break;
                case 1:
                    newPos.first++;
                    break;
                case 2:
                    newPos.second--;
                    break;
                case 3:
                    newPos.first--;
                    break;
                }
                /*
                * bounds checking
                */
                if ( newPos.first >= size_x ) {
                    newPos.first--;
                    continue;
                } else if ( newPos.first < 0 ) {
                    newPos.first++;
                    continue;
                } else if ( newPos.second >= size_y ) {
                    newPos.second--;
                    continue;
                } else if ( newPos.second < 0 ) {
                    newPos.second++;
                    continue;
                }
                break;
            }
            /*
            * Loop erased part
            */
            for ( int i = 0; i < loop.size() - 1; i++ ) {
                if ( loop[ i ] == newPos ) { 
                    loop.resize( i + 1 );
                    newPos = loop[ i ];
                }
            }
            
        }
    }
    for ( int i = 0; i < size_x; i++ ) {
        std::cout << "[ ";
        for ( int j = 0; j < size_y; j++ ) {
            std::cout << maze[ i ][ j ] << ", "; 
        }
        std::cout << "]\n";
    }

    while ( !WindowShouldClose() );

    return 0;
}