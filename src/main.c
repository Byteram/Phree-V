/**
 * @file main.c
 * @author Victor Matos (@SpacemanY2K38)
 * @date 2025-02-19 (yyyy-mm-dd)
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

char* extract_video_id(const char* url) {
    const char* v_param = "v="; 
    char* video_id = NULL;

    const char* start = strstr(url, v_param);
    if (start) {
        start += strlen(v_param); // Move past "v="
        const char* end = strchr(start, '&'); // Find the next "&" (if any)
        if (end) {
            // Allocate memory for the video ID
            video_id = (char*)malloc(end - start + 1);
            if (video_id) {
                strncpy(video_id, start, end - start);
                video_id[end - start] = '\0'; // Null-terminate the string
            }
        } else {
            // No "&" found, copy the rest of the string
            video_id = strdup(start);
        }
    }

    return video_id;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <youtube_link> -f=<format>\n", argv[0]);
        return 1;
    }

    // Parsing, Extract the video ID from the URL
    char* video_id = extract_video_id(argv[1]);
    if (!video_id) {
        fprintf(stderr, "Error: Invalid YouTube URL.\n");
        return 1;
    }

    // Parse the format argument (-f=mp4, -f=webm, etc.)
    char* format = NULL;
    if (strncmp(argv[2], "-f=", 3) == 0) {
        format = argv[2] + 3; // Skip "-f="
    } else {
        fprintf(stderr, "Error: Invalid format argument. Use -f=<format>.\n");
        free(video_id);
        return 1;
    }

    // Construct the yt-dlp command
    char command[256];
    snprintf(command, sizeof(command), "yt-dlp -f %s https://www.youtube.com/watch?v=%s", format, video_id);

    // Execute the command
    printf("Executing: %s\n", command);
    int result = system(command);
    if (result == 0) {
        printf("Download successful!\n");
    } else {
        printf("Download failed.\n");
    }

    // Clean up
    free(video_id);
    return 0;
}
