#pragma once

#include "taichi/lang_util.h"

namespace taichi {
namespace lang {

class Program;
class IRNode;
class FrontendContext;

class Callable {
 public:
  Program *program;
  std::unique_ptr<IRNode> ir;
  std::unique_ptr<FrontendContext> context;

  struct Arg {
    DataType dt;
    // For arr args
    bool is_external_array{false};
    std::size_t size{0};  // TODO: size is runtime information, maybe remove?
    std::size_t total_dim{0};              // total dim of array
    std::vector<int> element_shapes = {};  // shape of each element

    explicit Arg(const DataType &dt = PrimitiveType::unknown,
                 bool is_external_array = false,
                 std::size_t size = 0,
                 int total_dim = 0,
                 std::vector<int> element_shapes = {})
        : dt(dt),
          is_external_array(is_external_array),
          size(size),
          total_dim(total_dim),
          element_shapes(std::move(element_shapes)) {
    }
  };

  struct Ret {
    DataType dt;

    explicit Ret(const DataType &dt = PrimitiveType::unknown) : dt(dt) {
    }
  };

  std::vector<Arg> args;
  std::vector<Ret> rets;

  virtual ~Callable() = default;

  int insert_arg(const DataType &dt, bool is_external_array);

  int insert_arr_arg(const DataType &dt,
                     int total_dim,
                     std::vector<int> element_shapes);

  int insert_ret(const DataType &dt);

  [[nodiscard]] virtual std::string get_name() const = 0;

  class CurrentCallableGuard {
    Callable *old_callable;
    Program *program;

   public:
    CurrentCallableGuard(Program *program, Callable *callable);

    ~CurrentCallableGuard();
  };
};

}  // namespace lang
}  // namespace taichi
