#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include <google/protobuf/util/time_util.h>

#include "helper.h"
#ifdef BAZEL_BUILD
#include "api/deduction.grpc.pb.h"
#else
#include "deduction.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

using google::protobuf::util::TimeUtil;

using deduction::Deduction;
using deduction::Deductions;
using deduction::DeductionService;
using deduction::Request;

class DeductionServer final : public DeductionService::Service
{
public:
    Status GetDeductionServices(ServerContext *context, const Request *request, ServerWriter<Deductions> *response) override
    {
        std::cout << TimeUtil::SecondsToTimestamp(time(NULL)) << " @GetDeductionServices" << std::endl;
        std::fstream database_file(DATABASE_PATH, std::ios::in | std::ios::binary);
        Deductions deductions;
        if (!deductions.ParseFromIstream(&database_file))
        {
            return Status::CANCELLED;
        }

        response->Write(deductions);
        return Status::OK;
    }

    Status OpenDeduction(ServerContext *context, const Deduction *request, Deduction *response) override
    {
        std::cout << TimeUtil::SecondsToTimestamp(time(NULL)) << " @OpenDeduction: id=" << request->id() << std::endl;
        if (request->id() <= 0)
        {
            std::cout << "Error: Illegal Request ID!" << std::endl;
            return Status::CANCELLED;
        }

        // Search Database with request->id()
        Deductions deductions;
        int i = SearchDatabase(request->id(), deductions);
        if (i == -1)
        {
            return Status::CANCELLED;
        }

        // Update Database
        deductions.mutable_deduction()->at(i).set_status(1);
        *deductions.mutable_deduction()->at(i).mutable_opened_at() = TimeUtil::SecondsToTimestamp(time(NULL));
        deductions.mutable_deduction()->at(i).clear_closed_at();

        if (!SaveOrUpdateDatabase(deductions))
        {
            return Status::CANCELLED;
        }

        response->CopyFrom(deductions.deduction(i));
        return Status::OK;
    }

    Status CloseDeduction(ServerContext *context, const Deduction *request, Deduction *response) override
    {
        std::cout << TimeUtil::SecondsToTimestamp(time(NULL)) << " @CloseDeduction: id=" << request->id() << std::endl;
        if (request->id() <= 0)
        {
            std::cout << "Error: Illegal Request ID!" << std::endl;
            return Status::CANCELLED;
        }

        // Search Database with request->id()
        Deductions deductions;
        int i = SearchDatabase(request->id(), deductions);
        if (i == -1)
        {
            return Status::CANCELLED;
        }

        // Update Database
        deductions.mutable_deduction()->at(i).set_status(0);
        deductions.mutable_deduction()->at(i).clear_opened_at();
        *deductions.mutable_deduction()->at(i).mutable_closed_at() = TimeUtil::SecondsToTimestamp(time(NULL));

        if (!SaveOrUpdateDatabase(deductions))
        {
            return Status::CANCELLED;
        }

        response->CopyFrom(deductions.deduction(i));
        return Status::OK;
    }
};

void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    DeductionServer service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;

    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

void InitDatabase()
{
    if (deduction::IsFileExist(DATABASE_PATH))
    {
        return;
    }

    Deduction d;
    d.set_id(1);
    d.set_name("滴滴出行微信免密支付");
    d.set_provider("滴滴出行");
    d.set_description("服务描述");
    d.set_payment("零钱");
    d.set_status(0);

    Deduction d1;
    d1.set_id(2);
    d1.set_name("深圳公交免密支付");
    d1.set_provider("深圳公交");
    d1.set_description("服务描述");
    d1.set_payment("零钱");
    d1.set_status(0);

    Deductions deductions;
    deductions.mutable_deduction()->Add(std::move(d));
    deductions.mutable_deduction()->Add(std::move(d1));

    if (!SaveOrUpdateDatabase(deductions))
    {
        exit(-1);
    }
}

int main(int argc, char **argv)
{
    // Init Database
    InitDatabase();

    // Start server
    RunServer();

    return 0;
}
