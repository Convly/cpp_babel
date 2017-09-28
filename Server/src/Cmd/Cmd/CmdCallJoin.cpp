/*
** CmdCallJoin.cpp for Server in /home/enguerrand/delivery/Server/CmdCallJoin.cpp
**
** Made by Enguerrand Allamel
** Login   <enguerrand.allamel@epitech.eu>
**
** Started on  mer. sept. 27 16:43:05 2017 Enguerrand Allamel
** Last update mer. sept. 27 16:43:05 2017 Enguerrand Allamel
*/

#include "CmdCallJoin.hpp"
#include "Server.hpp"


babel::CmdCallJoin::CmdCallJoin(babel::Server &server):
	_server(server)
{
}

babel::CmdCallJoin::~CmdCallJoin()
{

}

bool babel::CmdCallJoin::run(size_t tunnelId, babel::NetworkData data)
{
  NetworkDataCSJoin networkDataCSJoin;
  std::copy_n(reinterpret_cast<const char *>(&data.data), sizeof(CLIENT_CALL_STRUCT), reinterpret_cast<char *>(&networkDataCSJoin));

  if (networkDataCSJoin.idCall != 0)
    {
      if (!this->_server.getCallManager().convIsExist(networkDataCSJoin.idCall) &&
	      !this->_server.getCallManager().isAllowToJoin(networkDataCSJoin.idCall,
							    this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).login))
	return false;

      CLIENT_CALL_STRUCT client;
      NetworkDataCSJoinSuccess networkDataCSJoinSuccess;
      networkDataCSJoinSuccess.idCall = networkDataCSJoin.idCall;

      auto participant = (*this->_server.getCallManager().getCalls().find(networkDataCSJoin.idCall)).second.getParticipants();
      auto it = participant.begin();
      for (auto i = 0 ; i < 8 ; i++)
	{
	  if (it != participant.end())
	    {
	      (*it).second.login.copy(networkDataCSJoinSuccess.client[i].login, 32);
	      (*it).second.ip.copy(networkDataCSJoinSuccess.client[i].ip, 15);
	      networkDataCSJoinSuccess.client[i].port = (*it).second.port;
	      it++;
	    }
	}

      std::array<char, 2048> dataSend;
      std::copy_n(reinterpret_cast<const char *>(&networkDataCSJoinSuccess), sizeof(NetworkDataCSJoinSuccess), dataSend.begin());

      this->_server.getNetworkManager().get()->write(tunnelId, NetworkData(9, sizeof(NetworkDataCSJoinSuccess), dataSend));

      NetworkDataSCJoin networkDataSCJoin;
      this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).login.copy(client.login, 32);
      this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).ip.copy(client.ip, 15);
      client.port = networkDataCSJoin.port;

      networkDataSCJoin.idCall = networkDataCSJoin.idCall;
      networkDataSCJoin.client = client;

      dataSend.fill(0);
      std::copy_n(reinterpret_cast<const char *>(&networkDataSCJoin), sizeof(NetworkDataSCJoin), dataSend.begin());

      for (auto it = participant.begin() ; it != participant.end() ; it++)
	{
	  if ((*it).second.login.compare(this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).login) != 0)
	    this->_server.getNetworkManager().get()->write((*it).first, NetworkData(9, sizeof(NetworkDataSCJoin), dataSend));
	}
      return true;
    }
  else
    {
      auto call = this->_server.getCallManager().create();
      std::array<char, 2048> dataSend;

      NetworkDataCSJoinSuccess networkDataCSJoinSuccess;

      networkDataCSJoinSuccess.idCall = call.getId();

      std::copy_n(reinterpret_cast<const char *>(&networkDataCSJoinSuccess), sizeof(NetworkDataCSJoinSuccess), dataSend.begin());
      this->_server.getNetworkManager().get()->write(tunnelId, NetworkData(9, sizeof(NetworkDataCSJoinSuccess), dataSend));
      return true;
    }
}