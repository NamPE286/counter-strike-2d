class MonoBehaviour {
	SDL_Renderer* renderer = nullptr;

	MonoBehaviour(SDL_Renderer* renderer);
	void start();
	void update();
	void fixed_update();
};