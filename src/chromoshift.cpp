#include "chromoshift.hpp"
#include "ingame.hpp"

ChromoShift::ChromoShift() :
    mInputHandler(mBus, mWindow)
{
}

void ChromoShift::setup(const std::vector<std::string>& args)
{
    mWindow.create(sf::VideoMode(800, 600), "ChromoShift");
    mWindow.setFramerateLimit(60);

    mStateMachine.addGameState("ingame", std::unique_ptr<InGameState>(new InGameState(mBus, mWindow)));
    mStateMachine.setCurrentState("ingame");
}

void ChromoShift::destroy()
{
    mWindow.close();
}

void ChromoShift::loop()
{
    mInputHandler.process();
    mStateMachine.run();
    mWindow.display();

    if(mStateMachine.isFinished())
        quit();
}
