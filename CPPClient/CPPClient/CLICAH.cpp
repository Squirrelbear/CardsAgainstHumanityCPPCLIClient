#include "CLICAH.h"
#include "StringUtility.h"

CLICAH::CLICAH()
{
	std::string playerName = choosePlayerName();
	_session = std::make_unique<GameSession>(playerName);
}

int CLICAH::playTillEnd()
{
	if (!createPlayerOnServer()) return EXIT_FAILURE;
	if (!createLobby()) return EXIT_FAILURE;
	if (!startGame()) return EXIT_FAILURE;
	if (!playCard()) return EXIT_FAILURE;
	if (!selectWinner()) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

std::string CLICAH::choosePlayerName()
{
	std::string playerName;
	do {
		std::cout << "Enter name: ";
		std::cin >> playerName;
	} while (!isValidName(playerName));
	return playerName;
}

bool CLICAH::createPlayerOnServer()
{
	std::cout << std::endl << "Creating Player..." << std::endl;
	if (!_session->createPlayerOnServer()) {
		return showErrorFailure("Error: failed to create player on server.");
	}
	std::cout << "Player created!" << std::endl;
	return true;
}

bool CLICAH::createLobby()
{
	std::cout << std::endl << "Creating Lobby..." << std::endl;
	if (!_session->createServer()) {
		return showErrorFailure("Error: failed to create server.");
	}
	std::cout << "Lobby created!" << std::endl;
	return true;
}

bool CLICAH::startGame()
{
	std::cout << "Starting Game..." << std::endl;
	if (!_session->startGame()) {
		return showErrorFailure("Error: failed to start game.");
	}
	std::cout << "Game started!" << std::endl;
	std::cout << std::endl;
	return true;
}

bool CLICAH::playCard()
{
	showHandCardOptions(*_session);
	std::cout << "Enter choice number: ";
	std::vector<int> chosenCards;
	for (int i = 1; i <= _session->getRoundData().questionRequiresCount; i++) {
		int cardChoiceInput;
		std::cin >> cardChoiceInput;
		int cardID = _session->getHand().at(cardChoiceInput - 1).first;
		chosenCards.emplace_back(cardID);
	}

	std::cout << std::endl << "Playing card(s)..." << std::endl;
	if (!_session->playCards(chosenCards)) {
		return showErrorFailure("Error: failed to play cards.");
	}
	std::cout << "Card played!" << std::endl;
	return true;
}

bool CLICAH::selectWinner()
{
	showCardsInPlay(*_session);
	std::cout << "Enter winnerID: ";
	int winnerID;
	std::cin >> winnerID;

	std::cout << "Passing winner..." << std::endl;
	if (!_session->chooseWinner(winnerID)) {
		return showErrorFailure("Error: failed to set winner.");
	}
	std::cout << "Winner was " << _session->getRoundData().lastRoundWinner << std::endl;

	std::cin.get();
	std::cin.get();
	return true;
}

bool CLICAH::showErrorFailure(std::string const& message) const {
	std::cout << message << std::endl;
	std::cin.get();
	std::cin.get();
	return false;
}

void CLICAH::showHandCardOptions(GameSession const& gameSession) const {
	std::cout << gameSession.getRoundData().questionText << std::endl;
	auto hand = gameSession.getHand();
	int relativeID = 1;
	for (auto const& card : hand) {
		std::cout << relativeID << ". " << card.second << std::endl;
		++relativeID;
	}
}

void CLICAH::showCardsInPlay(GameSession const& gameSession) const {
	std::string const& question = gameSession.getRoundData().questionText;
	std::cout << question << std::endl << std::endl;

	auto const& playedCards = gameSession.getRoundData().playedCards;
	for (auto const& playersCards : playedCards) {
		std::string combined = question;
		for (auto const& card : playersCards.cards) {
			std::cout << card.second << std::endl;
			std::size_t found = combined.find_first_of("_");
			if (found != std::string::npos) {
				combined.replace(found, 1, card.second);
			}
		}
		std::cout << combined << std::endl << std::endl;
	}
}

bool CLICAH::isValidName(std::string const & playerName) const
{
	std::string name = playerName;
	trim(name);
	if (name.length() < 3 || name.length() > 20) {
		std::cout << "Name must be between 3 and 20 characters with only letters or numbers." << std::endl;
		return false;
	}

	// If not alpha numeric
	for (auto c : name) {
		if (!isalnum(c)) {
			std::cout << "Name must be between 3 and 20 characters with only letters or numbers." << std::endl;
			return false;
		}
	}
	return true;
}
