#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "World.h"
#include "Split.h"

void MetaComponent::entity_info(int e_id, string e_name, string e_description){
	id = e_id;
	name = e_name;
	description = e_description;
}



int PositionComponent::locationid = 0;	//updated for all objects
void PositionComponent::update_position(int new_id){
	locationid = new_id;
}



LocationManagerComponent::LocationManagerComponent(map<int, Location> worldlocations){
	locations = worldlocations;
	Location current = locations.at(p.position_id());
};
Location LocationManagerComponent::current_location(int id){
	return locations.at(id);
}
void LocationManagerComponent::update_location(int location_id, Location newlocationinfo){
	map<int, Location>::iterator it;

	for (it = locations.begin(); it != locations.end(); it++){
		if (location_id == it->first){
			locations.erase(location_id);
			locations.emplace(location_id, newlocationinfo);
			return;
		}
	}
}



void HealthComponent::change_health(int affect){
	if (affect >= 0)
		health += affect;
	else if (affect < 0)
		health -= abs(affect);
}



void CollectionComponent::addItem(int id){
	//updates inventory collection -- add or remove
	//remove if already have item or add it if don't
	for (size_t k = 0; k < collection.size(); k++) {
		if (collection[k] == id){
			collection.erase(collection.begin() + k);
			return;
		}
	}
	collection.push_back(id);
}
void CollectionComponent::removeItem(int id){
	for (size_t i = 0; i < collection.size(); i++){
		//find and remove it
		if (collection.at(i) == id){
			collection.erase(collection.begin() + i);
			return;
		}
	}
}



void WorldResourcesComponent::add_locations(string locationstr){
	Split s;
	Location loc = Location(s.split(locationstr, '|'));
	locations.emplace(loc.Info().Id(), loc);
}
void WorldResourcesComponent::add_items(string itemstr){
	Split s;
	Item i = Item(s.split(itemstr, '|'));
	items.emplace(i.Info().Id(), i);
}
Item WorldResourcesComponent::item_location(Location l, int id){
	//from the ids in the location
	int item_id = l.Items().getItems().at(id);

	return items[item_id];
}
Item WorldResourcesComponent::item_player(Player p, int id){
	//from ids in the player collection
	int item_id = p.Items().getItems().at(id);

	return items[item_id];
}
Item WorldResourcesComponent::item_at(int id){
	return items[id];
}