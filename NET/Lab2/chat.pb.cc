// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: chat.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "chat.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* Request_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Request_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* Request_RequestType_descriptor_ = NULL;
const ::google::protobuf::Descriptor* Answer_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Answer_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* Answer_AnswerType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_chat_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AssignDesc_chat_2eproto() {
  protobuf_AddDesc_chat_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "chat.proto");
  GOOGLE_CHECK(file != NULL);
  Request_descriptor_ = file->message_type(0);
  static const int Request_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Request, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Request, text_),
  };
  Request_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      Request_descriptor_,
      Request::default_instance_,
      Request_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Request, _has_bits_[0]),
      -1,
      -1,
      sizeof(Request),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Request, _internal_metadata_),
      -1);
  Request_RequestType_descriptor_ = Request_descriptor_->enum_type(0);
  Answer_descriptor_ = file->message_type(1);
  static const int Answer_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Answer, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Answer, sender_name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Answer, list_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Answer, text_),
  };
  Answer_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      Answer_descriptor_,
      Answer::default_instance_,
      Answer_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Answer, _has_bits_[0]),
      -1,
      -1,
      sizeof(Answer),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Answer, _internal_metadata_),
      -1);
  Answer_AnswerType_descriptor_ = Answer_descriptor_->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_chat_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      Request_descriptor_, &Request::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      Answer_descriptor_, &Answer::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_chat_2eproto() {
  delete Request::default_instance_;
  delete Request_reflection_;
  delete Answer::default_instance_;
  delete Answer_reflection_;
}

void protobuf_AddDesc_chat_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AddDesc_chat_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\nchat.proto\"f\n\007Request\022\"\n\004type\030\001 \002(\0162\024."
    "Request.RequestType\022\014\n\004text\030\002 \001(\t\")\n\013Req"
    "uestType\022\006\n\002DC\020\000\022\010\n\004LIST\020\001\022\010\n\004TEXT\020\002\"\213\001\n"
    "\006Answer\022 \n\004type\030\001 \002(\0162\022.Answer.AnswerTyp"
    "e\022\023\n\013sender_name\030\002 \001(\t\022\014\n\004list\030\003 \003(\t\022\014\n\004"
    "text\030\004 \001(\t\".\n\nAnswerType\022\010\n\004LIST\020\000\022\010\n\004TE"
    "XT\020\001\022\014\n\010GREETING\020\002", 258);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "chat.proto", &protobuf_RegisterTypes);
  Request::default_instance_ = new Request();
  Answer::default_instance_ = new Answer();
  Request::default_instance_->InitAsDefaultInstance();
  Answer::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_chat_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_chat_2eproto {
  StaticDescriptorInitializer_chat_2eproto() {
    protobuf_AddDesc_chat_2eproto();
  }
} static_descriptor_initializer_chat_2eproto_;

// ===================================================================

const ::google::protobuf::EnumDescriptor* Request_RequestType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Request_RequestType_descriptor_;
}
bool Request_RequestType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const Request_RequestType Request::DC;
const Request_RequestType Request::LIST;
const Request_RequestType Request::TEXT;
const Request_RequestType Request::RequestType_MIN;
const Request_RequestType Request::RequestType_MAX;
const int Request::RequestType_ARRAYSIZE;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Request::kTypeFieldNumber;
const int Request::kTextFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Request::Request()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:Request)
}

void Request::InitAsDefaultInstance() {
}

Request::Request(const Request& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:Request)
}

void Request::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  type_ = 0;
  text_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Request::~Request() {
  // @@protoc_insertion_point(destructor:Request)
  SharedDtor();
}

void Request::SharedDtor() {
  text_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (this != default_instance_) {
  }
}

void Request::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Request::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Request_descriptor_;
}

const Request& Request::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_chat_2eproto();
  return *default_instance_;
}

Request* Request::default_instance_ = NULL;

Request* Request::New(::google::protobuf::Arena* arena) const {
  Request* n = new Request;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Request::Clear() {
// @@protoc_insertion_point(message_clear_start:Request)
  if (_has_bits_[0 / 32] & 3u) {
    type_ = 0;
    if (has_text()) {
      text_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  if (_internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->Clear();
  }
}

bool Request::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:Request)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .Request.RequestType type = 1;
      case 1: {
        if (tag == 8) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::Request_RequestType_IsValid(value)) {
            set_type(static_cast< ::Request_RequestType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_text;
        break;
      }

      // optional string text = 2;
      case 2: {
        if (tag == 18) {
         parse_text:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_text()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->text().data(), this->text().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "Request.text");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:Request)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:Request)
  return false;
#undef DO_
}

void Request::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:Request)
  // required .Request.RequestType type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->type(), output);
  }

  // optional string text = 2;
  if (has_text()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->text().data(), this->text().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "Request.text");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->text(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:Request)
}

