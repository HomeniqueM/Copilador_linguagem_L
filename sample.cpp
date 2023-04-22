#include <iostream>
#include <string>
#include <vector>

class State
{
public:
    virtual void handle(char c) = 0;
    virtual bool isComplete() = 0;
    virtual State *getNextState() = 0;
    virtual std::string getIdentifier() const = 0;
};

class IdentifierState : public State
{
public:
    void handle(char c) override
    {
        std::cout << "IdentifierState" << std::endl;
        if (isalpha(c) || isdigit(c) || c == '_')
        {
            identifier += c;
            completed = false;
        }
        else
        {
            completed = true;
        }
    }
    bool isComplete() override
    {
        return completed;
    }
    State *getNextState() override
    {
        return nullptr;
    }
    std::string getIdentifier() const override
    {
        return identifier;
    }

private:
    std::string identifier;
    bool completed;
};

class StartState : public State
{
public:
    void handle(char c) override
    {
        std::cout << "StartState" << std::endl;
        if (c == '_' || (c >= 'G' && c <= 'Z'))
        {

            identifier += c;
            completed = false;
            nextState = new IdentifierState();
        }
        else if (isdigit(c))
        {

            identifier += c;
            completed = false;
            nextState = new IdentifierState();
        }

        else
        {
            completed = true;
        }
    }
    bool isComplete() override
    {
        return completed;
    }
    State *getNextState() override
    {
        return nextState;
    }
    std::string getIdentifier() const override
    {
        return identifier;
    }

private:
    std::string identifier;
    bool completed;
    State *nextState = nullptr;
};

class Lexer
{
public:
    Lexer() : currentState(new StartState()) {}
    ~Lexer() { delete currentState; }
    void handle(char c)
    {
        currentState->handle(c);
        if (currentState->isComplete())
        {
            addToken();

            State *nextState = currentState->getNextState();
            if (nextState)
            {
                delete currentState;
                currentState = nextState;
            }
            if (currentState->isComplete())
            {
                currentState = new StartState();
            }
        }
    }
    std::vector<std::string> getTokens() const
    {
        return tokens;
    }

private:
    State *currentState;
    std::vector<std::string> tokens;
    void addToken()
    {
        tokens.push_back(currentState->getIdentifier());
    }
};

int main()
{
    Lexer lexer;
    std::string input = "G1234 _hello Zworld 1234";
    for (char c : input)
    {
        std::cout << "input: " << c << std::endl;
        lexer.handle(c);
    }
    for (const auto &token : lexer.getTokens())
    {
        std::cout << "Token: " << token << std::endl;
    }
    return 0;
}
