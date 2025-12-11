#include <cassert>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#define out_hex(v) std::hex << std::setw(2) << std::setfill('0') << size_t(v)

enum class OperandType
{
   IMMEDIATE,
   REGISTER,
   MEMORY
};

enum struct ByteMode : int
{
   BYTE = 1,
   WORD = 2,
   DWORD = 4,
   QWORD = 8,
};

static const ByteMode CURRENT_MODE = ByteMode::WORD;  // indicate that we are on "bits 16"

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

    {"eax", {0, ByteMode::DWORD}},  //
    {"ecx", {1, ByteMode::DWORD}},  //
    {"edx", {2, ByteMode::DWORD}},  //
    {"ebx", {3, ByteMode::DWORD}},  //
    {"esp", {4, ByteMode::DWORD}},  //
    {"ebp", {5, ByteMode::DWORD}},  //
    {"esi", {6, ByteMode::DWORD}},  //
    {"edi", {7, ByteMode::DWORD}},  //

    {"ax", {0, ByteMode::WORD}},  //
    {"cx", {1, ByteMode::WORD}},  //
    {"dx", {2, ByteMode::WORD}},  //
    {"bx", {3, ByteMode::WORD}},  //
    {"sp", {4, ByteMode::WORD}},  //
    {"bp", {5, ByteMode::WORD}},  //
    {"si", {6, ByteMode::WORD}},  //
    {"di", {7, ByteMode::WORD}},  //

    {"ah", {0, ByteMode::BYTE}},  //
    {"ch", {1, ByteMode::BYTE}},  //
    {"dh", {2, ByteMode::BYTE}},  //
    {"bh", {3, ByteMode::BYTE}},  //
    {"al", {0, ByteMode::BYTE}},  //
    {"cl", {1, ByteMode::BYTE}},  //
    {"dl", {2, ByteMode::BYTE}},  //
    {"bl", {3, ByteMode::BYTE}},  //
};

static const std::unordered_map<std::string, std::uint8_t> mnemonics = {
    {"mov", 0x89},     //
    {"add", 0x01},     //
    {"sub", 0x29},     //
    {"mul", 0xF7},     //
    {"xor", 0x30},     //
    {"int", 0xCD},     //
    {"jmpn", 0xEB},    //
                       //
    {"mov_ax", 0xB8},  //
    {"mov_cx", 0xB9},  //
    {"mov_dx", 0xBA},  //
    {"mov_bx", 0xBB},  //
    {"mov_sp", 0xBC},  //
    {"mov_bp", 0xBD},  //
    {"mov_si", 0xBE},  //
    {"mov_di", 0xBF},  //

    {"mov_al", 0xB0},  //
    {"mov_cl", 0xB1},  //
    {"mov_dl", 0xB2},  //
    {"mov_bl", 0xB3},  //
    {"mov_ah", 0xB4},  //
    {"mov_ch", 0xB5},  //
    {"mov_dh", 0xB6},  //
    {"mov_bh", 0xB7},  //

    {"nop", 0x90},

};  // I should probably add a way to have the same mnemonic for different instructions depending on
    // their "profile", so that i can have the different movs share a name, just like in nasm.

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
         if (arg.size() < 3 || arg.at(1) != 'x')
         {
            value = std::strtoull(arg.c_str(), nullptr, 0);
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
            min_mode = ByteMode((arg.size() - 2) / 2);
            value = std::stoull(arg, nullptr, 0);
         }
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
      std::cout << " operand of " << int(min_mode) << " bytes: " << value << std::endl;
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

   Instruction(const std::string& line);

   void Print() const;
   EncodedInstruction Encode(const std::unordered_map<std::string, std::uint64_t>& labels,
                             size_t output_len) const;

   std::vector<std::uint8_t> Assemble(const std::unordered_map<std::string, std::uint64_t>& labels,
                                      size_t output_len) const;
};

Instruction::Instruction(const std::string& line)
{
   size_t op_index = line.find(' ');
   mnemonic = line.substr(0, op_index);

   mode = ByteMode::BYTE;

   size_t arg_index = op_index;
   while (arg_index != std::string::npos)
   {
      arg_index = line.find(',', arg_index + 1);
      std::string arg = line.substr(op_index + 1, arg_index - op_index - 1);
      op_index = line.find(' ', op_index + 1);

      auto operand = Operand(arg);
      operands.push_back(operand);
      if (int(operand.min_mode) > int(mode))
         mode = operand.min_mode;
   }
}

void Instruction::Print() const
{
   std::cout << "Mnemonic: " << mnemonic << std::endl;
   for (const auto& operand : operands)
      operand.Print();
}

