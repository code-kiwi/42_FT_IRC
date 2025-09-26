
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

    const std::string &getName(void);
    Client &getOwner(void);

private:
    const std::string _name;
    Client &_owner;
};

#endif /* CHANNEL_HPP */
