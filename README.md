# use c/c++ to develop a small relational database based on B+ tree
Development environment: ubuntu

1. Enter `make` to compile, enter `./duck_db` to run;
2. `make clean` clears files.
3. The directory where the database binary file is stored: ./data/db.bin

The main features are as follows:

1. Use C/C++ to develop;
2. Basic CURD operations have been implemented, using console SQL:
   1. Create a new record (create), operate SQL: **insert db {index} {name} {age} {email};**
   2. Update record (update), operation statement: **update db {name} {age} {email} where id={index};**
   3. Single read record (read), operation statement: **select * from db where id={index};**
   4. Delete record (delete), operation statement: **delete from db where id ={index};**
3. The bottom layer uses B+ tree (B+ TREE) to build the index;
4. Use binary storage data table;
5. Custom table structure is not supported temporarily. The database table structure is fixed as: id, name, age, email

the main development steps are as follows:
1. Create a console dialogue interactive program (REPL: read-execute-print loop);
2. Create a simple lexical analyzer to parse SQL statements;
3. Write the CURD function to implement database addition, deletion, modification, and query operations;
4. Create a b+ tree index engine to perform database indexing and disk read and write operations. The data table will be stored in binary form.

**B tree**

**B-tree** can be seen as an extension of the 2-3 search tree, that is, it allows each node to have M-1 child nodes.

-The root node has at least two child nodes;
-Each node has M-1 keys, arranged in ascending order;
-The value of the child node located in M-1 and M key is between the value corresponding to M-1 and M key;
-Other nodes have at least M/2 child nodes;

**B+Tree**

**B+** tree is a kind of deformed tree of B tree. The difference between it and B tree is:

-A node with k sub-nodes must have k key codes;
-Non-leaf nodes only have an index function, and the information related to records is stored in the leaf nodes.
-All leaf nodes of the tree form an ordered linked list, which can traverse all records in the order of key code sorting.

The difference between B and B+ tree is that the non-leaf nodes of B+ tree only contain navigation information and do not contain actual values. All leaf nodes and connected nodes are connected by a linked list, which is convenient for interval searching and traversal.

**The advantages of B+ trees are:**

-Since the B+ tree does not contain data information on the internal nodes, more keys can be stored in the memory page. Data is stored more compactly and has better spatial locality. Therefore, access to the data associated on the leaf node also has a better cache hit rate.

-The leaf nodes of the B+ tree are all interlinked, so the convenience of the entire tree only needs to linearly traverse the leaf nodes once. And because the data is arranged in order and connected, it is convenient for interval searching and searching. The B-tree requires recursive traversal of each layer. Adjacent elements may not be adjacent in memory, so cache hits are not as good as B+ trees.

But the B-tree also has advantages. The advantage is that since each node of the B-tree contains a key and a value, the frequently accessed elements may be closer to the root node, so the access is faster. The following is the difference diagram between B-tree and B+ tree:
#### Evaluation

duck_db uses the B+ tree to implement the basic CURD operations of the database, but there is still a big gap from the real database, such as:

1. Unable to create a custom table;
2. Does not support transaction processing, IO optimization;
3. Remote login to the database is not supported, it can only be used locally;
4. CURD advanced functions such as functions, constraints, and table operations are not supported;
5. More advanced features, etc.
