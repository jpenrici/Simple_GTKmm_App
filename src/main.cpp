#include "controller.hpp"
#include <iostream>
#include <cstring>

using namespace std;

const string HELP {
    "Usage:\n"
    "      app --gui    Graphical User Interface\n"
    "      app          Console\n"
};

int main(int argc, char** argv)
{   
    Controller controller;
    if (argc > 1) {
        if (strcmp(argv[1], "--gui") == 0) {
            return controller.Gui();
        } else {
            cout << HELP;
            return EXIT_FAILURE;
        }
    }
    return controller.Console();
}