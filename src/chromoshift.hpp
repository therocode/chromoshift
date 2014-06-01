#include <fea/structure.hpp>
#include <fea/messaging.hpp>
#include <SFML/Graphics.hpp>
#include "inputhandler.hpp"
#include "audioplayer.hpp"

class ChromoShift : public fea::Application
{
    public:
        ChromoShift();
    protected:
        virtual void setup(const std::vector<std::string>& args) override;
        virtual void destroy() override; 
        virtual void loop() override;
    private:
        fea::MessageBus mBus;
        sf::RenderWindow mWindow;
        InputHandler mInputHandler;
        AudioPlayer mAudioPlayer;

        fea::GameStateMachine mStateMachine;
};
