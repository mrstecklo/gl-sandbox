#pragma once

#include <GLFW/glfw3.h>
#include "Noncopyable.h"

namespace GLFW {

class Scope : private Util::Nonmovable {
public:
    Scope();
    ~Scope();
private:
    static bool instantiated;
};

} // namespace GLFW