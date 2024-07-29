
#ifndef __MBT_TRACKER_FILE_TRANSFER_H__
# define __MBT_TRACKER_FILE_TRANSFER_H__

#include <stddef.h>

enum mbt_file_transfer_tracker_direction
{
	UPLOAD,
	DOWNLOAD
};

char* mbt_file_transfer_tracker_direction_to_string(enum mbt_file_transfer_tracker_direction direction);

typedef struct s_mbt_file_transfer_tracker_entry
{
	char* filename;
	size_t size;
	const char* username;
	enum mbt_file_transfer_tracker_direction direction;
	const char* connection_name;
} mbt_file_transfer_tracker_entry;

void mbt_report_file_transfer_tracker(const mbt_file_transfer_tracker_entry* entry);

#endif