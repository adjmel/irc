#pragma once

#include <iostream>
#include <string>


//ERROR REPLIES
#define ERR_NEEDMOREPARAMS(server_name, command)				"461 " + server_name + " " + command + " :Not enough parameters"
#define ERR_USERNOTINCHANNEL(server_name, nickname, channel)	"441 " + server_name + " " + nickname + " " + channel + " :They aren't on that channel"


#define ERR_NONICKNAMEGIVEN(server_name)						"431 " + server_name + " :Nickname not given"
#define ERR_NICKNAMEINUSE(server_name, nickname)				"433 " + server_name + " " + nickname + " :Nickname is already in use"
#define ERR_ALREADYREGISTERED(server_name)					    "462 " + server_name + " :You may not reregister"
#define ERR_PASSWDMISMATCH(server_name)						    "464 " + server_name + " :Password incorrect"
#define ERR_NOTREGISTERED(server_name)						    "451 " + server_name + " :You have not registered"
#define ERR_UNKNOWNCOMMAND(server_name, command)				"421 " + server_name + " " + command + " :Unknown command"
#define ERR_INVITEONLYCHAN(server_name, channel)			    "473 " + server_name + " " + channel + " :Cannot join channel (+i)"


#define ERR_CHANOPRIVSNEEDED(server_name, channel)		    	"482 " + server_name + " " + channel + " :You're not channel operator" + "\n"
#define ERR_NOSUCHCHANNEL(server_name, channel)				    "403 " + server_name + " " + channel + " :No such channel" + "\n"
#define ERR_NOTONCHANNEL(server_name, channel)				    "442 " + server_name + " " + channel + " :You're not on that channel" + "\n"
#define ERR_NOSUCHNICK(server_name, target)					    "401 " + server_name + " " + target + " :No such nick/channel" + "\n"
#define ERR_USERONCHANNEL(server_name, target, channel)		    "443 " + server_name + " " + target + " " + channel + " :is already on channel" + "\n"
#define ERR_CHANNELISFULL(server_name, channel)				    "471 " + server_name + " " + channel + " :Cannot join channel (+l)" + "\n"
#define ERR_BADCHANNELKEY(server_name, channel)				    "475 " + server_name + " " + channel + " :Cannot join channel (+k)" + "\n"
#define ERR_INVITONLYCHAN(server_name, channel)				    "473 " + server_name + " " + channel + " :Cannot join channel (+i)" + "\n"

// NUMERIC REPLIES
#define RPL_WELCOME(server_name, prefix)										"001 " + server_name + " :Welcome to the Internet Relay Network " + prefix + "\n"
#define RPL_NOTOPIC(server_name, channel)					                    "331 " + server_name + " " + channel + " :No topic is set" + "\n"


// COMMAND REPLIES
#define RPL_NICK(nick_previous, nick)					    ":" + nick_previous + " NICK :" + nick + "\n"
#define RPL_JOIN(nick, channel)					            ":" + nick + " JOIN :" + channel + "\n"
#define RPL_INVITE(server_name, target, channel)            ":" + server_name + " INVITE " + target + " :" + channel + "\n"
#define RPL_PART(server_name, channel, reason)					    ":" + server_name + " PART " + channel + " :" + reason + "\n"
#define RPL_KICK(server_name, channel, target, reason)	    ":" + server_name + " KICK " + channel + " " + target + " :" + reason + "\n"
#define RPL_PRIVMSG(server_name, target, message)		    ":" + server_name + " PRIVMSG " + target + " :" + message + "\n"
#define RPL_TOPIC(nick, channel, topic)                     ":" + nick + " TOPIC " + channel + " :" + topic + "\n"
#define RPL_MODE(server_name, channel, modes, args)		    ":" + server_name + " MODE " + channel + " " + modes + " " + args + "\n"
#define RPL_QUIT(server_name, message)					    ":" + server_name + " QUIT :" + message + "\n"
#define RPL_QSQUIT(server_name, message)                    ":" + server_name + " SQUIT :" + message + "\n"
#define RPL_NOTICE(server_name, target, message)            ":" + server_name + " NOTICE " + target + " :" + message + "\n"



