#include <gflags/gflags.h>

#include "text_query.h"

DEFINE_string(txt_path, "", "path of text");

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  cp::TextQuery tq(FLAGS_txt_path);
  if (tq.Init()) {
    tq.EventLoop();
  }

  return 0;
}
