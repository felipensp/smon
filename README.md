smon
======

Process segment monitor

```
$ ./samples/fgets &
[1] 12282

$ ./smon -s -n 1 -p 12282 &
[2] 12283

[1]+  Stopped                 ./samples/fgets

[+] attached to pid 12282
[+] stack found at 7fff15207000-7fff15228000 (135168 bytes)
fg 1
./samples/fgets
*HELLO*
[*] segment has been changed at 7fff15207000
0x7fff15207000  93 2a e0 0b a0 f7 11 0c be 7f ff ff ff ff 0c f7  |.*..............|
0x7fff15207010  e0 0b be 7f a0 f7 11 0c be 7f a0 e7 e0 0b be 7f  |................|
0x7fff15207020  0a a0 f7 11 0c be 7f 9c 06 40 00 00 00 2d dc e0  |.........@...-..|
0x7fff15207030  0b a0 f7 11 b0 6e 22 15 ff 7f 0a 00 00 00 00 00  |.....n".........|
0x7fff15207040  c0 6e 22 15 ff 7f 9c 06 40 00 00 00 a8 ff ff ff  |.n".....@.......|
0x7fff15207050  ff ff ff ff cc e7 dd 0b be 7f a6 06 40 b8 2b 6b  |............@.+k|
0x7fff15207060  15 38 ae ac 55 98 d3 50 23 68 d0 02 4a 30 29 df  |.8..U..P#h..J0).|
0x7fff15207070  0b be a0 f7 11 08 00 00 00 30 a0 6f 22 15 ff e0  |.........0.o"...|
0x7fff15207080  6e 22 15 ff 7f b8 6e 22 15 ff 01 68 03 40 00 00  |n"....n"...h.@..|
0x7fff15207090  00 00 90 04 40 00 00 71 22 15 ff 7f 00 00 00 00  |....@..q".......|
0x7fff152070a0  20 70 22 15 ff 7f 7a 94 de 0b be 7f 08 00 00 00  | p"...z.........|
0x7fff152070b0  30 a0 6f 22 15 ff e0 6e 22 15 ff 7f 00 00 00 00  |0.o"...n".......|
0x7fff152070c0  00 00 00 00 00 00 00 00 b0 6f 22 15 ff b0 6f 22  |.........o"...o"|
0x7fff152070d0  15 ff 7f 88 22 ad fb 00 00 10 0e 12 0c be 7f 00  |...."...........|
0x7fff152070e0  a7 32 0c 50 23 da 08 10 34 0c be 7f 00 00 00 00  |.2.P#...4.......|
0x7fff152070f0  00 00 20 70 22 15 ff 7f 00 55 8a 13 00 00 00 00  |.. p"....U......|
0x7fff15207100  00 00 88 22 ad fb b0 6f b0 6f 22 15 ff 7f 9c 06  |..."...o.o".....|
0x7fff15207110  40 10 0e 12 0c a7 32 0c be 7f c8 61 34 0c be 00  |@.....2....a4...|
0x7fff15207120  00 00 de 2a 48 45 4c 4c 4f 2a 0a 00 00 00 00 00  |...*HELLO*......|
0x7fff15207130  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  |................|

[*] segment has been changed at 7fff15207000
[*] segment has been changed at 7fff15207000
[*] segment has been changed at 7fff15207000
Received: *HELLO*
[*] segment has been changed at 7fff15207000

[*] segment has been changed at 7fff15207000
[!] program has exitted with status 0
[!] failed to detach from pid 12282

```
