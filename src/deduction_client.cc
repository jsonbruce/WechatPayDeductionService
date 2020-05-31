#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include <google/protobuf/util/time_util.h>

#include "helper.h"
#ifdef BAZEL_BUILD
#include "api/deduction.grpc.pb.h"
#else
#include "deduction.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;

using google::protobuf::util::TimeUtil;

using deduction::Deduction;
using deduction::Deductions;
using deduction::DeductionService;
using deduction::Request;


class DeductionClient
{
public:
  DeductionClient(std::shared_ptr<Channel> channel) : stub_(DeductionService::NewStub(channel)) {}

  void GetDeductionServices()
  {
    ClientContext context;
    Request request;
    Deductions deductions;

    std::unique_ptr<ClientReader<Deductions>> reader(stub_->GetDeductionServices(&context, request));

    while (reader->Read(&deductions))
    {
      for (int i = 0; i < deductions.deduction_size(); i++)
      {
        const Deduction &d = deductions.deduction(i);
        PrintDeduction(d);
      }
    }

    Status status = reader->Finish();
    if (!status.ok())
    {
      std::cout << "FAIL." << std::endl;
    }
  }

  void OpenDeduction(uint64_t id)
  {
    ClientContext context;
    Deduction response;
    Deduction deduction;
    deduction.set_id(id);

    Status status = stub_->OpenDeduction(&context, deduction, &response);
    if (status.ok())
    {
      PrintDeduction(response);
    }
  }

  void CloseDeduction(uint64_t id)
  {
    ClientContext context;
    Deduction response;
    Deduction deduction;
    deduction.set_id(id);

    Status status = stub_->CloseDeduction(&context, deduction, &response);
    if (status.ok())
    {
      PrintDeduction(response);
    }
  }

private:
  std::unique_ptr<DeductionService::Stub> stub_;
};

int main(int argc, char** argv)
{
  if (argc <= 1)
  {
    std::cout << "deduction_client communicates with deduction services." << std::endl << std::endl
              << "Usage:" << std::endl
              << "  deduction_client [action] [options]" << std::endl << std::endl
              << "Available Commands:" << std::endl
              << "  list               \t Get all deduction services" << std::endl
              << "  open <service_id>  \t Open a deduction service" << std::endl
              << "  close <service_id> \t Close a deduction service" << std::endl;
    return 0;
  }

  std::string target_str = "localhost:50051";
  DeductionClient client(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  
  for (int i = 1; i < argc; i++)
  {
    if (std::strcmp("list", argv[i]) == 0)
    {
      client.GetDeductionServices();
      break;
    } else if (std::strcmp("open", argv[i]) == 0)
    {
      if ((i+1) >= argc)
      {
        std::cout << "Usage: deduction_client open <service_id>" << std::endl;
        return 0;
      }
      client.OpenDeduction(std::strtoull(argv[i+1], NULL, 0));
      break;
    } else if (std::strcmp("close", argv[i]) == 0)
    {
      if ((i+1) >= argc)
      {
        std::cout << "Usage: deduction_client close <service_id>" << std::endl;
        return 0;
      }
      client.CloseDeduction(std::strtoull(argv[i+1], NULL, 0));
      break;
    } else
    {
      std::cout << "Available Commands: list, open, close" << std::endl;
      return 0;
    }
  }

  return 0;
}
