#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H
#include <iostream>
#include <vector>
#include "publisher/theo_utils/option_greeks.h"
using namespace std;
namespace volalpha::infra {
struct GreeksEvent {
  int productId;
};
template <typename T>
struct Sub {
  virtual void handleEvent(T) = 0;
};
template <typename T>
struct Sub<T&> {
  virtual void handleEvent(T&) = 0;
};
template <typename... Ts>
struct Subscriber : Sub<Ts>... {
  // virtual void handleEvent(T) = 0;
  using Sub<Ts>::handleEvent...;
};
template<typename T>
struct Listeners {
  vector<Sub<T>*> listeners_;
  void registerSub(Sub<T>* s) {
    listeners_.push_back(s);
  }
  void handleEvent(T event) {
    for (auto l : listeners_) {
      l->handleEvent(std::move(event));
    }
  }
  void handleEvent(T& event) {
    for (auto l : listeners_) {
      l->handleEvent(event);
    }
  }
};
template<typename... Ts>
struct EventHandler: Listeners<Ts>... {
  using Listeners<Ts>::registerSub...;
  using Listeners<Ts>::handleEvent...;
  template<typename... Es>
  void registerSubs(Subscriber<Es...>&  ss) {
    (registerSub(static_cast<Sub<Es>*>(&ss)), ...);
  };
  template<typename... Es>
  void registerSubs(Subscriber<Es...>*  ss) {
    (registerSub(static_cast<Sub<Es>*>(ss)), ...);
  };
};
// @todo
// - Can add EventType Interface to have more flexibility or maybe <enum, obj> pair
// - Possible to have listening control over type and data
}
#endif


