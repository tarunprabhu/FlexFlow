#ifndef _FLEXFLOW_PCG_INCLUDE_PCG_FILE_FORMAT_V1_DIM_ORDERED_H
#define _FLEXFLOW_PCG_INCLUDE_PCG_FILE_FORMAT_V1_DIM_ORDERED_H

#include "op-attrs/dim_ordered.h"
#include "utils/json.h"

namespace FlexFlow {

template <typename Idx, typename T>
struct V1DimOrdered {
  std::vector<T> contents;

  bool operator!=(const V1DimOrdered &o) {
    return this->contents != o.contents;
  }
};

template <typename Idx, typename T>
V1DimOrdered<Idx, T> to_v1(DimOrdered<Idx, T> const &dim);

template <typename T>
using V1FFOrdered = V1DimOrdered<ff_dim_t, T>;

template <typename T>
V1FFOrdered<T> to_v1(FFOrdered<T> const& o);

} // namespace FlexFlow

#endif