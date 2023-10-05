#include "pcg/file_format/v1/ops/attention.h"

namespace FlexFlow {

V1MultiHeadAttentionAttrs to_v1(MultiHeadAttentionAttrs const &a) {
  return {a.embed_dim,
          a.num_heads,
          a.kdim,
          a.vdim,
          a.dropout,
          a.bias,
          a.add_bias_kv,
          a.add_zero_attn};
}

MultiHeadAttentionAttrs from_v1(V1MultiHeadAttentionAttrs const &va) {
  return {va.embed_dim,
          va.num_heads,
          va.kdim,
          va.vdim,
          va.dropout,
          va.bias,
          va.add_bias_kv,
          va.add_zero_attn};
}

} // namespace FlexFlow
