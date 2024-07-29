
#include "mbt_tracker_file_transfer.h"
#include "mbt_tracker.h"

#include <unistd.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>

char* mbt_file_transfer_tracker_direction_to_string(enum mbt_file_transfer_tracker_direction direction)
{
	switch (direction)
	{
	case UPLOAD: return "UPLOAD";
	case DOWNLOAD: return "DOWNLOAD";
	default: return "UNKNOWN";
	}
}

static int add_file_transfer_direction(char* output, enum mbt_file_transfer_tracker_direction direction)
{
	char* direction_str = mbt_file_transfer_tracker_direction_to_string(direction);
	strcpy(output, direction_str);
	return strlen(direction_str);
}

static int stringify_entry(char* output, const mbt_file_transfer_tracker_entry* entry)
{
	char* start = output;
	output += mbt_tracker_set_timestamp(output);
	output += mbt_tracker_set_separator(output);
	strcpy(output, entry->connection_name);
	output += strlen(entry->connection_name);
	output += mbt_tracker_set_separator(output);
	strcpy(output, entry->username);
	output += strlen(entry->username);
	output += mbt_tracker_set_separator(output);
	output += add_file_transfer_direction(output, entry->direction);
	output += mbt_tracker_set_separator(output);
	strcpy(output, entry->filename);
	output += strlen(entry->filename);
	output += mbt_tracker_set_separator(output);
	output += mbt_tracker_set_human_readable_size(output, entry->size);
	output += mbt_tracker_set_terminator(output);

	return (output - start) / sizeof(char);
}

void mbt_report_file_transfer_tracker(const mbt_file_transfer_tracker_entry* entry)
{
	if (entry == NULL || entry->filename == NULL || entry->username == NULL || entry->connection_name == NULL) {
		return;
	}

	char entry_str[2048];
	int size = stringify_entry(entry_str, entry);

	FILE* output_csv =
		mbt_tracker_open_csv("downloads-%s.csv", "Timestamp, Connection, Username, Direction, Filename, Size\n");
	if (output_csv == NULL) {
		return;
	}

	fputs(entry_str, output_csv);
	fclose(output_csv);

	if (mbt_print_new_tracker_entries()) {
		fwrite("mbt: ", sizeof(char), 5, stderr);
		fwrite(entry_str, sizeof(char), size, stderr);
	}
}
