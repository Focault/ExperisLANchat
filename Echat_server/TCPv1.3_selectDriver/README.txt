
This Server Is a TCP server Driver Using the select method, which means it isn't busy waiting.

# Compilation Note #

The Makefile in the Server folder and the main.c file in it
are designed for testing with the Mega Client (which ping pong
1000 simulated clients for 500 iterations) And are not there 
for other using purposes in production.