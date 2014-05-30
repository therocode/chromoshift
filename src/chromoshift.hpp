#include <fea/structure.hpp>
#include <fea/messaging.hpp>

class ChromoShift : public fea::Application
{
    protected:
        virtual void setup(const std::vector<std::string>& args) override;
        virtual void destroy() override; 
        virtual void loop() override;
    private:
        fea::MessageBus bus;

        fea::GameStateMachine stateMachine;
};
