#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

enum class OperandType
{
   IMMEDIATE,
   REGISTER,
   MEMORY
};

enum class ByteMode
{
   BYTE,
   WORD,
   DWORD,
   QWORD,
};

class Register
{
 public:
   std::uint8_t code;
   ByteMode mode;
};

static const std::unordered_map<std::string, Register> registers = {
    {"rax", {0, ByteMode::QWORD}},  //
    {"rcx", {1, ByteMode::QWORD}},  //
    {"rdx", {2, ByteMode::QWORD}},  //
    {"rbx", {3, ByteMode::QWORD}},  //
    {"rsp", {4, ByteMode::QWORD}},  //
    {"rbp", {5, ByteMode::QWORD}},  //
    {"rsi", {6, ByteMode::QWORD}},  //
    {"rdi", {7, ByteMode::QWORD}},  //
};

static const std::unordered_map<std::string, std::uint8_t> mnemonics = {
    {"mov", 0x89},  //
    {"add", 0x01},  //
    {"sub", 0x29},  //
    {"mul", 0xF7},  //
    {"div", 0xF7},  //
};

class Operand
{
 public:
   OperandType type;
   ByteMode min_mode;
   std::uint64_t value;
   std::uint16_t offset;

   Operand(const std::string& arg)
   {
      if (registers.find(arg) != registers.end())
      {
         type = OperandType::REGISTER;
         Register reg = registers.at(arg);
         value = reg.code;
         min_mode = reg.mode;
      }
      else if (arg.find('[') != std::string::npos)
      {
         type = OperandType::MEMORY;
         size_t beg = arg.find('[') + 1;
         size_t end = arg.find(']');
         std::string contents = arg.substr(beg, end - beg);
         std::cout << "Memory access from " << contents << std::endl;
         value = std::stoull(contents, nullptr, 0);
         // TODO: parse for offset stuff. Also need a more complex system for how to tell what kind
         // of offset it has. Some may have a register involved, and then an immediate.
         // the segment may also be needed.
         // it might be good to actually make dereference be a boolean and the type not be specific
         // for memory
         if (value <= UINT8_MAX)
            min_mode = ByteMode::BYTE;
         else if (value <= UINT16_MAX)
            min_mode = ByteMode::WORD;
         else if (value <= UINT32_MAX)
            min_mode = ByteMode::DWORD;
         else
            min_mode = ByteMode::QWORD;
      }
      else
      {
         type = OperandType::IMMEDIATE;
         value = std::stoull(arg, nullptr, 0);
         if (value <= UINT8_MAX)
            min_mode = ByteMode::BYTE;
         else if (value <= UINT16_MAX)
            min_mode = ByteMode::WORD;
         else if (value <= UINT32_MAX)
            min_mode = ByteMode::DWORD;
         else
            min_mode = ByteMode::QWORD;
      }
   }

   void Print() const
   {
      switch (type)
      {
      case OperandType::IMMEDIATE:
         std::cout << "IMMEDIATE";
         break;
      case OperandType::REGISTER:
         std::cout << "REGISTER";
         break;
      case OperandType::MEMORY:
         std::cout << "MEMORY";
         break;
      }
      std::cout << " operand: " << value << std::endl;
   }
};

struct EncodedInstruction
{
   using byte = std::uint8_t;
   std::vector<byte> prefixes;
   byte opcode;
   byte modregrm;
   bool has_modregrm;
   byte sib;
   bool has_sib;
   std::vector<byte> displacement;
   std::vector<byte> immediate;
};

class Instruction
{
 public:
   std::string mnemonic;
   ByteMode mode;
   std::vector<Operand> operands;

   Instruction(const std::string& line)
   {
      size_t op_index = line.find(' ');
      mnemonic = line.substr(0, op_index);

      size_t arg_index = op_index;
      while (arg_index != std::string::npos)
      {
         arg_index = line.find(',', arg_index + 1);
         std::string arg = line.substr(op_index + 1, arg_index - op_index - 1);
         op_index = line.find(' ', op_index + 1);

         operands.push_back(Operand(arg));
      }
   }

   void Print() const
   {
      std::cout << "Mnemonic: " << mnemonic << std::endl;
      for (const auto& operand : operands)
         operand.Print();
   }

   EncodedInstruction Encode()
   {
      EncodedInstruction encoded;

      encoded.opcode = mnemonics.at(mnemonic);

      if (operands.size() == 2)
      {
      }
      else if (operands.size() == 1)
      {
         switch (operands[0].type)
         {
         case OperandType::IMMEDIATE:
            //
            break;
         case OperandType::REGISTER:
            //
            break;
         case OperandType::MEMORY:
            //
            break;
         }
      }

      return encoded;
   }

   void Assemble() {}
};

int main()
{
   std::ifstream infile("C:\\msys64\\home\\User\\tpc\\test.asm");

   std::string line;

   size_t line_num = 0;
   while (std::getline(infile, line))
   {
      std::cout << line_num++ << ": " << line << std::endl;

      Instruction instruction(line);

      instruction.Print();
   }

   return 0;
}
