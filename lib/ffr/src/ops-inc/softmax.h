#ifndef _FLEXFLOW_SOFTMAX_H
#define _FLEXFLOW_SOFTMAX_H

#include "device.h"
#include "fftype.h"
#include "layer.h"
#include "flexflow/node.h"
#include "op_meta.h"
#include "operator.h"
#include "op-meta/softmax_params.h"

namespace FlexFlow {

class Softmax : public Op {
public:
  using Params = SoftmaxParams;
  using Input = ParallelTensor;
  Softmax(FFModel &model,
          const ParallelTensor logit,
          int dim,
          char const *name);
  Softmax(FFModel &model,
          Params const &params,
          const Input input,
          char const *name = nullptr);
  void init(FFModel const &) override;
  void forward(FFModel const &) override;
  void backward(FFModel const &) override;
  bool get_int_parameter(PMParameter, int *) const override;
  void print_layer(FFModel const &model) override {
    assert(0);
  }
  static Op *
      create_operator_from_layer(FFModel &model,
                                 Layer const *layer,
                                 std::vector<ParallelTensor> const &inputs);
  static OpMeta *init_task(Legion::Task const *task,
                           std::vector<Legion::PhysicalRegion> const &regions,
                           Legion::Context ctx,
                           Legion::Runtime *runtime);
  static void forward_task(Legion::Task const *task,
                           std::vector<Legion::PhysicalRegion> const &regions,
                           Legion::Context ctx,
                           Legion::Runtime *runtime);
  static void backward_task(Legion::Task const *task,
                            std::vector<Legion::PhysicalRegion> const &regions,
                            Legion::Context ctx,
                            Legion::Runtime *runtime);
  bool measure_operator_cost(Simulator *sim,
                             MachineView const &pc,
                             CostMetrics &cost_metrics) const override;
  Params get_params() const;

private:
  template <int NDIM>
  static void
      forward_task_with_dim(Legion::Task const *task,
                            std::vector<Legion::PhysicalRegion> const &regions,
                            Legion::Context ctx,
                            Legion::Runtime *runtime);
  template <int NDIM>
  static void
      backward_task_with_dim(Legion::Task const *task,
                             std::vector<Legion::PhysicalRegion> const &regions,
                             Legion::Context ctx,
                             Legion::Runtime *runtime);

public:
  int dim;
};

}; // namespace FlexFlow

#endif // _FLEXFLOW_SOFTMAX_H