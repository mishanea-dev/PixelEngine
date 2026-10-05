#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

#include <vector>
#include <map>
#include <string>
#include <cstddef>
#include <utility>

using Rect = Rectangle;

namespace
{
    float playerPosX, playerPosY = 0;
    float playerVelocityX, playerVelocityY = 0;
    float playerSpeed = 300;
    int playerFacingDirection = 0;
 
    // Global pointer to the null texture
    const Texture2D* g_nullTexture = nullptr;
    void SetNullTexture(const Texture2D& texture){
        g_nullTexture = &texture;
    }
    const Texture2D* GetNullTexture()    {
        return g_nullTexture;
    }

    struct Sprite
    {
        const Texture2D* texture = nullptr;
        Rectangle source{};

        Sprite() = default;

        explicit Sprite(const Texture2D& texture)
            : texture(&texture), source{0, 0, (float)texture.width, (float)texture.height} {}

        Sprite(const Texture2D& texture, Rectangle rect)
            : texture(&texture), source(rect) {}

        bool isValid() const
        {
            return texture != nullptr && texture->id != 0;
        }
    };

    struct SpriteSheet
    {
        const Texture2D* texture = nullptr;
        std::vector<Rect> sources;

    public:
        // Split the spritesheet in equal amounts
        void extractSprites(const Texture2D& texture, int width, int height)
        {
            this->texture = &texture;
            sources.clear();

            if (width <= 0 || height <= 0)
            {
                TraceLog(LOG_ERROR, "Sprite dimensions must be greater than zero.");
                return;
            }

            const int columns = texture.width / width;
            const int rows = texture.height / height;
            sources.reserve(static_cast<std::size_t>(columns * rows));

            for (int posY = 0; posY + height <= texture.height; posY += height)
            {
                for (int posX = 0; posX + width <= texture.width; posX += width)
                {
                    sources.emplace_back(Rect{
                        static_cast<float>(posX),
                        static_cast<float>(posY),
                        static_cast<float>(width),
                        static_cast<float>(height)});
                }
            }
        }

        // Extract only a defined array of rects;
        void extractSprites(const Texture2D& texture, const std::vector<Rect>& clips)
        {
            this->texture = &texture;
            sources = clips;
        }

        Sprite getSprite(std::size_t index) const
        {
            if (texture == nullptr || index >= sources.size())
                return {};

            return Sprite(*texture, sources[index]);
        }
    };

    class Animation
    {
        std::vector<Sprite> frames;
        std::size_t currFrame = 0;
        float currTime = 0;
        float delay; // seconds between frames

    public:
        explicit Animation(std::vector<Sprite> frames, float delay)
            : frames(std::move(frames)), delay(delay){}

        void Update()
        {
            if (frames.empty() || delay <= 0.0f)
                return;

            currTime += GetFrameTime();
            while (currTime >= delay)
            {
                currTime -= delay;
                currFrame = (currFrame + 1) % frames.size();
            }
        }

        const Sprite* getCurrentFrame() const
        {
            if (frames.empty())
                return nullptr;

            return &frames[currFrame];
        }

        void reset()
        {
            currFrame = 0;
            currTime = 0.0f;
        }
    };
    class Animator{
        std::map<std::string, Animation> animations;
        std::string currentAnimation;
    public:
        void addAnimation(const std::string& name, Animation animation){
            animations.insert_or_assign(name, std::move(animation));
        }

        void setCurrentAnimation(const std::string& name){
            if (animations.find(name) != animations.end() && currentAnimation != name)
            {
                currentAnimation = name;
                animations.at(currentAnimation).reset();
            }
        }

        void update(){
            if (currentAnimation.empty())
                return;

            auto animation = animations.find(currentAnimation);
            if (animation != animations.end())
                animation->second.Update();
        }

        const Sprite* getCurrentFrame() const{
            if(currentAnimation.empty()) return nullptr;
            return animations.at(currentAnimation).getCurrentFrame();
        }
    };

    class TileMap{
        
    };

