/**
 * @file main.c
 * @author Victor Matos (@SpacemanY2K38)
 * @date 2023-10-25
 * @brief Phree-V: A simple YouTube video downloader using yt-dlp.
 *
 * This program extracts the video ID from a YouTube URL, constructs a yt-dlp
 * command, and downloads the video in the specified format.
 *
 * @license GPL-3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h> // For _mkdir on Windows
#define mkdir(dir, mode) _mkdir(dir) // Windows mkdir doesn't take a mode parameter
#else
#include <sys/stat.h> // For mkdir on Unix-like systems
#endif

/**
 * Extracts the video ID from a YouTube URL.
 *
 * @param url The YouTube URL containing the video ID.
 * @return A dynamically allocated string containing the video ID, or NULL if not found.
 */
char* extract_video_id(const char* url) {
    const char* v_param = "v="; // Parameter for video ID
    char* video_id = NULL;

    const char* start = strstr(url, v_param);
    if (start) {
        start += strlen(v_param); // Move past "v="
        const char* end = strchr(start, '&'); // Find the next "&" (if any)
        if (end) {
            video_id = (char*)malloc(end - start + 1);
            if (video_id) {
                strncpy(video_id, start, end - start);
                video_id[end - start] = '\0'; // Null-terminate the string
            }
        } else {
            video_id = strdup(start); // No "&" found, copy the rest of the string
        }
    }

    return video_id;
}

/**
 * Main function of the program.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return 0 on success, 1 on error.
 */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <youtube_link> -f=<format>\n", argv[0]);
        return 1;
    }

    char* video_id = extract_video_id(argv[1]);
    if (!video_id) {
        fprintf(stderr, "Error: Invalid YouTube URL.\n");
        return 1;
    }

    char* format = NULL;
    if (strncmp(argv[2], "-f=", 3) == 0) {
        format = argv[2] + 3; // Skip "-f="
    } else {
        fprintf(stderr, "Error: Invalid format argument. Use -f=<format>.\n");
        free(video_id);
        return 1;
    }

    char command[256];
    snprintf(command, sizeof(command), "yt-dlp -f %s https://www.youtube.com/watch?v=%s", format, video_id);

    printf("Executing: %s\n", command);
    int result = system(command);
    if (result == 0) {
        printf("Download successful!\n");
    } else {
        printf("Download failed.\n");
    }

    free(video_id);
    return 0;
}
