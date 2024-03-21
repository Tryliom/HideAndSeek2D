#include "AssetManager.h"
#include "Game.h"
#include "MyPackets.h"
#include "NetworkClientManager.h"
#include "Logger.h"
#include "GameManager.h"

inline static float HEIGHT = 1080.f;
inline static float WIDTH = 1920.f;

int main()
{
	MyPackets::RegisterMyPackets();
	AssetManager::Initialize();

	// Network
	NetworkClientManager _networkClientManager(HOST_NAME, PORT);

	// Set the size of the game
	sf::RenderWindow _window(sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Colorless Memory", sf::Style::Default));

	_window.setVerticalSyncEnabled(true);

	GameManager _gameManager;
	Game _game(_gameManager);

	//TODO: Ask for username and save it to file
	// Get player name
	std::string username = std::getenv("USERNAME") ? std::getenv("USERNAME") : "default";
	if (!username.empty()) {
		username[0] = std::toupper(username[0]);
	}
	_gameManager.SetUsername(username);

	sf::Clock clock;

	while (_window.isOpen())
	{
		sf::Event event{};

		while (_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				_window.close();
				break;
			}

			_game.CheckInputs(event);
		}

		while (Packet* packet = _networkClientManager.PopPacket())
		{
			_gameManager.OnPacketReceived(*packet);
			_game.OnPacketReceived(*packet);

			auto packetTypeValue = packet->Type;

			if (packetTypeValue >= 0 && packetTypeValue <= static_cast<char>(MyPackets::MyPacketType::COUNT))
			{
				delete packet;
			}
		}

		sf::Time elapsed = clock.restart();

		_game.Update(elapsed);

		_window.clear();

		_game.Draw(_window);

		_window.display();
	}

	_networkClientManager.Stop();

	return EXIT_SUCCESS;
}