::google::protobuf::uint8* Request::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:Request)
  // required .Request.RequestType type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->type(), target);
  }

  // optional string text = 2;
  if (has_text()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->text().data(), this->text().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "Request.text");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->text(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Request)
  return target;
}

int Request::ByteSize() const {
// @@protoc_insertion_point(message_byte_size_start:Request)
  int total_size = 0;

  // required .Request.RequestType type = 1;
  if (has_type()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
  }
  // optional string text = 2;
  if (has_text()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->text());
  }

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Request::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:Request)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  const Request* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const Request>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:Request)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:Request)
    MergeFrom(*source);
  }
}

void Request::MergeFrom(const Request& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Request)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_text()) {
      set_has_text();
      text_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.text_);
    }
  }
  if (from._internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->MergeFrom(from.unknown_fields());
  }
}

void Request::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:Request)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Request::CopyFrom(const Request& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Request)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Request::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void Request::Swap(Request* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Request::InternalSwap(Request* other) {
  std::swap(type_, other->type_);
  text_.Swap(&other->text_);
  std::swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Request::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Request_descriptor_;
  metadata.reflection = Request_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// Request

// required .Request.RequestType type = 1;
bool Request::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
void Request::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
void Request::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
void Request::clear_type() {
  type_ = 0;
  clear_has_type();
}
 ::Request_RequestType Request::type() const {
  // @@protoc_insertion_point(field_get:Request.type)
  return static_cast< ::Request_RequestType >(type_);
}
 void Request::set_type(::Request_RequestType value) {
  assert(::Request_RequestType_IsValid(value));
  set_has_type();
  type_ = value;
  // @@protoc_insertion_point(field_set:Request.type)
}

// optional string text = 2;
bool Request::has_text() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
void Request::set_has_text() {
  _has_bits_[0] |= 0x00000002u;
}
void Request::clear_has_text() {
  _has_bits_[0] &= ~0x00000002u;
}
void Request::clear_text() {
  text_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_text();
}
 const ::std::string& Request::text() const {
  // @@protoc_insertion_point(field_get:Request.text)
  return text_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void Request::set_text(const ::std::string& value) {
  set_has_text();
  text_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Request.text)
}
 void Request::set_text(const char* value) {
  set_has_text();
  text_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Request.text)
}
 void Request::set_text(const char* value, size_t size) {
  set_has_text();
  text_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Request.text)
}
 ::std::string* Request::mutable_text() {
  set_has_text();
  // @@protoc_insertion_point(field_mutable:Request.text)
  return text_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 ::std::string* Request::release_text() {
  // @@protoc_insertion_point(field_release:Request.text)
  clear_has_text();
  return text_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void Request::set_allocated_text(::std::string* text) {
  if (text != NULL) {
    set_has_text();
  } else {
    clear_has_text();
  }
  text_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), text);
  // @@protoc_insertion_point(field_set_allocated:Request.text)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

const ::google::protobuf::EnumDescriptor* Answer_AnswerType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Answer_AnswerType_descriptor_;
}
bool Answer_AnswerType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const Answer_AnswerType Answer::LIST;
const Answer_AnswerType Answer::TEXT;
const Answer_AnswerType Answer::GREETING;
const Answer_AnswerType Answer::AnswerType_MIN;
const Answer_AnswerType Answer::AnswerType_MAX;
const int Answer::AnswerType_ARRAYSIZE;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Answer::kTypeFieldNumber;
const int Answer::kSenderNameFieldNumber;
const int Answer::kListFieldNumber;
const int Answer::kTextFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Answer::Answer()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:Answer)
}

void Answer::InitAsDefaultInstance() {
}

Answer::Answer(const Answer& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:Answer)
}

void Answer::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  type_ = 0;
  sender_name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  text_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Answer::~Answer() {
  // @@protoc_insertion_point(destructor:Answer)
  SharedDtor();
}

void Answer::SharedDtor() {
  sender_name_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  text_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (this != default_instance_) {
  }
}

void Answer::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Answer::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Answer_descriptor_;
}

const Answer& Answer::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_chat_2eproto();
  return *default_instance_;
}

Answer* Answer::default_instance_ = NULL;

