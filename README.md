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

## Example Applications

Ignite includes several example applications to showcase its capabilities:

-   **Todo List**: A practical example demonstrating interactive components, state management, and dynamic UI updates.
-   **Counter Application**: A simple application illustrating basic component interaction and state changes.
-   **Paragraph Test**: Focuses on advanced text rendering and layout.
-   **Scrolling Test**: Highlights the implementation of scrollable UI elements.

Ignite aims to provide a robust and flexible foundation for developing custom desktop UI applications with a focus on performance and modern design principles.