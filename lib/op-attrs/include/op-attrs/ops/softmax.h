#ifndef _FLEXFLOW_SOFTMAX_ATTRS_H
#define _FLEXFLOW_SOFTMAX_ATTRS_H

#include "op-attrs/parallel_tensor_shape.h"
#include "op-attrs/ops/unary_op.h"
#include "utils/visitable.h"

namespace FlexFlow {

struct SoftmaxAttrs {
/* public: */
/*   OperatorType op_type() const override; */
/*   ParallelTensorShape output_shape(ParallelTensorShape const &input_shape) const override; */
/* public: */
  int dim;
};
bool operator==(SoftmaxAttrs const &, SoftmaxAttrs const &);
bool operator<(SoftmaxAttrs const &, SoftmaxAttrs const &);

}

VISITABLE_STRUCT(::FlexFlow::SoftmaxAttrs, dim);

namespace std {
template <>
struct hash<::FlexFlow::SoftmaxAttrs> {
  size_t operator()(::FlexFlow::SoftmaxAttrs const &) const;
};
}

#endif // _FLEXFLOW_SOFTMAX_ATTRS_H