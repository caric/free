#ifndef __VM_STAT_H__
#define __VM_STAT_H__

/**
 * Fill lots of global variables with memory info.
 */
extern void meminfo(void);

/* obsolete */
extern unsigned long kb_main_shared;
/* old but still kicking -- the important stuff */
extern unsigned long kb_main_wired;
extern unsigned long kb_main_inactive;
extern unsigned long kb_main_free;
extern unsigned long kb_main_total;
extern unsigned long kb_swap_free;
extern unsigned long kb_swap_total;
/* derived values */
extern unsigned long kb_swap_used;
extern unsigned long kb_main_used;



#endif // __VM_STAT_H__
