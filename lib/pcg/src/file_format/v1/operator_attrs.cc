#include "pcg/file_format/v1/operator_attrs.h"
#include "pcg/file_format/v1/v1.h"

namespace FlexFlow {

V1CompGraphOperatorAttrs to_v1(CompGraphOperatorAttrs const &attrs) {
  if (auto const *aggr = get_if<AggregateAttrs>(&attrs)) {
    return to_v1(*aggr);
  } else if (auto const *aggrSpec = get_if<AggregateSpecAttrs>(&attrs)) {
    return to_v1(*aggrSpec);
  } else if (auto const *batchMm = get_if<BatchMatmulAttrs>(&attrs)) {
    return to_v1(*batchMm);
  } else if (auto const *batchNorm = get_if<BatchNormAttrs>(&attrs)) {
    return to_v1(*batchNorm);
  } else if (auto const *cast = get_if<CastAttrs>(&attrs)) {
    return to_v1(*cast);
  } else if (auto const *concat = get_if<ConcatAttrs>(&attrs)) {
    return to_v1(*concat);
  } else if (auto const *conv2d = get_if<Conv2DAttrs>(&attrs)) {
    return to_v1(*conv2d);
  } else if (auto const *dropout = get_if<DropoutAttrs>(&attrs)) {
    return to_v1(*dropout);
  } else if (auto const *elemBin = get_if<ElementBinaryAttrs>(&attrs)) {
    return to_v1(*elemBin);
  } else if (auto const *elemUnSc = get_if<ElementScalarUnaryAttrs>(&attrs)) {
    return to_v1(*elemUnSc);
  } else if (auto const *elemUn = get_if<ElementUnaryAttrs>(&attrs)) {
    return to_v1(*elemUn);
  } else if (auto const *emb = get_if<EmbeddingAttrs>(&attrs)) {
    return to_v1(*emb);
  } else if (auto const *flat = get_if<FlatAttrs>(&attrs)) {
    return to_v1(*flat);
  } else if (auto const *gather = get_if<GatherAttrs>(&attrs)) {
    return to_v1(*gather);
  } else if (auto const *group = get_if<Group_byAttrs>(&attrs)) {
    return to_v1(*group);
  } else if (auto const *inp = get_if<InputAttrs>(&attrs)) {
    return to_v1(*inp);
  } else if (auto const *layerNorm = get_if<LayerNormAttrs>(&attrs)) {
    return to_v1(*layerNorm);
  } else if (auto const *lin = get_if<LinearAttrs>(&attrs)) {
    return to_v1(*lin);
  } else if (auto const *att = get_if<MultiHeadAttentionAttrs>(&attrs)) {
    return to_v1(*att);
  } else if (auto const *noop = get_if<NoopAttrs>(&attrs)) {
    return to_v1(*noop);
  } else if (auto const *pool2d = get_if<Pool2DAttrs>(&attrs)) {
    return to_v1(*pool2d);
  } else if (auto const *reduce = get_if<ReduceAttrs>(&attrs)) {
    return to_v1(*reduce);
  } else if (auto const *reverse = get_if<ReverseAttrs>(&attrs)) {
    return to_v1(*reverse);
  } else if (auto const *reshape = get_if<ReshapeAttrs>(&attrs)) {
    return to_v1(*reshape);
  } else if (auto const *split = get_if<SplitAttrs>(&attrs)) {
    return to_v1(*split);
  } else if (auto const *soft = get_if<SoftmaxAttrs>(&attrs)) {
    return to_v1(*soft);
  } else if (auto const *topk = get_if<TopKAttrs>(&attrs)) {
    return to_v1(*topk);
  } else if (auto const *trans = get_if<TransposeAttrs>(&attrs)) {
    return to_v1(*trans);
  } else if (auto const *bcast = get_if<BroadcastAttrs>(&attrs)) {
    return to_v1(*bcast);
  } else {
    NOT_REACHABLE();
  }
}

V1PCGOperatorAttrs to_v1(PCGOperatorAttrs const &attrs) {
  if (auto const *aggr = get_if<AggregateAttrs>(&attrs)) {
    return to_v1(*aggr);
  } else if (auto const *aggrSpec = get_if<AggregateSpecAttrs>(&attrs)) {
    return to_v1(*aggrSpec);
  } else if (auto const *batchMm = get_if<BatchMatmulAttrs>(&attrs)) {
    return to_v1(*batchMm);
  } else if (auto const *batchNorm = get_if<BatchNormAttrs>(&attrs)) {
    return to_v1(*batchNorm);
  } else if (auto const *cast = get_if<CastAttrs>(&attrs)) {
    return to_v1(*cast);
  } else if (auto const *concat = get_if<ConcatAttrs>(&attrs)) {
    return to_v1(*concat);
  } else if (auto const *conv2d = get_if<Conv2DAttrs>(&attrs)) {
    return to_v1(*conv2d);
  } else if (auto const *dropout = get_if<DropoutAttrs>(&attrs)) {
    return to_v1(*dropout);
  } else if (auto const *elemBin = get_if<ElementBinaryAttrs>(&attrs)) {
    return to_v1(*elemBin);
  } else if (auto const *elemUnSc = get_if<ElementScalarUnaryAttrs>(&attrs)) {
    return to_v1(*elemUnSc);
  } else if (auto const *elemUn = get_if<ElementUnaryAttrs>(&attrs)) {
    return to_v1(*elemUn);
  } else if (auto const *emb = get_if<EmbeddingAttrs>(&attrs)) {
    return to_v1(*emb);
  } else if (auto const *flat = get_if<FlatAttrs>(&attrs)) {
    return to_v1(*flat);
  } else if (auto const *gather = get_if<GatherAttrs>(&attrs)) {
    return to_v1(*gather);
  } else if (auto const *group = get_if<Group_byAttrs>(&attrs)) {
    return to_v1(*group);
  } else if (auto const *inp = get_if<InputAttrs>(&attrs)) {
    return to_v1(*inp);
  } else if (auto const *layerNorm = get_if<LayerNormAttrs>(&attrs)) {
    return to_v1(*layerNorm);
  } else if (auto const *lin = get_if<LinearAttrs>(&attrs)) {
    return to_v1(*lin);
  } else if (auto const *att = get_if<MultiHeadAttentionAttrs>(&attrs)) {
    return to_v1(*att);
  } else if (auto const *noop = get_if<NoopAttrs>(&attrs)) {
    return to_v1(*noop);
  } else if (auto const *pool2d = get_if<Pool2DAttrs>(&attrs)) {
    return to_v1(*pool2d);
  } else if (auto const *reduce = get_if<ReduceAttrs>(&attrs)) {
    return to_v1(*reduce);
  } else if (auto const *reverse = get_if<ReverseAttrs>(&attrs)) {
    return to_v1(*reverse);
  } else if (auto const *reshape = get_if<ReshapeAttrs>(&attrs)) {
    return to_v1(*reshape);
  } else if (auto const *split = get_if<SplitAttrs>(&attrs)) {
    return to_v1(*split);
  } else if (auto const *soft = get_if<SoftmaxAttrs>(&attrs)) {
    return to_v1(*soft);
  } else if (auto const *topk = get_if<TopKAttrs>(&attrs)) {
    return to_v1(*topk);
  } else if (auto const *trans = get_if<TransposeAttrs>(&attrs)) {
    return to_v1(*trans);
  } else if (auto const *combine = get_if<CombineAttrs>(&attrs)) {
    return to_v1(*combine);
  } else if (auto const *red = get_if<ReductionAttrs>(&attrs)) {
    return to_v1(*red);
  } else if (auto const *repart = get_if<RepartitionAttrs>(&attrs)) {
    return to_v1(*repart);
  } else if (auto const *repl = get_if<ReplicateAttrs>(&attrs)) {
    return to_v1(*repl);
  } else {
    NOT_REACHABLE();
  }
}

} // namespace FlexFlow
