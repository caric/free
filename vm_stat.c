#include <vm_stat.h>

/*
 * Copyright (c) 1999 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * "Portions Copyright (c) 1999 Apple Computer, Inc.  All Rights
 * Reserved.  This file contains Original Code and/or Modifications of
 * Original Code as defined in and that are subject to the Apple Public
 * Source License Version 1.0 (the 'License').  You may not use this file
 * except in compliance with the License.  Please obtain a copy of the
 * License at http://www.apple.com/publicsource and read it before using
 * this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License."
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*
 *	File:	vm_stat.c
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1986, Avadis Tevanian, Jr.
 *
 *
 *	Display Mach VM statistics.
 *
 ************************************************************************
 * HISTORY
 *  6-Jun-86  Avadis Tevanian, Jr. (avie) at Carnegie-Mellon University
 *	Use official Mach interface.
 *
 *  25-mar-99	A.Ramesh at Apple
 *		Ported to MacOS X
 *  13-nov-2003	Kevin Geiss
 *		Modified for use by the 'free' front end.
 ************************************************************************
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <mach/mach.h>

// Global variables we want to fill in for free.
/* obsolete */
unsigned long kb_main_shared = 0;
/* old but still kicking -- the important stuff */
unsigned long kb_main_wired = 0;
unsigned long kb_main_inactive = 0;
unsigned long kb_main_free = 0;
unsigned long kb_main_total = 0;
unsigned long kb_swap_free = 0;
unsigned long kb_swap_total = 0;
/* derived values */
unsigned long kb_swap_used = 0;
unsigned long kb_main_used = 0;

vm_statistics_data_t	vm_stat, last;
int	percent;
int	delay;
char	*pgmname;
mach_port_t myHost;
int pageSize = 4096; 	/* set to 4k default */

void usage();
void banner();
void snapshot();
void pstat(char *str, int n);
void print_stats();
void get_stats(struct vm_statistics *stat);

void meminfo()
{
	myHost = mach_host_self();

	// Get the page size.
	if(host_page_size(mach_host_self(), &pageSize) != KERN_SUCCESS) {
		fprintf(stderr, "%s: failed to get pagesize; defaulting to 4K.\n", pgmname);
		pageSize = 4096;
	}	

	snapshot();
}

void
snapshot()
{

	get_stats(&vm_stat);
	pageSize >>= 10;
	kb_main_total = (vm_stat.free_count +
			vm_stat.active_count +
			vm_stat.inactive_count +
			vm_stat.wire_count) * pageSize;
	kb_main_used = (vm_stat.active_count +
			vm_stat.inactive_count +
			vm_stat.wire_count) * pageSize;
	kb_main_free = vm_stat.free_count * pageSize;
	kb_main_inactive = vm_stat.inactive_count * pageSize;
	kb_main_wired = vm_stat.wire_count * pageSize;

	kb_main_shared = get_shared_kb();
}

void
pstat(str, n)
	char	*str;
	int	n;
{
	printf("%-25s %10d.\n", str, n);
}

void
get_stats(stat)
	struct vm_statistics	*stat;
{
	int count = HOST_VM_INFO_COUNT;
	if (host_statistics(myHost, HOST_VM_INFO, stat,&count) != KERN_SUCCESS) {
		fprintf(stderr, "%s: failed to get statistics.\n", pgmname);
		exit(2);
	}
	if (stat->lookups == 0)
		percent = 0;
	else
		percent = (stat->hits*100)/stat->lookups;
}
