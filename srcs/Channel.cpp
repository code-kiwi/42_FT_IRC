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

const std::string &Channel::getName(void)
{
	return (this->_name);
}

Client &Channel::getOwner(void)
{
	return (this->_owner);
}

