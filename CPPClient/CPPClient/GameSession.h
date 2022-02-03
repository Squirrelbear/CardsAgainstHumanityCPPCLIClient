#pragma once
#pragma comment(lib, "urlmon.lib")

#include <urlmon.h>
#include <vector>
#include "json.hpp"

// Stores the name and score data for each player in the session.
struct SessionPlayerData {
	std::string playerName;
	int playerID;
	int score;
};

// Stores the data about cards that have been played during a round.
struct InPlayData {
	int playerID;
	std::vector<std::pair<int, std::string>> cards;
};

// Stores the data about the current round of played 
struct CurrentRoundData {
	int lastRoundWinner;
	std::string questionText;
	int questionRequiresCount;
	std::vector<InPlayData> playedCards;
};

/*
Manages the state of the game session and communicates with the server to retrieve new state informatin.

@author: Peter Mitchell
@version: @2022.1
*/
class GameSession
{
public:
	// Initialises the session with a player name.
	explicit GameSession(std::string const& playerName);
	virtual ~GameSession() = default;

	// Requests a playerAuth from the server using the stored name.
	bool createPlayerOnServer();
	// Requests to join an existing sessionCode on the server.
	bool joinServer(std::string const& sessionCode);
	// Creates the server using this player as the first player on the server.
	bool createServer();
	// Starts the lobby for the server.
	bool startGame();

	// Plays the specified cards by sending the action to the server.
	bool playCards(std::vector<int> const& cardsToPlay);
	// Chooses the winner by sending the action to the server.
	bool chooseWinner(int playerID);

	// Gets this player's current hand.
	std::vector<std::pair<int, std::string>> const& getHand() const;
	// Gets a list of all players in the session.
	std::vector<SessionPlayerData> const& getPlayers() const;
	// Gets the round data about the current field of play.
	CurrentRoundData const& getRoundData() const;

private:
	std::string _playerName;
	std::string _playerAuth;
	std::vector<std::pair<int, std::string>> _hand;
	std::string _sessionCode;
	std::vector<SessionPlayerData> _players;
	CurrentRoundData _roundData;

	std::string _recentError;

	// Sends a request to the server and handles the response by converting it into JSON.
	nlohmann::json getServerResponse(std::vector<std::pair<std::string, std::string>> const& params);
	// Returns true if the response had an error in its JSON data and stores the error in _recentError.
	bool responseHasError(nlohmann::json & response);
	// Updates all the game's session variables using the latest data from the server.
	void updateCachedData(nlohmann::json & data);
};

