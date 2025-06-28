#include "ScrollingBackground.h"

ScrollingBackground::ScrollingBackground(): ShooterObject() {
  count = 0;
  maxcount = 10;
}

void ScrollingBackground::load(std::unique_ptr<LoaderParams> const &pParams) {
  
}