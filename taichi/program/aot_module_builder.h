#pragma once

#include <string>
#include <vector>

#include "taichi/ir/snode.h"

namespace taichi {
namespace lang {

class Kernel;
class DataType;

class AotModuleBuilder {
 public:
  virtual ~AotModuleBuilder() = default;

  void add(const std::string &identifier, Kernel *kernel);

  void add_field(const std::string &identifier,
                 const SNode *rep_snode,
                 bool is_scalar,
                 DataType dt,
                 std::vector<int> shape,
                 int row_num,
                 int column_num);

  void add_kernel_template(const std::string &identifier,
                           const std::string &key,
                           Kernel *kernel);

  virtual void dump(const std::string &output_dir,
                    const std::string &filename) const = 0;

 protected:
  /**
   * Intended to be overriden by each backend's implementation.
   */
  virtual void add_per_backend(const std::string &identifier,
                               Kernel *kernel) = 0;
  virtual void add_field_per_backend(const std::string &identifier,
                                     const SNode *rep_snode,
                                     bool is_scalar,
                                     DataType dt,
                                     std::vector<int> shape,
                                     int row_num,
                                     int column_num) = 0;
  virtual void add_ndarray_per_backend(const std::string &identifier,
                                       bool is_scalar,
                                       DataType dt,
                                       std::vector<int> shape,
                                       int row_num,
                                       int column_num) {
    TI_NOT_IMPLEMENTED;
  }

  virtual void add_per_backend_tmpl(const std::string &identifier,
                                    const std::string &key,
                                    Kernel *kernel) = 0;

  static bool all_fields_are_dense_in_container(const SNode *container);

  static int find_children_id(const SNode *snode);
};

}  // namespace lang
}  // namespace taichi
