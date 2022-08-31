# HashCracker
An attempt to brute force crack the Murmur3 Hashes used in the
Xenoblade 3 BDATs. C++20 is required for compilation.

The Xenoblade 3 BDAT files have their filenames and column labels hashed
with the Murmur3 x86 32-bit hash function and a seed of 0. This project
aims to help crack these filenames and labels so we can have a more
complete data mine of the game.

The current methodology is effectively brute-force. A producer thread
generates all potential strings of a given specification and adds them
to a buffer.

A series of consumer threads then pull strings from the buffer,
calculate the Murmur3 hash, and check for any matches against the hashes
we currently seek to decode.

If a match is located, the hash and string are printed out for human
verification against false positives.

In order to leverage existing knowledge of how other files and columns
are named in the BDATs, the strings that will be tested can be highly
customized using the configuration file Config.cfg. A sample config file
has been provided.

Future plans for the project include support for OpenCL and
investigations into parallelizing the string generation.