    class AssetManager
    {
        std::map<std::string, Texture2D> textures;

        void LoadAssets(){
            textures["null"] = LoadTexture("assets/NULL_SPRITE.png");

            textures["House"] = LoadTexture("assets/Sprout Lands/Objects/Free_Chicken_House.png");
            textures["spritesheet_Character"] = LoadTexture("assets/Sprout Lands/Characters/Basic Charakter Spritesheet.png");
        }

        void UnloadAssets(){
            for(auto& texture : textures) UnloadTexture(texture.second);
        }

    public:
        AssetManager(){
            LoadAssets();
        }

        ~AssetManager(){
            UnloadAssets();
        }

        Texture2D& getTexture(const std::string& name){
            return textures.at(name);
        }
    };
    
    struct EngineState
    {
        bool show_debug_window = false;
        bool is_FullScreen = false;
        float clear_color[3] = {32.0f / 255.0f, 36.0f / 255.0f, 46.0f / 255.0f};

        AssetManager assetManager;
    };

    void DrawEditorUi(EngineState& state)
    {
        if(!state.show_debug_window) return;


        ImGui::Begin("PixelEngine");
        ImGui::Text("raylib + Dear ImGui are ready.");
        ImGui::Text("This window is the starting point for your editor tools.");
        ImGui::Separator();
        ImGui::ColorEdit3("Clear color", state.clear_color);
        ImGui::Text("FPS: %d", GetFPS());
        ImGui::End();

        
    }



    void checkInput(EngineState& state){
        if(IsKeyPressed(KEY_F11)){
            ToggleFullscreen();
        }

        if(IsKeyPressed(KEY_GRAVE)){
            state.show_debug_window = !state.show_debug_window;
        }
    }

    void DrawSprite(const Sprite* sprite, const Rectangle& dest)
    {
        const Texture2D* nullTexture = GetNullTexture();
        if (nullTexture == nullptr || nullTexture->id == 0)
        {
            TraceLog(LOG_ERROR, "Cannot draw sprite: null texture has not been initialized.");
            return;
        }

        const bool spriteIsValid = sprite != nullptr && sprite->isValid();
        const Texture2D* texture = spriteIsValid ? sprite->texture : nullTexture;
        const Rectangle source = spriteIsValid
            ? sprite->source
            : Rectangle{0, 0, static_cast<float>(nullTexture->width), static_cast<float>(nullTexture->height)};

        DrawTexturePro(*texture, source, dest, {0, 0}, 0, RAYWHITE);
    }

}



void Start(){
    
}

