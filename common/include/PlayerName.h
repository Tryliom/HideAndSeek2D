#pragma once

#include <SFML/Network.hpp>

#include <array>
#include <string>

class PlayerName
{
public:
	PlayerName() = default;
	explicit PlayerName(std::string_view name) : Name{}
	{
		std::copy(name.begin(), name.end(), Name.begin());
	}

	[[nodiscard]] std::string AsString() const
	{
		return Name.data();
	}

	std::array<char, 10> Name{};
};

sf::Packet& operator <<(sf::Packet& packet, const PlayerName& playerName);
sf::Packet& operator >>(sf::Packet& packet, PlayerName& playerName);