cc_binary(
  name = "main",
  srcs = [
    "main.cc"
  ],
  deps = [
    ":str_blob",
    ":weak_str_blob"
  ]
)

cc_library(
  name = "str_blob",
  srcs = [
    "str_blob.cc",
  ],
  deps = [
    "#gtest"
  ]
)

cc_library(
  name = "weak_str_blob",
  srcs = [
    "weak_str_blob.cc"
  ],
  deps = []
)

## cc_test(
##   name = "str_blob_test",
##   srcs = [
##     "str_blob_test.cc"
##   ],
##   deps = [
##     ":str_blob"
##   ]
## )

cc_test(
  name = "weak_str_blob_test",
  srcs = [
    "weak_str_blob_test.cc"
  ],
  deps = [
    ":str_blob",
    ":weak_str_blob"
  ],
  defs = [
    "PTEST"
  ]
)
