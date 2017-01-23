#ifndef NETWORKING_H
#define NETWORKING_H

#define MESSAGE_BUFFER_SIZE 1000
#define MAX_PERSON_SIZE 100
#define KEY_NUM 1701
#define NUMBER_PEOPLE 4

//Some constant strings for all situations
#define ENTERUSERNAME "ENTERUSERNAME" //Size is 13
#define TESTMESSAGE "The message you tried to send was successful. Yay."
#define GOODANSWER "This Works" //size is 10
#define BADANSWER   "This does not work" //Size is 18
#define AMBIGUOUSANSWER "Yes" //Size is 3
#define GENERICERROR "You messed something up. Given that this error message was created for everything, your problem is probably something fairly simple that will be obvious in hindsight. The entire reason this message exists is to deal with situations like this. Because there are so many errors, and coming up with new error messages is lazy. So this entire situation has risen from the fact that you are now too lazy to write your own error messages. Honestly, you should feel ashamed. Anyway, you're about one error message away from giving up and becoming a goat herder is Iceland. You should actually consider doing that. It would probably be a lot more fun than figuring out what's wrong."
#define IMPROBBABLEERROR "This works fine on my computer."
#define IMPOSSIBLEERROR "If you recieve this message, all laws of reality are probably broken. You really shouldn't be seing this. If you do end up seeing this, don't bother with anything else because it quite litterally cannot happen."
#define SUCCESS "This code worked. It did what you want. Everything is fine. Everything is happy. Your code is one line away from collapsing and breaking, but let's not let that get in the way of your imaginary victory."
#define FAILURE "If you spent as much time actually coding as writing these messages, your code might actually be working."
#define KILL_ME "Am I talking about the code or real life?"
#define START "You have just been paired. Time to meet your partner!"
#define ENDING "The simulation is doen. Please name your favorite person."

void error_check( int i, char *s );

int server_setup();
int server_connect(int sd);
//void sub_server(int sd, int sd2);

int client_connect( char *host );

//server.c functions
int search_list_for_usernames(char * potential);
int * end;

#endif
