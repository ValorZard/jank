#pragma once

#include <jank/analyze/expression_base.hpp>
#include <jank/runtime/seq.hpp>

namespace jank::analyze::expr
{
  template <typename E>
  struct vector : expression_base
  {
    native_vector<native_box<E>> data_exprs;

    runtime::object_ptr to_runtime_data() const
    {
      runtime::object_ptr pair_maps(make_box<runtime::obj::vector>());
      for(auto const &e : data_exprs)
      {
        pair_maps = runtime::conj(pair_maps, e->to_runtime_data());
      }

      return runtime::obj::persistent_array_map::create_unique(make_box("__type"),
                                                               make_box("expr::vector"),
                                                               make_box("data_exprs"),
                                                               pair_maps);
    }
  };
}
