#include <memory>
#include "ui_component.hpp"

class StatefulComponent : public UIComponent {
 public:
  StatefulComponent() : isDirty_(false), needToRedraw_(true) {}
  virtual ~StatefulComponent() = default;

  virtual std::shared_ptr<UIComponent> body() = 0;

  bool isDirty() const;

  std::shared_ptr<UIComponent> getChild();

  void layout(float parentWidth, float parentHeight) override;
  void draw(SkCanvas *canvas) override;

  virtual const std::vector<std::shared_ptr<UIComponent>> &children() const override;

 protected:
  void markDirty();

  virtual bool processChildTaps(const UITapEvent &event) override;

 private:
  bool isDirty_;
  bool needToRedraw_;
  std::shared_ptr<UIComponent> cachedBody_;
};
