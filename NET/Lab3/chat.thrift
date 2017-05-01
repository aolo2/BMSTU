namespace cpp chat

/**
 * Исключение (ник занят)
 */
exception UsernameTaken {
    1: string message,
}

/**
 * Сообщение
 */
struct ChatMessage {
    1: string text,     // содержание
    2: i32 uid,         // id отправителя
    3: string username  // имя отправителя
}

/**
 * "Чат"
 */
service Chat {
    /**
     * Получить номер последнего сообщения
     */
    i32 getCurrentLogId(),


    /**
     * Получить список пользователей
     */	
    list<string> getUserList(),
    

    /**
     * Получить пропущенные сообщения. Параметры:
     * 1: номер последнего сообщение, которое пользователь уже "видел"
     * 2: id пользователя (не присылать ему его собственные сообщения)
     */
    list<ChatMessage> missedMessages(1: i32 last, 2: i32 uid),
    
    /**
     * Запросить id взамен на логин (выкидывает исключение, если логин занят)
     */
    i32 get_uid(1: string username) throws (1: UsernameTaken ouch),

    /**
     * Отправить сообщение
     */
    oneway void sendMessage(1: ChatMessage msg),
    
    /**
     * Запрос на отключение (на самом деле просто удаление из книги)
     */
    oneway void disconnectMe(1: i32 uid),
}
