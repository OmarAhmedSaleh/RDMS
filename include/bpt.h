#ifndef BPT_H
#define BPT_H

#include "predefined.h"
#include <stdio.h>
#include <stddef.h>
#include <iostream>


namespace bpt{

    // offsets
    #define OFFSET_META 0
    #define OFFSET_BLOCK OFFSET_META + sizeof(meta_t)
    #define SIZE_NO_CHILDREN sizeof(leaf_node_t) - BP_ORDER * sizeof(record_t)



    // meta information of B+ tree
    typedef struct {
        size_t order; /* `order` of B+ tree */
        size_t value_size; /* size of value */
        size_t key_size;   /* size of key */
        size_t internal_node_num; /* how many internal nodes */
        size_t leaf_node_num;     /* how many leafs */
        size_t height;            /* height of tree (exclude leafs) */
        off_t slot;        /* where to store new block */
        off_t root_offset; /* where is the root of internal nodes */
        off_t leaf_offset; /* where is the first leaf */
    } meta_t;
    
    // internal nods index segment
    struct index_t{
        key_t key;
        off_t child; // child's offset
    };

    // internal node block
    struct internal_node_t{
        typedef index_t *child_t;

        off_t parent; // parent node offset
        off_t next;
        off_t prev;
        size_t n; // how many children
        index_t children[BP_ORDER];
    };
    // the final record of value
    struct record_t{
        key_t key;
        value_t value;
    };
    // hte leaf node block
    struct leaf_node_t {
        typedef record_t *child_t;
        off_t parent; // parent node offset
        off_t prev, next;
        size_t n; // how many children
        record_t children[BP_ORDER];
    };

    // the encapsulated B+ tree
    class bplus_tree{
        public:
        meta_t meta;
        char path[512];
        mutable FILE *fp;
        mutable int fp_level;
        bplus_tree(const char *path, bool force_empty = false);
        void init_from_empty();
        int insert(const key_t &key,value_t value);
        off_t get_parent_offset(const key_t &key) const;
        off_t search_leaf(off_t index, const key_t &key) const;
        void insert_record_into_leaf_node(leaf_node_t *leaf, const key_t &key, const value_t &value);
        void insert_key_to_index(off_t offset, const key_t &key, off_t old, off_t after);
        void insert_key_to_index_no_split(internal_node_t &node, const key_t &key, off_t child_offset);
        void reset_index_children_parent(index_t *begin, index_t *end, off_t parent);
        int update(const key_t& key, value_t value);
        off_t search_index(const key_t &key) const;
        int remove(const key_t &key);
        // find leaf
        off_t search_leaf(const key_t &key) const;
        void adopt_key_in_parent_node(off_t parent_offset, const key_t &prev, const key_t &new_key);
        bool borrow_key(bool from_right, leaf_node_t &borrower);
        void merge_leafs(leaf_node_t *left, leaf_node_t *right);
        void remove_leaf(leaf_node_t *prev, leaf_node_t *leaf);
        void remove_from_index(off_t offset, internal_node_t &node, const key_t &key);
        template<class T>
        void node_create(off_t offset, T *node, T *next);
        /* borrow one key from other internal node */
        bool borrow_key(bool from_right, internal_node_t &borrower,
                    off_t offset);
        void merge_keys(index_t *where,
                            internal_node_t &node, internal_node_t &next);
        template<class T>
        void node_remove(T *prev, T *node);
        int search(const key_t& key, value_t *value) const;
        meta_t get_meta() const{
            return meta;
        };
        void open_file(const char *mode = "rb+") const
        {
            // `rb+` will make sure we can write everywhere without truncating file
            if (fp_level == 0){
                fp = fopen(path, mode);
            }
            ++fp_level;
        }
        void close_file() const{
            if(fp_level == 1){
                fclose(fp);
            }
            --fp_level;
        }
        // read block from disk
        // I need to pass in an unknown type of parameter into a function.
        /*
        When const qualifies a member function, the qualification refers to the implicit this argument. In other words, that function will not be allowed to alter the state of the object it is invoked on (the object pointed to by the implicit this pointer) - with the exception of mutable objects 
        */
        int map(void *block, off_t offset, size_t size) const {
            open_file();
            fseek(fp, offset, SEEK_SET);
            size_t rd = fread(block, size, 1, fp);
            close_file();
            return rd - 1;
        }
        template <class T>
        int map(T *block, off_t offset) const{
            return map(block, offset, sizeof(T));
        }

        // write block to dist
        int unmap(void *block, off_t offset, size_t size) const{
            open_file();
            fseek(fp, offset, SEEK_SET);
            size_t wd = fwrite(block, size, 1, fp);
            close_file();
            return wd - 1;
        }
        template <class T>
        int unmap(T *block, off_t offset) const{
            return unmap(block, offset, sizeof(T));
        }

        // alloc from disk
        off_t alloc(size_t size){
            off_t slot = meta.slot;
            meta.slot += size;
            return slot;
        }
        off_t alloc(leaf_node_t *leaf){
            leaf->n = 0;
            meta.leaf_node_num++;
            return alloc(sizeof(leaf_node_t));
        }
        off_t alloc(internal_node_t *node){
            node->n = 1;
            meta.internal_node_num++;
            return alloc(sizeof(internal_node_t));
        }
        void unalloc(bool isLeafNode){
            if(isLeafNode){
                --meta.leaf_node_num;
            }else{
                --meta.internal_node_num;
            }
        }
        void unalloc(leaf_node_t *leaf){
            unalloc(true);
        }
        void unalloc(internal_node_t *node){
            unalloc(false);
        }

    };

    

}


#endif
