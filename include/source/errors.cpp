#include "errors.hpp"

std::string out_of_boundary_change ::  what() {
    return std::string("Impossible to change an element out of boundaries\n");
}

std::string font_not_loaded  :: what()  {
            return std::string("Font not found  on the expected reliative path from the executable\n");
}

