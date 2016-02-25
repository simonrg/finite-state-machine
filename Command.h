#include <vector>

#include "Split.h"

//abstract command class
class ACommand{
protected:
	//all derived classes have a World reference
	World w;
public:
	ACommand(World world){};
	virtual void Process(vector<string>) = 0;

	//used to get the world after its been modified
	virtual World UpdatedWorld(){ return w; };
};


//concrete command classes
class CommandMove : public ACommand{
private:
	Location location;
	map<string, int> edges;
	map<string, int>::iterator it;
	string dir;
	Player p;
public:
	CommandMove(World world) : ACommand(world){ 
		w = world;
		p = w.getPlayer();
		location = w.LocationManager().current_location(p.Position().position_id());
		edges = location.LocationManager().getEdges();
	}
	void Process(vector<string> args){
		//a 'move' command requires a direction
		if (args.size() == 1){
			cout << "Move where??" << endl;
			return;
		}

		//move this way
		dir = args.at(1);

		for (it = edges.begin(); it != edges.end(); it++){
			if (it->first == dir){
				//player pos calculated from edge location id
				p.Position().update_position(it->second);

				w.setPlayer(p);										//update this world's player object
				w.Resources().getLocations().find(it->second);

				//new location, new paths
				location = w.LocationManager().current_location(p.Position().position_id());
				edges = location.LocationManager().getEdges();

				cout << "You moved " << dir << endl;
				return;
			}
		}
		cout << "There are no paths in that direction." << endl;	//single words that aren't in the look up arent a direction
	};
};

class CommandLook : public ACommand{
private:
	Location location;
	map<string, int> edges;
	string object;
	string look_arg;
	Player player;
public:
	CommandLook(World world) : ACommand(world){ 
		w = world;
		player = w.getPlayer();
		location = w.LocationManager().current_location(player.Position().position_id());
		edges = location.LocationManager().getEdges();
	}
	void Process(vector<string> args){
		//look modes

		//LOCATION
		if (args.size() == 1){
			//the location description
			cout << location.Info().Description() << endl;

			//edges
			cout << "There are paths leading:" << endl;
			for (map<string, int>::iterator it = edges.begin(); it != edges.end(); it++){
				cout << "- " << it->first << endl;
			}

			//items
			if (!location.Items().getItems().empty()){
				cout << "You can also see a:" << endl;
				for (size_t i = 0; i < location.Items().getItems().size(); i++){
					Item item = w.Resources().item_location(location, i);
					cout << "- " << item.Info().Name() << endl;
				}
			}
		}
		//ITEM
		else{
			//look in [item]
			//look at [item]
			//the item to look at
			object = args.at(2);
			look_arg = args.at(1);

			//check inventory
			for (size_t k = 0; k < player.Items().getItems().size(); k++){
				Item item = w.Resources().item_player(player, k);

				if (item.Info().Name() == object){
					Collections(item);
					return;
				}
			}
			//check location
			for (size_t i = 0; i < location.Items().getItems().size(); i++){
				Item item = w.Resources().item_location(location, i);

				if (item.Info().Name() == object){
					Collections(item);
					return;
				}
			}
			//not in current location, or in the player inventory
			cout << "I can't find a [" << object << "] anywhere." << endl;
		}
	};
	void Collections(Item item){
		//container
		if (look_arg == "in" && !item.Items().getItems().empty()){
			cout << "The [" << object << "] contains" << endl;

			//item(s) in the item (container)
			for (size_t i = 0; i < item.Items().getItems().size(); i++){
				int idx = item.Items().getItems().at(i);

				Item inside_item = w.Resources().item_at(idx);
				cout << "- " << inside_item.Info().Name() << "; " << inside_item.Info().Description() << endl;
			}
			return;
		}
		else if (look_arg == "in"){
			cout << "There's nothing in the [" << object << "]." << endl;
			return;
		}

		//regular item
		if (look_arg == "at" && (item.Info().Name() == object)){
			cout << "- " << item.Info().Name() << "; " << item.Info().Description() << endl;
			return;
		}
		else if (look_arg == "at"){
			cout << "I can't find a [" << object << "] anywhere." << endl;
			return;
		}
	}
};

