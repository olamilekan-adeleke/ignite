#include "basic/ui_component.hpp"

struct LayoutBoxParam {};

class LayoutBox : public UIComponent {
 public:
  LayoutBox(LayoutBoxParam param = {}) : params_(std::move(param)) {};

  void layout(UIConstraints size) override;
  void draw(SkCanvas *canvas) override;

  UISize getIntrinsicSize(UIConstraints constraints) noexcept override;

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

 protected:
  void debugFillProperties(std::ostringstream &os, int indent) const override;

  bool processChildTaps(const UITapEvent &event) override {
    // if (params_.child) return params_.child->processTap(event);
    return false;
  }

  bool wantsToFillMainAxis() const override { return false; }
  bool wantsToFillCrossAxis() const override { return false; }

 private:
  LayoutBoxParam params_;
  mutable std::vector<std::shared_ptr<UIComponent>> cached_children_;
};
