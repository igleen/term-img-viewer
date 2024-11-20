#include <iostream>
#include <vector>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

// Define the implementation of stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

// Get terminal size
void getTerminalSize(int& width, int& height) {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        width = w.ws_col;
        height = w.ws_row;
    } else {
        width = 80;  // Default width
        height = 24; // Default height
    }
}

// Generate ANSI escape code for foreground and background color
std::string getANSIColor(unsigned char r, unsigned char g, unsigned char b, bool isForeground) {
    return isForeground
               ? "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m"
               : "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

// Display the image using box characters and ANSI colors
void displayImageAsColor(const std::string& filename) {
    int imgWidth, imgHeight, channels;
    unsigned char* image = stbi_load(filename.c_str(), &imgWidth, &imgHeight, &channels, 3);
    if (!image) {
        std::cerr << "Error: Failed to load image '" << filename << "'\n";
        return;
    }

    int termWidth, termHeight;
    getTerminalSize(termWidth, termHeight);

    // Adjust terminal height for box characters (each row represents two pixels)
    termHeight *= 2;

    // Calculate scaling factor
    float xScale = static_cast<float>(imgWidth) / termWidth;
    float yScale = static_cast<float>(imgHeight) / termHeight;
    float scale = std::max(xScale, yScale); // Scale uniformly to fit

    int scaledWidth = static_cast<int>(imgWidth / scale);
    int scaledHeight = static_cast<int>(imgHeight / scale);

    // Render the image
    for (int y = 0; y < scaledHeight; y += 2) { // Each row represents two vertical pixels
        for (int x = 0; x < scaledWidth; ++x) {
            // Map scaled coordinates to original image
            int topIndex = (static_cast<int>(y * scale) * imgWidth + static_cast<int>(x * scale)) * 3;
            int bottomIndex = (static_cast<int>((y + 1) * scale) * imgWidth + static_cast<int>(x * scale)) * 3;

            // Top pixel color
            unsigned char r1 = image[topIndex];
            unsigned char g1 = image[topIndex + 1];
            unsigned char b1 = image[topIndex + 2];

            // Bottom pixel color
            unsigned char r2 = (y + 1 < scaledHeight) ? image[bottomIndex] : 0;
            unsigned char g2 = (y + 1 < scaledHeight) ? image[bottomIndex + 1] : 0;
            unsigned char b2 = (y + 1 < scaledHeight) ? image[bottomIndex + 2] : 0;

            // Output colored box character
            std::cout << getANSIColor(r1, g1, b1, false)    // Background color
                      << getANSIColor(r2, g2, b2, true)     // Foreground color
                      << "▄";                               // Lower half block
        }
        std::cout << "\033[0m\n"; // Reset color and move to the next line
    }

    stbi_image_free(image);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <image_file>\n";
        return 1;
    }

    displayImageAsColor(argv[1]);
    return 0;
}

