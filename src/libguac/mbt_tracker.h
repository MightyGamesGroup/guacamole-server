
#ifndef __MBT_TRACKER_H__
# define __MBT_TRACKER_H__

#include <stdio.h>

FILE* mbt_tracker_open_csv(const char* filename_format, const char* header);
int mbt_tracker_set_timestamp(char* output);
int mbt_tracker_set_separator(char* output);
int mbt_tracker_set_terminator(char* output);
int mbt_tracker_set_human_readable_size(char* output, size_t size);

int mbt_print_new_tracker_entries();

#endif