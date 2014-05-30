#include <fea/structure.hpp>
#include <fea/messaging.hpp>
#include <SFML/Window.hpp>
#include "inputhandler.hpp"

class ChromoShift : public fea::Application
{
    public:
        ChromoShift();
    protected:
        virtual void setup(const std::vector<std::string>& args) override;
        virtual void destroy() override; 
        virtual void loop() override;
    private:
        fea::MessageBus bus;
        sf::Window window;
        InputHandler inputHandler;

        fea::GameStateMachine stateMachine;
};
