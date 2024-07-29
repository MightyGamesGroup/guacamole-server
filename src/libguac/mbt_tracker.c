
#include "mbt_tracker.h"

#include <time.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

static void create_csv_full_path(char* output, size_t max_size, const char* filename_format)
{
	const char* tracker_dir = getenv("MBT_GUACD_TRACKER_OUTPUT");
	if (tracker_dir == NULL || tracker_dir[0] == '\0') {
		tracker_dir = "/tmp";
	}

	const int max_date_size = 11;
	char date[max_date_size];
	time_t now = time(NULL);
	struct tm* tm = localtime(&now);
	strftime(date, max_date_size, "%Y-%m-%d", tm);

	// Add directory (with trailing slash if needed)
	int index;
	if (tracker_dir[strlen(tracker_dir) - 1] == '/') {
		strcpy(output, tracker_dir);
		index = strlen(tracker_dir);
	}
	else
		index = snprintf(output, max_size, "%s/", tracker_dir);

	// Add filename (with today's date)
	index += snprintf(output + index, max_size, filename_format, date);

	output[index] = '\0';
}

FILE* mbt_tracker_open_csv(const char* filename_format, const char* header)
{
	char csv_file_path[PATH_MAX];
	create_csv_full_path(csv_file_path, PATH_MAX, filename_format);

	// if file does not exist, create it and add the csv header
	if (access(csv_file_path, F_OK) == -1)
	{
		FILE* log_file_fd = fopen(csv_file_path, "w");
		if (log_file_fd == NULL) {
			return NULL;
		}

		fputs(header, log_file_fd);
		fputc('\n', log_file_fd);
		return log_file_fd;
	}
	return fopen(csv_file_path, "a");
}

int mbt_tracker_set_timestamp(char* output)
{
	time_t now = time(NULL);
	struct tm* tm = localtime(&now);
	return strftime(output, 20, "%Y-%m-%d %H:%M:%S", tm);
}

int mbt_tracker_set_separator(char* output)
{
	output[0] = ',';
	output[1] = ' ';
	return 2;
}

int mbt_tracker_set_terminator(char* output)
{
	output[0] = '\n';
	output[1] = '\0';
	return 1;
}

int mbt_tracker_set_human_readable_size(char* output, size_t size)
{
	const char* units[] = { "B", "KB", "MB", "GB", "TB" };
	int unit = 0;
	double size_d = size;
	while (size_d >= 1024 && unit < 4) {
		size_d /= 1024;
		unit++;
	}
	return snprintf(output, 20, "%.2f %s", size_d, units[unit]);
}

int mbt_print_new_tracker_entries()
{
	char* env = getenv("MBT_GUAC_PRINT_TRACKER_ENTRIES");
	return (env != NULL && strcmp(env, "1") == 0) ? 1 : 0;
}