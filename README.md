# IRC Server in C++98

## Description
A simple **IRC (Internet Relay Chat) server** in **C++98**, tested with **irssi**. Supports multiple clients, password authentication, channels, private messaging, and operator commands.

## Running the Server:
```bash
./ircserv <port> <password>
./ircserv 6667 mdp
```

## Connecting with Irssi
### 1. Connect to Server
```
/server add localhost 6667 <password>
/connect localhost 6667
```
### 2. Authenticate and Set Nickname
```
/quote pass <password>
/nick <nickname>
/quote user <Username> 0 * :<RealName>
```

### 3. Verify Connection
```
/lag  (Should return a response if connected)
```

## Essential IRC Commands
### Private Message:
```
/quote privmsg <user> <message>
```
### Join Channel:
```
/join #<channel_name>
```
### Change Topic:
```
/topic <NewTopic>
/topic  (View current topic)
```
### Kick User:
```
/kick <user>
```
### Set Channel Modes:
```
/mode #<channel> +o <user>  (Give operator privileges)
/mode #<channel> +i         (Invite-only mode)
```
### Invite User:
```
/invite <user> #<channel>
```

## Channel Modes Summary:
- `+i`: Invite-only channel.
- `+t`: Only operators can change the topic.
- `+k`: Set/remove channel key (password).
- `+o`: Operator privileges.
- `+l`: Set/remove user limit.

## Troubleshooting Irssi Connection Issues:
- Check if the server is running:
  ```bash
  netstat -an | grep 6667
  ```
- Ensure server binds to `0.0.0.0`, not only `127.0.0.1`.
- Use the correct port and password.
- Confirm `/quote pass` is issued before `/nick` and `/user`.

---
*Tested and verified with **`irssi`** as the reference IRC client.*