void Update(float dt, Animator& player_anim){
    playerVelocityX = static_cast<float>(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    playerVelocityY = static_cast<float>(IsKeyDown(KEY_S) - IsKeyDown(KEY_W));

    playerPosX += playerVelocityX * playerSpeed * dt;
    playerPosY += playerVelocityY * playerSpeed * dt;

    // facing direction 0-down 1-up 2-left 3-right
    if(playerVelocityY > 0){
        playerFacingDirection = 0;
    } else
    if(playerVelocityY < 0) {
        playerFacingDirection = 1;
    }
    if(playerVelocityX > 0){
        playerFacingDirection = 3;
    } else
    if(playerVelocityX < 0) {
        playerFacingDirection = 2;
    }

    const bool isPlayerMoving = (playerVelocityX != 0 || playerVelocityY != 0);
    switch (playerFacingDirection) {
        case 0:
            player_anim.setCurrentAnimation(isPlayerMoving ? "walk_down" : "idle_down");
            break;

        case 1:
            player_anim.setCurrentAnimation(isPlayerMoving ? "walk_up" : "idle_up");
            break;

        case 2:
            player_anim.setCurrentAnimation(isPlayerMoving ? "walk_left" : "idle_left");
            break;

        case 3:
            player_anim.setCurrentAnimation(isPlayerMoving ? "walk_right" : "idle_right");
            break;
    }

}

int main()
{
    // Initialization of the window and OpenGL context
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "PixelEngine");
    SetTargetFPS(60);

    // Setup Dear ImGui context
    rlImGuiSetup(true);

    // Initialize engine state and load assets
    EngineState state;
    SetNullTexture(state.assetManager.getTexture("null"));
    

    Sprite house(state.assetManager.getTexture("House"));


    SpriteSheet player_sprites;
    std::vector<Rect> player_sprites_rects;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            player_sprites_rects.emplace_back(Rect{(float)(32*j+16*j+16),(float)(32*i+16*i+16),16.0,16.0});
        }
    }
    player_sprites.extractSprites(state.assetManager.getTexture("spritesheet_Character"), player_sprites_rects);


    // Player animations
    Animation player_idle_down({player_sprites.getSprite(0), player_sprites.getSprite(1)}, 0.3);
    Animation player_walk_down({player_sprites.getSprite(2), player_sprites.getSprite(1), player_sprites.getSprite(3), player_sprites.getSprite(1)}, 0.2);
    Animation player_idle_up({player_sprites.getSprite(4), player_sprites.getSprite(5)}, 0.3);
    Animation player_walk_up({player_sprites.getSprite(6), player_sprites.getSprite(5), player_sprites.getSprite(7), player_sprites.getSprite(5)}, 0.2);
    Animation player_idle_left({player_sprites.getSprite(8), player_sprites.getSprite(9)}, 0.3);
    Animation player_walk_left({player_sprites.getSprite(10), player_sprites.getSprite(9), player_sprites.getSprite(11), player_sprites.getSprite(9)}, 0.2);
    Animation player_idle_right({player_sprites.getSprite(12), player_sprites.getSprite(13)}, 0.3);
    Animation player_walk_right({player_sprites.getSprite(14),player_sprites.getSprite(13), player_sprites.getSprite(15), player_sprites.getSprite(13)}, 0.2);
    
    Animator player_anim;
    player_anim.addAnimation("idle_down", player_idle_down);
    player_anim.addAnimation("walk_down", player_walk_down);
    player_anim.addAnimation("idle_up", player_idle_up);
    player_anim.addAnimation("walk_up", player_walk_up);
    player_anim.addAnimation("idle_left", player_idle_left);
    player_anim.addAnimation("walk_left", player_walk_left);
    player_anim.addAnimation("idle_right", player_idle_right);
    player_anim.addAnimation("walk_right", player_walk_right);
    player_anim.setCurrentAnimation("idle_down");



    Start();
    while (!WindowShouldClose())
    {
        checkInput(state);
        Update(GetFrameTime(), player_anim);

        BeginDrawing();
        const Color clear_color = Color{
            static_cast<unsigned char>(state.clear_color[0] * 255.0f),
            static_cast<unsigned char>(state.clear_color[1] * 255.0f),
            static_cast<unsigned char>(state.clear_color[2] * 255.0f),
            255
        };
        ClearBackground(clear_color);

        player_anim.update();
        DrawSprite(player_anim.getCurrentFrame(), {playerPosX, playerPosY, 100, 100});

        // DrawText("PixelEngine", 32, 32, 32, RAYWHITE);
        // DrawText("Put game and engine code in src/ and grow from this loop.", 32, 78, 20, LIGHTGRAY);
        // DrawTexture(house_sprite.texture,1,1,RAYWHITE);
        // DrawTextureRec(house_sprite.texture,house_sprite.rect,{0, 0}, WHITE);
        // DrawSprite(house, {0, 0, 256, 256});
        // int indx = 0;
        // for(int i = 0; i < 4; i++){
        //     for(int j = 0; j < 4; j++){
        //         DrawSprite(player_sprites.getSprite(indx), {i*64, j*64, (float)64, (float)64});
        //         DrawRectangleLines(i*64, j*64, 64, 64, RED);
        //         indx++;
        //     }
        // }





        rlImGuiBegin();
        DrawEditorUi(state);
        rlImGuiEnd();

        EndDrawing();

    }


    rlImGuiShutdown();
    CloseWindow();


    return 0;
}
