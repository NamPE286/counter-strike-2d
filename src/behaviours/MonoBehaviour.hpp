class MonoBehaviour {
	SDL_Renderer* const renderer = nullptr;

	MonoBehaviour(SDL_Renderer* const renderer);
	void start();
	void update();
	void fixed_update();
};