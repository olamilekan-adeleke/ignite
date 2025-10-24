// // main_thread_dispatcher.hpp
// #include <functional>
// #include <mutex>
// #include <vector>
//
// class MainThreadDispatcher {
//  public:
//   void dispatch(std::function<void()> func) {
//     std::lock_guard<std::mutex> lock(queueMutex_);
//     eventQueue_.push_back(std::move(func));
//   }
//
//   void processEvents() {
//     std::vector<std::function<void()>> eventsToProcess;
//     {
//       std::lock_guard<std::mutex> lock(queueMutex_);
//       eventQueue_.swap(eventsToProcess);
//     }
//     for (const auto& func : eventsToProcess) {
//       func();
//     }
//   }
//
//  private:
//   std::vector<std::function<void()>> eventQueue_;
//   std::mutex queueMutex_;
// };
//
// // Create a single instance
// MainThreadDispatcher mainThreadDispatcher;
//
// // Inside your main render loop (e.g., the on_refresh lambda)
// auto on_refresh = [&]() {
//   // ...
//
//   // 1. Process all pending UI updates.
//   mainThreadDispatcher.processEvents();
//
//   // 2. Now, safely layout and paint the (potentially updated) UI.
//   // rootElement->getRenderObject()->performLayout(constraints);
//   // rootElement->getRenderObject()->paint(skiaRenderer.getCanvas());
//
//   // ...
// };
//
// // In CounterState
// void increment() {
//   // Dispatch the state update to be run on the main thread
//   mainThreadDispatcher.dispatch([this]() { updateState([this]() { count++; }); });
// }
