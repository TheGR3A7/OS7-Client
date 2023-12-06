#include <Windows.h>
#include <iostream>
#include <string>

int main() 
{
    system("chcp 1251");
    
    TCHAR pipeName[] = TEXT("\\\\.\\pipe\\MyPipe");

    HANDLE pipe = CreateFile(
        pipeName,                   // ��� ������
        GENERIC_READ | GENERIC_WRITE, // ���������� ������
        0,                           // ���������� �� ������ (0 ��� ������������� ��������� �� ���������)
        NULL,                        // �������� ������������ (NULL ��� ������������� ��������� �� ���������)
        OPEN_EXISTING,               // ��������� ������������ �����
        0,                           // �������� ����� (0 ��� ������������� ��������� �� ���������)
        NULL                         // ����� ������� ����� (NULL, ��� ��� ����� ��� ����������)
    );

    if (pipe == INVALID_HANDLE_VALUE) 
    {
        std::cerr << "������ � �������� Pipe. ��� ������: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "������ ��������� � �������. ������� ��������� ��� �������� (exit ��� ����������):" << std::endl;

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
            std::cerr << "������ ������ � Pipe. ��� ������: " << GetLastError() << std::endl;
            break;
        }

        
        char buffer[100];
        DWORD bytesRead;
        if (ReadFile(pipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) == FALSE) 
        {
            std::cerr << "������ ������ �� Pipe. ��� ������: " << GetLastError() << std::endl;
            break;
        }
        buffer[bytesRead] = '\0'; 


        std::cout << "������ �������: " << std::string(buffer, bytesRead) << std::endl;
    }

    
    CloseHandle(pipe);

    return 0;
}
