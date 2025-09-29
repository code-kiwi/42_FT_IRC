
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbutel <magikh@posteo.net>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 07:54:44 by lbutel            #+#    #+#             */
/*   Updated: 2025/09/19 07:55:17 by lbutel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include "Client.hpp"

class Channel {
public:
    Channel(const std::string &name, Client &owner);
    ~Channel(void);

	static bool isValidName(const std::string channel_name);

    const std::string &getName(void);
	size_t getMemberCount(void);
	Client &getMemberByIndex(size_t index);
    Client &getOwner(void);


	void addMember(Client &client);
	bool isMember(const Client &client);

private:
    const std::string _name;
    Client &_owner;
	std::vector<Client *> _members;
};

#endif /* CHANNEL_HPP */
