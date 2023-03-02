# http_web_server
https://www.youtube.com/watch?v=9J1nJOivdyw : Web Server Concepts and Examples

https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa : HTTP Server: Everything you need to know to Build a simple HTTP server from scratch

https://www.youtube.com/watch\?v\=0OrmKCB0UrQ\&list\=PLQnljOFTspQU6zO0drAYHFtkkyfNJw1IO\&index\=10 : Hyper Text Transfer Protocol Crash Course - HTTP 1.0, 1.1, HTTP/2, HTTP/3

https://www.youtube.com/watch?v=7IS7gigunyI : The OSI Model - Explained by Example

Principle of Application Layer | Introduction of Application Layer : https://www.youtube.com/watch?v=bj33lPUCpR8

What is port address in networking | Port Number | Purpose of Port Address : https://www.youtube.com/watch?v=SFfjUaNq5Xk

Introduction to TCP/IP and Sockets, part 1: Introducing the protocols and API : https://www.youtube.com/watch?v=C7CpfL1p6y0

Introduction to TCP/IP and Sockets, part 2: Example code Walk-Through : https://www.youtube.com/watch?v=OuYPe_HcLWc

https://www.rfc-editor.org/rfc/rfc2616

https://www.rfc-editor.org/rfc/rfc5322

------------------------

[sockaddr_in] is a struct in the C programming language that is used to represent an 
internet socket address for IPv4 addresses. It is defined in the <netinet/in.h> header file.

The [sockaddr_in] struct contains the following fields:

sin_family: specifies the address family, which is typically set to AF_INET for IPv4.
sin_port: specifies the port number for the socket address.
sin_addr: specifies the IPv4 address of the socket.
sin_zero: a padding field that is typically set to all zeros.

The [sockaddr_in] struct is commonly used in network programming to specify the endpoint 
of a network connection. It is used in conjunction with the socket() system call 
to create a new socket, and with the bind() system call to bind a socket to a particular 
IP address and port number.

-------------------------------