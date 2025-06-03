#include <memory>
#include <vector>

#include "ui_components/ui_component.hpp"

class Column : public UIComponent {
 public:
  Column() = default;

  // Template magic
  template <typename... Children>
  Column(std::shared_ptr<UIComponent> first, Children... rest);

  ~Column() override = default;
  Column(const Column&) = delete;
  Column& operator=(const Column&) = delete;
  Column& addChild(std::shared_ptr<UIComponent> child);
  void layout(float parentWidth, float parentHeight) override;
  void draw(SkCanvas* canvas) override;

 private:
  std::vector<std::shared_ptr<UIComponent>> children_;
};
