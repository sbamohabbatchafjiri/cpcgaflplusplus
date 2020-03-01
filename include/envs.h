const char *afl_environment_variables[] = {

    "AFL_ALIGNED_ALLOC", "AFL_ALLOW_TMP", "AFL_ANALYZE_HEX", "AFL_AS",
    "AFL_AUTORESUME",
    "AFL_AS_FORCE_INSTRUMENT", "AFL_BENCH_JUST_ONE", "AFL_BENCH_UNTIL_CRASH",
    "AFL_CAL_FAST", "AFL_CC", "AFL_CMIN_ALLOW_ANY", "AFL_CMIN_CRASHES_ONLY",
    "AFL_CODE_END", "AFL_CODE_START", "AFL_COMPCOV_BINNAME",
    "AFL_COMPCOV_LEVEL", "AFL_CUSTOM_MUTATOR_LIBRARY",
    "AFL_CUSTOM_MUTATOR_ONLY", "AFL_CXX", "AFL_DEBUG", "AFL_DEBUG_CHILD_OUTPUT",
    //"AFL_DEFER_FORKSRV", // not implemented anymore, so warn additionally
    "AFL_DISABLE_TRIM", "AFL_DONT_OPTIMIZE", "AFL_DUMB_FORKSRV",
    "AFL_ENTRYPOINT", "AFL_EXIT_WHEN_DONE", "AFL_FAST_CAL", "AFL_FORCE_UI",
    "AFL_GCC_WHITELIST", "AFL_GCJ", "AFL_afl->hang_tmout", "AFL_HARDEN",
    "AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES", "AFL_IMPORT_FIRST",
    "AFL_INST_LIBS", "AFL_INST_RATIO", "AFL_KEEP_TRACES", "AFL_KEEP_ASSEMBLY",
    "AFL_LD_HARD_FAIL", "AFL_LD_LIMIT_MB", "AFL_LD_NO_CALLOC_OVER",
    "AFL_LD_PRELOAD", "AFL_LD_VERBOSE", "AFL_LLVM_CMPLOG", "AFL_LLVM_INSTRIM",
    "AFL_LLVM_INSTRIM_LOOPHEAD", "AFL_LLVM_LAF_SPLIT_COMPARES",
    "AFL_LLVM_LAF_SPLIT_COMPARES_BITW", "AFL_LLVM_LAF_SPLIT_FLOATS",
    "AFL_LLVM_LAF_SPLIT_SWITCHES", "AFL_LLVM_LAF_TRANSFORM_COMPARES",
    "AFL_LLVM_NOT_ZERO", "AFL_LLVM_WHITELIST", "AFL_NO_AFFINITY",
    "AFL_NO_ARITH", "AFL_NO_BUILTIN", "AFL_NO_CPU_RED", "AFL_NO_FORKSRV",
    "AFL_NO_UI",
    "AFL_NO_X86",  // not really an env but we dont want to warn on it
    "AFL_PATH", "AFL_PERFORMANCE_FILE",
    //"AFL_PERSISTENT", // not implemented anymore, so warn additionally
    "AFL_POST_LIBRARY", "AFL_PRELOAD", "AFL_PYTHON_MODULE", "AFL_PYTHON_ONLY",
    "AFL_QEMU_COMPCOV", "AFL_QEMU_COMPCOV_DEBUG", "AFL_QEMU_DEBUG_MAPS",
    "AFL_QEMU_DISABLE_CACHE", "AFL_QEMU_PERSISTENT_ADDR",
    "AFL_QEMU_PERSISTENT_CNT", "AFL_QEMU_PERSISTENT_GPR",
    "AFL_QEMU_PERSISTENT_HOOK", "AFL_QEMU_PERSISTENT_RET",
    "AFL_QEMU_PERSISTENT_RETADDR_OFFSET", "AFL_QUIET",
    "AFL_RANDOM_ALLOC_CANARY", "AFL_REAL_PATH", "AFL_SHUFFLE_QUEUE",
    "AFL_SKIP_BIN_CHECK", "AFL_SKIP_CPUFREQ", "AFL_SKIP_CRASHES",
    "AFL_TMIN_EXACT", "AFL_TMPDIR", "AFL_TOKEN_FILE", "AFL_TRACE_PC",
    "AFL_USE_ASAN", "AFL_USE_MSAN", "AFL_USE_TRACE_PC", "AFL_USE_UBSAN",
    "AFL_WINE_PATH", NULL};

