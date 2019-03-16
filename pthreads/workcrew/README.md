workcrew(Daniel Robbins-Gentoo) program reconfigured to use multiple fifo queues and select().

A thread catches the potato, passes to another thread that peels it, it passes to another thread that puts it in the corect bucket; with logging and signal handling.


select():

http://stackoverflow.com/questions/4171270/select-function-in-socket-programming
The select function allows you to implement a event driven design pattern, when you have to deal with multiple event sources.
Let's say you want to write a program that responds to events coming from several different event sources e.g. network (via sockets), user input (via stdin), other programs (via pipes) or any other event source that can be represented by a fd. You could start separate threads to handle each event source, but you would have to manage the threads and deal with concurrency issues. The other option would be to use a mechanism where you can aggregate all the fds in a single entity (fdset), and then just call a function to wait on the fdset. The function would return whenever an event occurs on any of the fds. You could check which fd the event occurred on, read that fd, process the event and respond to it. After you have done that, you would go back and sit in that wait function - till another event on some fd arrives.
Select facility is such a mechanism. And the select function is the wait function. You can find the details on how to use it in any number of books and online resources.

http://en.wikipedia.org/wiki/File_descriptor#File_descriptors_as_capabilities
File descriptors as capabilities
Unix file descriptors behave in many ways as capabilities. They can be passed between processes across Unix domain sockets using the sendmsg() system call. Note, however, that what is actually passed is a reference to an "open file description" that has mutable state (the file offset, and the file status and access flags). This complicates the secure use of file descriptors as capabilities, since when programs share access to the same open file description, they can interfere with each other's use of it by changing its offset or whether it is blocking or non-blocking, for example.[5][6] In operating systems that are specifically designed as capability systems, there is very rarely any mutable state associated with a capability itself.

http://stackoverflow.com/questions/16549323/one-producer-and-multiple-consumer-should-i-use-n-semaphore-if-there-are-n-con
I have a program with 1 producer thread and N consumer threads. There are N types of producet, so I set up N FIFO queues(products should be delivered to the consumer in sequence). like: FIFO_queue_t* fifo_queque[N].
The consumer threads are blocked/waiting if there are no products on its FIFO queue. So for consumer thread i, they do:

        for(;;)
        {
            sem_wait(&sem[i]);
            product = fetch_one_product(queue);
            process(product);
         }

and for the producer, it call sem_post(&semp[i]) when it put a product for thread i into FIFO queue i.
it seems to me that I have to use N semaphores in this case, and if N is big, the resource consumption is big. is there anything missing in my thinking?
Update:
 The system is like a proxy, and my original design is like this:
I have a tcp server listening on a port, listen(listenfd, 20);
The producer is a libpcap instance, which capture TCP packets from that port, the products are the capture packets(including IP/TCP header);
For each connfd=accept(listenfd). I create a thread responsible for this connfd. These threads are consumers.
I create a lookup table, the fields in each entry are:
1 source IP/PORT, source address of incoming TCP connection,
2 FIFO Queue pointer: the fifo queue of captured spacket
3 pthread id :  the thread that is responsible for a tcp connection
when a producer thread fetch a packet, it parse the source IP/PORT from the packet, lookup the table, locate the FIFO queue and put it in the FIFO queue.
when a consumer thread gets notified that there are some packets in its FIFO queue, it fetches packets from the queue. And especially, for the intial 3-way packets: TCP SYN, SYN/ACK and ACK, it needs to maintain some states information, like MSS, SACK, etc. And it also needs put these packets(including TCP/IP header) as UDP payload, and send UDP packets to some other remote hosts.
