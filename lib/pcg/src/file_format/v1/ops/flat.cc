#include "pcg/file_format/v1/ops/flat.h"
#include "pcg/file_format/v1/v1.h"

namespace FlexFlow {

V1FlatAttrs to_v1(FlatAttrs const &a) {
  return {
      // No fields in FlatAttrs
  };
}

FlatAttrs from_v1(V1FlatAttrs const &va) {
  return {
    // No fields in FlatAttrs
  };
}

} // namespace FlexFlow