Answer* Answer::New(::google::protobuf::Arena* arena) const {
  Answer* n = new Answer;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Answer::Clear() {
// @@protoc_insertion_point(message_clear_start:Answer)
  if (_has_bits_[0 / 32] & 11u) {
    type_ = 0;
    if (has_sender_name()) {
      sender_name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    if (has_text()) {
      text_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
  }
  list_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  if (_internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->Clear();
  }
}

bool Answer::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:Answer)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .Answer.AnswerType type = 1;
      case 1: {
        if (tag == 8) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::Answer_AnswerType_IsValid(value)) {
            set_type(static_cast< ::Answer_AnswerType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_sender_name;
        break;
      }

      // optional string sender_name = 2;
      case 2: {
        if (tag == 18) {
         parse_sender_name:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_sender_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->sender_name().data(), this->sender_name().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "Answer.sender_name");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_list;
        break;
      }

      // repeated string list = 3;
      case 3: {
        if (tag == 26) {
         parse_list:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->add_list()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->list(this->list_size() - 1).data(),
            this->list(this->list_size() - 1).length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "Answer.list");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_list;
        if (input->ExpectTag(34)) goto parse_text;
        break;
      }

      // optional string text = 4;
      case 4: {
        if (tag == 34) {
         parse_text:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_text()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->text().data(), this->text().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "Answer.text");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:Answer)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:Answer)
  return false;
#undef DO_
}

void Answer::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:Answer)
  // required .Answer.AnswerType type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->type(), output);
  }

  // optional string sender_name = 2;
  if (has_sender_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->sender_name().data(), this->sender_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "Answer.sender_name");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->sender_name(), output);
  }

  // repeated string list = 3;
  for (int i = 0; i < this->list_size(); i++) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->list(i).data(), this->list(i).length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "Answer.list");
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->list(i), output);
  }

  // optional string text = 4;
  if (has_text()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->text().data(), this->text().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "Answer.text");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      4, this->text(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:Answer)
}

::google::protobuf::uint8* Answer::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:Answer)
  // required .Answer.AnswerType type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->type(), target);
  }

  // optional string sender_name = 2;
  if (has_sender_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->sender_name().data(), this->sender_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "Answer.sender_name");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->sender_name(), target);
  }

  // repeated string list = 3;
  for (int i = 0; i < this->list_size(); i++) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->list(i).data(), this->list(i).length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "Answer.list");
    target = ::google::protobuf::internal::WireFormatLite::
      WriteStringToArray(3, this->list(i), target);
  }

  // optional string text = 4;
  if (has_text()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->text().data(), this->text().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "Answer.text");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        4, this->text(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Answer)
  return target;
}

int Answer::ByteSize() const {
// @@protoc_insertion_point(message_byte_size_start:Answer)
  int total_size = 0;

  // required .Answer.AnswerType type = 1;
  if (has_type()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
  }
  if (_has_bits_[1 / 32] & 10u) {
    // optional string sender_name = 2;
    if (has_sender_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->sender_name());
    }

    // optional string text = 4;
    if (has_text()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->text());
    }

  }
  // repeated string list = 3;
  total_size += 1 * this->list_size();
  for (int i = 0; i < this->list_size(); i++) {
    total_size += ::google::protobuf::internal::WireFormatLite::StringSize(
      this->list(i));
  }

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Answer::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:Answer)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  const Answer* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const Answer>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:Answer)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:Answer)
    MergeFrom(*source);
  }
}

void Answer::MergeFrom(const Answer& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Answer)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  list_.MergeFrom(from.list_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_sender_name()) {
      set_has_sender_name();
      sender_name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.sender_name_);
    }
    if (from.has_text()) {
      set_has_text();
      text_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.text_);
    }
  }
  if (from._internal_metadata_.have_unknown_fields()) {
    mutable_unknown_fields()->MergeFrom(from.unknown_fields());
  }
}

