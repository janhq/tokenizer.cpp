#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include "cpp-tiktoken/emdedded_resource_reader.h"
#include "cpp-tiktoken/encoding.h"
#include "sentencepiece/src/sentencepiece_processor.h"

namespace tokenizer_cpp {
// Class Tokenizer is not thread-safe
class Tokenizer {
 public:
  Tokenizer() {}
  virtual ~Tokenizer() {}

  virtual std::string DecodeWithSpace(const int id) { return ""; }

  virtual std::string Decode(const std::vector<int32_t> ids) = 0;

  virtual std::vector<int> Encode(const std::string& input) = 0;
};

class SentencePieceTokenizer : public Tokenizer {

 public:
  SentencePieceTokenizer(const std::string& model_path);

  std::string DecodeWithSpace(const int id) final;

  std::string Decode(const std::vector<int32_t> ids) final;

  std::vector<int> Encode(const std::string& input) final;

 private:
  void ReplaceSubstring(std::string& base, const std::string& from,
                        const std::string& to);

 private:
  sentencepiece::SentencePieceProcessor processor_;
};

class TiktokenTokenizer : public Tokenizer {
 public:
  TiktokenTokenizer(const std::string& model_path);

  std::string Decode(const std::vector<int32_t> ids) final;

  std::vector<int> Encode(const std::string& input) final;

 private:
  std::shared_ptr<GptEncoding> encoder_;
};

}  // namespace tokenizer_cpp