#pragma once
namespace {
using namespace facebook::velox;
using namespace datagenerator;

class MultiWayJoinNode : public core::PlanNode {
public:
  MultiWayJoinNode(const core::PlanNodeId& id, std::vector<core::PlanNodePtr> sources)
      : PlanNode(id), sources_{sources} {}

  // Output type is the type of the first input
  const RowTypePtr& outputType() const override {
    static auto type =
        ROW({"a", "f"}, {CppToType<int64_t>::create(), CppToType<int64_t>::create()});
    return type;
  }

  const std::vector<core::PlanNodePtr>& sources() const override { return sources_; }

  std::string_view name() const override { return "three way join"; }

private:
  // One can add details about the plan node and its metadata in a textual
  // format.
  void addDetails(std::stringstream& /* stream */) const override {}

  std::vector<core::PlanNodePtr> sources_;
};

static std::vector<std::vector<std::pair<int64_t, int64_t>>> inputs;
static std::vector<std::pair<std::string, std::string>> inputNames;

// Second, let's define the operator. Here's where the main logic lives.
template <int phase> class MultiWayJoinOperator : public exec::Operator {
public:
  // The operator takes the plan node defined above, which could contain
  // additional metadata.
  MultiWayJoinOperator(int32_t operatorId, exec::DriverCtx* driverCtx,
                       std::shared_ptr<const MultiWayJoinNode> planNode)
      : Operator(driverCtx, nullptr, operatorId, planNode->id(), "DuplicateRow") {}

  // Called every time there's input available. We just save it in the `input_`
  // member defined in the base class, and process it on `getOutput()`.
  void addInput(RowVectorPtr input) override {
    if(phase == 0 && input) {
      auto buffer = input->childAt(0)->asFlatVector<int64_t>();
      auto buffer2 = input->childAt(1)->asFlatVector<int64_t>();
      std::vector<std::pair<int64_t, int64_t>> table;
      for(auto i = 0u; i < buffer->size(); i++) {
        table.emplace_back(buffer->valueAtFast(i), buffer2->valueAtFast(i));
      }
      static std::mutex input2Mutex;
      input2Mutex.lock();
      // keep the names of the input columns
      inputNames.emplace_back(dynamic_pointer_cast<const RowType>(input->type())->names().at(0),
                              dynamic_pointer_cast<const RowType>(input->type())->names().at(1));
      inputs.push_back(std::move(table));
      input2Mutex.unlock();
    }
    input_ = input;
  }

  bool needsInput() const override { return !noMoreInput_; }

  // Called every time your operator needs to produce data. It processes the
  // input saved in `input_` and returns a new RowVector.
  RowVectorPtr getOutput() override {
    if(phase == 0 || input_ == nullptr) {
      return nullptr;
    }
    while(inputs.size() < 2) { // wait input
      std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    // We move `input_` to signal the input has been processed.
    auto currentInput = std::move(input_);

    std::vector<int64_t> firstResultColumn, secondResultColumn; //a, f
    auto buffer = currentInput->childAt(0)->template asFlatVector<int64_t>();
    auto buffer2 = currentInput->childAt(1)->template asFlatVector<int64_t>();

    // make sure the inputs are ordered correctly
    auto& input0 = inputNames[0].first == "c" ? inputs[0] : inputs[1]; //c,d
    auto& input1 = inputNames[0].first != "c" ? inputs[0] : inputs[1]; //e,f

    //=============================================
    // HERE IS WHERE YOUR IMPLEMENTATION SHOULD GO!!!
    // HERE IS WHERE YOUR IMPLEMENTATION SHOULD GO!!!
    // HERE IS WHERE YOUR IMPLEMENTATION SHOULD GO!!!
    // HERE IS WHERE YOUR IMPLEMENTATION SHOULD GO!!!
    // HERE IS WHERE YOUR IMPLEMENTATION SHOULD GO!!!
    // HERE IS WHERE YOUR IMPLEMENTATION SHOULD GO!!!
    // HERE IS WHERE YOUR IMPLEMENTATION SHOULD GO!!!
    std::vector<int64_t> phase1_a;
    std::vector<int64_t> phase1_d;
    int buffer2_size = buffer2->size();
    int input0_size = input0.size();
    for (int i = 0; i < buffer2_size; i++) { //b
        for (int j = 0; j < input0_size; j++) {
            if (buffer2->valueAt(i) == input0[j].first){
                phase1_a.push_back(buffer->valueAt(i));
                phase1_d.push_back(input0[j].second);
            }
        }
    }

    std::cout << "Start Build \n";
    std::vector<std::pair<int64_t>> hash_table(16417, std::pair<int64_t, int64_t>());
    for (int i = 0; i < input1.size(); i++) {
        int64_t hash_val = hash_function(input1[i].first);
        while (hash_table[hash_val] != std::pair<int64_t, int64_t>()){
          hash_val = hash_function(hash_val);
        }
        hash_table[hash_val] = input1[i];
    }

    std::cout << "Start Probe \n";
    for (int j = 0; j < phase1_d.size(); j++) { 
        int64_t test_val = phase1_d[j];
        int64_t probe_val = hash_function(test_val);
        while (hash_table[probe_val] != std::pair<int64_t, int64_t>()){
          if (hash_table[probe_val].first == test_val){
            firstResultColumn.push_back(phase1_a[j]);
            secondResultColumn.push_back(hash_table[probe_val].second);
          }
        }
    }
    //=============================================

    inputs.clear();
    if(firstResultColumn.size() == 0)
      return nullptr;
    return makeRowVector({"a", "f"}, {makeFlatVector<int64_t>(firstResultColumn),
                                      makeFlatVector<int64_t>(secondResultColumn)});
  }

  int64_t hash_function(int64_t value) {
    return (value*8209) % 16417;
  }

  
  // This simple operator is never blocked.
  exec::BlockingReason isBlocked(ContinueFuture* future) override {
    return exec::BlockingReason::kNotBlocked;
  }

  bool isFinished() override { return !needsInput(); }
};

// Third, we need to define a plan translation logic to convert our custom plan
// node into our custom operator. Check `velox/exec/LocalPlanner.cpp` for more
// details.
class MultiWayJoinTranslator : public exec::Operator::PlanNodeTranslator {
  std::unique_ptr<exec::Operator> toOperator(exec::DriverCtx* ctx, int32_t id,
                                             const core::PlanNodePtr& node) override {
    if(auto dupRowNode = std::dynamic_pointer_cast<const MultiWayJoinNode>(node)) {
      return std::make_unique<MultiWayJoinOperator<1>>(id, ctx, dupRowNode);
    }
    return nullptr;
  }

  exec::OperatorSupplier toOperatorSupplier(const core::PlanNodePtr& node) override {
    if(auto dupRowNode = std::dynamic_pointer_cast<const MultiWayJoinNode>(node)) {
      return [dupRowNode](int32_t id, exec::DriverCtx* ctx) {
        return std::make_unique<MultiWayJoinOperator<0>>(id, ctx, dupRowNode);
      };
    }
    return nullptr;
  };
};
} // namespace
