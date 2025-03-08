#include <SDL2/SDL.h>
#include <stack>
#include <memory>
#include "../behaviours/MonoBehaviour.hpp"

namespace Scene {
	inline SDL_Renderer* renderer = nullptr;
	inline std::stack<std::unique_ptr<MonoBehaviour>> stack;
	
	void update();
	void fixed_update();
	void render();
	void event_handler(SDL_Event& event);
};