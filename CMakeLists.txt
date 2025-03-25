cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (MSVC AND WIN32 AND NOT MSVC_VERSION VERSION_LESS 142)
    target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:/ZI>)
endif()

project (main)
add_executable (main "src/main.cpp"  "src/game/Game.cpp" "src/common.h" "src/behaviours/MonoBehaviour.cpp" "src/gameObjects/Player.cpp" "src/scenes/GameScene.cpp"  "src/managers/Time.cpp" "src/geometry/Vec2.cpp" "src/gameObjects/Pointer.cpp" "src/gameObjects/Weapon.cpp" "src/gameObjects/Bullet.cpp" "src/utilities/Utils.cpp" "src/managers/Audio.cpp" "src/managers/Scene.cpp" "src/managers/Font.cpp" "src/ui/Text.cpp" "src/ui/HUD.cpp" "src/cameras/PlayerCamera.cpp" "src/ui/Scoreboard.cpp" "src/ui/MiniScoreboard.cpp" "src/game/Match.cpp" "src/game/Map.cpp" "src/managers/Mouse.cpp" "src/ui/Alert.cpp" "src/ui/Announcer.cpp")
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET main PROPERTY CXX_STANDARD 20)
endif()

include(${CMAKE_TOOLCHAIN_FILE})

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(tmx CONFIG REQUIRED)

target_link_libraries(main
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
    tmx
)