class CommandQuit : public ACommand{
private:
	Player p;
public:
	CommandQuit(World world) : ACommand(world){ w = world; }
	void Process(vector<string>){
		cout << "Your adventure has ended without fame or fortune." << endl;
		p.setPlayState(false);
		w.setPlayer(p);
		exit(0);
	}
};

class CommandInventory : public ACommand{
private:
	Player p;
public:
	CommandInventory(World world) : ACommand(world){ 
		w = world;
		p = w.getPlayer();
	}
	void Process(vector<string>){
		if (!w.getPlayer().Items().getItems().empty()){
			cout << "I'm carrying" << endl;
			for (size_t i = 0; i < w.getPlayer().Items().getItems().size(); i++){
				Item item = w.Resources().item_player(p, i);
				cout << "- " << item.Info().Name() << "; " << item.Info().Description() << endl;
			}
		}
		else{
			cout << "My inventory is empty." << endl;
			return;
		}
	}
};

class CommandTake : public ACommand{
private:
	Location location;
	Player p;
	string find_inside;
public:
	CommandTake(World world) : ACommand(world){ 
		w = world;
		p = w.getPlayer();
		location = w.LocationManager().current_location(p.Position().position_id());
	}
	void Process(vector<string> args){
		//the item to take
		string item_name = args.at(1);

		//take [item]
		if (args.size() == 2){
			//iterate through items in the location
			for (size_t i = 0; i < location.Items().getItems().size(); i++){
				//take an item from the current location
				Item item = w.Resources().item_location(location, i);

				//take the item
				if (item.Info().Name() == item_name){
					cout << "I picked up the [" << item.Info().Name() << "]." << endl;

					//add item to player inventory
					p.Items().addItem(item.Info().Id());
					w.setPlayer(p);

					//remove item from location
					location.Items().removeItem(item.Info().Id());

					//update the location
					w.LocationManager().update_location(location.Info().Id(), location);
					return;
				}
			}
			cout << "Take what..?" << endl;
		}
		//take [item] ... [item] (from, in, inside)
		else if (args.size() > 2){
			find_inside = args.at(1);
			string take_arg = args.at(2);
			string container_name = args.at(3);

			if (take_arg != "from" && take_arg != "in" && take_arg != "inside"){
				cout << "Invalid command." << endl;
				return;
			}

			//check container items in location
			for (size_t i = 0; i < location.Items().getItems().size(); i++){
				Item item = w.Resources().item_location(location, i);

				//item is a container
				if (!item.Items().getItems().empty() && container_name == item.Info().Name()){
					Collection(item);
					return;
				}
			}
			//check container items in inventory
			for (size_t i = 0; i < p.Items().getItems().size(); i++){
				Item item = w.Resources().item_player(p, i);

				//item is a container
				if (!item.Items().getItems().empty() && container_name == item.Info().Name()){
					Collection(item);
					return;
				}
			}

			cout << "Theres nothing in there to take." << endl;
		}
	}
	void Collection(Item item){
		for (size_t k = 0; k < item.Items().getItems().size(); k++){
			int id = item.Items().getItems().at(k);
			Item item_inside = w.Resources().item_at(id);

			//found it
			if (item_inside.Info().Name() == find_inside){
				cout << "I took the [" << find_inside << "] from the [" << item.Info().Name() << "]." << endl;

				//add item to player inventory
				p.Items().addItem(id);
				w.setPlayer(p);

				//remove item from item
				item.Items().getItems().erase(item.Items().getItems().begin() + k);

				//update the world item container collection
				w.Resources().getItems().at(item.Info().Id()).Items().getItems().erase(
					w.Resources().getItems().at(item.Info().Id()).Items().getItems().begin() + k);
				return;
			}
		}
	}
};

