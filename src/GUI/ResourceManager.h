#pragma once

#include "GLCPP.h"

namespace GUI {


class ResourceManager {
public:

    const GL::Program*      GetProgram(int idx);
    const ArrayBuffer*      GetArrayBuffer(int idx);
    const GL::Texture2D*    GetTexture(int idx);
    
private:

};

} // namespace GUI
