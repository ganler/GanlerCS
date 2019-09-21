# Software Development Process

#### Software Crisis

- Rising demands for softwares(almost exponentially).
- Slow programmers' productivity growth(hard to keep up with the rising demands).
- Increasing product complexity.

#### Software processes

- Waterfall(like assembly line).
- Evolutionary prototyping(iteratively update the software according to the demands of the customers.).
- Unified Software Process(based on UML).
- Agile software process(sacrifice the some of the disciplines of the traditional ways in order to gain more flexibility and be adaptive to changes.)

#### Software phases

1. Requirements engineering
2. Design
3. Implementation
4. Verification and validation
5. Maintenance.

#### Tools that enhance the productivity

- Integrated Development Environments(IDE)
- Version Control System(VCS, like Git)
- Coverage and varification tools.

## Life Cycle Models

#### Keys of traditional phases

**Verification**: Did we build the system correctly?

**Validation**: Did we build the system the customers want?

**Test**: Unit level -> integration level -> system level

#### Features of life cycle models

**Waterfall**: Strictly step by step. Once determined, do not change. (Easy for development. But not good for expansion to meet customers' needs.)

**Sipral**: objectives -> estimate **risks** -> develop & test -> plan for the next iteration

**Evolutional prototype**: Initial concept -> implement initial concept -> refine until acceptable -> complete and release.

**RUP**: Based on UML. 

Inception -> Elaboration -> Construction -> Transition

**Agile**

```c++
while(add_new_needs())
{
    while(!meet_the_needs)
	{
        write_enough_codes();
        try{
            meet_the_needs();
        }
	}// Met!
    refactor(); // Modify the code and the design of it.
}

```

#### Mistakes

- People;
- Process(Like bad schedules and plannings);
- Technology;
