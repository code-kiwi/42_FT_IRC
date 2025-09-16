/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCReplies.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 16:24:06 by mhotting          #+#    #+#             */
/*   Updated: 2025/09/15 16:35:07 by mhotting         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCREPLIES_HPP
#define IRCREPLIES_HPP

#include <string>

namespace IRC {
const int ERR_UNKNOWNCOMMAND = 421;
const std::string MSG_UNKNOWNCOMMAND = "Unknown command";
} // namespace IRC

#endif
