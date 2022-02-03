#pragma once
#include "GameSession.h"
#include <iostream>

/*
Manages the CLI representation of the Cards Against Humanity CLI game.

@author: Peter Mitchell
@version 2022.1
*/
class CLICAH
{
public:
	// Initialises the CLI application ready for playTillEnd() by selecting a player name and initialising the session.
	CLICAH();
	virtual ~CLICAH() = default;

	// Manages the game loop until the end of game. Returns EXIT_SUCCESS or EXIT_FAILURE.
	int playTillEnd();

private:
	// Reference to the session managed by this client.
	std::unique_ptr<GameSession> _session;

	// Used to loop until a valid player name entered and returned.
	std::string choosePlayerName();
	// Uses the chosen player name to get an authentication code unique for this client.
	bool createPlayerOnServer();
	// Creates a lobby for others to join.
	bool createLobby();
	// Starts the game with the players currently in the lobby. (only usable by host).
	bool startGame();
	// Enters card selection to choose what card to play for the current round.
	bool playCard();
	// Enters selection to choose the winner at the end of a round.
	bool selectWinner();

	// Shows an error message and returns false use for showing an error in the return.
	bool showErrorFailure(std::string const& message) const;
	// Shows all the cards available with numbers next to them that a player can currently play.
	void showHandCardOptions(GameSession const& gameSession) const;
	// Shows all the cards that have been played.
	void showCardsInPlay(GameSession const& gameSession) const;
	// Returns true if name is all alphanumeric, between 3 to 20 characters with trim applied.
	bool isValidName(std::string const& playerName) const;
};

