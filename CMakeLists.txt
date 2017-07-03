cmake_minimum_required(VERSION 3.6)
project(ColdSpace)

set(CMAKE_CXX_STANDARD 11)

set (CMAKE_MODULE_PATH "D:\\Program Files\\JetBrains\\SFML-2.4.2\\cmake\\Modules")
find_package(SFML 2.4 COMPONENTS system window graphics audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

set(SOURCE_FILES main.cpp BaseState.h EventManager.h EventManager.cpp Game.h Game.cpp
        keys.cfg StateManager.h StateManager.cpp SharedContext.h State_Game.h State_Game.cpp
        State_Intro.h State_Intro.cpp State_MainMenu.h State_MainMenu.cpp State_Paused.cpp State_Paused.h Window.h
        Window.cpp EnumHasher.h  Utilities.h ResourceManager.h TextureManager.h Textures.cfg
        SpriteSheet.cpp SpriteSheet.h Anim_Base.cpp Anim_Base.h Anim_Directional.cpp Anim_Directional.h
        Direction.h shlwapi.lib tiles.cfg  Map.cpp Map.h Entity_Manager.cpp
        Entity_Manager.h media DebugOverlay.h State_GameOver.cpp
        State_GameOver.h ECS_Types.h C_Base.h C_Position.h Bitmask.h C_Drawable.h C_SpriteSheet.h Event_Queue.h
        EntityEvents.h System_Manager.cpp System_Manager.h S_Base.cpp S_Base.h Observer.h Message.h Communicator.h
        MessageHandler.h EntityMessages.h S_Renderer.cpp S_Renderer.h C_Movable.h S_Movement.cpp S_Movement.h S_Control.cpp
        S_Control.h S_Collision.cpp S_Collision.h S_SheetAnimation.cpp S_SheetAnimation.h S_State.cpp S_State.h C_State.h
        C_Controller.h C_Collidable.h GUI_Style.h FontManager.h GUI_Element.cpp GUI_Element.h GUI_Event.h GUI_Interface.cpp
        GUI_Interface.h fonts.cfg GUI_Manager.cpp GUI_Manager.h GUI_Scrollbar.cpp GUI_Scrollbar.h GUI_Textfield.cpp GUI_Textfield.h GUI_Label.cpp GUI_Label.h)

add_executable(ColdSpace ${SOURCE_FILES})
target_link_libraries(ColdSpace ${SFML_LIBRARIES} shlwapi.lib)
