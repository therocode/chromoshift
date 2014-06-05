#include "chromoshift.hpp"
#include "ingame.hpp"
#include "menu.hpp"
#include <fea/ui/sdlwindowbackend.hpp>
#include <fea/ui/sdlinputbackend.hpp>

ChromoShift::ChromoShift() :
    mRenderer(fea::Viewport({800, 600}, {0, 0}, fea::Camera({400.0f, 300.0f}))),
    mWindow(new fea::SDLWindowBackend()),
    mFeaInputHandler(new fea::SDLInputBackend()),
    mInputHandler(mBus, mFeaInputHandler),
    mAudioPlayer(mBus)
{
}

void ChromoShift::setup(const std::vector<std::string>& args)
{
    mWindow.create(fea::VideoMode(800, 600), "ChromoShift");
    mRenderer.setup();
    mWindow.setFramerateLimit(60);

    mStateMachine.addGameState("ingame", std::unique_ptr<InGameState>(new InGameState(mBus, mRenderer)));
    mStateMachine.addGameState("menu", std::unique_ptr<MenuState>(new MenuState(mBus, mRenderer)));
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
    mWindow.swapBuffers();

    if(mStateMachine.isFinished())
        quit();
}
