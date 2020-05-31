#ifndef DEDUCTION_HELPER_H_
#define DEDUCTION_HELPER_H_

#include <string>
#include <fstream>
#include <filesystem>

#include <google/protobuf/util/time_util.h>

#ifdef BAZEL_BUILD
#include "api/deduction.grpc.pb.h"
#else
#include "deduction.grpc.pb.h"
#endif

#define DATABASE_PATH "deduction.db"
#define DEDUCTION_STATUS_0 "未启用"
#define DEDUCTION_STATUS_1 "生效中"

namespace deduction {
    bool IsFileExist(const std::string& fpath);

    void PrintDeduction(const Deduction &d);

    // Search database with specified id, return the index. -1 for not found or error.
    int SearchDatabase(uint64_t id, Deductions& deductions);

    // Save or update database with new or updated data. return true if saveOrupdate success.
    bool SaveOrUpdateDatabase(Deductions& deductions);
}

#endif