class CommandPut : public ACommand{
private:
	string drop_item;
	Player p;
	Location location;
public:
	CommandPut(World world) : ACommand(world){
		w = world;
		p = w.getPlayer();
		location = w.LocationManager().current_location(p.Position().position_id());
	}
	void Process(vector<string> args){
		//the item to drop
		drop_item = args.at(1);

		//[drop] item
		if (args.size() == 2){
			//iterate through items in the inventory
			for (size_t i = 0; i < p.Items().getItems().size(); i++){
				Item item = w.Resources().item_player(p, i);

				//drop the item
				if (item.Info().Name() == drop_item){
					cout << "I dropped the [" << drop_item << "]." << endl;

					//remove from player inventory
					p.Items().removeItem(i);
					w.setPlayer(p);

					//return the item to the location
					location.Items().addItem(item.Info().Id());

					//update the location
					w.LocationManager().update_location(location.Info().Id(), location);
					return;
				}
			}
		}
		//put [item] ... [item] (in, inside)
		else if (args.size() > 2){
			string put_arg = args.at(2);
			string insert_name = args.at(1);
			string container_name = args.at(3);

			if (put_arg != "in" && put_arg != "into" && put_arg != "inside"){
				cout << "Invalid command." << endl;
				return;
			}

			//get the Item to insert
			for (size_t k = 0; k < p.Items().getItems().size(); k++){
				Item item_insert = w.Resources().item_player(p, k);

				//find the container
				if (item_insert.Info().Name() == insert_name){
					for (size_t i = 0; i < location.Items().getItems().size(); i++){
						Item item_container = w.Resources().item_location(location, i);

						//found the container
						if (item_container.Info().Name() == container_name){
							cout << "I put the [" << insert_name << "] in the [" << container_name << "]." << endl;

							//put the item in the container
							item_container.Items().getItems().push_back(item_insert.Info().Id());

							//remove from inventory
							p.Items().removeItem(item_insert.Info().Id());
							w.setPlayer(p);

							//update world Item collection data
							w.Resources().getItems().at(item_container.Info().Id()).Items().getItems().push_back(item_insert.Info().Id());
							return;
						}
					}
				}
			}
			//ADD -- put inventory item in inventory item
			cout << "I'm not carrying that." << endl;
			return;
		}
	}
};

//organises commands
class CommandManager : public ACommand{
private:
	map<string, ACommand*> commands;
	map<string, ACommand*>::iterator it;
public:
	//the child initialises the world
	//because acommand is abstract
	CommandManager(World world) : ACommand(world){
		w = world;

		//concrete commands
		commands["move"] = new CommandMove(w);
		commands["go"] = commands["move"];

		commands["look"] = new CommandLook(w);
		commands["examine"] = commands["look"];

		commands["inventory"] = new CommandInventory(w);
		commands["i"] = commands["inventory"];
		commands["inv"] = commands["inventory"];

		commands["take"] = new CommandTake(w);
		commands["pick"] = commands["take"];
		commands["grab"] = commands["take"];
		commands["get"] = commands["take"];

		commands["put"] = new CommandPut(w);
		commands["drop"] = commands["put"];
		commands["remove"] = commands["put"];
		commands["insert"] = commands["put"];

		commands["quit"] = new CommandQuit(w);
		commands["exit"] = commands["quit"];
		commands["q"] = commands["quit"];
		commands["end"] = commands["quit"];
		commands["finish"] = commands["quit"];
	}
	void Process(vector<string> args){
		//each word in the command string
		for (it = commands.begin(); it != commands.end(); it++){
			//a direction is not a predefined command
			//becomes a directional command when move is appended
			if ((args.size() == 1) && (commands.find(args.at(0)) == commands.end())){
				args.insert(args.begin(), "move");
			}

			//is the first word a recognised command?
			if (it->first == args.at(0)){
				commands[args.at(0)]->Process(args);

				//the world passed back to the state will be updated
				w = commands[args.at(0)]->UpdatedWorld();
				return;
			}
			//its empty
			else if (args.empty())
				return;
		}

		cout << "Invalid command." << endl;
		return;
	}
};