#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp> // << friends

#include <random>
#include <sstream>

#include <SDL2/SDL.h>

using namespace std;
using namespace glm;


class GameObject {
public:
    virtual void tick(int dt) {};
    virtual void render(SDL_Renderer* renderer) {
	auto pts = vector<SDL_Point>();
	for(auto p : vertices)
	    pts.emplace_back( SDL_Point { static_cast<int>(p.x), static_cast<int>(p.y)});
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLines(renderer, pts.data(), pts.size());
    }

    vector<vec2> vertices;
    void applyDelta(vec2 delta) {
	for_each(begin(vertices), end(vertices), [delta] (vec2& o) { o += delta; });
    }
};

class Player : public GameObject  {
public:
    Player() {
	vertices.emplace_back(10,10);
	vertices.emplace_back(50,10);
	vertices.emplace_back(50,30);
	vertices.emplace_back(10,30);
	vertices.emplace_back(10,10);
    }

    

    void rightPressed() {
	applyDelta( vec2 { 10, 0});
    }

    void leftPressed() {
	applyDelta( vec2 { -10, 0});
    }

};

class Block : public GameObject{
public:
    

};



Player p1;
SDL_Renderer *renderer   = nullptr;
SDL_Window   *win        = nullptr;
void startDisplay(int w, int h) { 
    if (0 != SDL_Init(SDL_INIT_EVERYTHING) ) exit(42);
    SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL, &win, &renderer);
    if(nullptr == win || nullptr == renderer) exit(42);
}


void renderPlayers() {
}


int main(int argc, char *argv[]) {

    startDisplay(800, 800);

    SDL_Event e;
    float fps = 10.0;
    auto start_time = SDL_GetTicks();

    int frame = 0;
    while(1) {
	frame++;


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

// check input
	while (SDL_PollEvent(&e)){
	    auto key = e.key.keysym.sym;
	    if (e.type == SDL_QUIT || SDLK_q == key) exit(0);
	    
	    if (SDLK_RIGHT == key) {
		p1.rightPressed();
	    } else if (SDLK_LEFT == key) {
		p1.leftPressed();
	    }
	}

// display
	p1.render(renderer);


// calc FPS
	auto end_time = SDL_GetTicks();
	auto dt = end_time - start_time;
	fps = fps * .9 + .1 * (1000.0 / dt);
	start_time = end_time;

	char buf[512];
	sprintf(buf, "%s (%3d ms, %.2f fps)",
		argv[0], dt, fps);

	SDL_SetWindowTitle(win, buf);
	SDL_RenderPresent(renderer);

    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
    return 0;
}

