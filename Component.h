#include <vector>

class Item;
class Player;
class Location;


//component objects

//stores and handles meta data of an entity
//description info for any type e.g. a location, an item
class MetaComponent{
private:
	int id;
	string name;
	string description;
public:
	int Id(){ return id; }
	string Name(){ return name; }
	string Description(){ return description; }

	void entity_info(int id, string name, string description);
};

//stores and handles the entity's health
class HealthComponent{
private:
	int health;
public:
	HealthComponent(){ health = 100; }

	int health_status(){ return health; }

	//add or subtract health from total pool
	void change_health(int new_hp);
};

//stores and handles the entity position
//edges, items for a specific area
class PositionComponent{
private:
	static int locationid;
public:
	PositionComponent(){}

	int position_id(){ return locationid; }

	void update_position(int new_id);
};

//stores a collection of items
//handles adding/removing 'item ids' from an object collection
//e.g. a player, an enemy, a container, a location
class CollectionComponent{
private:
	vector<int> collection;
public:
	CollectionComponent(){}

	vector<int>& getItems(){ return collection; }

	void addItem(int id);
	void removeItem(int id);
};

//maintains collections of all world related stuff
//manages all Item and Location information
class WorldResourcesComponent{
private:
	map<int, Location> locations;
	map<int, Item> items;
public:
	WorldResourcesComponent(){};

	map<int, Location> getLocations(){ return locations; }
	map<int, Item>& getItems(){ return items; }

	Item item_location(Location l, int id);
	Item item_player(Player p, int id);
	Item item_at(int id);

	void add_locations(string filestring);
	void add_items(string filestring);
};

//manages location information
//e.g. edges, adding/removing item ids, updating location state
class LocationManagerComponent{
private:
	map<int, Location> locations;
	map<string, int> edges;
	PositionComponent p;
public:
	LocationManagerComponent(){};
	LocationManagerComponent(map<int, Location> worldlocations);

	map<string, int>& getEdges(){ return edges; }

	Location current_location(int id);
	void update_location(int, Location);
};