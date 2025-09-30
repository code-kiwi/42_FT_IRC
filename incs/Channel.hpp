
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

#include "Client.hpp"

#include <set>

class Channel {
public:
    Channel(const std::string &name);

    ~Channel(void);

    static bool isValidName(const std::string &channelName);
    static bool isValidKey(const std::string &channelKey);

    const std::string &getName(void) const;
    size_t getMemberCount(void) const;
    std::set<Client *> &getMembers(void);
    const std::string &getKey(void) const;
    const std::string &getTopic(void) const;

    bool isInviteOnly(void) const;
    bool isTopicOnly(void) const;
    bool hasKeyMode(void) const;
    bool hasMemberLimit(void) const;
    size_t getMemberLimit(void) const;

    void setKey(const std::string &key);
    void setTopic(const std::string &topic);
    void setInviteOnlyMode(bool val);
    void setTopicOnlyMode(bool val);
    void setKeyMode(bool val);
    void setLimitMode(bool val);
    void setMemberLimit(size_t limit);

    void addMember(Client *client);
    bool isMember(Client *client) const;
    void removeMember(Client *client);
    Client *getFirstMember(void) const;

    bool isOp(Client *client) const;
    void addOp(Client *client);
    void removeOp(Client *client);
    size_t getOpsCount(void) const;

private:
    std::string _name;
    std::string _key;
    std::string _topic;
    bool _inviteOnly;
    bool _topicOnly;
    bool _keyMode;
    bool _limitMode;
    size_t _limit;
    std::set<Client *> _members;
    std::set<Client *> _ops;
};

#endif
