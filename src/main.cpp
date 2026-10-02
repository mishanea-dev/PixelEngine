#include "raylib.h"
#include "rlImGui.h"

#include "imgui.h"

namespace
{
    struct EngineState
    {
        bool show_demo_window = false;
        float clear_color[3] = {32.0f / 255.0f, 36.0f / 255.0f, 46.0f / 255.0f};
    };

    void DrawEditorUi(EngineState& state)
    {
        ImGui::Begin("PixelEngine");
        ImGui::Text("raylib + Dear ImGui are ready.");
        ImGui::Text("This window is the starting point for your editor tools.");
        ImGui::Separator();
        ImGui::ColorEdit3("Clear color", state.clear_color);
        ImGui::Checkbox("Show ImGui demo", &state.show_demo_window);
        ImGui::Text("FPS: %d", GetFPS());
        ImGui::End();

        if (state.show_demo_window)
        {
            ImGui::ShowDemoWindow(&state.show_demo_window);
        }
    }
}

int main()
{
    InitWindow(1280, 720, "PixelEngine");
    SetTargetFPS(60);

    rlImGuiSetup(true);

    EngineState state;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        const Color clear_color = Color{
            static_cast<unsigned char>(state.clear_color[0] * 255.0f),
            static_cast<unsigned char>(state.clear_color[1] * 255.0f),
            static_cast<unsigned char>(state.clear_color[2] * 255.0f),
            255
        };
        ClearBackground(clear_color);

        DrawText("PixelEngine", 32, 32, 32, RAYWHITE);
        DrawText("Put game and engine code in src/ and grow from this loop.", 32, 78, 20, LIGHTGRAY);

        rlImGuiBegin();
        DrawEditorUi(state);
        rlImGuiEnd();

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
