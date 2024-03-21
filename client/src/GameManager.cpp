#include "GameManager.h"
#include "MyPackets.h"
#include "MyPackets/LobbyInformationPacket.h"
#include "MyPackets/StartGamePacket.h"
#include "MyPackets/TurnPacket.h"

void GameManager::OnPacketReceived(Packet& packet)
{
	if (packet.Type == static_cast<char>(MyPackets::MyPacketType::LobbyInformation))
	{
		auto joinLobbyPacket = *packet.as<MyPackets::LobbyInformationPacket>();

		_lobby.IsHost = joinLobbyPacket.IsHost;
		_lobby.WaitingForOpponent = joinLobbyPacket.WaitingForOpponent;
		_lobby.Player1.Name = joinLobbyPacket.Player1Name;
		_lobby.Player1.IconIndex = joinLobbyPacket.Player1Icon;
		_lobby.Player2.Name = joinLobbyPacket.Player2Name;
		_lobby.Player2.IconIndex = joinLobbyPacket.Player2Icon;
	}
	else if (packet.Type == static_cast<char>(MyPackets::MyPacketType::StartGame))
	{
		auto startGamePacket = *packet.as<MyPackets::StartGamePacket>();

		_lobby.DeckType = startGamePacket.ChosenDeckType;
		_game.Reset(_lobby);
		_game.YourTurn = startGamePacket.YourTurn;
	}
	else if (packet.Type == static_cast<char>(MyPackets::MyPacketType::Turn))
	{
		auto turnPacket = *packet.as<MyPackets::TurnPacket>();

		_game.YourTurn = turnPacket.YourTurn;
	}
}

void GameManager::SetUsername(const std::string& username)
{
	_player.Name = PlayerName(username);
}

void GameManager::JoinLobby()
{
	_lobby.IsHost = true;
	_lobby.WaitingForOpponent = true;
}

MyPackets::JoinLobbyPacket* GameManager::ToJoinLobbyPacket() const
{
	return new MyPackets::JoinLobbyPacket(_player.Name.AsString(), _player.IconIndex);
}

const PlayerData& GameManager::GetPlayer() const
{
	return _player;
}

const Lobby& GameManager::GetLobby() const
{
	return _lobby;
}

const GameData& GameManager::GetGame() const
{
	return _game;
}