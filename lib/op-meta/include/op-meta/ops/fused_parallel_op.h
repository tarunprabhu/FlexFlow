#ifndef _FLEXFLOW_FUSED_PARALLEL_OP_ATTRS_H
#define _FLEXFLOW_FUSED_PARALLEL_OP_ATTRS_H

#include "op-meta/parallel_op_info.h"
#include <vector>
#include "op-meta/parallel_tensor_shape.h"
#include "op-meta/ops/unary_op.h"
#include "visit_struct/visit_struct.hpp"

namespace FlexFlow {

struct FusedParallelOpAttrs : public UnaryOpAttrs {
public:
  ParallelTensorShape output_shape(ParallelTensorShape const &input_shape) const override;
  OperatorType op_type() const override;

public:
  std::vector<ParallelOpInfo> parallel_ops;
};
bool operator==(FusedParallelOpAttrs const &, FusedParallelOpAttrs const &);
bool operator<(FusedParallelOpAttrs const &, FusedParallelOpAttrs const &);

}

VISITABLE_STRUCT(::FlexFlow::FusedParallelOpAttrs, parallel_ops);

namespace std {
template <>
struct hash<::FlexFlow::FusedParallelOpAttrs> {
  size_t operator()(::FlexFlow::FusedParallelOpAttrs const &) const;
};
} 

#endif 