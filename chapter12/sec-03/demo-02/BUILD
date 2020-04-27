cc_binary(
  name = "main",
  srcs = [
    "main.cc"
  ],
  deps = [
    ":text_query",
    "#gflags",
    "#pthread"
  ]
)

cc_library(
  name = "text_query",
  srcs = [
    "text_query.cc",
    "text_query_impl.cc"
  ],
  deps = [
    ":string_util"
  ]
)

cc_library(
  name = "string_util",
  srcs = [
    "string_util.cc"
  ],
  deps = []
)

cc_test(
  name = "string_utl_test",
  srcs = [
    "string_util_test.cc"
  ],
  deps = [
    ":string_util"
  ]
)

cc_test(
  name = "text_query_impl_test",
  srcs = [
    "text_query_impl_test.cc"
  ],
  deps = [
    ":text_query"
  ],
  defs = [
    "PTEST"
  ]
)

cc_test(
  name = "text_query_test",
  srcs = [
    "text_query_test.cc"
  ],
  deps = [
    ":text_query"
  ],
  defs = [
    "PTEST"
  ]
)
