#include <jank/profile/time.hpp>

namespace jank::profile
{
  constexpr native_persistent_string_view tag{ "jank::profile" };
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static native_bool enabled{};
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static std::ofstream output;

  static auto now()
  {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
  }

  void configure(util::cli::options const &opts)
  {
    enabled = opts.profiler_enabled;

    if(enabled)
    {
      output.open(opts.profiler_file.data());
      if(!output.is_open())
      {
        enabled = false;
        fmt::println(stderr,
                     "Unable to open profile file: {}\nProfiling is now disabled.",
                     opts.profiler_file);
      }
    }
  }

  void enter(native_persistent_string_view const &region)
  {
    if(enabled)
    {
      fmt::println(output, "{} {} enter {}", tag, now(), region);
    }
  }

  void exit(native_persistent_string_view const &region)
  {
    if(enabled)
    {
      fmt::println(output, "{} {} exit {}", tag, now(), region);
    }
  }

  void report(native_persistent_string_view const &boundary)
  {
    if(enabled)
    {
      fmt::println(output, "{} {} report {}", tag, now(), boundary);
    }
  }

  timer::timer(native_persistent_string_view const &region)
    : region{ region }
  {
    enter(region);
  }

  timer::~timer()
  {
    exit(region);
  }

  void timer::report(native_persistent_string_view const &boundary) const
  {
    jank::profile::report(boundary);
  }
}
