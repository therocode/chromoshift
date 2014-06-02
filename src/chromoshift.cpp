#include "chromoshift.hpp"
#include "ingame.hpp"
#include "menu.hpp"

ChromoShift::ChromoShift() :
    mInputHandler(mBus, mWindow),
    mAudioPlayer(mBus)
{
}

void ChromoShift::setup(const std::vector<std::string>& args)
{
    mWindow.create(sf::VideoMode(800, 600), "ChromoShift");
    mWindow.setFramerateLimit(60);

    mStateMachine.addGameState("ingame", std::unique_ptr<InGameState>(new InGameState(mBus, mWindow)));
    mStateMachine.addGameState("menu", std::unique_ptr<MenuState>(new MenuState(mBus, mWindow)));
    mStateMachine.setCurrentState("menu");
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
