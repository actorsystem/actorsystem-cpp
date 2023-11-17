
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

class Actor {
public:
    // the constructor should require a name string
    Actor(const std::string& name) : name_(name) {}
    
    virtual ~Actor() {}

    const std::string& name() const { return name_; }
private:
    std::string name_;

};

int main(int argc, const char* argv[]) {

    Actor actor { "respond_to_linkedin_posts" };

    std::cout << "Create Actor: " << actor.name() << std::endl;

    return 0;
}

