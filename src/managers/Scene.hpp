#include "../behaviours/MonoBehaviour.hpp"
#include <memory>
#include <stack>

namespace Scene {
	inline SDL_Renderer *renderer = nullptr;
	inline std::stack<std::unique_ptr<MonoBehaviour>> stack;

	void init(SDL_Renderer *_renderer);
	void update();
	void fixed_update();
	void render();
	void event_handler(SDL_Event &event);
};