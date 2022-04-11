#ifndef _FLEXFLOW_ELEMENT_BINARY_H
#define _FLEXFLOW_ELEMENT_BINARY_H

#include "flexflow/model.h"

namespace FlexFlow {

class ElementBinaryMeta : public OpMeta {
public:
  ElementBinaryMeta(FFHandler handle);
#if defined (FF_USE_CUDA) || defined (FF_USE_HIP_CUDA)
  cudnnTensorDescriptor_t input1Tensor, input2Tensor, outputTensor;
  cudnnOpTensorDescriptor_t opDesc;
  cudnnReduceTensorDescriptor_t reduceAddDesc;
#else
  miopenTensorDescriptor_t input1Tensor, input2Tensor, outputTensor;
  miopenTensorOp_t opDesc;
  miopenReduceTensorDescriptor_t reduceAddDesc;
#endif
  OperatorType op_type;
  bool inplace_a, has_same_operands;
  bool broadcast_input1, broadcast_input2;
};

class ElementBinary : public Op {
public:
  ElementBinary(FFModel& model,
                OperatorType type,
                const ParallelTensor x,
                const ParallelTensor y,
                bool inplace_a,
                const char* name);
  void init(const FFModel&) override;
  void forward(const FFModel&) override;
  void backward(const FFModel&) override;
  void print_layer(const FFModel& model) override {assert(0);}
  bool can_inplace_output() override;
  bool has_inplace_output() override;
  void do_inplace_output() override;
  static Op* create_operator_from_layer(FFModel& model,
                                        const Layer* layer,
                                        const std::vector<ParallelTensor>& inputs);
  static OpMeta* init_task(const Legion::Task *task,
                           const std::vector<Legion::PhysicalRegion> &regions,
                           Legion::Context ctx, Legion::Runtime *runtime);
  static void forward_task(const Legion::Task *task,
                           const std::vector<Legion::PhysicalRegion> &regions,
                           Legion::Context ctx, Legion::Runtime *runtime);
  static void backward_task(const Legion::Task *task,
                            const std::vector<Legion::PhysicalRegion> &regions,
                            Legion::Context ctx, Legion::Runtime *runtime);
  static void init_kernel(ElementBinaryMeta* m,
                          const Legion::Domain& input1_domain,
                          const Legion::Domain& input2_domain,
                          const Legion::Domain& output_domain);
  static void forward_kernel(const ElementBinaryMeta* m,
                             const float* in1_ptr,
                             const float* in2_ptr,
                             float* out_ptr,
                             ffStream_t stream);
  static void forward_kernel_wrapper(const ElementBinaryMeta* m,
                                     const float* in1_ptr,
                                     const float* in2_ptr,
                                     float* out_ptr);
  static void backward_kernel(const ElementBinaryMeta* m,
                              const float* out_grad_ptr,
                              const float* in1_ptr,
                              const float* in2_ptr,
                              float* in1_grad_ptr,
                              float* in2_grad_ptr,
                              ffStream_t stream);
  static void backward_kernel_wrapper(const ElementBinaryMeta* m,
                                      const float* out_grad_ptr,
                                      const float* in1_ptr,
                                      const float* in2_ptr,
                                      float* in1_grad_ptr,
                                      float* in2_grad_ptr);
  size_t get_params_hash() const override;
  bool measure_operator_cost(Simulator* sim,
                            const MachineView& pc,
                            CostMetrics& cost_metrics) const override;
public:
  bool inplace_a, has_same_operands;
  bool broadcast_input1, broadcast_input2;
};

}; // namespace FlexFlow

#endif // _FLEXFFLOW_ELEMENT_BINARY_H