void Answer::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:Answer)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Answer::CopyFrom(const Answer& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Answer)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Answer::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void Answer::Swap(Answer* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Answer::InternalSwap(Answer* other) {
  std::swap(type_, other->type_);
  sender_name_.Swap(&other->sender_name_);
  list_.UnsafeArenaSwap(&other->list_);
  text_.Swap(&other->text_);
  std::swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Answer::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Answer_descriptor_;
  metadata.reflection = Answer_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// Answer

// required .Answer.AnswerType type = 1;
bool Answer::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
void Answer::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
void Answer::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
void Answer::clear_type() {
  type_ = 0;
  clear_has_type();
}
 ::Answer_AnswerType Answer::type() const {
  // @@protoc_insertion_point(field_get:Answer.type)
  return static_cast< ::Answer_AnswerType >(type_);
}
 void Answer::set_type(::Answer_AnswerType value) {
  assert(::Answer_AnswerType_IsValid(value));
  set_has_type();
  type_ = value;
  // @@protoc_insertion_point(field_set:Answer.type)
}

// optional string sender_name = 2;
bool Answer::has_sender_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
void Answer::set_has_sender_name() {
  _has_bits_[0] |= 0x00000002u;
}
void Answer::clear_has_sender_name() {
  _has_bits_[0] &= ~0x00000002u;
}
void Answer::clear_sender_name() {
  sender_name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_sender_name();
}
 const ::std::string& Answer::sender_name() const {
  // @@protoc_insertion_point(field_get:Answer.sender_name)
  return sender_name_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void Answer::set_sender_name(const ::std::string& value) {
  set_has_sender_name();
  sender_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Answer.sender_name)
}
 void Answer::set_sender_name(const char* value) {
  set_has_sender_name();
  sender_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Answer.sender_name)
}
 void Answer::set_sender_name(const char* value, size_t size) {
  set_has_sender_name();
  sender_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Answer.sender_name)
}
 ::std::string* Answer::mutable_sender_name() {
  set_has_sender_name();
  // @@protoc_insertion_point(field_mutable:Answer.sender_name)
  return sender_name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 ::std::string* Answer::release_sender_name() {
  // @@protoc_insertion_point(field_release:Answer.sender_name)
  clear_has_sender_name();
  return sender_name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void Answer::set_allocated_sender_name(::std::string* sender_name) {
  if (sender_name != NULL) {
    set_has_sender_name();
  } else {
    clear_has_sender_name();
  }
  sender_name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), sender_name);
  // @@protoc_insertion_point(field_set_allocated:Answer.sender_name)
}

// repeated string list = 3;
int Answer::list_size() const {
  return list_.size();
}
void Answer::clear_list() {
  list_.Clear();
}
 const ::std::string& Answer::list(int index) const {
  // @@protoc_insertion_point(field_get:Answer.list)
  return list_.Get(index);
}
 ::std::string* Answer::mutable_list(int index) {
  // @@protoc_insertion_point(field_mutable:Answer.list)
  return list_.Mutable(index);
}
 void Answer::set_list(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:Answer.list)
  list_.Mutable(index)->assign(value);
}
 void Answer::set_list(int index, const char* value) {
  list_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:Answer.list)
}
 void Answer::set_list(int index, const char* value, size_t size) {
  list_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:Answer.list)
}
 ::std::string* Answer::add_list() {
  // @@protoc_insertion_point(field_add_mutable:Answer.list)
  return list_.Add();
}
 void Answer::add_list(const ::std::string& value) {
  list_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:Answer.list)
}
 void Answer::add_list(const char* value) {
  list_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:Answer.list)
}
 void Answer::add_list(const char* value, size_t size) {
  list_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:Answer.list)
}
 const ::google::protobuf::RepeatedPtrField< ::std::string>&
Answer::list() const {
  // @@protoc_insertion_point(field_list:Answer.list)
  return list_;
}
 ::google::protobuf::RepeatedPtrField< ::std::string>*
Answer::mutable_list() {
  // @@protoc_insertion_point(field_mutable_list:Answer.list)
  return &list_;
}

// optional string text = 4;
bool Answer::has_text() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
void Answer::set_has_text() {
  _has_bits_[0] |= 0x00000008u;
}
void Answer::clear_has_text() {
  _has_bits_[0] &= ~0x00000008u;
}
void Answer::clear_text() {
  text_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_text();
}
 const ::std::string& Answer::text() const {
  // @@protoc_insertion_point(field_get:Answer.text)
  return text_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void Answer::set_text(const ::std::string& value) {
  set_has_text();
  text_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Answer.text)
}
 void Answer::set_text(const char* value) {
  set_has_text();
  text_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Answer.text)
}
 void Answer::set_text(const char* value, size_t size) {
  set_has_text();
  text_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Answer.text)
}
 ::std::string* Answer::mutable_text() {
  set_has_text();
  // @@protoc_insertion_point(field_mutable:Answer.text)
  return text_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 ::std::string* Answer::release_text() {
  // @@protoc_insertion_point(field_release:Answer.text)
  clear_has_text();
  return text_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void Answer::set_allocated_text(::std::string* text) {
  if (text != NULL) {
    set_has_text();
  } else {
    clear_has_text();
  }
  text_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), text);
  // @@protoc_insertion_point(field_set_allocated:Answer.text)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
