/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "chat_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace chat {


UsernameTaken::~UsernameTaken() throw() {
}


uint32_t UsernameTaken::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    xfer += iprot->skip(ftype);
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t UsernameTaken::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("UsernameTaken");

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(UsernameTaken &a, UsernameTaken &b) {
  using ::std::swap;
  (void) a;
  (void) b;
}

UsernameTaken::UsernameTaken(const UsernameTaken& other0) : TException() {
  (void) other0;
}
UsernameTaken& UsernameTaken::operator=(const UsernameTaken& other1) {
  (void) other1;
  return *this;
}
void UsernameTaken::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "UsernameTaken(";
  out << ")";
}

const char* UsernameTaken::what() const throw() {
  try {
    std::stringstream ss;
    ss << "TException - service has thrown: " << *this;
    this->thriftTExceptionMessageHolder_ = ss.str();
    return this->thriftTExceptionMessageHolder_.c_str();
  } catch (const std::exception&) {
    return "TException - service has thrown: UsernameTaken";
  }
}


ChatMessage::~ChatMessage() throw() {
}


void ChatMessage::__set_text(const std::string& val) {
  this->text = val;
}

void ChatMessage::__set_uid(const int32_t val) {
  this->uid = val;
}

void ChatMessage::__set_username(const std::string& val) {
  this->username = val;
}

uint32_t ChatMessage::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->text);
          this->__isset.text = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->uid);
          this->__isset.uid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->username);
          this->__isset.username = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ChatMessage::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("ChatMessage");

  xfer += oprot->writeFieldBegin("text", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->text);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("uid", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->uid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("username", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->username);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ChatMessage &a, ChatMessage &b) {
  using ::std::swap;
  swap(a.text, b.text);
  swap(a.uid, b.uid);
  swap(a.username, b.username);
  swap(a.__isset, b.__isset);
}

ChatMessage::ChatMessage(const ChatMessage& other2) {
  text = other2.text;
  uid = other2.uid;
  username = other2.username;
  __isset = other2.__isset;
}
ChatMessage& ChatMessage::operator=(const ChatMessage& other3) {
  text = other3.text;
  uid = other3.uid;
  username = other3.username;
  __isset = other3.__isset;
  return *this;
}
void ChatMessage::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "ChatMessage(";
  out << "text=" << to_string(text);
  out << ", " << "uid=" << to_string(uid);
  out << ", " << "username=" << to_string(username);
  out << ")";
}

} // namespace
