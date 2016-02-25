#include <iostream>
#include <string>
#include <conio.h>
using namespace std;

class AGameState;
class Adventure;

class GameStateMachine{
private:
	//states
	AGameState* mainPage;
	AGameState* chooseAdventurePage;
	AGameState* welcomeWorldHeader;
	AGameState* gameplayPage;
	AGameState* helpPage;
	AGameState* aboutPage;
	AGameState* hofPage;
	AGameState* newScorePage;

	AGameState* currentState;

	//adventure reference
	Adventure* adventure;
public:
	GameStateMachine();
	AGameState* getCurrentState() { return this->currentState; }
	void setCurrentState(AGameState* newState) { this->currentState = newState; }

	//core state methods
	void Draw();
	void Input();
	void Update();

	//accessors for every page
	AGameState* getMainPageState(){ return mainPage; }
	AGameState* getChooseAdventurePageState() { return chooseAdventurePage; }
	AGameState* getGameplayPageState(){ return gameplayPage; }
	AGameState* getHelpPageState(){ return helpPage; }
	AGameState* getAboutPageState(){ return aboutPage; }
	AGameState* getHofPageState(){ return hofPage; }
	AGameState* getNewHighScorePageState(){ return newScorePage; }
	AGameState* getWorldWelcomeHeader(){ return welcomeWorldHeader; }

	//reference to the adventure
	Adventure* getAdventure(){ return adventure; }
	void setAdventure(Adventure* newAdv){ adventure = newAdv; }
};