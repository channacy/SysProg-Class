1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_The remote client determines when a command's output is fully recieved by checking if the chunk of data ends with a NULL character or a linux EOF charcter to indicate all of the data is being recieved. To handle partial reads, the program will add a null if the data is the last chunk but if not, it continues to read the data and print it out if needed. Then, the loop continues and more data is read._

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_Networked shell protocol define and detect the beginning/end of a command using delimiters and also fixed-length characters as well as packet size of the transfered messages. Challenges can arise with error handling if the typed command is a mispelled or it is incomplete and has delimiters affecting the way the shell protocol reads the command(s)._

3. Describe the general differences between stateful and stateless protocols.

_Stateless protocols treat each request as independent whereas stateful protocols are able to store information about previous transfers of data. TCP is stateful and HTTP is stateless. There are less resources such as storage or memory needed to maintain stateless protocols but data could be sent repetitively causing a reduction of network performance while also limiting functionality without a saved state to use._

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_Even though UDP is considered to be "unreliable," it offers more speed and efficieny in deliverying data needed especially for real-time scenarios. UDP also supports broadcast and multicast transmissions allowing it to send one transmission that can reach multiple endpoints. The packet-based protocol is also different from TCP which is stream based. However, because of UDP's strengths despite its drawback of not being absolutely reliable in ensuring data transmission in a proper sequential order, it is still widely used today._

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_Some examples of interface/abstraction includes APIs (application programming interface) such as a RESTful API to request network services through endpoints to transfer data. Network sockets, which uses a two-way communication channel also allow real-time data to be sent across the network with the TCP/IP protocol suite defining the rules of data exchange over the Internet._