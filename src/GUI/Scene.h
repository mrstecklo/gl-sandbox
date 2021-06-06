#pragma once

#include "MainWindowFwd.h"
#include "Camera.h"
#include "Model.h"
#include "Util/Point.h"
#include "GLFW/Input.h"
#include <functional>
#include <chrono>

namespace GUI {

class Scene {
public:
    using ObjectCb = std::function<void(const Object&)>;
    using ModelCb = std::function<void(const Model&)>;
    using KeyCb = std::function<GLFW::KeyState (int)>;

    void Init(const MainWindow* w) { window = w, InitImpl(); }
    void Resize(int width, int height) { ResizeImpl(width, height); }
    void ProcessInput(const Util::PointD& cursor, std::chrono::microseconds timeSinceLastFrame) { ProcessInputImpl(cursor, timeSinceLastFrame); }

    const Camera& GetCamera() const { return GetCameraImpl(); }
    void ForEachObject(ObjectCb cb) { ForEachModel(cb); }
    void ForEachModel(ModelCb cb) { ForEachModelImpl(cb); }

    GLFW::CursorInputModeValue GetMouseMode() const { return mouseMode; }

    Scene() = default;
    virtual ~Scene() = default;

protected:
    
    void SetMouseMode(GLFW::CursorInputModeValue m) { mouseMode = m; }
    const MainWindow* GetWindow() const { return window; }

private:
    virtual void InitImpl() = 0;
    virtual void ResizeImpl(int width, int height) = 0;
    virtual void ProcessInputImpl(const Util::PointD& cursor, std::chrono::microseconds timeSinceLastFrame) = 0;

    virtual const Camera& GetCameraImpl() const = 0;
    virtual void ForEachModelImpl(ModelCb cb) = 0;

    const MainWindow* window = nullptr;
    GLFW::CursorInputModeValue mouseMode = GLFW::CURSOR_NORMAL;
};

} // namespace GUI
