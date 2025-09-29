/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbutel <magikh@posteo.net>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:14:31 by lbutel            #+#    #+#             */
/*   Updated: 2025/09/19 11:18:26 by lbutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string &name, Client &owner) : _name(name), _owner(owner) {}

Channel::~Channel(void) {}

bool Channel::isValidName(const std::string channel_name)
{
	// # is for global (federated) channels, & is for local channels
	if (channel_name[0] != '&')
		return (false);
	if (channel_name.find(' ') != std::string::npos)
		return (false);
	if (channel_name.find('\a') != std::string::npos)
		return (false);
	if (channel_name.find(',') != std::string::npos)
		return (false);
	return (true);
}

const std::string &Channel::getName(void)
{
	return (this->_name);
}

Client &Channel::getOwner(void)
{
	return (this->_owner);
}

size_t Channel::getMemberCount(void)
{
	return (this->_members.size());
}

Client &Channel::getMemberByIndex(size_t index)
{
	return (*(this->_members[index]));
}

bool Channel::isMember(const Client &client)
{
	for (size_t i = 0 ; i < this->_members.size() ; i++)
	{
		if (client.getUsername() == this->_members[i]->getUsername())
			return (true);
	}
	return (false);
}

void Channel::addMember(Client &client)
{
	this->_members.push_back(&client);
}
