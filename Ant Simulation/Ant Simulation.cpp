#include <SFML\Graphics.hpp>
#include"Game.h"

int main()
{
    srand((unsigned)time(NULL));
    Game game;
    game.Run();
    
    return 0;
}
