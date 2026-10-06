#include "App.hpp"

#include <iostream>
#include <exception>

int main()
{
    App app;

    try {
        app.Run();
    } catch (const std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
