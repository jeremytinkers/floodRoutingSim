# floodRoutingSim
A simple **flood routing** simulator in **C++** with solutions to overcome looping. 

**For no concurrency :** <br/>
`Run codeNonConcurrent.cpp`

**For concurrency :** <br/>
`Run codeConcurrent.cpp`

There are 2 levels of concurrency here:-
- `Level 1` : **Multiple packets are all sent at the same time on different
threads** but concurrent flooding of a particular packet from a source
does not occur.
- `Level 2` : Here both are ensured. **Packets are sent/received
concurrently while also flooding from a source concurrently.**

(You have the option to choose your level of concurrency during execution)

Kindly find attached the project report for further details regarding implementation
