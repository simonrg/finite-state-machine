#include "World.h"

class Adventure{
private:
	World world;
public:
	World getWorld(){ return world; }
	void setWorld(World new_world){ world = new_world; }

	Adventure(){};
	void Load(std::string);
	void Play();
};