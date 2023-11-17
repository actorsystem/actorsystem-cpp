# AMQP Actor System

This C++ application lays the foundation for an Actor System based on the Advanced Message Queuing Protocol (AMQP) using the rabbitmq-c library. The Actor System is designed to provide a scalable and distributed architecture for building resilient and responsive applications.

## Getting Started

### Prerequisites

Ensure that you have the following dependencies installed on your system:

- [Boost C++ Libraries](https://www.boost.org/)
- [rabbitmq-c](https://github.com/alanxz/rabbitmq-c)

### Building the Application

1. Clone the repository:

   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

2. Build the application using CMake:

   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

### Running the Application

Run the built executable to create an Actor:

```bash
./cpp_actor_system
```

## Actor Class

The `Actor` class represents an actor within the system. Each actor has a unique name and encapsulates the logic and state associated with its role.

### Constructor

```cpp
Actor(const std::string& name);
```

Creates an actor with the specified name.

### Public Methods

#### `const std::string& name() const`

Returns the name of the actor.

## Example Usage

```cpp
int main(int argc, const char* argv[]) {
    // Create an actor named "respond_to_linkedin_posts"
    Actor actor { "respond_to_linkedin_posts" };

    // Print the actor's name
    std::cout << "Create Actor: " << actor.name() << std::endl;

    return 0;
}
```

## Future Development

The goal of this project is to evolve into a comprehensive AMQP-based Actor System. Future features include:

- **Actor Library**: A reusable C++ library providing abstractions for actors, message handling, and communication.
  
- **Daemon Server**: A server application that manages the lifecycle of actors, handles message routing, and ensures fault tolerance.

Stay tuned for updates as the project progresses toward a full-fledged Actor System. Your feedback and contributions are highly appreciated!