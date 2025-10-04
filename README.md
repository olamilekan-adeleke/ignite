# Ignite
Ignite is a C++ UI framework designed for building desktop applications with a modern component-based architecture. It leverages the Skia graphics library for high-performance rendering and GLFW for robust window management and input handling.

## Core Technologies
-   **Skia**: High-performance 2D graphics library for rendering.
-   **GLFW**: Lightweight, open-source library for creating windows with OpenGL contexts and handling user input.
-   **OpenGL**: Used for graphics context management.
-   **Boost.Asio**: Provides networking capabilities, though its primary use in the UI framework context might be for specific features or extensions.

## Project Structure
The project is organized to provide a clear separation of concerns:

-   `core/`: Contains fundamental utilities and base classes.
-   `ui_components/`: Houses the UI component system, including basic elements, interactive components, and layout managers.
-   `skia/`: Integrates the Skia rendering engine.
-   `window/`: Manages window creation and events using GLFW.
-   `example/`: Demonstrates various features and usage patterns through sample applications (e.g., Todo List, Counter App).

## Features
Ignite aims to provide a robust and flexible foundation for developing custom desktop UI applications with a focus on performance and modern design principles.

### UI Component Progress
Here's a breakdown of the current UI component plans and current implementation status:

#### Layout Components
- [x] Container - Basic wrapper with padding/margin
- [x] Horizontal Flex Box
- [x] Vertical Flex Box
- [ ] GridLayout - 2D grid positioning
- [ ] StackLayout - Overlay children (z-index stacking)
- [ ] Spacer - Fixed or flexible spacing element

#### Basic Elements
- [x] Text - Text rendering with styling
- [x] Image - Bitmap/vector image display
- [x] Separator - Visual dividers (horizontal/vertical)

#### Interactive Components
- [x] Button - Clickable button with states
- [x] Toggle/Checkbox - Boolean input
- [ ] RadioButton - Single selection from a group
- [ ] Slider - Numeric range input
- [x] TextInput - Text entry field
- [ ] DropDown - Selection from list

#### Specialized Widgets
- [ ] ProgressBar - Progress indication
- [ ] LoadingSpinner - Activity indicator
- [ ] Modal/Dialog - Overlay windows
- [x] ListView - Scrollable item list

#### Utility Components
- [x] Clip - Bounds clipping for children
- [ ] Transform - Rotation, scale, skew
- [x] Opacity - Alpha transparency
- [x] Background - Background colors/gradients
- [ ] Shadow - Drop shadow effects
- [ ] Border - Border drawing wrapper

#### Navigation Components
- [ ] NavigationBar - Top navigation
- [ ] Sidebar - Collapsible side panel

## Contributing
We welcome contributions to the Ignite project! If you're interested in improving this framework, please consider the following:

-   **Bug Reports**: If you find a bug, please open an issue on the GitHub repository with a clear description and steps to reproduce.
-   **Feature Requests**: We're open to new ideas! Feel free to suggest new features or enhancements by opening an issue.
-   **Pull Requests**: If you'd like to contribute code, please fork the repository, create a new branch, and submit a pull request. Ensure your code adheres to the project's coding style and includes appropriate tests.

Before contributing, please make sure to:

1.  Read the `README.md` thoroughly.
2.  Familiarize yourself with the project's architecture and existing components.
3.  Follow the existing code style and conventions.

Thank you for helping to make Ignite better!


## Getting Started
To build and run the Ignite project, follow these steps:

### Prerequisites
Make sure you have the following installed:

-   **CMake**: Version 3.10 or higher.
-   **Vcpkg**: For managing C++ libraries.
-   **C++ Compiler**: A C++17 compatible compiler (e.g., GCC, Clang, MSVC).

### Dependencies
The project uses [Vcpkg](https://vcpkg.io/) for dependency management. The required libraries are:

-   `boost-asio`, `fmt`, `glfw3`, `opengl`, `skia`

To install these dependencies using Vcpkg, navigate to the project root directory and run:

```bash
vcpkg install
```

### Building the Project
 WILL ADD LATER