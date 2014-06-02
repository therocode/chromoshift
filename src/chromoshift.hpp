#include <fea/structure.hpp>
#include <fea/render2d.hpp>
#include <fea/messaging.hpp>
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
        fea::Renderer2D mRenderer;
        fea::Window mWindow;
        fea::InputHandler mFeaInputHandler;
        InputHandler mInputHandler;
        AudioPlayer mAudioPlayer;

        fea::GameStateMachine mStateMachine;
};
