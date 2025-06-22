#include "ui_components/column_component.hpp"

#include <chrono>
#include <cmath>
#include <memory>
#include <utility>

#include "ui_component.hpp"
#include "ui_edge_insets.hpp"

/**
 * @brief Adds a child UI component to the column.
 *
 * Transfers ownership of the given child component into the column's list of children.
 */
void Column::addChild(std::shared_ptr<UIComponent> child) { children_.push_back(std::move(child)); }

/**
 * @brief Constructs a Column with specified layout parameters and child components.
 *
 * Initializes the column's spacing, cross-axis alignment, and main-axis alignment from the provided parameters, and adds each child component to the column.
 *
 * @param params Layout configuration and initial child components for the column.
 */
Column::Column(const ColumnParams &params)
    : spacing_(params.spacing),
      cross_axis_alignment_(params.crossAxisAlignment),
      main_axis_alignment_(params.mainAxisAlignment) {
  for (const auto &c : params.children) {
    addChild(c);
  }
}

/**
 * @brief Calculates the horizontal position for a child component based on cross-axis alignment.
 *
 * Determines the x-coordinate where a child should be placed within a parent container, according to the specified cross-axis alignment (start, center, or end).
 *
 * @param axis The cross-axis alignment (start, center, or end).
 * @param parentWidth The width of the parent container.
 * @param childWidth The width of the child component.
 * @return float The computed x-coordinate for the child component.
 */
inline float getXPosition(CrossAxisAlignment axis, float parentWidth, float childWidth) {
  switch (axis) {
    case CrossAxisAlignment::START:
      return 0;
    case CrossAxisAlignment::CENTER:
      return (parentWidth - childWidth) / 2.0f;
    case CrossAxisAlignment::END:
      return parentWidth - childWidth;
  }
}

/**
 * @brief Arranges child components vertically within the column, applying spacing and horizontal alignment.
 *
 * Lays out each child with the given parent width, determines the maximum child width for the column, and positions children vertically with the configured spacing. Each child is horizontally aligned according to the column's cross-axis alignment setting. Updates the column's width and height to fit its children.
 */
void Column::layout(float parentWidth, float parentHeight) {
  bounds_.width = parentWidth;

  float maxChildWidth = 0.0f;
  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];

    child->layout(parentWidth, 0);

    maxChildWidth = std::fmax(maxChildWidth, child->getBounds().width);
  }

  bounds_.width = maxChildWidth;

  float currentY = 0.0f;
  for (size_t index = 0; index < children_.size(); index++) {
    auto &child = children_[index];
    float crossAxisPosistion = getXPosition(cross_axis_alignment_, bounds_.width, child->getBounds().width);
    child->setPosition(crossAxisPosistion, currentY);

    currentY += child->getBounds().height;
    if (index + 1 != children_.size()) {
      currentY += spacing_;
    }
  }
  bounds_.height = currentY;

  // bounds_.width = parentWidth;
}

/**
 * @brief Renders the column and its child components onto the provided canvas.
 *
 * Saves the current canvas state, translates to the column's position, draws all child components, restores the canvas state, and then draws any additional visuals from the base UI component.
 *
 * @param canvas The canvas on which to render the column and its children.
 */
void Column::draw(SkCanvas *canvas) {
  canvas->save();
  canvas->translate(bounds_.x, bounds_.y);

  for (auto &child : children_) {
    child->draw(canvas);
  }

  canvas->restore();
  UIComponent::draw(canvas);
}

/**
 * @brief Returns a constant reference to the column's child components.
 *
 * Use this to access the list of UI components arranged within the column.
 *
 * @return Constant reference to the vector of child UI components.
 */
const std::vector<std::shared_ptr<UIComponent>> &Column::children() const { return children_; }
