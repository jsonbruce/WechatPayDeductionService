#include "helper.h"

using google::protobuf::util::TimeUtil;

namespace deduction
{
  bool IsFileExist(const std::string &fpath)
  {
    std::filesystem::path p(fpath);
    return std::filesystem::exists(p);
  }

  void PrintDeduction(const Deduction &d)
  {
    std::cout << "ID=" << d.id() << std::endl
              << "\t name=" << d.name() << std::endl
              << "\t provider=" << d.provider() << std::endl
              << "\t description=" << d.description() << std::endl
              << "\t payment=" << d.payment() << std::endl
              << "\t status=" << (d.status() == 0 ? DEDUCTION_STATUS_0 : DEDUCTION_STATUS_1) << std::endl;
    if (d.has_opened_at())
    {
      std::cout << "\t opened_at=" << TimeUtil::ToString(d.opened_at()) << std::endl;
    }
    if (d.has_closed_at())
    {
      std::cout << "\t closed_at=" << TimeUtil::ToString(d.closed_at()) << std::endl;
    }
  }

  int SearchDatabase(uint64_t id, Deductions &deductions)
  {
    std::fstream database_file(DATABASE_PATH, std::ios::in | std::ios::binary);
    if (!deductions.ParseFromIstream(&database_file))
    {
      std::cout << "Error: Open database!" << std::endl;
      return -1;
    }
    int i;
    for (i = 0; i < deductions.deduction_size() && deductions.deduction(i).id() != id; i++)
    {
    }
    if (i >= deductions.deduction_size())
    {
      std::cout << "Error: Not Found id=" << id << std::endl;
      return -1;
    }
    return i;
  }

  bool SaveOrUpdateDatabase(Deductions &deductions)
  {
    std::fstream database_file(DATABASE_PATH, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!deductions.SerializeToOstream(&database_file))
    {
      std::cout << "Error: SaveOrUpdate database!" << std::endl;
      return false;
    }
    return true;
  }
} // namespace deduction