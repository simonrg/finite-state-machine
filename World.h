#include <map>
#include <vector>

#include "Component.h"


class Location{
private:
	MetaComponent info;
	CollectionComponent item_ids;
	PositionComponent position;
	LocationManagerComponent l;
public:
	Location(){};
	Location(vector<string>);

	MetaComponent& Info(){ return info; }
	CollectionComponent& Items(){ return item_ids; }
	PositionComponent& Position(){ return position; }
	LocationManagerComponent& LocationManager(){ return l; }
};


class Item{
private:
	MetaComponent info;
	CollectionComponent items;
public:
	Item(){};
	Item(vector<string>);

	MetaComponent& Info(){ return info; }
	CollectionComponent& Items(){ return items; }
};


class Player{
private:
	HealthComponent health;
	PositionComponent position;
	CollectionComponent items;
	bool playing = true;
public:
	HealthComponent& Health(){ return health; }
	PositionComponent& Position(){ return position; }
	CollectionComponent& Items(){ return items; }

	//information specific to a player object
	bool getPlayState(){ return playing; }
	void setPlayState(bool state){ playing = state; }
};


class World{
private:
	Player player = Player();

	MetaComponent info;
	WorldResourcesComponent w;
	LocationManagerComponent lm;
public:
	MetaComponent& Info(){ return info; }
	WorldResourcesComponent& Resources(){ return w; }
	LocationManagerComponent& LocationManager(){ return lm; }

	//information specific to a world object
	void setPlayer(Player player_instance){ player = player_instance; }
	Player getPlayer(){ return player; }
};