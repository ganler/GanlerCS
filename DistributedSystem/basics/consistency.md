> TOREAD:
>
> * https://lotabout.me/2019/Raft-Consensus-Algorithm/
> * https://www.cnblogs.com/bangerlee/p/5767845.html
> * https://www.sohu.com/a/427592773_268033?qq-pf-to=pcqq.group

## CAP Theorem

- Consistency: For `x` in machines, `x`s are equivalent in content/storage/...
- Availability: All machines are available for services;
- Partition Tolerance: Machines are allowed to be divided into several (> 1) groups;

> CAP Theorem: You can at most have 2 of them;

## BASE Theorem

* Basically available: When unpredictable errors occur, it is allowed to lose some availability, but not all of them;
  * Worse latency;
  * Less functionalities;
* Soft State: Allow systems to have latency in synchronizing data;
* Eventually consistent: ...

> If you want high availability, then you must compromise fault tolerance or consistency;

## Consistency

### Concepts

- Weak/Eventual consistency: Make it consistent later;
- Strong consistency: Make it consistent NOW!
  - Order consistency: All threads/workers see the same ordering of the whole program; Operations have strict order preferences; like (op1 is ahead of op2)
  - Linearizability: Every operation has a global time stamp; Operations with lower time stamp are executed first;

### Why we need it

To make your data safe, we need to replicate our important data. 

Or to ensure fault tolerance, we need **state machine replication**;

### Strong Consistency Algorithms

#### Master-slave Replication

- Master: accept writes;
- Master: copy loggings to slaves;
- Master: wait until all copies are finished;

> When one node/slave is failed that the master is blocked, we lose the availability but keep the consistency;

#### How to test them!

Jepsen: https://jepsen.io/

#### Election: How to select the master/leader

Bully algorithm:

![img](https://images2015.cnblogs.com/blog/116770/201608/116770-20160814110211906-1201598126.png)

Every node has a number; higher number is the leader; when leader dies,  the node with semi-high number is selected as the next leader;

a. **When slaves found the leader is dead**, they will tell send election message (including its number) to the node with higher number; 

bc. 5,6 nodes found the author's number of the message is less than them (i.e., someone wants to re-elect) then approve the re-election;

> * Every one votes the node whose number is higher than him self;
> * After receiving votes, the nodes send an `OK` message back to the voters;
> * If there is a node that nobody says OK to, then that's the new leader;

d. The new leader will broadcast to tell every one that he is the new leader;

> But when the leader is not dead **to all nodes**?
>
> Look at Raft;

Sometimes the nodes may be partitioned that there may be multiple leaders: Thus we need 

#### Lease

So how to know if our leader is dead? 

- Heart beat. 

> HB may produce multiple leaders; i.e., when the network is not working, you thought your leader is gone (i.e., the group is partitioned) and you elect another leader; then the network gets healthy again and you find 2 leaders in one cluster;
>
> Like what mwish said:
>
> <img src="C:\Users\ganler\Documents\GanlerCS\DistributedSystem\basics\image-20201113211804036.png" alt="image-20201113211804036" style="zoom:50%;" />

A decent approach is called <u>lease</u>;

Think about when you are the landlord and you lease your room to someone else, say Jack; As Jack didn't buy your room, so he needs to pay you periodically. That said, multiple nodes may register themselves on the landlord node(not the master node); And the landlord node will only lease the room (leader) to one node. When the leader is gone and timeout occurs, the landlord node will re-elect the leader;

> Long term short. The **landlord node** determine who is the leader and monitor the existence of the leader;
>
> > There can be multiple leaders in HB because multiple nodes can determine the leader not only one.

#### Quorum/majority

Say we have N nodes, we make sure we write k (> N/2) nodes; and we make sure we read u (> N/2) nodes and use the latest record;

> Thus, we have more availability: We only need to make sure N/2 nodes are written;