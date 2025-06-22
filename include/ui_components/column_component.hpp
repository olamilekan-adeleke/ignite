#include <memory>
#include <vector>

#include "ui_components/ui_component.hpp"

struct ColumnParams {
  float spacing = 0.0f;
  std::vector<std::shared_ptr<UIComponent>> children;
};

class Column : public UIComponent {
 public:
  Column(const ColumnParams &params = {});

  void addChild(std::shared_ptr<UIComponent> child);

  void layout(float parentWidth, float parentHeight) override;
  void draw(SkCanvas *canvas) override;

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

 protected:
  virtual bool processChildTaps(const UITapEvent &event) override {
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
      if ((*it)->processTap(event)) return true;
    }
    return false;
  }

 private:
  float spacing_;
  std::vector<std::shared_ptr<UIComponent>> children_;
};
