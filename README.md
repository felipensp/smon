smon
======

Process segment monitor


```
felipe@sig11:~/dev/smon$ ./samples/fgets  &
[1] 9222
felipe@sig11:~/dev/smon$ ./smon -s -p 9222 &
[2] 9223

[1]+  Stopped                 ./samples/fgets
felipe@sig11:~/dev/smon$ [+] attached to pid 9222
[+] stack found at 7fff37c40000-7fff37c61000=135168 bytes
fg 1
./samples/fgets
foobar
[*] segment has been changed!
[*] segment has been changed!
[*] segment has been changed!
[*] segment has been changed!
Received: foobar
[*] segment has been changed!

[*] segment has been changed!
[!] program has exitted with status 0
[!] failed to detach from pid 9222

```
