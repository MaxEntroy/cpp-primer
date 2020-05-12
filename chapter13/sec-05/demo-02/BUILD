cc_library(
  name = "str_vec",
  srcs = [
    "str_vec.cc",
  ],
  deps = [
    ":str_vec_impl"
  ]
)

cc_library(
  name = "str_vec_impl",
  srcs = [
    "str_vec_impl.cc"
  ],
  deps = []
)

cc_test(
  name = "str_vec_test",
  srcs = [
    "str_vec_test.cc"
  ],
  deps = [
    ":str_vec",
  ]
)

## cc_test(
##   name = "str_vec_impl_test",
##   srcs = [
##     "str_vec_impl_test.cc"
##   ],
##   deps = [
##     ":str_vec_impl"
##   ]
## )
