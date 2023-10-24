# Client-Server Interaction: OS Lab 2

Hey! This is a small project I worked on for my Operating Systems course. The task was to design client-server applications using `fork()` and threads, derived from the insights and lessons of our previous lab sessions.

## Overview:
1. A server application that spawns a new process with `fork()` for every new connection.
2. A another server variant that spins up a thread for each connection.
3. A client applciation that stress-tests both server models by establishing a multitude of connections through threading, aiming to push them to the point of unresponsiveness.
