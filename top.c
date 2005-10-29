/*
 *  Top users display for Berkeley Unix
 *  Version 1.8
 *
 *  This program may be freely redistributed to other Unix sites, but this
 *  entire comment MUST remain intact.
 *
 *  Copyright (c) 1984, William LeFebvre, Rice University
 *
 *  This program is designed to run on either Berkeley 4.1 or 4.2 Unix.
 *  Compile with the preprocessor constant "FOUR_ONE" set to get an
 *  executable that will run on Berkeley 4.1 Unix.
 *
 *  The Sun kernel uses scaled integers instead of floating point so compile
 *  with the preprocessor variable "SUN" to get an executable that will run
 *  on Sun Unix version 1.1 or later.
 *
 *  Fixes and enhancements since version 1.5:
 *
 *  Jonathon Feiber at sun:
 *	added "#ifdef SUN" code to make top work on a Sun,
 *	fixed race bug in getkval for getting user structure,
 *	efficiency improvements:  added register variables and
 *	removed the function hashit
 *
 *	added real and virtual memory status line
 *
 *	added second "key" to the qsort comparisn function "proc_compar"
 *	which sorts by on cpu ticks if percentage is equal
 *
 **********************************************************************
 * HISTORY
 * 22-Apr-99  Avadis Tevanian (avie) at Apple
 *	Another rewrite for Mach 3.0
 *
 * 21-Apr-90  Avadis Tevanian (avie) at NeXT
 *	Completely rewritten again for processor sets.
 *
 *  6-May-88  David Golub (dbg) at Carnegie-Mellon University
 *	Completely rewritten for MACH.  This version will NOT run on any
 *	other version of BSD.
 *
 */

#include <mach/mach.h>
#include <top.h>

unsigned long get_shared_kb()
{       register int i;
        register int total_size;
	register int total_num;
	register struct object_info *sl, *next;
  
	total_size = total_num = 0;

        for (i = 0; i < OBJECT_TABLE_SIZE; i++) {
	        sl = shared_hash_table[i];
		shared_hash_table[i] = 0;
		
	        while (sl) {
		        if (sl->share_type != SM_PRIVATE_ALIASED) {
			        total_size += sl->resident_page_count;
				total_num++;
			}
			next = sl->next;

			sl->next = of_free_list;
			of_free_list = sl;

			sl = next;
		}
	}
	//*number = total_num;
	return  total_size * vm_page_size / 1024;
}
