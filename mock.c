#include <stdio.h>
#include <stdlib.h>

int main() {
    char command[256];
    char url[] = "https://www.youtube.com/watch?v=TB54dZkzZOY";
    char output[] = "output.mp4";

    // Construct the yt-dlp command
    snprintf(command, sizeof(command), "yt-dlp -o %s %s", output, url);

    // Execute the command
    int result = system(command);
    if (result == 0) {
        printf("Download successful!\n");
    } else {
        printf("Download failed.\n");
    }

    return 0;
}
