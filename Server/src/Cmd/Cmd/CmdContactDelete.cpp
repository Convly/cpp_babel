/*
** CmdContactDelete.cpp for Server in /home/enguerrand/delivery/Server/CmdContactDelete.cpp
**
** Made by Enguerrand Allamel
** Login   <enguerrand.allamel@epitech.eu>
**
** Started on  mer. sept. 27 09:38:01 2017 Enguerrand Allamel
** Last update mer. sept. 27 09:38:01 2017 Enguerrand Allamel
*/

#include "CmdContactDelete.hpp"
#include "Server.hpp"


babel::CmdContactDelete::CmdContactDelete(babel::Server &server):
	_server(server)
{
}

babel::CmdContactDelete::~CmdContactDelete()
{
}

bool babel::CmdContactDelete::run(size_t tunnelId, babel::NetworkData data)
{
  std::string login(data.data.begin(), data.data.begin() + 31);

  std::cout << "Login: " << login << std::endl;

  try
    {
      this->_server.getAccountManager().getAccountByLogin(login);
      //Todo: Check get Tunnel info throw
      this->_server.getAccountManager().removeContact(
	      this->_server.getNetworkManager().get()->getTunnelInfoByTunnelId(tunnelId).login,
	      login);
      this->_server.getNetworkManager().get()->write(tunnelId, NetworkData(42, 0, {}));
      return true;
    }
  catch (AccountManagerException & e)
    {
      this->_server.getNetworkManager().get()->write(tunnelId, NetworkData(505, 0, {}));
      return false;
    }
}