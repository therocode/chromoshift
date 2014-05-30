#include "chromoshift.hpp"
#include "ingame.hpp"

void ChromoShift::setup(const std::vector<std::string>& args)
{
    window.create(sf::VideoMode(800, 600), "The Operation");
    window.setFramerateLimit(60);

    stateMachine.addGameState("ingame", std::unique_ptr<InGameState>(new InGameState(bus)));
    stateMachine.setCurrentState("ingame");
}

void ChromoShift::destroy()
{
    window.close();
}

void ChromoShift::loop()
{
    stateMachine.run();
    window.display();

    if(stateMachine.isFinished())
        quit();
}
