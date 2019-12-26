# File System

[TOC]

## Key Abstraction

1. File => A big contiguous string of bytes
2. File name
3. Directory Tree

## Access Rights

> For one user:

- Reading
- Writing
- Executing
  - For files, this determines if we can execute a file.
  - For dirs, this determines if we can enter it.

> Logically, we can have a giant matrix!(But it's very inefficient)

So UNIX like system uses **owner(user, typically the creator) & group** to manage this.

> For each file we have records about:
>
> - permission
> - user
> - group
> - file name
>
> For each file we have a permission like: `-rw-r--r--`
>
> 9 bits => `-(directory or not) rw-(user) r--(group) r--(other)`

## Dev Interface

- read
- write
- lseek: repositions the cursor
- close

### mmap

`mmap(..., fd, ...)`  returns a region of memory that can be manipulated by our code.

What we've done with file will get reflected in the file.

When we are done with the file we call `munmap(buf, ...)`.

Then `close(fd)`.

## Allocation Strategies

### Free or not

- Free list
- Busy bit vector

### File Allocation Table(FAT)

> DOS, windows, SD card.
>
> Still in use with small devices.
>
> Proprietary to MicroSoft.

#### 2 Types of clusters

1. **Data cluster** containing the file contents.
2. **Directory clusters** containing directory entry structures.

#### Directory Entries

> <u>Here, it's a tree.</u>
>
> Data of directory entries is in the `directory cluster`s, which have the information to go to the position(in data cluster) of **the start of the file** .

- File system metadata for all files.
  - File names
  - Timestamps
  - Starting cluster for files
- <u>A cluster used for directory entries will not be used for file data.</u>

> The first set of directory entries: **ROOT DIRECTORY**
>
> `[file/folder name][timestamp][starting cluster]`

#### FAT Table for contents

> An abstration for all clusters in the system.
>
> <u>Here it is a table(or just a array).</u>

- Provides **file to cluster** mapping
  - `cluster_cell_abstration = FAT_table[starting_cluster_id]`
- Each cell represents a cluster with a value.
- Addressing is dependent of FAT version

##### Cell value

- 0 : not alloc
- ff7/fff7/fff fff7(fat 12, 16, 32) : bad alloc
- ff8/fff8/fff fff8(fat 12, 16, 32) : end of file
- All other values mean the cluster is allocated.

![](https://i.loli.net/2019/12/26/AXzDNY4Is5TcdvH.png)

e.g. `Alice.txt` starts at 3 and ends at 3.

### Extended Format

#### Inode(metadata) for each file

- 15 data block address(This is how it is optimized compared with fat32)
  - [1 ~ 12 => **Direct**] The first 12 point directly to the first 12 blocks of the file(efficient for small files).
  - [13 => **Indirect**] Others point to a block that consists of a table of addresses for next blocks of the file.
  - [14 =>  **x2 indirect**] ... table of table
  - [15 =>  **x3 indirect**] ... table of table of table

### For exFAT...

- **File creation**
  - Grab an inode.
  - Update his father inode(`directory cluster`).
  - Same as FAT.
- **File growth**
  - New block and add it to inode.
- **Access time**
  - Metadata
    - exFAT: In the data block
    - FAT: In the node.
  - Inode link the data in a tree like structure.
  - While FAT just view it as a linked list.(Bad for large files.)

## Buffer Cache

> Software optimizations.

Unified buffer cache.

Data will be stored on the UBC first. Or just call `fsync/msync` to flush out hte unified buffer cache.

## Journaling

What if system crash occurs before the data in the UBC is written back? How can we overcome this?

A nice idea is if we could periodically(e.g. some fixed number of dirty blocks is reached.) copy all the changes reflected in the memory to the disk. 

- Write the changes quickly.
  - Why fast? Cuz the changes can be recored **sequentially**(this is fast. And random access is slow!!!).
- Mark the dirty block in memory as clean.
- At a more **opportune** time, we do all the changes.

## DMA

> Hardware-based.

DMA is a hardware controller. (Cycle stealing)

"Just let the CPU go, DMA can handle the copy."

