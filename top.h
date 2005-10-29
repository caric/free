#ifndef _TOP_H_
#define _TOP_H_

struct object_info {
        int 	            id;
        int                 pid;
        int                 share_type;
        int                 resident_page_count;
        int                 ref_count;
        int                 task_ref_count;
        int                 size;
        struct object_info  *next;
};

#define OBJECT_TABLE_SIZE	537
#define OT_HASH(object) (((unsigned)object)%OBJECT_TABLE_SIZE)

struct object_info      *shared_hash_table[OBJECT_TABLE_SIZE];

struct object_info *of_free_list = 0;

unsigned long get_shared_kb();

#endif // _TOP_H_
