#pragma once
#include "config.h"
#include <iostream>
#include "render/window.h"

namespace Input2
{
    enum Keycode
    {
        SPACE,
        UP,
        DOWN,
        LEFT,
        RIGHT,
        Q,
        E,
        LEFT_CLICK,
        ESCAPE,
        NUM_KEYS
    };

    inline Keycode GetKeyCodeFromGLFW(int32 glfwKey)
    {
        switch (glfwKey)
        {
        case GLFW_KEY_SPACE: return Keycode::SPACE;
        case GLFW_KEY_W: return Keycode::UP;
        case GLFW_KEY_S: return Keycode::DOWN;
        case GLFW_KEY_A: return Keycode::LEFT;
        case GLFW_KEY_D: return Keycode::RIGHT;
        case GLFW_KEY_Q: return Keycode::Q;
        case GLFW_KEY_E: return Keycode::E;
        case GLFW_KEY_ESCAPE: return Keycode::ESCAPE;
        case GLFW_MOUSE_BUTTON_LEFT: return Keycode::LEFT_CLICK;
        default:
            printf("INVALID KEYCODE\n");
            break;
        }
    }

    struct Keyboard
    {
        bool pressed[Keycode::NUM_KEYS];
        bool held[Keycode::NUM_KEYS];
        bool released[Keycode::NUM_KEYS];
    };

    struct Mouse
    {
        float64 xCurrentPos, yCurrentPos, xPrevPos, yPrevPos;
        bool pressed[Keycode::NUM_KEYS];
        bool held[Keycode::NUM_KEYS];
        bool released[Keycode::NUM_KEYS];
    };

    class InputManager
    {
    public:
        static InputManager* Instance()
        {
            static InputManager instance;
            return &instance;
        }

        Keyboard keyboard;
        Mouse mouse;

        void BeginFrame()
        {
            for (int i = 0; i < Keycode::NUM_KEYS; i++)
            {
                keyboard.pressed[i] = false;
                keyboard.released[i] = false;
            }
        }

        void HandleKeyEvent(int32 key, int32 action)
        {
            if (action == GLFW_PRESS)
            {
                keyboard.pressed[GetKeyCodeFromGLFW(key)] = true;
                keyboard.held[GetKeyCodeFromGLFW(key)] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                keyboard.released[GetKeyCodeFromGLFW(key)] = true;
                keyboard.held[GetKeyCodeFromGLFW(key)] = false;
            }
        }

        void HandleMouseEvent(int32 key, int32 action)
        {
            if (action == GLFW_PRESS)
            {
                mouse.pressed[GetKeyCodeFromGLFW(key)] = true;
                mouse.held[GetKeyCodeFromGLFW(key)] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                mouse.released[GetKeyCodeFromGLFW(key)] = true;
                mouse.held[GetKeyCodeFromGLFW(key)] = false;
            }
        }

    private:
        static InputManager* instance;
        InputManager() {}
        ~InputManager() {}
    };
}