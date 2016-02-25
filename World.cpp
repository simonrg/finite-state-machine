#include <vector>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

#include "World.h"
#include "Split.h"

Split s;


//location methods
Location::Location(vector<string> bits){
	//initialise location fields
	info.entity_info(stoi(bits[1]), bits[2], bits[3]);

	//edge map of this location (e.g. n=1,e=3)
	string dir;
	int loc_id;
	for (string bit : s.split(bits[4], ',')){
		dir = s.split(bit, '=')[0];
		loc_id = stoi(s.split(bit, '=')[1]);
		LocationManager().getEdges()[dir] = loc_id;
	}

	//items in this location
	if (!bits[5].find(' '))
		item_ids.getItems().push_back(stoi(bits[5]));			//only one item
	else{
		for (string itemids : s.split(bits[5], ','))
			item_ids.getItems().push_back(stoi(itemids));		//more than one item
	}
}



//item methods
Item::Item(vector<string> bits){
	//initialise item fields
	info.entity_info(stoi(bits[1]), bits[2], bits[3]);
	if (!bits[4].find(' '))
		items.getItems().push_back(stoi(bits[4]));
	else{
		for (string containeditems : s.split(bits[4], ','))
			items.getItems().push_back(stoi(containeditems));
	}
}