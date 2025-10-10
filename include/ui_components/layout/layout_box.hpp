#include <sstream>
#include <vector>

#include "basic/ui_component.hpp"
#include "size.hpp"

struct LayoutBoxParam {
  Axis axis = Axis::VERTICAL;
  float childGap = 0;
  std::vector<std ::shared_ptr<UIComponent>> children;
};

class LayoutBox : public UIComponent {
 public:
  LayoutBox(LayoutBoxParam param = {}) : params_(std::move(param)) {};

  void layout(UIConstraints size) override;
  void draw(SkCanvas *canvas) override;

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

 protected:
  void debugFillProperties(std::ostringstream &os, int indent) const override;

  bool processChildTaps(const UITapEvent &event) override {
    // if (params_.child) return params_.child->processTap(event);
    return false;
  }

  bool wantsToFillMainAxis() const override { return false; }
  bool wantsToFillCrossAxis() const override { return false; }

  float getMainAxisSize(const UISizing &size) const noexcept;
  float getCrossAxisSize(const UISizing &size) const noexcept;

 private:
  LayoutBoxParam params_;
};

inline void LayoutBox::debugFillProperties(std::ostringstream &os, int indent) const {
  UIComponent::debugFillProperties(os, indent);
  std::string pad(indent, ' ');
  os << pad << "axis: " << axisToString(params_.axis) << "\n";
  os << pad << "children: " << params_.children.size() << "\n";
  os << pad << "childGap: " << params_.childGap << "\n";
}
