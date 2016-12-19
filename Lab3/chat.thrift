namespace cpp chat

exception UsernameTaken {}

struct ChatMessage {
    1: string text,
    2: i32 uid,
    3: string username
}

service Chat {
    i32 getCurrentLogId(),

    list<string> getUserList(),
    
    list<ChatMessage> missedMessages(1: i32 last, 2: i32 uid),
    
    i32 get_uid(1: string username),

    oneway void sendMessage(1: ChatMessage msg),
    
    oneway void disconnectMe()
}