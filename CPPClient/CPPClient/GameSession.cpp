#include "GameSession.h"
#include "CBindStatusCallback.h"

GameSession::GameSession(std::string const & playerName)
	: _playerName(playerName)
{

}

bool GameSession::createPlayerOnServer()
{
	std::vector< std::pair<std::string, std::string>> queryData{
		std::make_pair("action", "createPlayer"),
		std::make_pair("playerName", _playerName)
	};
	auto data = getServerResponse(queryData);
	if (responseHasError(data)) {
		return false;
	}

	_playerAuth = data["playerAuth"];
	return true;
}

bool GameSession::joinServer(std::string const & sessionCode)
{
	_sessionCode = sessionCode;
	// TODO
	return true;
}

bool GameSession::createServer()
{
	std::vector< std::pair<std::string, std::string>> queryData{
		std::make_pair("action", "createServer"),
		std::make_pair("playerAuth", _playerAuth)
	};
	auto data = getServerResponse(queryData);
	if (responseHasError(data)) {
		return false;
	}

	_sessionCode = data["sessionCode"];
	return true;
}

bool GameSession::startGame()
{
	std::vector< std::pair<std::string, std::string>> queryData{
		std::make_pair("action", "startGame"),
		std::make_pair("playerAuth", _playerAuth), 
		std::make_pair("sessionCode", _sessionCode)
	};
	auto data = getServerResponse(queryData);
	if (responseHasError(data)) {
		return false;
	}

	updateCachedData(data);

	return true;
}

bool GameSession::playCards(std::vector<int> const & cardsToPlay)
{
	std::vector< std::pair<std::string, std::string>> queryData{ 
		std::make_pair("action", "playCards"), 
		std::make_pair("playerAuth", _playerAuth), 
		std::make_pair("sessionCode", _sessionCode) 
	};
	for (int i = 0; i < cardsToPlay.size(); ++i) {
		queryData.emplace_back(std::make_pair("playCard" + std::to_string(i + 1), std::to_string(cardsToPlay.at(i))));
	}
	auto data = getServerResponse(queryData);
	if (responseHasError(data)) {
		return false;
	}

	updateCachedData(data);
	return true;
}

bool GameSession::chooseWinner(int playerID)
{
	std::vector< std::pair<std::string, std::string>> queryData{
		std::make_pair("action", "chooseWinner"),
		std::make_pair("playerAuth", _playerAuth),
		std::make_pair("sessionCode", _sessionCode),
		std::make_pair("winnerID",std::to_string(playerID)) 
	};
	auto data = getServerResponse(queryData);
	if (responseHasError(data)) {
		return false;
	}

	updateCachedData(data);
	return true;
}

std::vector<std::pair<int, std::string>> const & GameSession::getHand() const
{
	return _hand;
}

std::vector<SessionPlayerData> const & GameSession::getPlayers() const
{
	return _players;
}

CurrentRoundData const & GameSession::getRoundData() const
{
	return _roundData;
}

nlohmann::json GameSession::getServerResponse(std::vector<std::pair<std::string, std::string>> const & params)
{
	std::string url = "http://localhost:3000";
	if (!params.empty()) {
		url += "?" + params.at(0).first + "=" + params.at(0).second;
	}
	for (size_t i = 1; i < params.size(); i++) {
		url += "&" + params.at(i).first + "=" + params.at(i).second;
	}

	IStream* stream = nullptr;
	CBindStatusCallback callback;
	HRESULT result = URLOpenBlockingStream(nullptr, url.c_str(), &stream, 0, &callback);
	if (result != 0)
	{
		return nlohmann::json::parse("{ \"error\" : \"Failed connection.\"}");
	}
	char buffer[100];
	unsigned long bytesRead;
	std::stringstream ss;
	stream->Read(buffer, 100, &bytesRead);
	while (bytesRead > 0U)
	{
		ss.write(buffer, (long long)bytesRead);
		stream->Read(buffer, 100, &bytesRead);
	}
	stream->Release();
	std::string resultString = ss.str();
	return nlohmann::json::parse(resultString);
}

bool GameSession::responseHasError(nlohmann::json & response)
{
	if (response.contains("error")) {
		_recentError = response["error"];
		return true;
	}
	return false;
}

void GameSession::updateCachedData(nlohmann::json & data)
{
	auto currentRound = data["currentRound"];
	_roundData.questionText = currentRound["question"]["text"];
	_roundData.questionRequiresCount = currentRound["question"]["numAnswers"];
	if (currentRound.contains("lastRoundWinner")) {
		_roundData.lastRoundWinner = currentRound["lastRoundWinner"];
	}

	auto playedCards = currentRound["playedCards"];
	_roundData.playedCards.clear();
	for (auto const& playersPlayedCards : playedCards) {
		std::vector<std::pair<int, std::string>> cards;
		for (auto const& card : playersPlayedCards["played"]) {
			cards.emplace_back(std::make_pair(card["id"], card["text"]));
		}
		_roundData.playedCards.emplace_back(InPlayData{ playersPlayedCards["playerID"], cards });
	}

	auto cards = data["hand"];
	_hand.clear();
	for (auto const& card : cards) {
		_hand.emplace_back(std::make_pair(card["id"], card["text"]));
	}

	// TODO Players
}
