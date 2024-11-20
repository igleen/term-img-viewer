# Terminal Image Viewer

A lightweight, terminal-based image viewer written in C++. This program uses ASCII box characters and ANSI escape codes to display images in full color directly in the terminal. It works without requiring an X server, making it ideal for minimalist setups or remote environments.

## Features
- Renders images as colored ASCII art using the `▄` (lower half block) character.
- Fits images to the terminal size while maintaining the original aspect ratio.
- Supports full 24-bit color (true color) rendering using ANSI escape codes.
- Works on any Linux terminal that supports ANSI color codes.

## Requirements
- A Linux system with a modern terminal emulator (e.g., `gnome-terminal`, `xterm`, `kitty`) that supports 24-bit colors.
- A C++ compiler supporting C++11 or newer.
- **stb_image.h**: A lightweight image-loading library included in this project.

## Installation

1. Clone the repository or download the source files.
   ```bash
   git clone https://github.com/igleen/term-img-viewer.git
   cd term-img-viewer
   ```

2. Compile the program:
   ```bash
   g++ -o img-sh main.cpp -std=c++11
   ```

## Usage
```bash
./img-sh <image_file>
```

- `<image_file>`: Path to the image file you want to view. The program supports common image formats like `.jpg`, `.png`, `.bmp`, etc.

The output will display the image rendered in full color within your terminal, scaled to fit the terminal's size.

## How It Works
1. **Image Decoding**: The program uses `stb_image.h` to load and decode the image into raw pixel data.
2. **Terminal Dimensions**: It dynamically retrieves the terminal's size using the `ioctl` system call.
3. **Scaling**: The image is resized to fit within the terminal dimensions, maintaining its aspect ratio.
4. **Rendering**: Each character cell represents two vertical pixels, with the top pixel rendered as the foreground color and the bottom pixel as the background color using the `▄` character and ANSI escape codes.

## Known Limitations
- Requires a terminal that supports 24-bit colors.
- Rendering performance may vary for very large images or small terminal windows.
- Images with extreme detail may lose some clarity due to terminal resolution limitations.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.
