# UI Components Documentation

This document provides detailed explanations and usage instructions for each UI component available in the Ignite framework.

## Table of Contents

- [Introduction to UI Components](#introduction-to-ui-components)
- [Layout Components](#layout-components)
  - [View](#view)
  - [Horizontal Flex Box](#horizontal-flex-box)
  - [Vertical Flex Box](#vertical-flex-box)

- [Basic Elements](#basic-elements)
  - [Paragraph Builder](#paragraph-builder)
  - [Text](#text)
  - [TextRenderer](#textrenderer)
  - [Image](#image)
  - [Separator](#separator)

- [Interactive Components](#interactive-components)
  - [CheckBox](#checkbox)
  - [Button](#button)
  - [TextFieldRenderer/TextInput](#textfieldrenderer)
  - [Toggle/Checkbox](#checkbox)

- [Specialized Widgets](#specialized-widgets)
  - [ListView](#listview)
  - [ScrollView](#scrollview)

- [Utility Components](#utility-components)
  - [Clip](#clip)
  - [Opacity](#opacity)
  - [Background](#background)

- [Core Managers](#core-managers)
  - [UIManager](#uimanager)
  - [UICacheManager](#uicachemanager)



## Introduction to UI Components

Ignite's UI components are the fundamental building blocks for creating interactive user interfaces. At its core, the `UIComponent` class serves as the base for all UI elements, providing essential functionalities through inheritance from `UIRenderObject`, `UITapHandler`, `UITextInputHandler`, and `UIHoverHandler`. This design enables components to handle rendering, user interactions like taps and hover events, and text input.

Key aspects of Ignite's UI component system include:

-   **Component Hierarchy**: UI components can contain other components as children, forming a hierarchical, tree-like structure. This allows for the creation of complex layouts by nesting simpler components.
-   **Global Positioning**: Each component manages a `globalOffset_` to determine its absolute position on the screen. This offset is updated recursively from parent components, ensuring accurate placement within the UI.
-   **Event Propagation**: User interactions such as taps, key presses, and hover movements are propagated through the component hierarchy. This enables individual components to respond to specific events relevant to their functionality.
-   **Efficient Updates**: The `markHasDirty` mechanism indicates that components can be marked for re-rendering or re-layout, suggesting an optimized approach to updating the UI only when necessary.

This modular and hierarchical approach allows for flexible and efficient UI development within the Ignite framework.

## Layout Components

### View

**Description**: A fundamental UI component that acts as a container for a single child, providing essential styling and layout properties such as padding, margin, background color, and border radius. It also handles tap events and participates in the layout system.

**Parameters**:

-   `insets`: `UIEdgeInsets` to apply padding around the child.
-   `margin`: `UIEdgeInsets` to apply margin around the view itself.
-   `backgroundColor`: `Color` to fill the view's background.
-   `borderRadius`: `float` value for rounding the corners of the view's background.
-   `antiAlias`: `bool` to enable or disable anti-aliasing for the background.
-   `tappable`: `bool` to enable or disable tap event handling for the view.
-   `onTap`: `TapListener` callback function to be executed when the view is tapped.
-   `mainAxisSize`: `MainAxisSize` to control how the view sizes itself along the main axis.
-   `child`: `std::shared_ptr<UIComponent>` representing the single child component within this view.

**Usage**:

```cpp
// Example usage of View
auto myView = UI::UIView({
    .insets = {.top = 10, .left = 10, .bottom = 10, .right = 10},
    .margin = {.top = 20, .left = 20},
    .backgroundColor = Color::Blue(),
    .borderRadius = 8.0f,
    .tappable = true,
    .onTap = []() {
        LOG_INFO("View tapped!");
    },
    .child = UI::Text("Hello from inside a View!", {.color = Color::White()}),
});
```

### Horizontal Flex Box

**Description**: A layout component that arranges its children in a single horizontal row, allowing for flexible spacing and alignment. It's built upon the `FlexBox` component with a predefined horizontal axis.

**Parameters**:

-   `spacing`: `float` value representing the space between children.
-   `crossAxisAlignment`: `CrossAxisAlignment` to control how children are aligned along the cross axis (vertically in this case).
-   `children`: `std::vector<std::shared_ptr<UIComponent>>` containing the child components to be arranged horizontally.

**Usage**:

```cpp
// Example usage of HFlexBox
auto myHFlexBox = UI::HFlexBox({
    .spacing = 10.0f,
    .crossAxisAlignment = CrossAxisAlignment::CENTER,
    .children = {
        UI::Text("Item 1"),
        UI::Text("Item 2"),
        UI::Text("Item 3"),
    },
});
```

### Vertical Flex Box

**Description**: A layout component that arranges its children in a single vertical column, allowing for flexible spacing and alignment. It's built upon the `FlexBox` component with a predefined vertical axis.

**Parameters**:

-   `spacing`: `float` value representing the space between children.
-   `crossAxisAlignment`: `CrossAxisAlignment` to control how children are aligned along the cross axis (horizontally in this case).
-   `children`: `std::vector<std::shared_ptr<UIComponent>>` containing the child components to be arranged vertically.

**Usage**:

```cpp
// Example usage of VFlexBox
auto myVFlexBox = UI::VFlexBox({
    .spacing = 5.0f,
    .crossAxisAlignment = CrossAxisAlignment::START,
    .children = {
        UI::Text("Line 1"),
        UI::Text("Line 2"),
        UI::Text("Line 3"),
    },
});
```

### Paragraph Builder

**Description**: The `ParagraphBuilder` class is a crucial component for advanced text rendering in Ignite. It leverages Skia's text layout engine to handle complex text formatting, including line breaking, text alignment, and styling. Its primary purpose is to prepare text for rendering by calculating its intrinsic size and layout based on given constraints and styles. This separation of concerns allows `TextRenderer` to focus solely on drawing the pre-laid-out text.

**Implementation Details**:
-   `ParagraphBuilder` encapsulates Skia's `skia::textlayout::Paragraph` and `skia::textlayout::FontCollection` to manage fonts and text layout.
-   It takes a string and a `TextStyle` object to configure the text's appearance (color, font size, weight, decoration, etc.) and layout properties (text alignment, max lines, ellipsis).
-   The `buildParagraph()` method is responsible for constructing the Skia `Paragraph` object, applying the specified styles and text.
-   `getIntrinsicSize()` calculates the required size for the text based on the provided constraints, ensuring proper text wrapping and sizing.
-   `layout()` performs the actual text layout, determining the position of each glyph.
-   `draw()` renders the laid-out text onto the canvas at a specified point.
-   The `setText()` method allows for dynamic updates of the text content, triggering a re-build of the internal Skia paragraph.

**Why it was added**:
The `ParagraphBuilder` was introduced to provide robust and flexible text rendering capabilities. Directly handling text layout with Skia's low-level APIs can be complex. By abstracting this functionality into `ParagraphBuilder`, `TextRenderer` (and other components that might need text) can easily render text with various styles and layouts without needing to manage the intricacies of Skia's text engine. This promotes cleaner code, better maintainability, and more consistent text rendering across the framework.


### TextRenderer

**Description**: The `TextRenderer` component is a specialized UI element responsible for rendering text on the screen. It acts as a wrapper around `ParagraphBuilder`, delegating the complex tasks of text layout and styling to it. `TextRenderer` provides a convenient way to display text within the UI, inheriting from `UIComponent` to integrate seamlessly into the component hierarchy and participate in layout and drawing cycles.

**Parameters**:

-   `text`: `std::string` containing the text content to be displayed.
-   `style`: `TextStyle` object defining the visual properties of the text, such as color, font size, weight, and text decoration.

**Usage**:

```cpp
// Example usage of TextRenderer
auto myTextRenderer = UI::TextRenderer("Hello, Ignite TextRenderer!", {
    .color = Color::Red(),
    .fontSize = 30,
    .weight = FontWeight::Bold,
    .textAlignment = TextAlign::CENTER,
});

auto multiLineTextRenderer = UI::TextRenderer(
    "This is a longer piece of text that demonstrates how TextRenderer "
    "handles multi-line text and various styling options.",
    {
        .color = Color::Blue(),
        .fontSize = 18,
        .maxLines = 4,
        .overflow = TextOverflow::Ellipsis,
    }
);
```

### Image

**Description**: The `Image` component is used to display raster images (e.g., PNG, JPEG) within the UI. It supports loading images from a specified file path and allows for control over its dimensions and opacity. The component utilizes an internal cache to efficiently manage image data, preventing redundant loading of the same image.

**Parameters**:

-   `path`: `std::string` representing the file path to the image asset. The path is relative to the project's root directory.
-   `width`: `float` value to explicitly set the width of the image. If 0.0f, the image's intrinsic width will be used.
-   `height`: `float` value to explicitly set the height of the image. If 0.0f, the image's intrinsic height will be used.
-   `opacity`: `float` value between 0.0f and 1.0f to control the transparency of the image.

**Usage**:

```cpp
// Example usage of Image
auto myImage = UI::UIImage({
    .path = "assets/test_one.jpeg",
    .width = 100.0f,
    .height = 100.0f,
    .opacity = 0.8f,
});

auto fullSizeImage = UI::UIImage({
    .path = "assets/Check.png",
});
```

### Separator

**Description**: The `Separator` component is a simple UI element used to draw a horizontal or vertical line, typically to visually divide content. It can be configured with a specific thickness, axis (horizontal or vertical), and background color.

**Parameters**:

-   `axis`: `Axis` enum (`Axis::HORIZONTAL` or `Axis::VERTICAL`) to specify the orientation of the separator.
-   `thinkness`: `float` value representing the thickness of the separator line.
-   `backgroundColor`: `Color` to set the color of the separator.

**Usage**:

```cpp
// Example usage of Separator
auto horizontalSeparator = UI::Separator({
    .axis = Axis::HORIZONTAL,
    .thinkness = 2.0f,
    .backgroundColor = Color::LightGray(),
});

auto verticalSeparator = UI::Separator({
    .axis = Axis::VERTICAL,
    .thinkness = 1.0f,
    .backgroundColor = Color::DarkGray(),
});
```

### CheckBox

**Description**: The `CheckBox` component is an interactive UI element that allows users to toggle between checked and unchecked states. It supports custom styling, including size, radius, fill color, and tap events.

**Parameters**:

-   `enable`: `bool` to set the initial state of the checkbox (checked or unchecked).
-   `size`: `UISize` to specify the dimensions of the checkbox.
-   `radius`: `float` to set the corner radius of the checkbox.
-   `fillColor`: `Color` to define the fill color when the checkbox is checked.
-   `inactiveBorderColor`: `Color` to define the border color when the checkbox is unchecked.
-   `checkColor`: `Color` to set the color of the checkmark.
-   `onTap`: `TapListener` callback function to handle tap events.

**Usage**:

```cpp
// Example usage of CheckBox
auto myCheckBox = UI::CheckBox({
    .enable = true,
    .size = {25, 25},
    .radius = 4.0f,
    .fillColor = Color::Blue(),
    .checkColor = Color::White(),
    .onTap = [](const UITapEvent& e) {
        LOG_INFO("CheckBox tapped!");
    },
});
```

### Button

**Description**: The `Button` component is an interactive UI element that triggers an action when tapped. It supports custom styling, including insets, radius, fill color, and tap events.

**Parameters**:

-   `child`: `std::shared_ptr<UIComponent>` to specify the child component (e.g., text or icon) inside the button.
-   `enable`: `bool` to enable or disable the button.
-   `radius`: `float` to set the corner radius of the button.
-   `insets`: `UIEdgeInsets` to define padding around the child component.
-   `fillColor`: `Color` to set the background color of the button.
-   `inactiveBorderColor`: `Color` to define the border color when the button is disabled.
-   `onTap`: `TapListener` callback function to handle tap events.

**Usage**:

```cpp
// Example usage of Button
auto myButton = UI::Button({
    .child = UI::Text("Click Me", {.color = Color::White(), .fontSize = 16}),
    .insets = {.top = 8, .left = 16, .bottom = 8, .right = 16},
    .fillColor = Color::Blue(),
    .onTap = [](const UITapEvent& e) {
        LOG_INFO("Button tapped!");
    },
});
```

### TextFieldRenderer

**Description**: The `TextFieldRenderer` component is an interactive UI element for text input. It supports custom styling, including padding, background color, border radius, placeholder text, and event handlers for text changes.

**Parameters**:

-   `padding`: `UIEdgeInsets` to define padding around the text content.
-   `backgroundColor`: `Color` to set the background color of the text field.
-   `borderColor`: `Color` to define the border color.
-   `radius`: `float` to set the corner radius of the text field.
-   `placeholder`: `std::string` to specify placeholder text when the field is empty.
-   `placeholderStyle`: `TextStyle` to style the placeholder text.
-   `value`: `std::string` to set the initial text content.
-   `textStyle`: `TextStyle` to style the entered text.
-   `width`: `float` to specify the width of the text field.
-   `minHeight`: `float` to set the minimum height of the text field.
-   `multiline`: `bool` to enable or disable multiline input.
-   `onChanged`: `ValueChangedListener<std::string>` callback function to handle text changes.

**Usage**:

```cpp
// Example usage of TextFieldRenderer
auto myTextField = UI::TextFieldRenderer({
    .padding = {.top = 8, .left = 12, .bottom = 8, .right = 12},
    .backgroundColor = Color::White(),
    .borderColor = Color::LightGray(),
    .radius = 6.0f,
    .placeholder = "Enter text...",
    .placeholderStyle = {.color = Color::Gray(), .fontSize = 14},
    .textStyle = {.color = Color::Black(), .fontSize = 14},
    .onChanged = [](const std::string& value) {
        LOG_INFO("Text changed: {}", value);
    },
});
```

## Specialized Widgets

### ScrollView

**Description**: The `ScrollView` component enables its child content to be scrollable, either vertically or horizontally. It's designed to handle content larger than its visible area, providing a smooth scrolling experience.

**Parameters**:

-   `child`: `std::shared_ptr<UIComponent>` representing the single child component that will be made scrollable.
-   `direction`: `ScrollDirection` enum (`ScrollDirection::Vertical` or `ScrollDirection::Horizontal`) to specify the primary scrolling axis. Defaults to `ScrollDirection::Vertical`.

**Usage**:

```cpp
// Example usage of ScrollView with vertical scrolling
auto verticalScrollView = UI::ScrollView({
    .child = UI::VFlexBox({
        .spacing = 10,
        .children = {
            UI::Text("Scrollable Item 1"),
            UI::Text("Scrollable Item 2"),
            // ... many more items ...
            UI::Text("Scrollable Item N"),
        },
    }),
});

// Example usage of ScrollView with horizontal scrolling
auto horizontalScrollView = UI::ScrollView({
    .direction = ScrollDirection::Horizontal,
    .child = UI::HFlexBox({
        .spacing = 10,
        .children = {
            UI::Text("Wide Item 1"),
            UI::Text("Wide Item 2"),
            // ... many more wide items ...
            UI::Text("Wide Item N"),
        },
    }),
});
```
## Utility Components

### Opacity

**Description**: The `Opacity` component controls the transparency of its child component. It applies a uniform opacity value to its child, making it partially or fully transparent.

**Parameters**:

-   `opacity`: `float` value between 0.0f and 1.0f, where 0.0f is fully transparent and 1.0f is fully opaque. Values outside this range will be clamped.
-   `child`: `std::shared_ptr<UIComponent>` representing the single child component whose opacity will be controlled.

**Usage**:

```cpp
// Example usage of Opacity
auto semiTransparentText = UI::Opacity({
    .opacity = 0.5f,
    .child = UI::Text("I am semi-transparent", {.color = Color::Black()}),
});

auto fullyOpaqueImage = UI::Opacity({
    .opacity = 1.0f,
    .child = UI::UIImage({.path = "assets/test_one.jpeg"}),
});
```

---

## Core Managers

### UIManager

**Description**: The `UIManager` is a singleton class responsible for managing the overall UI state, handling user input events (tap, keyboard, mouse), and orchestrating the UI component tree. It acts as the central hub for UI interactions and rendering preparation.

**Key Responsibilities**:

-   **Singleton Access**: Provides a global access point via `UIManager::instance()`.
-   **Font Management**: Initializes and manages `SkFontMgr` and `SkTypeface` for consistent text rendering across the application.
-   **Event Handling**: Dispatches `UITapEvent`, `KeyEvent`, `CharEvent`, and mouse events to the appropriate UI components in the tree.
-   **UI Tree Management**: Sets the root of the UI component tree (`currentTreeRoot_`), performs layout calculations, and efficiently updates the UI by diffing the new tree against the previous one (`diffAndRebuild`).
-   **Focus Management**: Provides mechanisms to request, release, and manage focus for interactive UI components.
-   **Hover Management**: Tracks the currently hovered UI component and triggers `onHoverEnter` and `onHoverExit` events.

**How it Works**:

1.  **Initialization**: Upon first access, the `UIManager` constructor initializes the Skia font manager and attempts to load a default typeface. If a platform-specific font manager is available, it's used; otherwise, a fallback mechanism is employed.
2.  **UI Tree Setup**: The `setTree()` method is called with a new UI component tree. It updates the global offsets of all components, performs a layout pass, and then intelligently rebuilds or updates only the necessary parts of the UI by comparing the new tree with the previous one.
3.  **Event Processing**: When user input events occur (e.g., a tap on the screen, a key press, or mouse movement), the `UIManager` receives these events and propagates them down the UI component tree. Components can then handle these events based on their implementation.
4.  **Layout and Drawing**: The `UIManager` orchestrates the layout phase, where each component determines its size and position, and the drawing phase, where components render themselves onto the canvas.

**Usage**:

The `UIManager` is primarily an internal component, and its methods are typically called by the application framework or event handlers. Developers interact with it indirectly by creating and managing UI component trees and responding to events within their custom components.

```cpp
// Example of how the UIManager might be used internally by the application framework
// (Not typically called directly by UI component developers)

// Get the singleton instance
UIManager& uiManager = UIManager::instance();

// Set a new UI tree (e.g., when navigating to a new screen)
std::shared_ptr<UIComponent> newRoot = /* ... create your UI component tree ... */;
uiManager.setTree(newRoot, screenWidth, screenHeight, true);

// Simulate a tap event (would typically come from an OS event handler)
// UITapEvent tapEvent = { .x = 100, .y = 200, .type = TapEventType::SINGLE };
// uiManager.sendTapEvent(tapEvent);

// Simulate a key event
// uiManager.sendKeyEvent(GLFW_KEY_A, 0, GLFW_PRESS, 0);
```

### UICacheManager

**Description**: The `UICacheManager` is a default-constructed class that currently serves as a placeholder for future UI-related caching mechanisms. In its current state, it does not implement any explicit caching logic but is designed to be extended for managing cached resources such as fonts, images, or layout results to optimize UI performance.

**Key Responsibilities (Future)**:

-   **Resource Caching**: Intended to cache frequently used UI resources to reduce load times and improve rendering efficiency.
-   **Cache Invalidation**: Will likely include mechanisms to invalidate cached items when underlying data changes.
-   **Memory Management**: Will manage the lifecycle of cached objects to prevent memory leaks and optimize memory usage.

**How it Works (Current)**:

In its current implementation, `UICacheManager` is a simple class with a default constructor. It does not perform any operations or manage any resources. Its presence suggests an architectural consideration for future performance optimizations.

**Usage**:

Currently, `UICacheManager` is not directly used or exposed for public interaction within the UI framework. Its methods and functionalities are expected to be developed as caching requirements evolve.

```cpp
// Example of UICacheManager declaration (from ui_manager.hpp)
// class UICacheManager {
// public:
//   UICacheManager() = default;
//   // Future caching methods would go here
// };
```

