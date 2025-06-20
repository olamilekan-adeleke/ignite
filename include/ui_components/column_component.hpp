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

  // Template magic
  // template <typename... Children>
  // Column(std::shared_ptr<UIComponent> first, Children... rest);

  void addChild(std::shared_ptr<UIComponent> child);

  void layout(float parentWidth, float parentHeight) override;
  void draw(SkCanvas *canvas) override;

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

 private:
  float spacing_;
  std::vector<std::shared_ptr<UIComponent>> children_;
};
