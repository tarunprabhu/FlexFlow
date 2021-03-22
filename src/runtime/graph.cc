/* Copyright 2021 CMU, Facebook, LANL, MIT, and Stanford (alphabetical)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "graph.h"

const MachineView MachineView::NO_VIEW = MachineView();

size_t MachineView::hash() const
{
  size_t ret = 17;
  ret = ret * 31 + std::hash<int>()(device_type);
  ret = ret * 31 + std::hash<int>()(ndims);
  ret = ret * 31 + std::hash<int>()(start_device_id);
  for (int i = 0; i < ndims; i++) {
    ret = ret * 31 + std::hash<int>()(dim[i]);
    ret = ret * 31 + std::hash<int>()(stride[i]);
  }
  return ret;
}

size_t MachineResource::hash() const
{
  size_t ret = 17;
  ret = ret * 31 + std::hash<int>()(num_nodes);
  ret = ret * 31 + std::hash<int>()(gpus_per_node);
  ret = ret * 31 + std::hash<int>()(cpus_per_node);
  return ret;
}

Edge::Edge(void)
: srcOp(NULL), dstOp(NULL), srcIdx(-1), dstIdx(-1), weightEdge(false)
{}

Edge::Edge(const Op* _srcOp, const Op* _dstOp,
           int _srcIdx, int _dstIdx, bool _weightEdge)
: srcOp(_srcOp), dstOp(_dstOp), srcIdx(_srcIdx), dstIdx(_dstIdx),
  weightEdge(_weightEdge)
{}

bool Edge::operator==(const Edge& rhs) const
{
  if (srcOp != rhs.srcOp) return false;
  if (dstOp != rhs.dstOp) return false;
  if (srcIdx != rhs.srcIdx) return false;
  if (dstIdx != rhs.dstIdx) return false;
  if (weightEdge != rhs.weightEdge) return false;
  return true;
}

Graph::Graph(FFModel* _model)
: model(_model)
{
}

void Graph::add_edge(const Op* srcOp,
                     const Op* dstOp,
                     int srcIdx,
                     int dstIdx,
                     bool weightEdge)
{
  if (inEdges.find(dstOp) == inEdges.end()) {
    inEdges[dstOp];
  }
  if (outEdges.find(srcOp) == outEdges.end()) {
    outEdges[srcOp];
  }
  Edge e(srcOp, dstOp, srcIdx, dstIdx, weightEdge);
  inEdges[dstOp].insert(e);
  outEdges[srcOp].insert(e);
}

void Graph::add_edge(const Edge& e)
{
  inEdges[e.dstOp].insert(e);
  outEdges[e.srcOp].insert(e);
}

bool Graph::has_edge(const Op* srcOp,
                     const Op* dstOp,
                     int srcIdx,
                     int dstIdx,
                     bool weightEdge)
{
  Edge e(srcOp, dstOp, srcIdx, dstIdx, weightEdge);
  return (inEdges[dstOp].find(e) != inEdges[dstOp].end());
}

bool Graph::has_edge(const Edge& e)
{
  return (inEdges[e.dstOp].find(e) != inEdges[e.dstOp].end());
}

void Graph::print(void)
{
  for (const auto& it : inEdges) {
    if (it.first->op_guid == 0) continue;
    printf("	op_guid(%zu) type(%d): ", it.first->op_guid, it.first->op_type);
    const std::unordered_set<Edge>& list = it.second;
    for (const auto& it2 : list) {
      Edge e = it2;
      printf(" inEdge(op_guid(%zu) idx(%d))", e.srcOp->op_guid, e.srcIdx);
    }
    printf("\n");
    // if (it->first.ptr->type == OP_CONV2D) {
    //   it->first.ptr->inputs[1].print_info("conv weight");
    // }
    // else if (it->first.ptr->type == OP_BROADCAST_ADD) {
    //   it->first.ptr->inputs[1].print_info("conv new bias");
    // }
    // else if (it->first.ptr->type == OP_BATCHNORM) {
    //   it->first.ptr->inputs[1].print_info("gamma");
    //   it->first.ptr->inputs[2].print_info("beta");
    //   it->first.ptr->inputs[3].print_info("mean");
    //   it->first.ptr->inputs[4].print_info("var");
    // }
  }
}

bool Graph::has_loop(void)
{
  std::unordered_map<const Op*, int> todos;
  std::vector<const Op*> opList;
  for (const auto& it : inEdges) {
    const auto& inList = it.second;
    todos[it.first] = (int)inList.size();
    if (todos[it.first] == 0)
      opList.push_back(it.first);
  }
  size_t i = 0;
  while (i < opList.size()) {
    const Op* op = opList[i++];
    const auto& outList = outEdges[op];
    for (const auto& it2 : outList) {
      todos[it2.dstOp] --;
      if (todos[it2.dstOp] == 0) {
        opList.push_back(it2.dstOp);
      }
    }
  }
  return (opList.size() < inEdges.size());
}

bool Graph::check_correctness(void)
{
  bool okay = true;
  for (auto it = outEdges.begin(); it != outEdges.end(); it++) {
    const auto& list = it->second;
    for (auto it2 = list.begin(); it2 != list.end(); it2++) {
      Edge e = *it2;
      if (!has_edge(e)) assert(false);
      if (e.srcOp == NULL) continue;
      Tensor srcTensor = e.srcOp->outputs[e.srcIdx];
      Tensor dstTensor = e.dstOp->inputs[e.dstIdx];
      if (srcTensor->num_dims != dstTensor->num_dims) assert(false);
      for (int i = 0; i < srcTensor->num_dims; i++) {
        assert(srcTensor->dims[i] == dstTensor->dims[i]);
      }
    }
  }
  return okay;
}

float FFModel::graph_cost(const Graph* graph,
                          const Op* sink_node,
                          const MachineView& sink_view,
                          const Op* source_node,
                          const MachineView& source_view,
                          const MachineResource& resources,
                          bool include_sink_compute_time)
{
  size_t hash = dp_state_hash(graph, sink_node, sink_view,
                              source_node, source_view, resources);
  assert(graph->inEdges.find(sink_node) != graph->inEdges.end());
  if (source_node != NULL)
    assert(graph->outEdges.find(source_node) != graph->outEdges.end());
  if (cached_graph_costs.find(hash) != cached_graph_costs.end()) {
    // cached_graph_costs does not include sink_compute_time
    if (include_sink_compute_time) {
      CostMetrics metrics = simulator->measure_operator_cost(sink_node, sink_view);
      return cached_graph_costs[hash]+metrics.forward_time+metrics.backward_time;
    } else 
      return cached_graph_costs[hash];
  }
  float cost = 1e7;
  if (graph->inEdges.size() == 1) {
    if (source_node == NULL)
      cost = 0.0f;
    else {
      cost = 0.0f;
      const auto& inList = graph->inEdges.find(sink_node)->second;
      for (const auto& it2 : inList) {
        assert(it2.srcOp == source_node);
        cost += simulator->estimate_xfer_cost(source_node->outputs[it2.srcIdx],
                                              source_view, sink_view);
      }
    }
  } else {
    std::unordered_set<const Op*> used_nodes;
    Op* bn_node = graph->find_bottleneck_node(sink_node, source_node, used_nodes);
    if (bn_node != NULL) {
      // We found a bottleneck node
      Graph* first_graph = new Graph(this);
      Graph* second_graph = new Graph(this);
      for (const auto& it : graph->inEdges) {
        const auto& inList = it.second;
        if (used_nodes.find(it.first) != used_nodes.end()) {
          // Add all in-edges of used_nodes in to the first_graph
          for (const auto& it2 : inList) {
            first_graph->add_edge(it2);
          }
        } else {
          // Add all in-edges of not_used_nodes into the second_graph
          for (const auto& it2 : inList) {
            second_graph->add_edge(it2);
          }
        }
      }
      std::vector<MachineView> valid_views;
      assert(get_valid_machine_views(bn_node, valid_views));
      for (size_t i = 0; i < valid_views.size(); i++) {
        MachineView bn_view = valid_views[i];
        if (!resources.is_valid_view(bn_view)) continue;
        float first_cost = graph_cost(first_graph, bn_node, bn_view,
                                      source_node, source_view, resources, true);
        float second_cost = graph_cost(second_graph, sink_node, sink_view, 
                                       bn_node, bn_view, resources, false);
        if (first_cost + second_cost < cost)
          cost = first_cost + second_cost;
      }
      delete first_graph;
      delete second_graph;
    } else {
      // sink node must have multiple branches
      // otherwise we should not be here
      assert(graph->inEdges.find(sink_node)->second.size() > 1);
      Graph* first_graph = new Graph(this);
      Graph* second_graph = new Graph(this);
      const Op* bn_node = NULL;
      // Find sink_node's first input
      {
        const auto& inList = graph->inEdges.find(sink_node)->second;
        for (const auto& it2 : inList) {
          if (it2.dstIdx != 0) continue;
          if (it2.weightEdge) continue;
          bn_node = it2.srcOp;
        }
      }
      assert(bn_node != NULL);
      std::unordered_set<const Op*> used_node;
      std::vector<const Op*> queue;
      queue.push_back(bn_node);
      used_node.insert(bn_node);
      size_t i = 0;
      while (i < queue.size()) {
        const Op* node = queue[i++];
        const auto& inList = graph->inEdges.find(node)->second;
        for (const auto& it2 : inList) {
          if (used_nodes.find(it2.srcOp) == used_nodes.end()) {
            used_nodes.insert(it2.srcOp);
            queue.push_back(it2.srcOp);
          }
        }
      }
      for (const auto& it : graph->inEdges) {
        if (it.first == sink_node) continue;
        const auto& inList = it.second;
        if (used_nodes.find(it.first) != used_nodes.end()) {
          // Add all in-edges of used_nodes in to the first_graph
          for (const auto& it2 : inList) {
            first_graph->add_edge(it2);
          }
        } else {
          // Add all in-edges of not_used_nodes into the second_graph
          for (const auto& it2 : inList) {
            second_graph->add_edge(it2);
          }
        }
      }
      // Split sink_node's inedges between the two graphs
      {
        const auto& inList = graph->inEdges.find(sink_node)->second;
        for (const auto& it2 : inList) {
          if (used_nodes.find(it2.srcOp) != used_nodes.end()) {
            first_graph->add_edge(it2);
          } else {
            second_graph->add_edge(it2);
          }
        }
      }
      // Assert there must be at least on sink_source's inEdges in the second graph
      assert(second_graph->inEdges.find(sink_node) != second_graph->inEdges.end());
      // Run the two sequentially
      cost = graph_cost(first_graph, sink_node, sink_view, 
                        source_node, source_view, resources, false)
           + graph_cost(second_graph, sink_node, sink_view,
                        source_node, source_view, resources, false);
      // Consider run the two in parallel
      // Split resources vertically
      for (int i = 1; i < resources.num_nodes; i++) {
        MachineResource firstRes, secondRes;
        firstRes = resources; secondRes = resources;
        firstRes.num_nodes = i;
        secondRes.num_nodes = resources.num_nodes - i;
        float new_cost;
        new_cost = std::max(graph_cost(first_graph, sink_node, sink_view,
                                       source_node, source_view, firstRes, false),
                            graph_cost(second_graph, sink_node, sink_view,
                                       source_node, source_view, secondRes, false));
        if (new_cost < cost)
          cost = new_cost;
      }
      // Split resources horizontally
      for (int i = 1; i < resources.gpus_per_node; i++) {
        MachineResource firstRes, secondRes;
        firstRes = resources; secondRes = resources;
        firstRes.gpus_per_node = i;
        secondRes.gpus_per_node = resources.gpus_per_node - i;
        float new_cost;
        new_cost = std::max(graph_cost(first_graph, sink_node, sink_view,
                                       source_node, source_view, firstRes, false),
                            graph_cost(second_graph, sink_node, sink_view,
                                       source_node, source_view, secondRes, false));
        if (new_cost < cost)
          cost = new_cost;
      }
      delete first_graph;
      delete second_graph;
    }
  }
  cached_graph_costs[hash] = cost;
  if (include_sink_compute_time) {
    CostMetrics metrics = simulator->measure_operator_cost(sink_node, sink_view);
    cost += metrics.forward_time + metrics.backward_time;
  }
  return cost;
}

Op* Graph::find_bottleneck_node(const Op* sink_node,
                                const Op* source_node,
                                std::unordered_set<const Op*>& used_nodes) const
{
  std::unordered_map<const Op*, int> todos;
  std::unordered_map<const Op*, size_t> indices;
  std::vector<const Op*> queue;
  for (const auto& it : inEdges) {
    const auto& inList = it.second;
    int cnt = 0;
    for (const auto& it2 : inList) {
      // Ignore weight edge
      if (it2.weightEdge) continue;
      if (it2.srcOp != source_node) continue;
        cnt ++;
    }
    todos[it.first] = cnt;
    if (cnt == 0) {
      indices[it.first] = queue.size();
      queue.push_back(it.first);
    }
  }
  size_t index = 0;
  while (index < queue.size()) {
    const Op* op = queue[index++];
    const auto& outList = outEdges.find(op)->second;
    for (const auto& it2 : outList) {
      todos[it2.dstOp] --;
      if (todos[it2.dstOp] == 0) {
        indices[it2.dstOp] = queue.size();
        queue.push_back(it2.dstOp);
      }
    }
  }
  // assert(queue.size() == inEdges.size());
  // assert that sink_node must be the last one
  assert(indices[sink_node] == queue.size() - 1);
  std::vector<bool> available(queue.size(), true);
  for (const auto& it : inEdges) {
    const auto& inList = it.second;
    for (const auto& it2 : inList) {
      if (indices.find(it2.srcOp) == indices.end()) continue;
      if (indices.find(it2.dstOp) == indices.end()) continue;
      size_t start_idx = indices[it2.srcOp];
      size_t end_idx = indices[it2.dstOp];
      for (size_t i = start_idx+1; i < end_idx; i++)
        available[i] = false;
    }
  }
  const Op* bn_node = NULL;
  for (size_t i = 0; i < queue.size(); i++) {
    if (!available[i]) continue;
    if (queue[i]->op_type == OP_INPUT) continue;
    if (queue[i]->op_type == OP_WEIGHT) continue;
    bn_node = queue[i];
    break;
  }
  if (bn_node == NULL)
    return NULL;
  used_nodes.insert(bn_node);
  queue.clear();
  queue.push_back(bn_node);
  index = 0;
  while (index < queue.size()) {
    const Op* op = queue[index++];
    const auto& inList = inEdges.find(op)->second;
    for (const auto& it2 : inList) {
      if (used_nodes.find(it2.srcOp) == used_nodes.end()) {
        used_nodes.insert(it2.srcOp);
        queue.push_back(it2.srcOp);
      }
    }
  }
  return (Op*)bn_node;
}

float Graph::total_cost(void)
{
  // Find sink_nodes
  // i.e., nodes without no out edge
  const Op* sink_node = NULL;
  for (const auto& it : inEdges) {
    if (outEdges.find(it.first) == outEdges.end()) {
      assert(sink_node == NULL);
      sink_node = it.first;
    }
  }
  MachineResource resource;
  resource.num_nodes = model->config.numNodes;
  resource.cpus_per_node = model->config.cpusPerNode;
  resource.gpus_per_node = model->config.workersPerNode;
  std::vector<MachineView> valid_views;
  assert(model->get_valid_machine_views(sink_node, valid_views));
  float total_cost = 1e7;
  for (size_t i = 0; i < valid_views.size(); i++) {
    total_cost = std::min(total_cost,
                          model->graph_cost(this, sink_node, valid_views[i],
                                            NULL, MachineView::NO_VIEW,
                                            resource, true));
  }
  return total_cost;
}

size_t Graph::hash(void) const
{
  // Graph hash should be additive and independent to the ordering of the nodes
  size_t total_hash = 0;
  for (const auto& it : inEdges) {
    const auto& inList = it.second;
    size_t node_hash = std::hash<size_t>()(it.first->op_guid);
    for (const auto& e : inList) {
      size_t edge_hash = 17;
      edge_hash = edge_hash * 31 + std::hash<size_t>()(e.srcOp->op_guid);
      edge_hash = edge_hash * 31 + std::hash<int>()(e.srcIdx);
      edge_hash = edge_hash * 31 + std::hash<int>()(e.dstIdx);
      node_hash *= edge_hash;
    }
    total_hash += node_hash;
  }
  return total_hash;
}

size_t FFModel::dp_state_hash(const Graph* graph,
                              const Op* sink_node,
                              const MachineView& sink_view,
                              const Op* source_node,
                              const MachineView& source_view,
                              const MachineResource& resource)
{
  size_t key = graph->hash();
  key = key * 31 + sink_node->op_guid;
  key = key * 31 + sink_view.hash();
  key = key * 31 + source_node->op_guid;
  key = key * 31 + source_view.hash();
  key = key * 31 + resource.hash();
  return key;
}
