#include <Windows.h>
#include <iostream>
#include <string>

int main() 
{
    system("chcp 1251");
    
    TCHAR pipeName[] = TEXT("\\\\.\\pipe\\MyPipe");

    HANDLE pipe = CreateFile(
        pipeName,                   // Имя канала
        GENERIC_READ | GENERIC_WRITE, // Дуплексный доступ
        0,                           // Разрешения на доступ (0 для использования атрибутов по умолчанию)
        NULL,                        // Атрибуты безопасности (NULL для использования атрибутов по умолчанию)
        OPEN_EXISTING,               // Открывает существующий канал
        0,                           // Атрибуты файла (0 для использования атрибутов по умолчанию)
        NULL                         // Хэндл шаблона файла (NULL, так как канал уже существует)
    );

    if (pipe == INVALID_HANDLE_VALUE) 
    {
        std::cerr << "Ошибка в открытии Pipe. Код ошибки: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Клиент подключен к серверу. Введите сообщение для отправки (exit для завершения):" << std::endl;

    while (true) 
    {
        std::string message;
        std::getline(std::cin, message);

        if (message == "exit") 
        {
            break;
        }

        DWORD bytesWritten;
        if (WriteFile(pipe, message.c_str(), static_cast<DWORD>(message.size()), &bytesWritten, NULL) == FALSE) 
        {
            std::cerr << "Ошибка записи в Pipe. Код ошибки: " << GetLastError() << std::endl;
            break;
        }

        
        char buffer[100];
        DWORD bytesRead;
        if (ReadFile(pipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) == FALSE) 
        {
            std::cerr << "Ошибка чтения из Pipe. Код ошибки: " << GetLastError() << std::endl;
            break;
        }
        buffer[bytesRead] = '\0'; 


        std::cout << "Сервер ответил: " << std::string(buffer, bytesRead) << std::endl;
    }

    
    CloseHandle(pipe);

    return 0;
}