EncodedInstruction Instruction::Encode(const std::unordered_map<std::string, std::uint64_t>& labels,
                                       size_t output_len) const
{
   EncodedInstruction encoded{};

   encoded.opcode = mnemonics.at(mnemonic);

   ByteMode bm = ByteMode::BYTE;

   for (const auto& op : operands)
      if (int(op.min_mode) > int(bm))
         bm = op.min_mode;

   std::uint8_t mode_pref = 0x00;
   switch (bm)
   {
   case ByteMode::QWORD:
      mode_pref = 0x48;
      break;
   case ByteMode::DWORD:
      break;
   case ByteMode::WORD:
      mode_pref = 0x66;
      break;
   case ByteMode::BYTE:
      break;
   }
   if (mode_pref && CURRENT_MODE != bm)
      encoded.prefixes.push_back(mode_pref);

   if (operands.size() == 2)
   {
      encoded.has_modregrm = true;
      if (operands[0].type == OperandType::REGISTER && operands[1].type == OperandType::REGISTER)
      {
         if (bm != ByteMode::BYTE)
            encoded.opcode |= 0b01;
         encoded.modregrm = 0b11 << 6;
         encoded.modregrm |= operands[1].value << 3;
         encoded.modregrm |= operands[0].value;
      }
   }
   else if (operands.size() == 1)
   {
      switch (operands[0].type)
      {
      case OperandType::IMMEDIATE:
         for (int i = 0; i < int(bm); i++)
            encoded.immediate.push_back((operands[0].value >> (8 * i)) & 0xFF);
         break;
      case OperandType::REGISTER:
         encoded.opcode += operands[0].value & 0xFF;
         break;
      case OperandType::MEMORY:
         //
         break;
      }
   }

   return encoded;
}

std::vector<std::uint8_t> Instruction::Assemble(
    const std::unordered_map<std::string, std::uint64_t>& labels, size_t output_len) const
{
   auto out = std::vector<std::uint8_t>();
   auto encoded = Encode(labels, output_len);

   for (const auto& pref : encoded.prefixes)
      out.push_back(pref);

   out.push_back(encoded.opcode);

   if (encoded.has_modregrm)
      out.push_back(encoded.modregrm);

   for (const auto& dis : encoded.displacement)
      out.push_back(dis);

   for (const auto& imm : encoded.immediate)
      out.push_back(imm);

   return out;
}

void ParseDenoterLine(const std::string& line,
                      std::unordered_map<std::string, std::uint64_t>& labels,
                      std::vector<std::uint8_t>& output);

void ParseLine(const std::string& line, std::unordered_map<std::string, std::uint64_t>& labels,
               std::vector<std::uint8_t>& output)
{
   switch (line.at(0))
   {
   case ';':  // comment
      break;
   case '.':  // denoter
      ParseDenoterLine(line, labels, output);
      break;
   case ':':
      labels.insert({line.substr(1), output.size()});
      break;  // label
   default:
      std::cout << "line:" << line << std::endl;

      Instruction instruction(line);

      instruction.Print();

      auto assembled = instruction.Assemble(labels, output.size());
      output.insert(output.end(), assembled.begin(), assembled.end());
      break;
   }
}

void ParseDenoterLine(const std::string& line,
                      std::unordered_map<std::string, std::uint64_t>& labels,
                      std::vector<std::uint8_t>& output)
{
   auto op_index = line.find(' ');
   auto mnemonic = line.substr(1, op_index - 1);

   std::cout << "Special line " << line << std::endl;
   std::cout << "Mnemonic |" << mnemonic << "|" << std::endl;

   if (mnemonic == "db")
   {
      auto op1_end = line.find(' ', op_index);
      auto op1 = line.substr(op_index + 1, op1_end + 2);
      std::cout << "Denote byte: " << op1 << std::endl;
      output.push_back(static_cast<std::uint8_t>(std::strtoul(op1.c_str(), nullptr, 0)));
   }

   else if (mnemonic == "times")
   {
      std::cout << "Multiplier denoter" << std::endl;
      auto op1_end = line.find(' ', op_index + 1);
      auto op1 = line.substr(op_index + 1, op1_end + 2);  // comma space for the sake of consistency

      auto repeats = std::stoull(op1);

      for (auto i = 0llu; i < repeats; i++)
         ParseLine(line.substr(op1_end + 1), labels, output);
   }
}

int main(int argc, char* argv[])
{
   // assert(argc == 3 && "Wrong arguments! Pattern is /path/to/tpc <input file> <output file>");
   std::ifstream infile(argc == 3 ? argv[1] : "C:/msys64/home/User/tpc/test.asm");
   assert(infile && "Could not find the file specified");

   std::string line;

   auto output = std::vector<std::uint8_t>();

   infile.seekg(infile.beg);

   auto labels = std::unordered_map<std::string, std::uint64_t>();

   while (std::getline(infile, line))
   {
      ParseLine(line, labels, output);
   }
   infile.close();

   std::ofstream outfile(argc == 3 ? argv[2] : "C:/msys64/home/User/tpc/test.bin",
                         std::ios::binary);
   assert(outfile && "Could not find the file specified");
   for (const auto& a : output)
      std::cout << out_hex(a) << " ";
   std::cout << std::endl << std::dec << output.size() << " bytes written" << std::endl;
   outfile.write(reinterpret_cast<const char*>(output.data()), output.size());
   outfile.close();

   return 0;
}
