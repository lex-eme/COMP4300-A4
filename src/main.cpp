#include "imgui.h"
#include "imgui-SFML.h"
#include "GameEngine.h"

#include <SFML/Graphics.hpp>

int main()
{
    GameEngine engine("assets/assets.txt");
    engine.run();
    return 0;
}