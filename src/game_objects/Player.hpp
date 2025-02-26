#include <SDL2/SDL.h>
#include "../behaviours/MonoBehaviour.hpp"

class Player : private MonoBehaviour {
public:
	using MonoBehaviour::MonoBehaviour
};