#include "App.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

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
