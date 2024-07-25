# Packet Transmission with Correct Sequence Numbers and Packet Loss Handling
## Overview
This README provides a guide on how to manage packet transmission between a server and client, ensuring packets are sent and received in the correct order, handling packet loss, and updating network congestion parameters like cwnd (congestion window) and ssthresh (slow start threshold).

## Features
1. Sending packets in correct sequence order
2. Detecting and handling packet loss using 3-duplicate ACK detection
3. Simulating packet loss for testing
4. Updating cwnd and ssthresh based on network conditions
5. Correct sequence number management in packet acknowledgment (ACK)
6. Sending Packets with Correct Sequence Numbers

### Connection Establishment:

1. The server and client establish a connection.
2. Both entities send and receive packets in the correct sequence order.

### Packet Transmission:

1. The server sends a packet to the client.
2. The client receives the packet and sends an ACK back to the server.
3. If a packet is lost, the server detects the loss and resends the packet.

### 
Handling Packet Loss:

1. 3-duplicate ACK detection is used to detect packet loss.
2. If more than one packet is lost in the same loop, the server resends the lost packet after detecting mismatched sequence numbers.

### Server-Side Logic:

1. Two if statements handle normal packet sending and resending in case of packet loss.

2. Simulating Packet Loss: Packet loss is simulated by setting data_seq.seq_num == 8.
For more frequent loss, generate a random number and check the remainder.

### Detecting 3 Duplicate ACKs
1. Every received ACK is compared to the previous one.
2. If the ACK is the same as the previous one, a duplicate counter is incremented.
3. When the counter reaches 3, a message is printed indicating 3 duplicate ACKs along with the sequence number and ssthresh.

### Updating cwnd and ssthresh

1. The number of packets sent depends on the current value of cwnd.
2. After sending, the receiver sends ACKs, which are used to update cwnd and ssthresh.

### Conditions:

1. If fast retransmit is required (3 duplicate ACKs detected):
If cwnd >= ssthresh, the state is congestion avoidance, and cwnd increases by 1.
If cwnd < ssthresh, the state is slow start, and cwnd is doubled.
On detecting 3 duplicate ACKs, cwnd is halved, and cwnd is set to 1.
Client-Side Sequence Number Management

### Receiving Data and Sending ACKs:

1. The client checks if the sequence number is the expected one.
2. If the sequence number matches the expected value, the client sends an ACK.
3. If not, the client continues to send ACKs for the lost packet.

### Client Logic:

1. An array is used to store the count of lost packets and unACKed packets.
2. When the lost packet is resent and received, the client ACKs the correct packet and continues with the correct sequence number.
Output
3. The system prints messages indicating the detection of 3 duplicate ACKs, the sequence number, and the value of ssthresh.
4. The correct handling of packets, including sending, receiving, and resending, is managed according to the described logic.


# read report for more details
